// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/ContaminationCloudComponent.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/physics/collision/BulletSphereCollisionComponent.h>
#include <qsf/debug/group/DebugGroupManager.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ContaminationCloudComponent::COMPONENT_ID = qsf::StringHash("em5::ContaminationCloudComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ContaminationCloudComponent::ContaminationCloudComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mState(STATE_INACTIVE),
		mMinContaminationDelay(0.0f),
		mMaxContaminationDelay(0.0f),
		mContaminationRadius(0.0f),
		mTimerId(qsf::getUninitialized<uint32>())
	{
		// Nothing to do here
	}

	void ContaminationCloudComponent::startSimulation()
	{
		if (STATE_SHUTDOWN == mState || STATE_INACTIVE == mState)
		{
			qsf::MessageConfiguration message(Messages::EM5_ENTITY_TIMER_SIGNAL, getEntityId());
			mMessageProxy.registerAt(message, boost::bind(&ContaminationCloudComponent::startContaminationSpreading, this, _1));
			setTimer(message, qsf::Time::fromSeconds(qsf::Random::getRandomFloat(mMinContaminationDelay, mMaxContaminationDelay)));
			mState = STATE_STARTUP;

			{ // Add contamination area icon to minimap
				static qsf::Color4 contaminationAreaColor = qsf::Color4::GREEN;
				contaminationAreaColor.a = 0.5f;

				float radius = getContaminationRadius();
				qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					// We want a pixel perfect representation of the cloud cicle area on the minimap (the radius of the circle)
					// So compensate the scale, because when the polygon in generated the scale of the entity transform is applied
					radius /= transformComponent->getScale().x;
				}

				// The contamination cloud my move so mark it as "non static"
				EM5_GAME.getMiniMapIconRegistrationLogic().registerCirclePolygon(getEntityId(), radius, 40, contaminationAreaColor, true);
			}
		}
	}

	void ContaminationCloudComponent::stopSimulation()
	{
		if (STATE_STARTUP == mState || STATE_ACTIVE == mState)
		{
			qsf::MessageConfiguration message(Messages::EM5_ENTITY_TIMER_SIGNAL, getEntityId());
			mMessageProxy.registerAt(message, boost::bind(&ContaminationCloudComponent::stopContaminationSpreading, this, _1));
			setTimer(message, qsf::Time::fromSeconds(qsf::Random::getRandomFloat(mMinContaminationDelay, mMaxContaminationDelay)));
			mState = STATE_SHUTDOWN;

			// Delete the AI representation of the fire
			getEntity().destroyComponent<qsf::BulletSphereCollisionComponent>();

			// Remove contamination area icon from minimap
			EM5_GAME.getMiniMapIconRegistrationLogic().unregisterPolygon(getEntityId());
		}
	}

	void ContaminationCloudComponent::setContaminationRadius(float contaminationRadius)
	{
		if (mContaminationRadius != contaminationRadius)
		{
			mContaminationRadius = contaminationRadius;
			onPropertyChange();

			if (isSimulating())
			{
				// Update the ai component
				qsf::BulletSphereCollisionComponent& sphereCollision = getEntity().getOrCreateComponentSafe<qsf::BulletSphereCollisionComponent>();

				// Setup the values - but only once normally. Is this correct here?
				sphereCollision.setRadius(mContaminationRadius);
				sphereCollision.setCollisionFilterGroup(collision::SENSORTRIGGER | collision::KINEMATICFILTER);
				sphereCollision.setCollisionMask(collision::TOXICCLOUDFILTER);
			}
		}
	}

	bool ContaminationCloudComponent::isInsideContaminationRadius(const glm::vec3& position) const
	{
		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		return qsf::game::DistanceHelper::get2dDistance(transformComponent.getPosition(), position) < mContaminationRadius;
	}

	bool ContaminationCloudComponent::getHealthComponentsInsideContaminationRadius(std::vector<HealthComponent*>& healthComponents) const
	{
		qsf::Entity& entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();

		// Search for persons in range
		return qsf::ComponentMapQuery(entity.getMap()).getInstancesInCircle(transformComponent.getPosition(), mContaminationRadius, healthComponents);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void ContaminationCloudComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint32>(mState);
		serializer.serialize(mMinContaminationDelay);
		serializer.serialize(mMaxContaminationDelay);
		serializer.serialize(mContaminationRadius);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool ContaminationCloudComponent::onStartup()
	{
		onSetDebug(GameDebugGroup::getInstanceSafe().getShowContaminationRadius());
		QSF_DEBUGGROUP.PropertyChanged.connect(boost::bind(&ContaminationCloudComponent::onDebugGroupPropertyChanged, this, _1, _2));

		// Make sure  particle effect is disabled
		setParticlesState(false, false);
		return true;
	}

	void ContaminationCloudComponent::onShutdown()
	{
		QSF_DEBUGGROUP.PropertyChanged.disconnect(boost::bind(&ContaminationCloudComponent::onDebugGroupPropertyChanged, this, _1, _2));
		mDebugDrawProxy.unregister();

		// Only unregister the polygon icon if we are still inside of the game
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning())
		{
			// Remove contamination area icon from minimap
			game->getMiniMapIconRegistrationLogic().unregisterPolygon(getEntityId());
		}
	}

	void ContaminationCloudComponent::onSetSimulating(bool simulating)
	{
		setParticlesState(false, false);
		if (simulating)
		{
			// Don't show debug visualization in game
			mDebugDrawProxy.unregister();
		}
		else
		{
			mSimulationJobProxy.unregister();
		}
	}

	void ContaminationCloudComponent::onSetDebug(bool debug)
	{
		if (debug || GameDebugGroup::getInstanceSafe().getShowContaminationRadius())
		{
			onPropertyChange();
		}
		else
		{
			mDebugDrawProxy.unregister();
		}
	}

	void ContaminationCloudComponent::onComponentPropertyChange(const Component& component, uint32 propertyId)
	{
		if (qsf::TransformComponent::COMPONENT_ID == component.getId())
		{
			// Do not add debug draw requests for pure prototypes, or in the game
			if (getPrototype().isEntity() && !isSimulating())
			{
				const qsf::TransformComponent& transformComponent = static_cast<const qsf::TransformComponent&>(component);
				mDebugDrawProxy.unregister();
				addRequest(transformComponent);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ContaminationCloudComponent::startContaminationSpreading(const qsf::MessageParameters& parameters)
	{
		// Uninitialize timer ID because it is not valid any more
		qsf::setUninitialized(mTimerId);

		if (mState == STATE_STARTUP)
		{
			// Starts particle and starts spreading damage shortly after that
			qsf::MessageConfiguration message(Messages::EM5_ENTITY_TIMER_SIGNAL, getEntityId());
			mMessageProxy.registerAt(message, boost::bind(&ContaminationCloudComponent::startContaminationSpreading, this, _1));
			setParticlesState(true, true);
			setTimer(message, qsf::Time::fromSeconds(1.5f));
			mState = STATE_ACTIVE;
		}
		else if (mState == STATE_ACTIVE)
		{
			// There must be a link component or this component is used wrong
			const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
			QSF_CHECK(nullptr != linkComponent, "ContaminationCloudComponent needs a link component", QSF_REACT_NONE);
			if (nullptr != linkComponent && linkComponent->hasParent())
			{
				qsf::Entity* parentEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
				QSF_CHECK(nullptr != parentEntity, "ContaminationCloudComponent needs a parent entity", QSF_REACT_NONE);
				if (parentEntity != nullptr)
				{
					qsf::MessageParameters messageParameters;
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_CONTAMINATION_START_SPREADING, parentEntity->getId(), getEntityId()), messageParameters);
				}
			}

			// Start job for injure persons inside the cloud
			mSimulationJobProxy.registerAt(Jobs::SIMULATION_HEALTH, boost::bind(&ContaminationCloudComponent::manageContaminationSpreading, this, _1));
		}
	}

	void ContaminationCloudComponent::stopContaminationSpreading(const qsf::MessageParameters& parameters)
	{
		// Uninitialize timer ID because it is not valid any more
		qsf::setUninitialized(mTimerId);

		if (mState == STATE_SHUTDOWN)
		{
			mSimulationJobProxy.unregister();
			setParticlesState(true, false);
			mState = STATE_INACTIVE;
		}
	}

	void ContaminationCloudComponent::setParticlesState(bool active, bool emitterEnabled)
	{
		// Get the particles component. If there is no particles component there is a failure in the use of this component.
		qsf::ParticlesComponent* particlesComponent = getEntity().getComponent<qsf::ParticlesComponent>();
		QSF_CHECK(nullptr != particlesComponent, "Contamination cloud component could not find the required particles component in Entity " << getEntityId(), return);
		particlesComponent->setActive(active);
		particlesComponent->setEmitterEnabled(emitterEnabled);
		particlesComponent->setRepeatAnimation(emitterEnabled);

		// Update the AI component
		if (active)
		{
			// Set the AI representation of the fire. Used for detecting and avoiding the fire area in the pathfinding
			qsf::BulletSphereCollisionComponent* bulletSphereCollisionComponent = getEntity().getComponent<qsf::BulletSphereCollisionComponent>();
			if (nullptr != bulletSphereCollisionComponent)
			{
				// To avoid setting the AI information more than necessary, set them only after creating the component
				bulletSphereCollisionComponent = getEntity().getOrCreateComponent<qsf::BulletSphereCollisionComponent>();
				bulletSphereCollisionComponent->setRadius(mContaminationRadius);
				bulletSphereCollisionComponent->setCollisionFilterGroup(collision::SENSORTRIGGER | collision::KINEMATICFILTER);
				bulletSphereCollisionComponent->setCollisionMask(collision::TOXICCLOUDFILTER);
			}
		}
		else
		{
			// Delete the AI representation of the fire
			getEntity().destroyComponent<qsf::BulletSphereCollisionComponent>();
		}
	}

	void ContaminationCloudComponent::setTimer(const qsf::MessageConfiguration& message, const qsf::Time& timerTime)
	{
		qsf::game::GameTimerManager& gameTimerManager = EM5_GAMETIMERS;
		if (qsf::isInitialized(mTimerId))
		{
			qsf::game::GameTimer* gameTimer = gameTimerManager.findTimerById(mTimerId);
			if (nullptr != gameTimer)
			{
				gameTimer->resetTimer(message, timerTime);
				return;
			}
		}

		mTimerId = gameTimerManager.addTimer(message, timerTime);
	}

	void ContaminationCloudComponent::manageContaminationSpreading(const qsf::JobArguments& jobArguments)
	{
		// Search for other persons in range
		std::vector<HealthComponent*> healthComponents;
		getHealthComponentsInsideContaminationRadius(healthComponents);

		// There must be a link component or this component is used wrong
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->hasParent())
		{
			qsf::Entity* parentEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			QSF_CHECK(nullptr != parentEntity, "Invalid parent entity", return);

			// There must be a contamination component or this is used wrong
			ContaminationComponent* contaminationComponent = parentEntity->getComponent<ContaminationComponent>();
			QSF_CHECK(nullptr != contaminationComponent, "There must be a industrial plant component in the parent entity", return);

			// Contaminate the poor guys
			FreeplayEvent* freeplayEvent = EntityHelper(*parentEntity).getFreeplayEvent();
			const ContaminationType contaminationType = contaminationComponent->getContaminationType();
			for (HealthComponent* healthComponent : healthComponents)
			{
				EntityHelper victimEntityHelper(healthComponent->getEntity());

				// Only valid targets
				if (!victimEntityHelper.isEntityValidEventTarget(freeplayEvent))
					continue;

				// Don't contaminate them while they are still in vehicles (otherwise units arrive contaminated...)
				if (victimEntityHelper.isEntityInVehicle())
					continue;

				// Don't contaminated them while they are linked to a parent (makes it somewhat easier to code... and once parent gets ill they still might get contaminated when that releases them)
				if (nullptr != victimEntityHelper.getParentEntity())
					continue;

				if (HealthHelper(*healthComponent).injurePersonByContamination(freeplayEvent, contaminationType))
				{
					// Count the number of victims inside the contamination Component (parent link)
					contaminationComponent->increaseContaminatedPersons();
				}
				else
				{
					// Do we have a firefighter?
					CommandableComponent* commandableComponent = healthComponent->getEntity().getComponent<CommandableComponent>();
					if (commandableComponent != nullptr && commandableComponent->isFireFighterPerson())
					{
						// Emit a message
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_FIREFIGHTER_INSIDE_CONTAMINATION_AREA, healthComponent->getEntityId(), getEntityId()));
					}
				}
			}
		}
		else
		{
			QSF_ERROR("The contamination cloud component must be linked to a parent entity, with industrial plant component", QSF_REACT_NONE);
		}
	}

	void ContaminationCloudComponent::addRequest(const qsf::TransformComponent& transformComponent)
	{
		if (isDebug())
		{
			mDebugDrawProxy.registerForComponent(*this);
			mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(transformComponent.getPosition(), qsf::CoordinateSystem::getUp(), mContaminationRadius, qsf::Color4::GREEN, true));
		}
	}

	void ContaminationCloudComponent::onPropertyChange()
	{
		if (getPrototype().isEntity())
		{
			mDebugDrawProxy.unregister();
			addRequest(getEntity().getOrCreateComponentSafe<qsf::TransformComponent>());
		}
	}

	void ContaminationCloudComponent::onDebugGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId)
	{
		const GameDebugGroup& gameDebugGroup = GameDebugGroup::getInstanceSafe();
		if (&gameDebugGroup == &parameterGroup && propertyId == gameDebugGroup.SHOW_CONTAMINATION_RADIUS)
		{
			setDebug(gameDebugGroup.getShowContaminationRadius());
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
