// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/InjurePersonAreaComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/map/EntityHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/injury/InjuryManager.h"
#include "em5/health/HealthSystem.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

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
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 InjurePersonAreaComponent::COMPONENT_ID = qsf::StringHash("em5::InjurePersonAreaComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	InjurePersonAreaComponent::InjurePersonAreaComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mAffectedAreaRadius(1.0f),
		mInjuriesToUse("HeartAttack")	// "em5::injury::HEARTATTACK"
	{
		// Nothing to do here
	}

	void InjurePersonAreaComponent::setAffectedAreaRadius(float affectedAreaRadius)
	{
		if (mAffectedAreaRadius != affectedAreaRadius)
		{
			mAffectedAreaRadius = affectedAreaRadius;
			onPropertyChange();
		}
	}

	void InjurePersonAreaComponent::setInjuriesToUse(const std::string& injuriesToUse)
	{
		if (mInjuriesToUse != injuriesToUse)
		{
			mInjuriesToUse = injuriesToUse;
		}
	}

	bool InjurePersonAreaComponent::isInsideAffectedAreaRadius(const glm::vec3& position)
	{
		const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		return (qsf::game::DistanceHelper::get2dDistance(transformComponent.getPosition(), position) < mAffectedAreaRadius);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void InjurePersonAreaComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mAffectedAreaRadius);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool InjurePersonAreaComponent::onStartup()
	{
		onSetDebug(GameDebugGroup::getInstanceSafe().getShowContaminationRadius());
		QSF_DEBUGGROUP.PropertyChanged.connect(boost::bind(&InjurePersonAreaComponent::onDebugGroupPropertyChanged, this, _1, _2));
		return true;
	}

	void InjurePersonAreaComponent::onShutdown()
	{
		QSF_DEBUGGROUP.PropertyChanged.disconnect(boost::bind(&InjurePersonAreaComponent::onDebugGroupPropertyChanged, this, _1, _2));
		mDebugDrawProxy.unregister();
	}

	void InjurePersonAreaComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// Don't show debug visualization in game
			mDebugDrawProxy.unregister();

			{ // Setup injury list
				mInjuriesToUseList.clear();

				// Split the string
				std::vector<std::string> injuriesNameList;
				boost::split(injuriesNameList, mInjuriesToUse, boost::is_any_of(","));

				for (const std::string& injuryName : injuriesNameList)
				{
					const Injury* injury = EM5_HEALTH.getInjuryById(qsf::StringHash(injuryName.c_str()));
					if (nullptr != injury)
					{
						mInjuriesToUseList.push_back(injury->getInjuryId());
					}
				}

				if (mInjuriesToUseList.empty())
				{
					// Fallback to have at least one injury to select from
					mInjuriesToUseList.push_back(injury::HEARTATTACK);
				}
			}
		}

		internalSetActive(simulating && isActive());
	}

	void InjurePersonAreaComponent::onSetActive(bool active)
	{
		internalSetActive(active && isSimulating());
	}

	void InjurePersonAreaComponent::onSetDebug(bool debug)
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

	void InjurePersonAreaComponent::onComponentPropertyChange(const Component& component, uint32 propertyId)
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
	void InjurePersonAreaComponent::internalSetActive(bool active)
	{
		if (active)
		{
			if (!mSimulationJobProxy.isValid())
			{
				qsf::jobs::JobConfiguration jobConfiguration;
				jobConfiguration.mTimeBetweenCalls.setSeconds(0.2f); // No need to update each frame
				mSimulationJobProxy.registerAt(Jobs::SIMULATION_HEALTH, boost::bind(&InjurePersonAreaComponent::manageContaminationSpreading, this, _1), jobConfiguration);
			}
		}
		else
		{
			mSimulationJobProxy.unregister();
		}
	}

	void InjurePersonAreaComponent::manageContaminationSpreading(const qsf::JobArguments& jobArguments)
	{
		qsf::Entity& entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();

		// Search for other persons in range
		std::vector<HealthComponent*> healthComponents;
		qsf::ComponentMapQuery(entity.getMap()).getInstancesInCircle(transformComponent.getPosition(), mAffectedAreaRadius, healthComponents);

		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();

		for (HealthComponent* healthComponent : healthComponents)
		{
			EntityHelper victimEntityHelper(healthComponent->getEntity());

			// Only valid victims
			if (!victimEntityHelper.isEntityValidEventTarget())
				continue;

			// Don't contaminate them while they are still in vehicles (otherwise units arrive injured...)
			if (victimEntityHelper.isEntityInVehicle())
				continue;

			// Don't injure them while they are linked to a parent (makes it somewhat easier to code... and once parent get's ill they still might get injured when that releases them)
			if (nullptr != victimEntityHelper.getParentEntity())
				continue;

			const uint32 injuryId = qsf::Random::getAnyOf(mInjuriesToUseList, injury::BURN_OF_BREATH.getHash());
			healthComponent->injurePersonByEventById(injuryId, freeplayEvent);

			if (nullptr != freeplayEvent)
			{
				// Re add the entity to the event with a different event spread reason to allow special handling for this
				freeplayEvent->addEntityToEvent(healthComponent->getEntity(), eventspreadreason::INJURE_PERSON_AREA, false);
			}
		}
	}

	void InjurePersonAreaComponent::addRequest(const qsf::TransformComponent& transformComponent)
	{
		if (isDebug())
		{
			mDebugDrawProxy.registerForComponent(*this);
			mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(transformComponent.getPosition(), qsf::CoordinateSystem::getUp(), mAffectedAreaRadius, qsf::Color4::RED, true));
		}
	}

	void InjurePersonAreaComponent::onPropertyChange()
	{
		if (getPrototype().isEntity())
		{
			mDebugDrawProxy.unregister();
			addRequest(getEntity().getOrCreateComponentSafe<qsf::TransformComponent>());
		}
	}

	void InjurePersonAreaComponent::onDebugGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId)
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
