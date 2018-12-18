// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/LiftUpByCraneAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/tech/LiftUpByCraneCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{

		bool isPositionInBoundingBox(const glm::vec3& position, const glm::vec3& min, const glm::vec3& max)
		{
			return (position.x >= min.x && position.x <= max.x && position.y >= min.y && position.y <= max.y && position.z >= min.z && position.z <= max.z);
		}

		glm::vec3 getCraneHookTargetPosition(qsf::TransformComponent& targetTransformComponent)
		{
			qsf::Entity& targetEntity = targetTransformComponent.getEntity();

			// Check if there is a special "crane_hook_target" child defining exactly where to hook the object
			qsf::Entity* craneTargetEntity = nullptr;
			{
				qsf::LinkComponent* linkComponent = targetEntity.getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
					{
						if (EntityHelper(childLinkComponent->getEntity()).getName() == "crane_hook_target")
						{
							craneTargetEntity = &childLinkComponent->getEntity();
							break;
						}
					}
				}
			}

			if (nullptr == craneTargetEntity)
			{
				// Use pivot and bounding box to define a good position where to place the hook
				glm::vec3 min(0.0f, 0.0f, 0.0f);
				glm::vec3 max(0.0f, 0.0f, 0.0f);
				qsf::EntityHelper(targetEntity).getAxisAlignedBoundingBox(min, max);

				glm::vec3 targetPosition = targetTransformComponent.getPosition();
				if (isPositionInBoundingBox(targetPosition, min - 0.5f, max + 0.5f))
				{
					targetPosition.y = max.y;		// Using bounding box
				}
				else
				{
					// The bounding box doesn't surround the entity position -> using a fallback
					targetPosition.y += 1.0f;
				}
				return targetPosition;
			}
			else
			{
				return EntityHelper(*craneTargetEntity).getPosition();
			}
		}

	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier LiftUpByCraneAction::ACTION_ID = "em5::LiftUpByCraneAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LiftUpByCraneAction::LiftUpByCraneAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mObjectHeight(0.0f),
		mCraneHeight(0.0f),
		mInWater(false),
		mTargetPlaced(false),
		mWatersprayEntity(nullptr),
		mWaterdropsEntity(nullptr)
	{
		// Nothing here
	}

	LiftUpByCraneAction::~LiftUpByCraneAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void LiftUpByCraneAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the action starts, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 LiftUpByCraneAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void LiftUpByCraneAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool LiftUpByCraneAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);

		if (nullptr == targetEntity)
			return false;

		// Check distance
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, getEntity());
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getHeavyCraneDistance();
		if (currentDistance >= maxDistance)
			return false;

		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			qsf::Entity* buoyEntity = getMap().getEntityById(linkComponent->getParentId());
			if (buoyEntity != nullptr)
				targetEntity = buoyEntity;
		}

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkContext" of the commando for validation
		LiftUpByCraneCommand* liftUpByCraneCommand = static_cast<LiftUpByCraneCommand*>(QSFGAME_COMMAND.getCommandManager().getById<LiftUpByCraneCommand>(LiftUpByCraneCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != liftUpByCraneCommand, "LiftUpByCraneAction::onStartup(): Could not find place lift up by crane command instance", return false);
		if (!liftUpByCraneCommand->checkContext(commandContext))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, callerEntity))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result LiftUpByCraneAction::updateAction(const qsf::Clock&)
	{
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		if (nullptr == vehicleCraneComponent)
			return qsf::action::RESULT_DONE;

		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity && !mTargetPlaced)
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				// Make vehicle invincible
				EntityHelper(getEntity()).setInvincible(true);

				// Change reserve logic from "moveto" to "towcar" (has higher priority)
				ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
				ReserveLogic::createReservation<ReserveHeavyCraneLogic>(*targetEntity, getEntityId());

				// Calculate target position
				qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& vehicleTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

				{ // TODO(sw) Only for test -> make the crane only be rotated around the y-axis
					glm::vec3 eulerAngles = qsf::EulerAngles::quaternionToEuler(vehicleTransformComponent.getRotation());
					eulerAngles.y = 0.0f;
					eulerAngles.z = 0.0f;
					vehicleTransformComponent.setRotation(qsf::EulerAngles::eulerToQuaternion(eulerAngles));
				}

				mTargetPosition = getCraneHookTargetPosition(targetTransformComponent);

				mInWater = CollisionHelper(getMap()).isInWater(mTargetPosition);
				mObjectHeight = mTargetPosition.y - targetTransformComponent.getPosition().y;
				mCraneHeight = std::max(vehicleTransformComponent.getPosition().y + 10.0f, mTargetPosition.y + 5.0f);

				if (nullptr != supportLegsComponent)
				{
					supportLegsComponent->startExtending();
				}

				mState = STATE_SUPPORTLEGS_EXPAND;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SUPPORTLEGS_EXPAND:
			{
				if (nullptr == supportLegsComponent || !supportLegsComponent->isAnimationPlaying())
				{
					glm::vec3 cranePosition = mTargetPosition;
					cranePosition.y = mCraneHeight;
					vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_LIFT_BY_CRANE_ALIGN_ARM);
					vehicleCraneComponent->alignArm(cranePosition);
					mState = STATE_CRANE_ALIGN_ARM;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_ALIGN_ARM:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->hookMove(mTargetPosition, 1.0f);
					mState = STATE_CRANE_MOVE_DOWN_HOOK;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_DOWN_HOOK:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->hookMove(mTargetPosition, 0.98f);
					mState = STATE_CRANE_MOVE_SHAKE_UP_HOOK;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_SHAKE_UP_HOOK:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->hookMove(mTargetPosition, 1.0f);
					mState = STATE_CRANE_MOVE_SHAKE_DOWN_HOOK;
				}
			}

			case STATE_CRANE_MOVE_SHAKE_DOWN_HOOK:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					if (findDisposePosition(*targetEntity, 10))
					{
						if (mInWater)
						{
							mWatersprayEntity = getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERSPRAY);
							mWaterdropsEntity = getMap().createObjectByLocalPrefabAssetId(assets::PARTICLE_WATERDROPS);

							linkParticles(*targetEntity, mWatersprayEntity, 0.0f, 3.0f);
							linkParticles(*targetEntity, mWaterdropsEntity, 3.0f, 10.0f);

							glm::vec3 watersurfacePosition = mTargetPosition;
							qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_LIQUIDS_ONLY).getHeightAt(watersurfacePosition, watersurfacePosition.y);
							MapHelper(getMap()).spawnParticle(assets::PARTICLE_WATERSPRAY2, 3.0f, watersurfacePosition);
							deleteBuoy(*targetEntity);
						}

						vehicleCraneComponent->linkTargetToHook(*targetEntity);
						unhideBuriedEntities(*targetEntity);
						vehicleCraneComponent->hookMove(mTargetPosition, 0.3f);

						EntityHelper::setupEntityAsGhost(*targetEntity, getEntity());
						mState = STATE_CRANE_MOVE_UP_HOOK_AND_ENTITY;
					}
					else
					{
						unhideBuriedEntities(*targetEntity);
						mState = STATE_CRANE_MOVE_UP_HOOK;
					}

					if (nullptr != targetEntity->getComponent<BuryComponent>())
					{
						// Remove status icon now already, just in case there is one at all
						StatusOverlayComponent::hideIcon(*targetEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE);
					}

					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_LIFT_ENTITY_WITH_CRANE, mTargetEntityId, getEntityId()));
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_UP_HOOK_AND_ENTITY:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					// Move to Dispose Position
					glm::vec3 cranePosition = mDisposePosition;
					cranePosition.y = mCraneHeight;
					vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_NONE);
					vehicleCraneComponent->alignArm(cranePosition);
					mState = STATE_CRANE_ALIGN_TO_DISPOSE;

				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_ALIGN_TO_DISPOSE:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->hookMove(mDisposePosition, 1.0f);
					mState = STATE_CRANE_MOVE_DOWN_HOOK_AND_ENTITY;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_DOWN_HOOK_AND_ENTITY:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					if (mInWater)
					{
						unlinkParticles(*targetEntity);
					}
					vehicleCraneComponent->unlinkTargetFromHook();

					vehicleCraneComponent->hookMove(mDisposePosition, 0.98f);
					mState = STATE_CRANE_MOVE_SHAKE_2_UP_HOOK;

					mTargetPlaced = true;
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PLACE_ENTITY_FROM_CRANE_ACTION, mTargetEntityId, getEntityId()));
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_SHAKE_2_UP_HOOK:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->hookMove(mDisposePosition, 1.0f);
					mState = STATE_CRANE_MOVE_SHAKE_2_DOWN_HOOK;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_SHAKE_2_DOWN_HOOK:
			{
				if (vehicleCraneComponent->isAnimationPlaying())
				{
					// Wait for finish animation
					return qsf::action::RESULT_CONTINUE;
				}

				if (nullptr != targetEntity)
				{
					finishTargetEntity(*targetEntity);
				}
				vehicleCraneComponent->hookMoveBack();
				mState = STATE_CRANE_MOVE_UP_HOOK;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_MOVE_UP_HOOK:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					if (canReachNextTargetFromSamePosition())
					{
						// Restart action with new target, skip flooding and hide/show support legs and take new target from current position
						mState = STATE_INIT;
						return qsf::action::RESULT_CONTINUE;
					}

					vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_LIFT_BY_CRANE_FOLD_ARM);
					vehicleCraneComponent->foldArm();

					mState = STATE_CRANE_FOLD;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CRANE_FOLD:
			{
				if (!vehicleCraneComponent->isAnimationPlaying())
				{
					if (nullptr != supportLegsComponent)
					{
						supportLegsComponent->hideExtending();
					}
					mState = STATE_SUPPORTLEGS_HIDE;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SUPPORTLEGS_HIDE:
			{
				if (nullptr == supportLegsComponent || !supportLegsComponent->isAnimationPlaying())
				{
					vehicleCraneComponent->stopAllAnimation();
					mState = STATE_DONE;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				// Let entity receive damage again
				EntityHelper(getEntity()).setInvincible(false);
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void LiftUpByCraneAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void LiftUpByCraneAction::unhideBuriedEntities(qsf::Entity& targetEntity)
	{
		BuryComponent* buryComponent = targetEntity.getComponent<BuryComponent>();
		if (nullptr != buryComponent)
		{
			buryComponent->unlinkAndShowBuriedEntities();
		}
	}

	void LiftUpByCraneAction::finishTargetEntity(qsf::Entity& targetEntity)
	{
		BuryComponent* buryComponent = targetEntity.getComponent<BuryComponent>();
		if (nullptr != buryComponent)
		{
			targetEntity.destroyComponent<BuryComponent>();

			// If target is burning, at least now we should stop the fire
			EntityHelper(targetEntity).stopFire();

			// Add a fade out effect on the target
			qsf::ActionComponent* actionComponent = targetEntity.getOrCreateComponent<qsf::ActionComponent>();
			if (nullptr != actionComponent)
			{
				qsf::MeshComponent* meshComponent = targetEntity.getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					meshComponent->setCastShadows(false);
				}
				actionComponent->pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(2.0f));
			}
		}
		else
		{
			VehicleComponent* vehicleComponent = targetEntity.getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				vehicleComponent->setMustBeRecovered(false);
			}

			EntityHelper::teardownEntityAsGhost(targetEntity);
		}
	}

	void LiftUpByCraneAction::deleteBuoy(qsf::Entity& targetEntity)
	{
		BuryComponent* buryComponent = targetEntity.getComponent<BuryComponent>();
		if (nullptr != buryComponent)
			return;

		qsf::LinkComponent* linkComponent = targetEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		qsf::Entity* buoyEntity = getMap().getEntityById(linkComponent->getParentId());

		if (buoyEntity != nullptr)
		{
			linkComponent->setParentId(qsf::getUninitialized<uint64>());

			// Add a fade out effect on the target
			qsf::ActionComponent& actionComponent = buoyEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(1.0f));
		}

		targetEntity.setActive(true);
		targetEntity.setHidden(false);
	}

	bool LiftUpByCraneAction::findDisposePosition(qsf::Entity& targetEntity, uint32 numberOfChecks)
	{
		qsf::TransformComponent& vehicleTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& targetTransformComponent = targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

		for (uint32 i = 0; i < numberOfChecks; ++i)
		{
			// If numberOfChecks == 1, we have a division by 0, so give a small offset
			float angle = glm::half_pi<float>() + static_cast<float>(i) * glm::pi<float>() / (static_cast<float>(numberOfChecks - 1) + 0.001f);

			const glm::quat& pivotRotation = qsf::EulerAngles::eulerToQuaternion(angle, 0.0f, 0.0f);
			const glm::vec3& vehiclePosition = vehicleTransformComponent.getPosition();

			// Rotate transform around the vehicleEntity pivot
			glm::vec3 position = targetTransformComponent.getPosition();
			glm::quat rotation = targetTransformComponent.getRotation();

			position = ((position - vehiclePosition) * pivotRotation) + vehiclePosition;
			rotation = rotation * pivotRotation;

			// Update with the help of the ground map and the Height of the object before
			qsf::GroundMapQuery(getEntity().getMap()).getHeightAtXZPosition(position.x, position.z, position.y);

			CollisionHelper collisionHelper(getMap());
			// Hack to prevent dropping it to the edge of water
			const float checkDistance = 3.0f;
			bool isInWater = collisionHelper.isInWater(position)
						|| collisionHelper.isInWater(position + glm::vec3( checkDistance, 0.0f, 0.0f))
						|| collisionHelper.isInWater(position + glm::vec3( -checkDistance, 0.0f, 0.0f))
						|| collisionHelper.isInWater(position + glm::vec3( 0.0f, 0.0f, checkDistance))
						|| collisionHelper.isInWater(position + glm::vec3( 0.0f, 0.0f, -checkDistance));

			if (collisionHelper.findFreePositionEM3(targetEntity, qsf::Transform(position, rotation), 3.0f, 0.2f, position)	&& !isInWater)
			{
				// Adjust the position of the hook (which is the dispose position) with the height of the lifted object
				position.y += mObjectHeight;

				// TODO(tl): Reserve this position so that no one can move there
				mDisposePosition = position;
				return true;
			}
		}
		mDisposePosition = targetTransformComponent.getPosition();
		return false;
	}

	void LiftUpByCraneAction::linkParticles(qsf::Entity& targetEntity, qsf::Entity* particleEntity, float delay, float timeout)
	{
		if (nullptr != particleEntity)
		{
			qsf::ParticlesComponent& particlesComponent = particleEntity->getOrCreateComponentSafe<qsf::ParticlesComponent>();
			qsf::LinkComponent& linkComponent = particleEntity->getOrCreateComponentSafe<qsf::LinkComponent>();

			//TODO(tl): Delay Time doesn't work
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

	void LiftUpByCraneAction::unlinkParticles(qsf::Entity& targetEntity)
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

	bool LiftUpByCraneAction::canReachNextTargetFromSamePosition()
	{
		qsf::ActionComponent& actionComponent = getComponent();

		// Check the action plan if we have the situation of a reachable target at next.
		const qsf::ActionPlan& actionPlan = actionComponent.getPlan();
		if (actionPlan.getActions().size() < 3)
			return false;

		if (actionPlan.getActions()[1]->getTypeId() != MoveAction::ACTION_ID)
			return false;
		MoveAction* nextMoveAction = static_cast<MoveAction*>(actionPlan.getActions()[1]);

		if (actionPlan.getActions()[2]->getTypeId() != LiftUpByCraneAction::ACTION_ID)
			return false;
		LiftUpByCraneAction* nextLiftUpAction = static_cast<LiftUpByCraneAction*>(actionPlan.getActions()[2]);

		if (nullptr == nextMoveAction || nullptr == nextLiftUpAction)
			return false;

		// The onStartup() needs a the targetEntity.
		uint64 backupTargetEntityId = mTargetEntityId;
		mTargetEntityId = nextLiftUpAction->getTargetId();

		if (!onStartup())
		{
			// Target is invalid, use backup and go back without changes
			mTargetEntityId = backupTargetEntityId;
			return false;
		}

		// Remove the next actions, we skip the move action (we are already in position) and taken the target information of the next action
		actionComponent.removeAction(*nextMoveAction);
		actionComponent.removeAction(*nextLiftUpAction);

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
