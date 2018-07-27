// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/RepairOrUseAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/tech/RepairVehicleCommand.h"
#include "em5/command/tech/UseWithEngineerCommand.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/game/targetpoint/GotoObjectFrontsideTargetPointProvider.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	// TODO(mz): trouble in art assets (animations expected toolbox to stand and not lie) so opening/closing box for now not used
	bool DO_OPEN_CLOSE_TOOLBOX = false;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RepairOrUseAction::ACTION_ID = "em5::RepairOrUseAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RepairOrUseAction::RepairOrUseAction() :
		Action(ACTION_ID),
		mPosture(STANDING),
		mTarget(VEHICLE),
		mState(INIT),
		mToolboxPlaced(false),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mTimeUntilObjectIsUsed(qsf::Time::ZERO),
		mTimeUntilNextRepairStep(qsf::Time::ZERO),
		mTimeUntilRepairSteps(qsf::Time::fromSeconds(0.5f)),
		mAmountDamagePerRepairStep(0.01f) // 2% per second (with mTimeUntilRepairSteps at 0.5)
	{
		// Nothing here
	}

	RepairOrUseAction::~RepairOrUseAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());

		if (mToolboxPlaced)
		{
			takeToolbox();
		}

		// Ensure progress bar is hidden
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			StatusOverlayComponent::hideBarOverlay(*targetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);
			BuildingOverlayComponent::hideOverlay(*targetEntity, getEntity());
		}
	}

	void RepairOrUseAction::init(const qsf::Entity& targetEntity, Posture posture, Target target)
	{
		mTargetEntityId = targetEntity.getId();
		mPosture = posture;
		mTarget = target;
	}

	uint64 RepairOrUseAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RepairOrUseAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mToolboxPlaced);
		serializer.serialize(mTimeUntilNextRepairStep);
		serializer.serialize(mTimeUntilObjectIsUsed);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RepairOrUseAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Find the distance to the target
		float currentDistance = FLT_MAX;
		std::vector<qsf::logic::TargetPoint> targetPoints;
		if (mTarget == BUILDING)
		{
			EM5_GAME.getTargetPointManager().getTargetPoints(GetIntoBuildingTargetPointProvider::TARGET_POINT_ID, callerEntity, *targetEntity, targetPoints);
		}
		else if (mTarget == OBJECT)
		{
			EM5_GAME.getTargetPointManager().getTargetPoints(GotoObjectFrontsideTargetPointProvider::TARGET_POINT_ID, callerEntity, *targetEntity, targetPoints);
		}
		else
		{
			EM5_GAME.getTargetPointManager().getTargetPoints(OnTouchTargetPointProvider::TARGET_POINT_ID, callerEntity, *targetEntity, targetPoints);
		}
		if (targetPoints.empty())
			return false;

		qsf::TransformComponent& callerTransformComponent = callerEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3& callerPos = callerTransformComponent.getPosition();
		for (qsf::logic::TargetPoint targetPoint : targetPoints)
		{
			const float distance = qsf::game::DistanceHelper::getDistance(targetPoint.mPosition, callerPos);
			if (distance < currentDistance)
				currentDistance = distance;
		}

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getRepairObjectDistance();
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		if (mTarget == VEHICLE)
		{
			RepairVehicleCommand* command = static_cast<RepairVehicleCommand*>(QSFGAME_COMMAND.getCommandManager().getById<RepairVehicleCommand>(RepairVehicleCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != command, "Could not find the 'RepairVehicleCommand' instance", return false);
			if (!(command->checkCaller(*commandContext.mCaller) && command->checkContext(commandContext)))
				return false;
		}
		else
		{
			UseWithEngineerCommand* command = static_cast<UseWithEngineerCommand*>(QSFGAME_COMMAND.getCommandManager().getById<UseWithEngineerCommand>(UseWithEngineerCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != command, "Could not find the 'UseWithEngineerCommand' instance", return false);
			if (!(command->checkCaller(*commandContext.mCaller) && command->checkContext(commandContext)))
				return false;

			// Set time it should take
			UsableByEngineerComponent* usableByEngineerComponent = targetEntity->getComponent<UsableByEngineerComponent>();
			if (nullptr != usableByEngineerComponent)
			{
				mTimeUntilObjectIsUsed = usableByEngineerComponent->getUseTime();
				usableByEngineerComponent->setIsInUse(true);
			}
		}

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		ReserveLogic::createReservation<ReserveRepairOrUseLogic>(*targetEntity, getEntityId());

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, callerEntity))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result RepairOrUseAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case INIT:
			{
				if (mTarget == BUILDING)
				{
					// Different handling for buildings, the technician enters the building
					// Display only a progress bar, skip the bag relevant stuff
					mState = START_REPAIRING;
					return qsf::action::RESULT_CONTINUE;
				}

				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerGetDown(), true);
				getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);

				mState = STATE_GET_DOWN;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_GET_DOWN:
			{
				placeToolbox();

				if (DO_OPEN_CLOSE_TOOLBOX)
				{
					openToolbox();

					// Play audio
					AudioProxy audioProxy;
					AudioHelper::playDoctorOpenCase(audioProxy, getEntity());
					audioProxy.detach();
					audioProxy.synchronizeWithMultiplay();
				}

				mState = STATE_OPEN_TOOLBOX;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_OPEN_TOOLBOX:
			{
				if (DO_OPEN_CLOSE_TOOLBOX && (AnimationHelper(getEntity()).isAnimationPlaying() || (mToolboxEntity.valid() && AnimationHelper(*mToolboxEntity).isAnimationPlaying())))
				{
					// Wait until animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				if (mPosture == STANDING)
				{
					getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerGetDown(), true, false, true);
				}

				mState = START_REPAIRING;
				return qsf::action::RESULT_CONTINUE;
			}

			case START_REPAIRING:
			{
				AnimationHelper animationHelper(getEntity());
				if (mPosture == INBULIDING || mTarget == BUILDING)
				{
					// TODO(fw): We should better fade the engineer out, and then let him enter the building in another state
					EntityHelper(getEntity()).fadeOut();

					qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
					QSF_CHECK(nullptr != targetEntity, "RepairOrUseAction: Invalid target building " << mTargetEntityId,
						return qsf::action::RESULT_CLEARLIST);

					EntityHelper(getEntity()).enterContainer(*targetEntity, container::CONTAINERTYPE_PROGRESS);
				}
				else if (mPosture == STANDING)
				{
					animationHelper.playAnimation(animationHelper.getAnimationEngineerRepairStandingLoop(), true, false, qsf::Time::fromSeconds(0.4f));
				}
				else if (mPosture == KNEELING)
				{
					animationHelper.playAnimation(animationHelper.getAnimationEngineerRepairSittingLoop(), true, false, qsf::Time::fromSeconds(0.4f));
				}

				if (mTarget == VEHICLE)
				{
					mState = REPAIRING;
				}
				else if (mTarget == BUILDING)
				{
					mState = REPARING_BUILDING;
				}
				else
				{
					mState = USING_OBJECT;
				}

				// Play audio
				if (mTarget == BUILDING)
				{
					AudioHelper::playEngineerReparingBuildingSound(mAudioProxy, getEntity());
				}
				else
				{
					AudioHelper::playEngineer(mAudioProxy, getEntity());
				}
				mAudioProxy.synchronizeWithMultiplay();

				return qsf::action::RESULT_CONTINUE;
			}

			case REPAIRING:
			{
				// Get the entity of the vehicle
				qsf::Entity* vehicleEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr != vehicleEntity)
				{
					// Get the road vehicle component
					RoadVehicleComponent* roadVehicle = vehicleEntity->getComponent<RoadVehicleComponent>();
					if (nullptr != roadVehicle)
					{
						if (roadVehicle->canBeRepaired())
						{
							updateRepairVehicle(clock.getTimePassed(), *roadVehicle, *vehicleEntity);

							return qsf::action::RESULT_CONTINUE;
						}

						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, vehicleEntity->getId(), getEntityId()));
					}

					// Handle icons
					StatusOverlayComponent::hideIcon(*vehicleEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE);

					// Stop audio
					mAudioProxy.stop();
					mAudioProxy.synchronizeWithMultiplay();

					StatusOverlayComponent::hideBarOverlay(*vehicleEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);
				}

				if (mPosture == STANDING)
				{
					getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerGetDown(), true, false, false, qsf::Time::fromSeconds(0.4f));
				}

				mState = STATE_CLOSE_TOOLBOX;
				return qsf::action::RESULT_CONTINUE;
			}

			case REPARING_BUILDING:
			{
				// Get the target entity
				qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr != targetEntity)
				{
					UsableByEngineerComponent* usableByEngineerComponent = targetEntity->getComponent<UsableByEngineerComponent>();
					if (nullptr != usableByEngineerComponent)
					{
						// Repair
						mTimeUntilObjectIsUsed -= clock.getTimePassed();
						if (mTimeUntilObjectIsUsed > qsf::Time::ZERO)
						{
							BuildingOverlayComponent::showProgress(*targetEntity, 1, getEntity(), 1.0f - (mTimeUntilObjectIsUsed / usableByEngineerComponent->getUseTime()));
							return qsf::action::RESULT_CONTINUE;
						}

						// Finished repairing
						usableByEngineerComponent->setUsed(true);
						usableByEngineerComponent->setIsInUse(false);
					}

					BuildingOverlayComponent::hideOverlay(*targetEntity, getEntity());

					// Show the engineer again
					EntityHelper(getEntity()).leaveContainer();
					EntityHelper(getEntity()).fadeIn();

					// Emit message for gameplay
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, mTargetEntityId, getEntityId()));
				}

				// Here skip the last states, just finish
				return qsf::action::RESULT_DONE;
			}

			case USING_OBJECT:
			{
				// Get the target entity
				qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr != targetEntity)
				{
					UsableByEngineerComponent* usableByEngineerComponent = targetEntity->getComponent<UsableByEngineerComponent>();
					if (nullptr != usableByEngineerComponent)
					{
						mTimeUntilObjectIsUsed -= clock.getTimePassed();
						if (mTimeUntilObjectIsUsed > qsf::Time::ZERO)
						{
							StatusOverlayComponent::showBarOverlay(*targetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS, 1.0f - (mTimeUntilObjectIsUsed / usableByEngineerComponent->getUseTime()));

							return qsf::action::RESULT_CONTINUE;
						}
						else
						{
							usableByEngineerComponent->setUsed(true);
							usableByEngineerComponent->setIsInUse(false);

							EboxComponent* eboxComponent = targetEntity->getComponent<EboxComponent>();
							if (eboxComponent != nullptr)
							{
								eboxComponent->setEboxIntact();
							}

							QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, targetEntity->getId(), getEntityId()));
						}
					}

					// Stop audio
					mAudioProxy.stop();
					mAudioProxy.synchronizeWithMultiplay();

					StatusOverlayComponent::hideBarOverlay(*targetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);
				}

				if (mPosture == STANDING)
				{
					getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerGetDown(), true, false, false, qsf::Time::fromSeconds(0.4f));
				}
				else if (!DO_OPEN_CLOSE_TOOLBOX)
				{
					// Just for transitioning the hand back to the toolbox
					getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerOpenToolbox(), true, false, false, qsf::Time::fromSeconds(0.3f), qsf::Time::fromSeconds(0.3f));
				}

				mState = STATE_CLOSE_TOOLBOX;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_TOOLBOX:
			{
				if (DO_OPEN_CLOSE_TOOLBOX)
					closeToolbox();

				qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
				if (nullptr != targetEntity)
				{
					StatusOverlayComponent::hideIcon(*targetEntity, StatusOverlayComponent::STATUS_ICON_DEFECT);
					StatusOverlayComponent::hideIcon(*targetEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE);
				}

				mAudioProxy.stop();		// just to be sure
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_GET_UP;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_GET_UP:
			{
				if (DO_OPEN_CLOSE_TOOLBOX && (AnimationHelper(getEntity()).isAnimationPlaying() || (mToolboxEntity.valid() && AnimationHelper(*mToolboxEntity).isAnimationPlaying())))
				{
					// Play audio
					AudioProxy audioProxy;
					AudioHelper::playDoctorOpenCase(audioProxy, getEntity());
					audioProxy.detach();
					audioProxy.synchronizeWithMultiplay();

					// Wait until animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				takeToolbox();

				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerGetDown(), true, false, true, qsf::Time::fromSeconds(0.4f));
			// no break;
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void RepairOrUseAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());

		// Get the target entity
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return;

		// Reset the is in use flag
		UsableByEngineerComponent* usableByEngineerComponent = targetEntity->getComponent<UsableByEngineerComponent>();
		if (nullptr != usableByEngineerComponent)
		{
			usableByEngineerComponent->setIsInUse(false);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RepairOrUseAction::updateRepairVehicle(const qsf::Time& timePassed, RoadVehicleComponent& roadVehicleComponent, qsf::Entity& vehicleEntity)
	{
		mTimeUntilNextRepairStep -= timePassed;
		if (mTimeUntilNextRepairStep <= qsf::Time::ZERO)
		{
			mTimeUntilNextRepairStep = mTimeUntilRepairSteps;
			roadVehicleComponent.applyDamage(-mAmountDamagePerRepairStep, nullptr);
		}

		StatusOverlayComponent::showBarOverlay(vehicleEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS, roadVehicleComponent.getLifeEnergy());
	}

	void RepairOrUseAction::placeToolbox()
	{
		EntityHelper entityHelper(getEntity());
		if (entityHelper.checkIsActiveEquipment(equipment::TECH_TOOLBOX))
		{
			qsf::game::EquipmentComponent* equipmentComponent = entityHelper.getActiveEquipmentComponent();
			if (nullptr != equipmentComponent)
			{
				mToolboxEntity = equipmentComponent->getEntity();
				if (mToolboxEntity.valid())
				{
					mToolboxPlaced = true;

					// To be sure, delete both kinds of possible links
					mToolboxEntity->destroyComponent<qsf::game::BoneToBoneLinkComponent>();
					mToolboxEntity->destroyComponent<qsf::game::BoneLinkComponent>();

					// Destroy parent link
					qsf::LinkComponent* linkComponent = mToolboxEntity->getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						linkComponent->setParentId(qsf::getUninitialized<uint64>());
					}
				}
			}
		}
	}

	void RepairOrUseAction::takeToolbox()
	{
		mToolboxPlaced = false;

		if (mToolboxEntity.valid())
		{
			if (DO_OPEN_CLOSE_TOOLBOX)
			{
				// Now we have to change the meshes back again
				qsf::MeshComponent* meshComponent = mToolboxEntity->getOrCreateComponent<qsf::MeshComponent>();
				meshComponent->setMesh(qsf::AssetProxy(equipment::MESH_TECH_TOOLBOX_CLOSED.getHash()));
			}

			InventoryComponent& inventoryComponent = getEntity().getOrCreateComponentSafe<InventoryComponent>();
			inventoryComponent.setActiveEquipmentId(mToolboxEntity->getId());

			EntityHelper(mToolboxEntity.getSafe()).linkEquipmentToEntity(getEntity());
		}

		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	void RepairOrUseAction::openToolbox() const
	{
		// Push open toolbox animation to the unit

		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerOpenToolbox(), true);

		// Open the toolbox via animation for the equipment
		if (!mToolboxEntity.valid())
			return;

		// Now we have to change the closed toolbox with the open toolbox
		// Switch the current mesh with the open variant
		qsf::MeshComponent* meshComponent = mToolboxEntity->getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			meshComponent->setMesh(qsf::AssetProxy(equipment::MESH_TECH_TOOLBOX_OPEN.getHash()));
		}

		qsf::ActionComponent* actionComponent = mToolboxEntity->getOrCreateComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return;

		// Push open toolbox animation to the equipments
		actionComponent->pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationEquipmentOpenToolbox());
	}

	void RepairOrUseAction::closeToolbox() const
	{
		// This is the same as the openToolbox(), just all animations and processes reverted
		// Push close toolbox animation to the unit
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationEngineerOpenToolbox(), true, false, true);

		// Open the toolbox via animation for the equipment
		if (!mToolboxEntity.valid())
			return;

		qsf::ActionComponent* actionComponent = mToolboxEntity->getOrCreateComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return;

		// Push close toolbox animation to the equipments
		actionComponent->pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationEquipmentCloseToolbox());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
