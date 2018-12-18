// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/UseDivingSpotAction.h"
#include "em5/action/fire/PickupDrowningPersonWithDiverAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/fire/UseDivingSpotCommand.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Anonymous namespace                                   ]
	//[-------------------------------------------------------]
	namespace
	{
		const std::string PARTICLE_ASSET_ID("em5/prefab/particles/gischt");
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UseDivingSpotAction::ACTION_ID = "em5::UseDivingSpotAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseDivingSpotAction::UseDivingSpotAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mDivingTimeMax(10.0f),
		mParticlesId(qsf::getUninitialized<uint64>()),
		mUnderwaterEntity(nullptr),
		mLootType(LOOT_NONE),
		mWasInvincible(false)
	{
		// Nothing here
	}

	UseDivingSpotAction::~UseDivingSpotAction()
	{
		// Ensure diver is visible again (did go wrong in one situation which wasn't reproducible so far)
		qsf::game::HiddenComponent::setHidden(getEntity(), false);

		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void UseDivingSpotAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 UseDivingSpotAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UseDivingSpotAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mState);
		serializer.serialize(mDiveTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool UseDivingSpotAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
		const float currentDistance = qsf::game::DistanceHelper::get2dDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" and "checkContext" of the commando for validation
		UseDivingSpotCommand* useDivingSpotCommand = static_cast<UseDivingSpotCommand*>(QSFGAME_COMMAND.getCommandManager().getById<UseDivingSpotCommand>(UseDivingSpotCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != useDivingSpotCommand, "UseDivingSpotAction::onStartup(): Could not find use diving spot command instance", return false);
		if (!(useDivingSpotCommand->checkCallerWithoutPriority(*commandContext.mCaller) && useDivingSpotCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		HealthComponent* healthComponent = callerEntity.getComponent<HealthComponent>();
		if (healthComponent)
		{
			mWasInvincible = healthComponent->isInvincible();
			healthComponent->setInvincible(true);
		}

		ReserveLogic::createReservation<ReserveDivingSpotLogic>(*targetEntity, getEntityId());

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	void UseDivingSpotAction::onShutdown()
	{
		HealthComponent* healthComponent = getEntity().getComponent<HealthComponent>();
		if (healthComponent)
		{
			healthComponent->setInvincible(mWasInvincible);
		}

		// Destroy the created splash particles
		if (qsf::isInitialized(mParticlesId))
		{
			MapHelper(getMap()).destroyEntityById(mParticlesId);
		}

		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}

	qsf::action::Result UseDivingSpotAction::updateAction(const qsf::Clock& clock)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(targetEntity != nullptr, "UseDivingSpotAction::updateAction(): Target entity is not valid", return qsf::action::RESULT_DONE)

		switch (mState)
		{
			case STATE_INIT:
			{
				// Show him diving down
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDiveDown(), true);

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playDiverDive(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mState = STATE_DIVE_DOWN;

				// Give it 1 frame to start the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DIVE_DOWN:
			{
				// Play some "splash" particle effect
				qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
				qsf::Entity* particleEntity = MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash(PARTICLE_ASSET_ID));
				if (nullptr != particleEntity && nullptr != transformComponent)
				{
					// Save particle ID in order to destroy them when this action is shut down
					mParticlesId = particleEntity->getId();
					qsf::MovableComponent& particleMovableComponent = particleEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
					particleMovableComponent.warpToPositionAndRotation(transformComponent->getPosition(), transformComponent->getRotation());
				}

				// Make diver invisible
				qsf::game::HiddenComponent::setHidden(getEntity(), true);
				mDiveTime = qsf::Time::fromSeconds(mDivingTimeMax);

				mState = STATE_DIVING;

				// Immediately go to the next state; no break by intent
			}

			case STATE_DIVING:
			{
				mDiveTime -= clock.getTimePassed();

				qsf::Entity* overlayTarget = getVisibleDivingSpotEntity();

				// Still under water?
				if (mDiveTime > qsf::Time::ZERO)
				{
					// Show progress bar for time
					if (nullptr != overlayTarget)
					{
						StatusOverlayComponent::showBarOverlay(*overlayTarget, StatusOverlayComponent::BAR_TYPE_PROGRESS, (mDivingTimeMax - mDiveTime.getSeconds()) / mDivingTimeMax);
					}

					// Wait for ending of diving
					return qsf::action::RESULT_CONTINUE;
				}

				// Hide progress bar
				if (nullptr != overlayTarget)
				{
					StatusOverlayComponent::hideBarOverlay(*overlayTarget, StatusOverlayComponent::BAR_TYPE_PROGRESS);
				}

				// Make diver and targets visible
				qsf::game::HiddenComponent::setHidden(getEntity(), false);
				handleDivingState();

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playDiverEmerge(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				// Show them coming up
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDiveUp(), true);

				mState = STATE_DIVING_END;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DIVING_END:
			{
				flagDivingSpotAsUsed();

				// Blink the minimap
				EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(getEntityId());

				// Show hint
				showHint();

				if (mLootType == LOOT_PERSON && mUnderwaterEntity.valid())
				{
					// Move outside and show hidden target
					//  -> In case we have alternative targets from diving spot, target may be inside a container or hidden in another way
					EntityHelper(mUnderwaterEntity).leaveContainer();
					qsf::game::HiddenComponent::setHidden(*mUnderwaterEntity, false);

					getComponent().pushAction<PickupDrowningPersonWithDiverAction>(action::BLOCKING).init(mUnderwaterEntity->getId());
				}
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void UseDivingSpotAction::handleDivingState()
	{
		mUnderwaterEntity = spawnUnderWaterTarget();
		if (mUnderwaterEntity.valid())
		{
			handleUnderWaterTarget(*mUnderwaterEntity);

			// Send message
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FIND_ENTITY_FROM_DIVINGSPOT, mTargetEntityId, mUnderwaterEntity->getId()));
		}
		else
		{
			handleResultNone();
		}
	}

	qsf::Entity* UseDivingSpotAction::spawnUnderWaterTarget()
	{
		qsf::Map& map = getEntity().getMap();
		qsf::Entity* targetEntity = map.getEntityById(mTargetEntityId);
		qsf::Entity* underWaterTarget = nullptr;
		if (nullptr != targetEntity)
		{
			DivingSpotComponent* divingSpotComponent = targetEntity->getComponent<DivingSpotComponent>();
			if (nullptr != divingSpotComponent)
			{
				// In case an target is linked to the diving spot, use this target
				if (qsf::isInitialized(divingSpotComponent->getAlternativeTargetId()))
				{
					// Get underwater target from diving spot
					underWaterTarget = map.getEntityById(divingSpotComponent->getAlternativeTargetId());

					// Unregister old target
					divingSpotComponent->setAlternativeTargetId(qsf::getUninitialized<uint64>());
				}
				else
				{
					// Spawn random entity
					underWaterTarget = divingSpotComponent->spawnRandomEntityFromPool();
				}
			}
		}

		// Return the underWaterTarget, can be a null pointer
		return underWaterTarget;
	}

	void UseDivingSpotAction::handleUnderWaterTarget(qsf::Entity& underWaterTarget)
	{
		if (EntityHelper(underWaterTarget).isRoadVehicle())
		{
			mLootType = LOOT_CAR;

			// In case of underwater target is an vehicle, create an buoy entity and link it
			qsf::Entity* bouyEntity = spawnBuoyEntity();
			if (nullptr != bouyEntity)
			{
				// Link vehicle to the buoy
				qsf::LinkComponent& linkComponent = underWaterTarget.getOrCreateComponentSafe<qsf::LinkComponent>();
				linkComponent.setParentId(bouyEntity->getId());
				linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

				// You only see the buoy
				qsf::game::HiddenComponent::setHidden(underWaterTarget);
			}
		}
		else
		{
			mLootType = LOOT_PERSON;

			// Move right to the diver (so it is definitely not too far away)
			underWaterTarget.getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition() - glm::vec3(0.0f, 2.0f, 0.0f));

			// Make sure person is initially hidden
			qsf::game::HiddenComponent::setHidden(underWaterTarget);
		}
	}

	qsf::Entity* UseDivingSpotAction::spawnBuoyEntity() const
	{
		// Create buoy
		qsf::Entity* buoyEntity = MapHelper(getEntity().getMap()).createObjectByLocalPrefabAssetId(equipment::PREFAB_FIRE_BUOY);
		if (nullptr != buoyEntity)
		{
			// Set the position to the diver position + some offset (so they are not inside each other)
			static const glm::vec3 OFFSET(0.5f, 0.0f, 0.0f);
			qsf::MovableComponent& buoyTransformComponent = buoyEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
			const qsf::TransformComponent& diverTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			buoyTransformComponent.warpToPositionAndRotation(diverTransformComponent.getPosition() + OFFSET, diverTransformComponent.getRotation());

			AnimationHelper animationHelper(*buoyEntity);
			animationHelper.playAnimation(animationHelper.getAnimationBuoy(), true);
		}
		return buoyEntity;
	}

	void UseDivingSpotAction::handleResultNone()
	{
		mLootType = LOOT_NONE;
	}

	void UseDivingSpotAction::flagDivingSpotAsUsed()
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			DivingSpotComponent* divingSpotComponent = targetEntity->getComponent<DivingSpotComponent>();
			if (nullptr != divingSpotComponent)
			{
				divingSpotComponent->setIntact(false, divingSpotComponent->getFreeplayEvent());
			}
		}
	}

	qsf::Entity* UseDivingSpotAction::getVisibleDivingSpotEntity() const
	{
		return getMap().getEntityById(mTargetEntityId);
	}

	void UseDivingSpotAction::showHint() const
	{
		// TODO(mk) Found this comment, think here is the right place: "Yes, this is a action sending a hint - as was discussed. There is no point passing around messages for this which only will be forgotten in half the places."
		if (mLootType == LOOT_NONE)
		{
			HintHelper::showHint("ID_DIVER_DIVING_NO_RESULT_01");
		}
		else if (mLootType == LOOT_PERSON)
		{
			HintHelper::showHint("ID_DIVER_DIVING_MAN_RESCUED_01");
		}
		else if (mLootType == LOOT_CAR)
		{
			HintHelper::showHint("ID_DIVER_DIVING_BUOY_SET_01");
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
