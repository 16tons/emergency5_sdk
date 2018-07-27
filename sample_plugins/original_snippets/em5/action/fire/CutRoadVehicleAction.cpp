// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/CutRoadVehicleAction.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/fire/CutRoadVehicleCommand.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CutRoadVehicleAction::ACTION_ID = "em5::CutRoadVehicleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CutRoadVehicleAction::CutRoadVehicleAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mPlayAllRescueAnimations(false)
	{
		// Nothing here
	}

	CutRoadVehicleAction::~CutRoadVehicleAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());

		if (mCompressorPumpEntity.valid())
		{
			mCompressorPumpEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
		}
	}

	void CutRoadVehicleAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();

		// Default cut time when there is no road vehicle component
		const RoadVehicleComponent* roadVehicle = targetEntity.getComponent<RoadVehicleComponent>();
		mTime = (roadVehicle != nullptr) ? roadVehicle->getCutDuration() : qsf::Time::fromSeconds(10.0f);

		// To avoid multiple firefighter at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the action starts, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 CutRoadVehicleAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	bool CutRoadVehicleAction::isDone() const
	{
		return (mState == STATE_DONE);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CutRoadVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mState);
		serializer.serialize(mTime);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CutRoadVehicleAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const RoadVehicleComponent* roadVehicleComponent = targetEntity->getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return false;

		std::vector<qsf::Transform> driverDoors;
		roadVehicleComponent->getDoorPositions(DoorComponent::DOORTYPE_DRIVER, driverDoors, targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getTransform());

		bool distanceFound = false;
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getCutCarDistance();

		for (const qsf::Transform& transform : driverDoors)
		{
			const float currentDistance = qsf::game::DistanceHelper::getDistance(transform.getPosition(), callerEntity);
			if (currentDistance <= maxDistance)
			{
				if (currentDistance <= 1.0f)
				{
					mPlayAllRescueAnimations = true;
				}
				distanceFound = true;
			}
		}

		// Fallback if there are no doors: Use center of vehicle
		if (!distanceFound)
		{
			const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
			if (currentDistance <= maxDistance)
			{
				if (currentDistance <= 1.0f)
				{
					mPlayAllRescueAnimations = true;
				}

				distanceFound = true;
			}
		}

		if (!distanceFound)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		CutRoadVehicleCommand* cutRoadVehicleCommand = static_cast<CutRoadVehicleCommand*>(QSFGAME_COMMAND.getCommandManager().getById<CutRoadVehicleCommand>(CutRoadVehicleCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != cutRoadVehicleCommand, "CutRoadVehicleAction::onStartup(): Could not find cut road vehicle command instance", return false);
		if (!(cutRoadVehicleCommand->checkCallerWithoutPriority(*commandContext.mCaller) && cutRoadVehicleCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		// TODO(mk) Make firefighter invincible while he doing this

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result CutRoadVehicleAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;

		// Get the road vehicle component
		RoadVehicleComponent* roadVehicleComponent = targetEntity->getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return qsf::action::RESULT_DONE;

		AnimationHelper animationHelper(getEntity());

		switch (mState)
		{
			case STATE_INIT:
			{
				// Change reserve logic from "moveto" to "paramedic" (has higher priority)
				ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
				ReserveLogic::createReservation<ReserveCutRoadVehicleLogic>(*targetEntity, getEntityId());

				const qsf::Entity* doorEntity = getMap().getEntityById(roadVehicleComponent->getDriverVehicleDoorId());
				if (nullptr != doorEntity)
				{
					DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
					if (nullptr != doorComponent)
					{
						getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).initWithRotation(doorComponent->getDoorRotationWorldSpaceRotation());
					}
				}

				// Play compressor drop audio
				AudioProxy audioProxy;
				AudioHelper::playFirefighterScissorsCompressorDrop(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mState = STATE_CREATE_COMPRESSOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CREATE_COMPRESSOR:
			{
				createCompressorPump();

				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationFirefighterUseRescueCutter1());

				// Emit message that this action has started
				// (oi) Pass target and source entity IDs to react to action on specific targets
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_STARTED, ACTION_ID, mTargetEntityId, getEntityId()));

				mState = STATE_PLACE_CUTTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLACE_CUTTER:
			{
				static const qsf::Time TIME_PLACE_CUTTER = qsf::Time::fromSeconds(1.3f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= TIME_PLACE_CUTTER)
				{
					// Wait for the right moment in the animation
					return qsf::action::RESULT_CONTINUE;
				}

				linkCutterToCompressor();

				// Play compressor audio
				AudioHelper::playFirefighterScissorsCompressorRunning(mCompressorAudioProxy, getEntity());
				mCompressorAudioProxy.synchronizeWithMultiplay();

				mState = STATE_PICKUP_CUTTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PICKUP_CUTTER:
			{
				static const qsf::Time TIME_PICKUP_CUTTER = qsf::Time::fromSeconds(3.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= TIME_PICKUP_CUTTER)
				{
					// Wait for the right moment in the animation
					return qsf::action::RESULT_CONTINUE;
				}

				pickupCutterFromCompressor();

				mState = STATE_START_CUTTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_START_CUTTING:
			{
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getOffset() < 0.99f)
				{
					// Wait until animation finishes
					return qsf::action::RESULT_CONTINUE;
				}

				// Play animation
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFirefighterUseRescueCutter2(), true);

				// Play audio
				AudioHelper::playFirefighterScissorsSound(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_LOOP_CUTTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOOP_CUTTING:
			{
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationFirefighterUseRescueCutter2()))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Play looped animation
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationFirefighterUseRescueCutter3(), false, true);

				// Start "Cut door" animation
				const qsf::Entity* doorEntity = getMap().getEntityById(roadVehicleComponent->getDriverVehicleDoorId());
				if (nullptr != doorEntity)
				{
					DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
					if (nullptr != doorComponent)
					{
						// Start "Cut door" animation
						doorComponent->cutDoor();
					}
				}

				mState = STATE_END_CUTTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END_CUTTING:
			{
				// The full action should take ~15s
				mTime -= clock.getTimePassed();

				// Animation is looped, wait to for ending
				if (mTime > qsf::Time::ZERO)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationFirefighterUseRescueCutter1());

				// Stop cutting sound
				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_PLACE_CUTTER_AGAIN;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLACE_CUTTER_AGAIN:
			{
				static const qsf::Time TIME_PLACE_CUTTER = qsf::Time::fromSeconds(1.3f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= TIME_PLACE_CUTTER)
				{
					// Wait for the right moment in the animation
					return qsf::action::RESULT_CONTINUE;
				}

				linkCutterToCompressor();

				// Stop compressor sound
				mCompressorAudioProxy.stop();
				mCompressorAudioProxy.synchronizeWithMultiplay();

				mState = STATE_UNLOAD;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_UNLOAD:
			{
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getOffset() < 0.99f)
				{
					// Wait until animation finishes
					return qsf::action::RESULT_CONTINUE;
				}

				const uint32 numberOfPersonsInside = unloadPersonFromVehicle(*roadVehicleComponent);
				if (numberOfPersonsInside <= 1)		// I.e. afterwards nobody is inside any more
				{
					// Remove the "can be cut" flag to prevent multiple cutting open for special cuttable vehicles with no persons inside
					roadVehicleComponent->setCanBeCut(false);
				}

				// Someone rescued at all?
				if (numberOfPersonsInside >= 1)
				{
					// TODO(mk) Add check for enough place for pulling, otherwise place the person direct on the final position (with place)
					if (mPlayAllRescueAnimations)
					{
						pushPullingPersonFromVehicleActions();
					}
					else
					{
						// Skip the rescue animation, place the person on a save position
						pushFallbackAnimationsAndPlaceRescuedPerson();
					}
				}

				// Take cutter again
				mState = STATE_FINALLY_START_PICKUP_CUTTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINALLY_START_PICKUP_CUTTER:
			{
				if (mRescuedPerson.valid())
				{
					// TODO(sw) We emit here the message for the first entities. (I need this for the tutorial)
					// Emit message
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_TOOKOUTSIDE_PERSON, mRescuedPerson->getId()));
				}

				if (roadVehicleComponent->isIntact() || roadVehicleComponent->getCanReallyNotBeTowed())
				{
					// Hide icon for defective vehicle (relevant for some EM20 missions)
					StatusOverlayComponent::hideIcon(*targetEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE);
				}

				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationFirefighterUseRescueCutter1());

				mState = STATE_FINALLY_PICKUP_CUTTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINALLY_PICKUP_CUTTER:
			{
				static const qsf::Time TIME_PICKUP_CUTTER = qsf::Time::fromSeconds(3.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= TIME_PICKUP_CUTTER)
				{
					// Wait for the right moment in the animation
					return qsf::action::RESULT_CONTINUE;
				}

				pickupCutterFromCompressor();

				if (mCompressorPumpEntity.valid())
				{
					mCompressorPumpEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
				}

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getOffset() < 0.99f)
				{
					// Wait until animation finishes
					return qsf::action::RESULT_CONTINUE;
				}

				animationHelper.clearAnimation();

				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_CAR_CUT_OPEN_ACTION, mTargetEntityId));

				// Done
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void CutRoadVehicleAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CutRoadVehicleAction::createCompressorPump()
	{
		if (!mCompressorPumpEntity.valid())
		{
			mCompressorPumpEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(assets::PREFAB_FIREFIGHTING_CUTTER_PUMP);
		}

		if (mCompressorPumpEntity.valid())
		{
			const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& compressorTransformComponent = mCompressorPumpEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

			static const glm::vec3 COMPRESSOR_PUMP_OFFSET(-0.677f, 0, 0);
			glm::vec3 newPosition = transformComponent.getTransform().vec3PositionLocalToWorld(COMPRESSOR_PUMP_OFFSET);

			compressorTransformComponent.setPositionAndRotation(newPosition, transformComponent.getRotation());

			mCompressorPumpEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
		}
	}

	void CutRoadVehicleAction::linkCutterToCompressor() const
	{
		const qsf::game::EquipmentComponent* equipmentComponent = EntityHelper(getEntity()).getActiveEquipmentComponent();
		if (nullptr != equipmentComponent)
		{
			qsf::Entity& rescueCutterEntity = equipmentComponent->getEntity();

			// Link cutter to compressor
			if (mCompressorPumpEntity.valid())
			{
				rescueCutterEntity.destroyComponent<qsf::game::BoneLinkComponent>();
				rescueCutterEntity.getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(mCompressorPumpEntity->getId());

				// Set position of the cutter on the compressor
				const qsf::TransformComponent& compressorTransformComponent = mCompressorPumpEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& cutterTransformComponent = rescueCutterEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

				static const glm::vec3 CUTTEROFFSET(0.1f, 0.4f, 0);
				glm::vec3 newPosition = compressorTransformComponent.getTransform().vec3PositionLocalToWorld(CUTTEROFFSET);

				cutterTransformComponent.setPositionAndRotation(newPosition, qsf::Math::GLM_QUAT_IDENTITY);
			}
		}
	}

	void CutRoadVehicleAction::pickupCutterFromCompressor() const
	{
		// Link cutter to compressor
		const qsf::game::EquipmentComponent* equipmentComponent = EntityHelper(getEntity()).getActiveEquipmentComponent();
		if (nullptr != equipmentComponent && mCompressorPumpEntity.valid())
		{
			EntityHelper(equipmentComponent->getEntity()).linkEquipmentToEntity(getEntity());
		}
	}

	uint32 CutRoadVehicleAction::unloadPersonFromVehicle(const qsf::game::PrototypeContainer& roadVehicleComponent)
	{
		// Go over all entities in the container
		// -> Do this in a separate step before actually exiting the entities, so we don't run into problems because of manipulation of the iterated list
		qsf::EntityVectorScratchBuffer entitiesToExit;
		for (auto iterator : roadVehicleComponent.getLinks())
		{
			// Passengers can't exit the vehicle
			const qsf::ContainerLink& containerLink = *iterator.second;
			if (containerLink.mContainerCategory == container::CONTAINERTYPE_PASSENGER)
			{
				entitiesToExit.push_back(&containerLink.getTargetEntity());
			}
		}

		// This is what we want to return:
		//  0 if vehicle is empty, 1 if one person that will be rescued, 2 if there are more people inside
		const uint32 numberOfEntitiesInside = static_cast<uint32>(entitiesToExit.size());
		if (numberOfEntitiesInside >= 1)
		{
			// We can only get out one at a time
			qsf::Entity& containedEntity = *entitiesToExit->front();

			QSF_CHECK(EntityHelper(containedEntity).isPersonInjured(), "Contained entity to cut out with ID " << containedEntity.getId() << " is not injured", QSF_REACT_NONE);

			// Get him out of here!
			EntityHelper(containedEntity).leaveContainer();
			containedEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();

			// Set person on firefighter position
			const qsf::TransformComponent& ownerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& targetTransformComponent = containedEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

			// Place the person in front of the firefighter, ready for pulling
			const glm::vec3 targetPosition = ownerTransformComponent.getTransform().vec3PositionLocalToWorld(-PullPersonTargetPointProvider::getOffset());
			targetTransformComponent.setPositionAndRotation(targetPosition, ownerTransformComponent.getRotation());

			// Register the player as "has interacted with" event object
			EventIdComponent::registerPlayer(containedEntity, getEntity());

			if (mRescuedPerson.valid())
			{
				// TODO(sw) We only emit the message here only for additional entities. The message for the first one is emitted, when pull out animation is finished (I need this for the tutorial)
				// Emit message
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_TOOKOUTSIDE_PERSON, containedEntity.getId()));
			}
			else
			{
				mRescuedPerson = containedEntity;
			}
		}

		return numberOfEntitiesInside;
	}

	void CutRoadVehicleAction::pushPullingPersonFromVehicleActions() const
	{
		// Push pulling actions
		qsf::ActionComponent& actionComponent = getComponent();
		actionComponent.pushAction<PullPersonAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mRescuedPerson.getSafe().getId(), PullPersonAction::START, true);

		{ // Move to end location (with pulling logic)
			static const float MAXPULLDISTANCE = 2.0f;
			const qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

			const glm::vec3 positionOffset = transformComponent.getRotation() * -qsf::CoordinateSystem::getIn() * MAXPULLDISTANCE;
			const glm::vec3 finalPosition = transformComponent.getPosition() + positionOffset;

			qsf::logic::TargetPoint moveTargetConfig;
			moveTargetConfig.mPosition = finalPosition;
			actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
		}

		// End pulling
		actionComponent.pushAction<PullPersonAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mRescuedPerson.getSafe().getId(), PullPersonAction::STOP, true);

		{ // Move back to rescue cutter
			qsf::logic::TargetPoint moveTargetConfig;
			moveTargetConfig.mPosition = EntityHelper(mCompressorPumpEntity.getSafe()).getPosition();
			actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
		}
	}

	void CutRoadVehicleAction::pushFallbackAnimationsAndPlaceRescuedPerson() const
	{
		// Push fallback animations
		AnimationHelper animationHelper(getEntity());
		animationHelper.playAnimation(animationHelper.getAnimationUseObject());

		getComponent().pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(qsf::Time::fromSeconds(2.0f));

		// Place rescued person to final position
		static const float MAXPULLDISTANCE = 2.0f;
		qsf::TransformComponent& transformComponent = mRescuedPerson->getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3 positionOffset = transformComponent.getRotation() * -qsf::CoordinateSystem::getIn() * MAXPULLDISTANCE;
		const glm::vec3 finalPosition = transformComponent.getPosition() + positionOffset;
		transformComponent.setPosition(finalPosition);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
