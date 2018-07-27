// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/ConnectHydrantAction.h"
#include "em5/action/base/PlayAudioAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/AudioAssets.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/objects/HistoricWellComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/fire/GetHoseHistoricalCommand.h"
#include "em5/command/fire/ExtinguishWithWellCommand.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ConnectHydrantAction::ACTION_ID = "em5::ConnectHydrantAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ConnectHydrantAction::ConnectHydrantAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mCanCreateFireHose(true)
	{
		// Nothing to do in here
	}

	ConnectHydrantAction::~ConnectHydrantAction()
	{
		// Disconnect from the hydrant
		const qsf::Entity* hydrantEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (nullptr != hydrantEntity)
		{
			// Get the hydrant component
			HydrantComponent* hydrantComponent = hydrantEntity->getComponent<HydrantComponent>();
			if (nullptr != hydrantComponent)
			{
				// The hydrant is no longer reserved
				hydrantComponent->setReserved(false);
			}
		}
	}

	void ConnectHydrantAction::init(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();

		// Get the hydrant component
		HydrantComponent* hydrantComponent = entity.getComponent<HydrantComponent>();
		if (nullptr != hydrantComponent)
		{
			// The hydrant is now reserved
			hydrantComponent->setReserved(true);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ConnectHydrantAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ConnectHydrantAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		if (nullptr == targetEntity->getComponent<HydrantComponent>())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getGotoHydrantDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result ConnectHydrantAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		qsf::Entity* hydrantEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != hydrantEntity, "The EM5 connect hydrant action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Get the entity of the caller
		const qsf::Entity& callerEntity = getEntity();

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Get the game logic component
				const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					// Get the fireman logic
					FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
					if (nullptr != firemanLogic)
					{
						firemanLogic->connectToHydrant(*hydrantEntity);
					}
				}

				// Play animation
				AnimationHelper animationHelper(getEntity());
				const bool useGroundHydrant = (hydrantEntity->getOrCreateComponentSafe<HydrantComponent>().getHydrantType() == HydrantComponent::GROUND_HYDRANT);
				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationFirefighterInstallHydrant(useGroundHydrant));
				mMeshAnimationChannel->setSpeed(2.0f);		// Double speed, otherwise this is just too much waiting
				getComponent().pushAction<PlayAudioAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(audio::EM5_FIREFIGHTER_HOSE_01, hydrantEntity, true, false, false);

				handleConnectedRoadVehicle(*hydrantEntity);

				mCurrentState = STATE_CONNECT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CONNECT:
			{
				static const qsf::Time HIDE_HOSE = qsf::Time::fromSeconds(1.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= HIDE_HOSE)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				getComponent().pushAction<GetEquipmentAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init("");

				mCurrentState = STATE_CREATE_FIREHOSE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CREATE_FIREHOSE:
			{
				static const qsf::Time SHOW_NOZZLE = qsf::Time::fromSeconds(4.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= SHOW_NOZZLE)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Check what equipment is important for switching back
				std::string equipmentName = equipment::HOSE_NOZZLE;
				CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					for (auto commandId : commandableComponent->getCommandIds())
					{
						if (commandId == GetHoseHistoricalCommand::PLUGINABLE_ID ||
							commandId == ExtinguishWithWellCommand::PLUGINABLE_ID)
						{
							// Special case for historical unit
							equipmentName = equipment::HOSE_NOZZLE_HISTORICAL;
							break;
						}
					}
				}

				getComponent().pushAction<GetEquipmentAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(equipmentName);
				mCanCreateFireHose = true;

				mCurrentState = FINISH_CONNECTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case FINISH_CONNECTING:
			{
				// We have to wait till the fire man has the nozzle as equipment
				if (mCanCreateFireHose)
				{
					mCanCreateFireHose = false;

					// Get the game logic component
					const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
					if (nullptr != gameLogicComponent)
					{
						// Get the fireman logic
						FiremanLogic* firemanLogic = gameLogicComponent->getGameLogic<FiremanLogic>();
						if (nullptr != firemanLogic)
						{
							firemanLogic->createFireHose();
						}
					}

					// Handle historic well
					handleHistoricWell(*hydrantEntity);
				}

				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getOffset() < 0.99f)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}
				break;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ConnectHydrantAction::handleConnectedRoadVehicle(const qsf::Entity& hydrantEntity) const
	{
		// Check if hydrant is an vehicle type
		const qsf::LinkComponent* linkComponent = hydrantEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			RoadVehicleComponent* roadVehicleComponent = linkComponent->getComponentFromEntityOrLinkedParent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent)
			{
				// Stop the vehicle movement
				EntityHelper(roadVehicleComponent->getEntity()).makeImmovable();

				// Open the trunk door
				if (!roadVehicleComponent->areAllDoorsOfTypeInState(DoorComponent::DOORTYPE_TRUNK, DoorComponent::DOOR_OPEN))
				{
					roadVehicleComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, true);
				}
			}
		}
	}

	void ConnectHydrantAction::handleHistoricWell(const qsf::Entity& hydrantEntity) const
	{
		// Start pump animation for historic wells
		const qsf::LinkComponent* linkComponent = hydrantEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			HistoricWellComponent* historicWellComponent = linkComponent->getComponentFromEntityOrLinkedParent<HistoricWellComponent>();
			if (nullptr != historicWellComponent)
			{
				historicWellComponent->startPumpAnimation();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
