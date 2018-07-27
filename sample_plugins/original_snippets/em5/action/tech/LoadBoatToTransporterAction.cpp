// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/LoadBoatToTransporterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/movement/SwingComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/Game.h"
#include "em5/plugin/Messages.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/logic/action/WaitAction.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier LoadBoatToTransporterAction::ACTION_ID = "em5::LoadBoatToTransporterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LoadBoatToTransporterAction::LoadBoatToTransporterAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mWatersprayEntity(nullptr),
		mWaterdropsEntity(nullptr)
	{
		// Nothing here
	}

	LoadBoatToTransporterAction::~LoadBoatToTransporterAction()
	{
		// Nothing here
	}

	void LoadBoatToTransporterAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 LoadBoatToTransporterAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void LoadBoatToTransporterAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mDefaultRotation);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool LoadBoatToTransporterAction::onStartup()
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Check distance
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, getEntity());
		const float maxDistance = 10.0f;	// High distance just to ensure that boat and transporter are at the same droppoint
		if (currentDistance >= maxDistance)
			return false;

		// Block boat from doing anything
		qsf::ActionComponent& targetActionComponent = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		targetActionComponent.clearPlan();
		targetActionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(qsf::Time::fromHours(1000.0f));

		return true;
	}

	qsf::action::Result LoadBoatToTransporterAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
			SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
			if (nullptr != vehicleCraneComponent && nullptr != supportLegsComponent)
			{
				qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

				switch (mState)
				{
					case STATE_INIT:
					{
						// Make vehicle invincible
						EntityHelper(getEntity()).setInvincible(true);

						// Rotate boat to ensure that there are no problems
						const glm::vec3 transporterEuler = qsf::EulerAngles::quaternionToEuler(transformComponent.getRotation());
						const glm::vec3 boatEuler = qsf::EulerAngles::quaternionToEuler(targetTransformComponent.getRotation());
						const glm::quat rotation = qsf::EulerAngles::eulerToQuaternion(transporterEuler.x, boatEuler.y, boatEuler.z);

						targetTransformComponent.setRotation(rotation);
						EntityHelper(*targetEntity).applyPlacement();

						// Extend support legs
						supportLegsComponent->startExtending();

						mState = STATE_SUPPORTLEGS_EXPAND;
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_SUPPORTLEGS_EXPAND:
					{
						if (!supportLegsComponent->isAnimationPlaying())
						{
							mDropPosition = targetTransformComponent.getPosition();
							mDropPosition.y = 2.0f + transformComponent.getPosition().y;
							vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_LOAD_BOAT_ALIGN_ARM);
							vehicleCraneComponent->alignArm(mDropPosition);

							mState = STATE_CRANE_ALIGN_ARM;
						}
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_CRANE_ALIGN_ARM:
					{
						if (!vehicleCraneComponent->isAnimationPlaying())
						{
							mDropPosition.y = 2.0f + targetTransformComponent.getPosition().y;
							vehicleCraneComponent->alignArm(mDropPosition);

							mState = STATE_CRANE_ARM_DOWN;
						}
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_CRANE_ARM_DOWN:
					{
						if (!vehicleCraneComponent->isAnimationPlaying())
						{
							vehicleCraneComponent->setVisibleStrap(true);
							linkBoatToCraneEntity(*targetEntity);
							vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_LOAD_BOAT_LOAD_CARGO_ARM);
							vehicleCraneComponent->loadCargoArm();
							mState = STATE_CRANE_LOAD_CARGO;
						}
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_CRANE_LOAD_CARGO:
					{
						if (!vehicleCraneComponent->isAnimationPlaying())
						{
							linkBoatToVehicle(*targetEntity);
							unlinkParticles(*targetEntity);

							vehicleCraneComponent->stopAllAnimation();
							supportLegsComponent->hideExtending();

							mState = STATE_SUPPORTLEGS_HIDE;
						}
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_SUPPORTLEGS_HIDE:
					{
						if (supportLegsComponent->isAnimationPlaying())
						{
							// Wait for the end of the animation
							return qsf::action::RESULT_CONTINUE;
						}
						vehicleCraneComponent->stopAllAnimation();

						// Make vehicle invincible
						EntityHelper(getEntity()).setInvincible(false);
						mState = STATE_DONE;
						return qsf::action::RESULT_CONTINUE;
					}

					case STATE_DONE:
					{
						// set routerComponent inactive again
						qsf::ai::RouterComponent* routerComponent = targetEntity->getComponent<qsf::ai::RouterComponent>();
						if (nullptr != routerComponent)
							routerComponent->setActive(false);

						// Fall through by design
					}
				}
			}

			// Unblock boat
			targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void LoadBoatToTransporterAction::linkBoatToCraneEntity(qsf::Entity& boatEntity)
	{
		BoatComponent* boatComponent = boatEntity.getComponent<BoatComponent>();
		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		qsf::TransformComponent& targetTransformComponent = boatEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		if (nullptr != vehicleCraneComponent && nullptr != boatComponent)
		{
			qsf::Entity* jibEntity = getFirstChildEntity(getMap().getEntityById(vehicleCraneComponent->getCraneEntityId()));
			qsf::Entity* stageEntity = getFirstChildEntity(jibEntity);
			qsf::Entity* strapEntity = getFirstChildEntity(stageEntity);

			if (nullptr != strapEntity)
			{
				// Reposition the boat
				targetTransformComponent.setPosition(strapEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition() + glm::vec3(0.0f, -2.47f, 0.0f));

				{ // Create or reset link
					qsf::LinkComponent& linkComponent = boatEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
					linkComponent.setParentId(strapEntity->getId());
					linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);
				}

				// Set lake ID
				boatComponent->setLakeId(qsf::getUninitialized<uint64>());

				// Deactivate boat bobbing
				boatEntity.getOrCreateComponentSafe<SwingComponent>().setActive(false);

				// Deactivate lights
				boatComponent->enableLights(false);

				// The boat radar should stop
				boatComponent->enableRadar(false);

				{ // Play audio
					AudioProxy audioProxy;
					AudioHelper::playBoatLoadFromWater(audioProxy, getEntity());
					audioProxy.detach();
					audioProxy.synchronizeWithMultiplay();
				}

				// Add particles
				mWatersprayEntity = getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERSPRAY);
				mWaterdropsEntity = getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERDROPS);

				linkParticles(boatEntity, mWatersprayEntity, 0.0f, 2.0f);
				linkParticles(boatEntity, mWaterdropsEntity, 0.0f, 4.0f);

				// Selection handling
				{
					SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
					const bool wasSelected = selectionManager.isIdSelected(boatEntity.getId());
					const bool selectTransporter = wasSelected && (selectionManager.getSelectionSize() == 1);

					// Deselect unit if selected
					selectionManager.removeIdFromSelection(boatEntity.getId());

					// In case only the boat was selected, select the transporter after this
					if (selectTransporter)
					{
						selectionManager.setSelectionById(getEntityId());
					}

					// If selection changed, an update is needed
					if (wasSelected)
					{
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
					}
				}
			}
		}
	}

	void LoadBoatToTransporterAction::linkBoatToVehicle(qsf::Entity& boatEntity)
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return;

		qsf::LinkComponent* linkComponent = boatEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		qsf::Prototype* prefab = getEntity().getComponentSafe<qsf::MetadataComponent>().getBasePrototype();
		if (nullptr == prefab)
			return;

		RoadVehicleComponent* prototypeRoadVehicleComponent = prefab->getComponent<RoadVehicleComponent>();
		if (nullptr == prototypeRoadVehicleComponent)
			return;

		qsf::Prototype* prototypeBoatEntity = prefab->getPrototypeManager().getPrototypeById(prototypeRoadVehicleComponent->getSpecialEntity());
		if (nullptr == prototypeBoatEntity)
			return;

		qsf::LinkComponent* prototypeLinkComponent = prototypeBoatEntity->getComponent<qsf::LinkComponent>();
		if (nullptr == prototypeLinkComponent)
			return;

		// Link to transporter
		EntityHelper(boatEntity).enterContainer(getEntity());

		// Make transporter visible (extra handling for motorboat)
		qsf::game::HiddenComponent::setHidden(boatEntity, false);

		linkComponent->setLocalPosition(prototypeLinkComponent->getLocalPosition());
		linkComponent->setLocalRotation(prototypeLinkComponent->getLocalRotation());
		roadVehicleComponent->setSpecialEntity(boatEntity.getId());

		// TODO(tl): Dirty hack to update linkComponent
		qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		transformComponent.setPosition(transformComponent.getPosition() + glm::vec3(0.0f, -0.01f, 0.0f));

		// Deactivate navigation component
		qsf::ai::NavigationComponent* navigationComponent = boatEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}

		// Play audio
		AudioProxy audioProxy;
		AudioHelper::playBoatDropOnTransporter(audioProxy, getEntity());
		audioProxy.detach();
		audioProxy.synchronizeWithMultiplay();

		// Send message
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_VEHICLE_TO_TRANSPORTER, mTargetEntityId));
	}

	qsf::Entity* LoadBoatToTransporterAction::getFirstChildEntity(qsf::Entity* entity)
	{
		// Just give back the first child entity
		if (nullptr == entity)
			return nullptr;

		const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		boost::container::flat_set<qsf::LinkComponent*>::const_iterator iterator = linkComponent->getChildLinks().begin();
		if (iterator == linkComponent->getChildLinks().end())
			return nullptr;

		return &((*iterator)->getEntity());
	}

	void LoadBoatToTransporterAction::linkParticles(const qsf::Entity& targetEntity, qsf::Entity* particleEntity, float delay, float timeout)
	{
		if (nullptr != particleEntity)
		{
			qsf::ParticlesComponent& particlesComponent = particleEntity->getOrCreateComponentSafe<qsf::ParticlesComponent>();
			qsf::LinkComponent& linkComponent = particleEntity->getOrCreateComponentSafe<qsf::LinkComponent>();

			// TODO(tl): Delay Time doesn't work
			particlesComponent.setDelayTime(delay);
			particlesComponent.setFixedTimeout(timeout);
			{
				// Force restart
				particlesComponent.setEmitterEnabled(false);
				particlesComponent.setEmitterEnabled(true);
				particlesComponent.setActive(false);
				particlesComponent.setActive(true);
			}
			linkComponent.setParentId(targetEntity.getId());
			linkComponent.setLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	void LoadBoatToTransporterAction::unlinkParticles(qsf::Entity& targetEntity)
	{
		for (const qsf::LinkComponent* childLink : targetEntity.getOrCreateComponentSafe<qsf::LinkComponent>().getChildLinks())
		{
			if (nullptr != childLink->getEntity().getComponent<qsf::ParticlesComponent>())
			{
				// Only delete our own particle effects
				if ((nullptr != mWatersprayEntity && childLink->getEntityId() == mWatersprayEntity->getId())
					|| (nullptr != mWaterdropsEntity && childLink->getEntityId() == mWaterdropsEntity->getId()))
				{
					MapHelper(getMap()).destroyEntity(childLink->getEntity());
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
