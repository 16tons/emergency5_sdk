// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/fire/component/WaterjetComponent.h"
#include "em5/fire/FireHelper.h"
#include "em5/game/achievement/Achievements.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/units/OrderInfo.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/logic/local/firefighters/VehicleExtinguishLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/component/base/DecorationComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
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
	const qsf::NamedIdentifier ExtinguishFireAction::ACTION_ID = "em5::ExtinguishFireAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishFireAction::ExtinguishFireAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mExtinguishPower(10.0f),
		mCoolingPower(-10.f),
		mTargetIsBuring(false),
		mEffectEntityId(qsf::getUninitialized<uint64>()),
		mOpticalTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mMaxExtinguishRange(10.f)
	{
		// Nothing here
	}

	ExtinguishFireAction::~ExtinguishFireAction()
	{
		stopExtinguishEffect();
	}

	void ExtinguishFireAction::init(const qsf::Entity& targetEntity, float extinguishPower, float coolingPower)
	{
		mTargetEntityId = targetEntity.getId();
		mExtinguishPower = extinguishPower;
		mCoolingPower = coolingPower;

		FireReceiverComponent* fireReceiverComponent = getFireComponentFromTarget();
		if (nullptr != fireReceiverComponent)
		{
			// Set initial value of target is burning, make it in the init function to detect if the target is not burning anymore in the moment we arrived.
			mTargetIsBuring = fireReceiverComponent->isBurning();
		}
	}

	uint64 ExtinguishFireAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ExtinguishFireAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mExtinguishPower);
		serializer.serialize(mEffectEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ExtinguishFireAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Get maximum extinguish distance once
		// -> Increase the maximum range to avoid problems with targets at the border of the normal range. We don't take it too serious here.
		mMaxExtinguishRange = FireHelper::getExtinguishRangeFromEntity(getEntity()) * 1.4f;

		qsf::Entity* opticalTargetEntity = FireHelper::getEffectTargetInRange(getEntity(), *targetEntity, mMaxExtinguishRange);
		if (nullptr == opticalTargetEntity)
		{
			// Caller is too far away from target
			return false;
		}
		mOpticalTargetEntityId = opticalTargetEntity->getId();

		EntityHelper entityHelper(getEntity());
		if (entityHelper.isSquadPerson())
		{
			// For persons, check if equipment is right
			const qsf::game::EquipmentComponent* equipmentComponent = entityHelper.getActiveEquipmentComponent();
			if (nullptr != equipmentComponent)
			{
				const std::string& equipmentName = equipmentComponent->getEquipmentName();
				if (equipmentName != equipment::FIRE_EXTINGUISHER && equipmentName != equipment::FIRE_EXTINGUISHER_HISTORICAL &&
					equipmentName != equipment::HOSE_NOZZLE && equipmentName != equipment::HOSE_NOZZLE_HISTORICAL)
				{
					// Squad person has invalid equipment, don't allow extinguish
					return false;
				}
			}
		}

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	void ExtinguishFireAction::onShutdown()
	{
		FireReceiverComponent* fireReceiverComponent = getFireComponentFromTarget();
		if (nullptr != fireReceiverComponent)
		{
			// Remove us as a source of cooling energy
			fireReceiverComponent->removeCoolingEnergySource(getEntityId());
		}

		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}

	qsf::action::Result ExtinguishFireAction::updateAction(const qsf::Clock&)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				// Push turn to target action if we don't have a vehicle
				VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
				if (nullptr == vehicleComponent)
				{
					getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).init(mOpticalTargetEntityId, false);
				}

				// Look with cannon to the target
				WaterCannonComponent* waterCannonComponent = getWaterCannonComponent();
				if (nullptr != waterCannonComponent)
				{
					waterCannonComponent->setTargetEntityId(mOpticalTargetEntityId);
				}

				mState = STATE_START_EFFECT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_START_EFFECT:
			{
				const WaterCannonComponent* waterCannonComponent = getWaterCannonComponent();
				if (nullptr != waterCannonComponent && !waterCannonComponent->getIsCannonInFinalRotation())
				{
					// Wait till the water cannon is turned to target
					return qsf::action::RESULT_CONTINUE;
				}

				FireReceiverComponent* fireReceiverComponent = getFireComponentFromTarget();
				if (nullptr != fireReceiverComponent)
				{
					// Register as cooling energy source and the value
					fireReceiverComponent->addCoolingEnergySource(getEntityId(), mExtinguishPower, mCoolingPower);

					// check with the current burning state
					if (fireReceiverComponent->isBurning())
					{
						// Register the player as "has interacted with" event object
						EventIdComponent::registerPlayer(fireReceiverComponent->getEntity(), getEntity());
					}

					if (!fireReceiverComponent->isBurning() && mTargetIsBuring)
					{
						// We started extinguishing and now the target is not burning -> abort (no error)
						return qsf::action::RESULT_DONE;
					}
				}

				startExtinguishEffect();

				if (!mTargetIsBuring)
				{
					// Unregister the lock again, when the target isn't burning -> we are doing cooling here
					EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
				}

				// Send message that extinguish of the fires has started
				qsf::MessageParameters parameters;
				parameters.setParameter("actorId", getEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_EXTINGUISH_FIRE_STARTED, mTargetEntityId), parameters);

				mState = STATE_EXTINGUISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_EXTINGUISH:
			{
				FireReceiverComponent* fireReceiverComponent = getFireComponentFromTarget();

				// Detect the end of extinguish and search for the next target in range
				bool needNewTarget = false;
				if (nullptr == fireReceiverComponent)
				{
					needNewTarget = true;
				}
				else
				{
					// The is active check because we want to stop cooling while cutting trees
					if (fireReceiverComponent->isBurned() || !fireReceiverComponent->isActive())
						needNewTarget = true;

					if (!fireReceiverComponent->isBurning())
					{
						if (mTargetIsBuring)
						{
							// Target was burning, but is extinguished
							needNewTarget = true;
						}

						// Count the burning object achievement
						if (fireReceiverComponent->canCountedForExtinguishAchievement())
						{
							EM5_ACHIEVEMENT.addProgress(Achievements::EM5_ACHIEVEMENT_02, 1.0f);
							fireReceiverComponent->setCountedForExtinguishAchievement(false);
						}

						// Else we give the cooling command, ignore if object is burning or cooling. Just do it.
					}

					if (!isTargetInExtinguishRange(fireReceiverComponent->getEntity(), EntityHelper(getEntity()).getPosition()))
					{
						// Target is out of range. Can happen for moving targets, like vehicles in cooling process. Happens very rare and looks very funny.
						needNewTarget = true;
					}
				}

				// Check if the energy has to change
				if (needNewTarget)
				{
					FireComponent* newTargetInRange = searchNewTargetInRange();
					if (nullptr != newTargetInRange)
					{
						// Push extinguish action, if everything fits, the new action deletes this current action
						pushExtinguishAction(*newTargetInRange);
					}
					else
					{
						// No other burning object in range, everything is done. Abort action.
						return qsf::action::RESULT_DONE;
					}
				}
				else
				{
					// Use old target
					// Detect if target is start burning
					if (fireReceiverComponent->isBurning() != mTargetIsBuring)
					{
						mTargetIsBuring = !mTargetIsBuring;

						// Register as cooling energy source and the value
						fireReceiverComponent->addCoolingEnergySource(getEntityId(), mExtinguishPower, mCoolingPower);
					}
				}

				// Audio (if its not playing already then its the extinguisher)
				if (!mAudioProxy.isPlaying())
				{
					if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC))
					{
						// Play historic audio
						AudioHelper::playFirefighterHistoricExtinguisherRunning(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();
					}
					else
					{
						// Play audio
						AudioHelper::playFirefighterExtinguisherExtinguishSound(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();
					}
				}

				break;
			}
		}

		return qsf::action::RESULT_CONTINUE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ExtinguishFireAction::startExtinguishEffect()
	{
		if (qsf::isUninitialized(mEffectEntityId))
		{
			EntityHelper entityHelper(getEntity());
			const bool withExtinguisher = (entityHelper.checkIsActiveEquipment(equipment::FIRE_EXTINGUISHER) || entityHelper.checkIsActiveEquipment(equipment::FIRE_EXTINGUISHER_HISTORICAL));
			qsf::Entity* effectEntity = (withExtinguisher ? createExtinguisherEffectEntity() : createWaterJetEffectEntity());
			if (nullptr != effectEntity)
			{
				mEffectEntityId = effectEntity->getId();
				linkEffectEntity(*effectEntity);
			}

			// Start animation
			if (entityHelper.isSquadPerson())
			{
				if (withExtinguisher)
				{
					// Start the extinguish animation
					getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseExtinguisher(), false, true);

					if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_HISTORIC))
					{
						// Play historic audio
						AudioHelper::playFirefighterHistoricExtinguisherStart(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();
					}
					else
					{
						// Play audio
						AudioHelper::playFirefighterExtinguisherStartSound(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();
					}
				}
				else
				{
					// Start the extinguish animation
					getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationFirefighterUseFireHose(), false, true);

					// Play audio
					AudioHelper::playFirefighterHoseExtinguishSound(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
			}
			else
			{
				// Else case is vehicle with cannon, no animation played

				// Play extinguish sound
				if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DLK))
				{
					AudioHelper::playDLKExtinguish(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
				else if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FLB))
				{
					AudioHelper::playFLBExtinguish(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
				else //if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_TLF))
				{
					AudioHelper::playExtinguishFireWithTLFSound(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}
			}
		}
	}

	void ExtinguishFireAction::stopExtinguishEffect()
	{
		if (qsf::isInitialized(mEffectEntityId))
		{
			// Use the map helper to destroy the entity -> In case of multiplay the client gets informed
			MapHelper(getMap()).destroyEntityById(mEffectEntityId);
			qsf::setUninitialized(mEffectEntityId);
		}

		// Delete old fireman animation
		AnimationHelper(getEntity()).clearAnimation();

		// Tell vehicles to stop look at the target
		WaterCannonComponent* waterCannonComponent = getWaterCannonComponent();
		if (nullptr != waterCannonComponent)
		{
			// "forget" the cannon target
			waterCannonComponent->setTargetEntityId(qsf::getUninitialized<uint64>());
		}

		// Stop playing sound
		mAudioProxy.stop();
		mAudioProxy.detach();
		mAudioProxy.synchronizeWithMultiplay();
	}

	qsf::Entity* ExtinguishFireAction::createExtinguisherEffectEntity() const
	{
		// Show the particle effect for fire extinguisher
		// TODO(sw) Currently this works for multiplay that the particle effect is visible on multiplayer client side, because the particle is already active from the prefab
		//			If this changes the state of an particle must be also transferred :(
		// We use the map helper here to create the entity from prefab -> We have in case of multiplay the same entity id on multiplay host and client side
		qsf::Entity* effectEntity = MapHelper(getEntity().getMap()).createObjectByLocalPrefabAssetId(equipment::PREFAB_FIRE_EXTINGUISHER_EFFECT);
		if (nullptr != effectEntity)
		{
			// Show the particles
			qsf::ParticlesComponent* particlesComponent = effectEntity->getOrCreateComponent<qsf::ParticlesComponent>();
			if (nullptr != particlesComponent)
			{
				particlesComponent->setActive(true);
				particlesComponent->showAnimation();
				particlesComponent->setEmitterEnabled(true);
			}
		}
		return effectEntity;
	}

	qsf::Entity* ExtinguishFireAction::createWaterJetEffectEntity()
	{
		// Create the big waterjet effect, used for vehicles and persons with hose
		// We use the map helper here to create the entity from prefab -> We have in case of multiplay the same entity id on multiplay host and client side
		qsf::Entity* effectEntity = MapHelper(getEntity().getMap()).createObjectByLocalPrefabAssetId(assets::PREFAB_FIRE_WATERJET_EFFECT);
		QSF_CHECK(nullptr != effectEntity, "EM5: Extinguish fire action failed to create the waterjet entity instance", return nullptr);

		WaterjetComponent* waterjetComponent = effectEntity->getOrCreateComponent<WaterjetComponent>();
		if (nullptr != waterjetComponent)
		{
			waterjetComponent->setTargetEntityId(mOpticalTargetEntityId);

			// Use the decoration component as a marker for entities that should be ignored by in-game selection and game logics
			effectEntity->getOrCreateComponent<qsf::game::DecorationComponent>();
		}

		return effectEntity;
	}

	void ExtinguishFireAction::linkEffectEntity(qsf::Entity& effectEntity)
	{
		// Link on target entity
		qsf::LinkComponent* linkComponent = effectEntity.getOrCreateComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			uint64 targetId = getTargetIdForLinkingEffectEntity();

			// TODO(mk) Work around for the broken waterjet
			qsf::Entity* nozzleEntity = getMap().getEntityById(targetId);
			if (nullptr != nozzleEntity)
			{
				qsf::TransformComponent& effectTransformComponent = effectEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& nozzleTransformComponent = nozzleEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				effectTransformComponent.setTransform(nozzleTransformComponent.getTransform());
			}

			// Link effect entity to target
			linkComponent->setParentId(targetId);
			linkComponent->setParentLinkType(qsf::LinkComponent::HARD_LINK);

			// Link to bone of the parent
			qsf::game::BoneLinkComponent* boneLinkComponent = effectEntity.createComponent<qsf::game::BoneLinkComponent>();
			if (nullptr != boneLinkComponent)
			{
				boneLinkComponent->setBoneName(equipment::STANDARD_BONE_EFFECT);
			}
		}
	}

	FireReceiverComponent* ExtinguishFireAction::getFireComponentFromTarget()
	{
		const qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		return (nullptr != targetEntity) ? targetEntity->getComponent<FireReceiverComponent>() : nullptr;
	}

	FireComponent* ExtinguishFireAction::searchNewTargetInRange()
	{
		qsf::Entity& callerEntity = getEntity();

		// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
		static std::vector<FireComponent*> burningFireComponentVector;
		FireHelper::getSortedTargetsInRange(burningFireComponentVector, callerEntity, FireHelper::getAutomatismDistanceFromCaller(callerEntity));

		return (burningFireComponentVector.empty()) ? nullptr : burningFireComponentVector.front();
	}

	uint64 ExtinguishFireAction::getTargetIdForLinkingEffectEntity()
	{
		// Link on water cannon
		WaterCannonComponent* waterCannonComponent = getWaterCannonComponent();
		if (nullptr != waterCannonComponent)
		{
			// Look with cannon to the target
			waterCannonComponent->setTargetEntityId(mOpticalTargetEntityId);

			return waterCannonComponent->getCannonEntityId();
		}

		qsf::game::EquipmentComponent* equipmentComponent = EntityHelper(getEntity()).getActiveEquipmentComponent();
		if (nullptr != equipmentComponent)
		{
			// Found equipment, return ID
			return equipmentComponent->getEntityId();
		}

		// TODO(mk) Handle boats

		// Error, return ID to owner as fallback
		return getEntityId();
	}

	void ExtinguishFireAction::pushExtinguishAction(FireComponent&) const
	{
		const qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
			if (nullptr != firemanLogic)
			{
				firemanLogic->performAutomatism();
				return;
			}
			VehicleExtinguishLogic* vehicleExtinguishLogic = gameLogicComponent->getGameLogic<VehicleExtinguishLogic>();
			if (nullptr != vehicleExtinguishLogic)
			{
				vehicleExtinguishLogic->extinguishAutomatism();
				return;
			}
		}

		// Error
		const OrderInfo* orderInfo = EntityHelper(getEntity()).getOrderInfo();
		QSF_ERROR_ONCE("ExtinguishFireAction::pushExtinguishAction(): Internal error while pushing the next extinguish action, no valid entity type of entity " << getEntityId() << " of type " << (orderInfo != nullptr ? orderInfo->getName() : ""), return);
	}

	WaterCannonComponent* ExtinguishFireAction::getWaterCannonComponent() const
	{
		WaterCannonComponent* waterCannonComponent = nullptr;
		const RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr != roadVehicleComponent)
		{
			waterCannonComponent = roadVehicleComponent->getWaterCannonComponent();
			if (nullptr != waterCannonComponent)
			{
				waterCannonComponent->setIsOnBoat(false);
			}
		}
		else
		{
			const BoatComponent* boatComponent = getEntity().getComponent<BoatComponent>();
			if (nullptr != boatComponent)
			{
				waterCannonComponent = boatComponent->getWaterCannonComponent();
				if (nullptr != waterCannonComponent)
				{
					waterCannonComponent->setIsOnBoat(true);
				}
			}
		}
		return waterCannonComponent;
	}

	bool ExtinguishFireAction::isTargetInExtinguishRange(qsf::Entity& targetEntity, const glm::vec3& callerPosition) const
	{
		return FireHelper::isTargetInExtinguishRange(targetEntity, callerPosition, mMaxExtinguishRange);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
