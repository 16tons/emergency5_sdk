// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/drone/StartDroneAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/component/effect/FireDroneScanComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier StartDroneAction::ACTION_ID = "em5::StartDroneAction";

	const DoorComponent::DoorType USED_DOOR_TYPE = DoorComponent::DOORTYPE_CODRIVER;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartDroneAction::StartDroneAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	StartDroneAction::~StartDroneAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void StartDroneAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) add serialization
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool StartDroneAction::onStartup()
	{
		mRoadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (!mRoadVehicleComponent.valid())
			return false;

		mDroneEntity = getMap().getEntityById(mRoadVehicleComponent->getSpecialEntity());
		if (!mDroneEntity.valid())
			return false;

		return true;
	}

	qsf::action::Result StartDroneAction::updateAction(const qsf::Clock&)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				mRoadVehicleComponent->openDoor(USED_DOOR_TYPE, true);

				// Disable owner movement while the drone is in the air
				qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(false);
				}

				// Set drone commandable infos, unit and player ID
				CommandableComponent& commandableComponent = mDroneEntity->getOrCreateComponentSafe<CommandableComponent>();
				commandableComponent.setPlayerId(getEntity().getOrCreateComponentSafe<CommandableComponent>().getPlayerId());

				if (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
				{
					commandableComponent.setUnitType(CommandableComponent::UNITTAG_POLICE_DRONE);

					AudioProxy audioProxy;
					AudioHelper::playSEKDroneStart(audioProxy, getEntity());
					audioProxy.detach();
					audioProxy.synchronizeWithMultiplay();
				}

				mState = STATE_OPEN_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_OPEN_DOOR:
			{
				if (!mRoadVehicleComponent->areAllDoorsOfTypeInState(USED_DOOR_TYPE, DoorComponent::DOOR_OPEN))
				{
					// Wait for finish opening process
					return qsf::action::RESULT_CONTINUE;
				}

				// Unlink drone from vehicle (needed here already for smooth entity interpolation while the drone is moving up)
				EntityHelper(mDroneEntity.getSafe()).leaveContainer();

				// Start helicopter flying effect
				qsf::ActionComponent& actionComponent = mDroneEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<StartHelicopterAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

				mState = STATE_DRONE_STARTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DRONE_STARTING:
			{
				const qsf::ActionComponent& actionComponent = mDroneEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				const qsf::Action* currentAction = actionComponent.getCurrentAction();
				if (nullptr != currentAction && currentAction->getTypeId() == StartHelicopterAction::ACTION_ID)
				{
					// Wait till the drone starting is finished
					return qsf::action::RESULT_CONTINUE;
				}

				mRoadVehicleComponent->openDoor(USED_DOOR_TYPE, false);

				// Activate fire drone scan
				FireDroneScanComponent* fireDroneScanComponent = mDroneEntity->getComponent<FireDroneScanComponent>();
				MiniMapIconType minimapIconType = MiniMapIconType::UNIT_VEHICLE_POLICE;
				if (nullptr != fireDroneScanComponent)
				{
					fireDroneScanComponent->setActive(true);

					minimapIconType = MiniMapIconType::UNIT_VEHICLE_FIREFIGHTER;
				}

				// Create minimap icon
				MinimapHelper::registerIcon(*mDroneEntity, minimapIconType);

				// Unlink the drone from transporter
				const qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
					if (nullptr != droneTransportLogic)
					{
						droneTransportLogic->setIsDroneUnitedWithLeader(false);
					}
				}

				// Add collision box
				qsf::CollisionComponent* collisionComponent = mDroneEntity->getComponent<qsf::CollisionComponent>();
				if (nullptr != collisionComponent)
					collisionComponent->setActive(true);
				qsf::ai::RouterComponent* routerComponent = mDroneEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->setActive(true);

				mState = STATE_CLOSE_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_DOOR:
			{
				if (!mRoadVehicleComponent->areAllDoorsOfTypeInState(USED_DOOR_TYPE, DoorComponent::DOOR_CLOSED))
				{
					// Wait for finish opening process
					return qsf::action::RESULT_CONTINUE;
				}

				// Everything finished correctly, abort action
				return qsf::action::RESULT_DONE;
			}

			case STATE_DONE:
				// Nothing to do in this case
				break;
		}

		// Error, abort action
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
