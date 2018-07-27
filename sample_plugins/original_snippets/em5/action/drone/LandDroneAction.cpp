// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/drone/LandDroneAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier LandDroneAction::ACTION_ID = "em5::LandDroneAction";

	const DoorComponent::DoorType USED_DOOR_TYPE = DoorComponent::DOORTYPE_CODRIVER;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LandDroneAction::LandDroneAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	LandDroneAction::~LandDroneAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void LandDroneAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) add serialization
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool LandDroneAction::onStartup()
	{
		mRoadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (!mRoadVehicleComponent.valid())
			return false;

		mDroneEntity = getMap().getEntityById(mRoadVehicleComponent->getSpecialEntity());
		if (!mDroneEntity.valid())
			return false;

		return true;
	}

	qsf::action::Result LandDroneAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				{
					// Call drone back to the transporter
					qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
					if (nullptr == gameLogicComponent)
						return qsf::action::RESULT_DONE;

					DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
					if (nullptr == droneTransportLogic)
						return qsf::action::RESULT_DONE;
					mLocalDroneOffset = droneTransportLogic->getLocalDronePodPosition();
					mFinalDronePosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform().vec3PositionLocalToWorld(mLocalDroneOffset);

					// Push actions on drone
					qsf::ActionComponent& actionComponent = mDroneEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.clearPlan();

					qsf::logic::TargetPoint moveTargetConfig;
					moveTargetConfig.mPosition = mFinalDronePosition;
					actionComponent.pushAction<MoveAction>(action::COMMAND_LOW).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
					actionComponent.pushAction<LandHelicopterAction>(action::BLOCKING).init(true);
				}

				// Disable owner movement
				qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(false);
				}

				mState = STATE_WAIT_FOR_START_LANDING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_START_LANDING:
			{
				if (nullptr != mDroneEntity->getOrCreateComponentSafe<qsf::ActionComponent>().getCurrentAction())
				{
					// Wait for finish the drone movement
					return qsf::action::RESULT_CONTINUE;
				}

				// Open the door, don't wait for opening the doors
				mRoadVehicleComponent->openDoor(USED_DOOR_TYPE, true);

				// Remove collision box
				qsf::ai::RouterComponent* routerComponent = mDroneEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->setActive(false);

				mState = STATE_LANDING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LANDING:
			{
				// Lower the drone
				qsf::TransformComponent& droneTransformComponent = mDroneEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 position = droneTransformComponent.getPosition();

				const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();
				float flyHeightChangeSpeed = roadVehicleSpecsGroup.getHelicopterFlyHeightSpeed() * clock.getTimePassed().getSeconds();

				const float heightDifference = position.y - mFinalDronePosition.y;
				flyHeightChangeSpeed *= (heightDifference < roadVehicleSpecsGroup.getHelicopterStartLandingHeightLower()) ? 0.4f : 0.7f;

				// Set the drone position
				position.y -= flyHeightChangeSpeed;
				if (position.y > mFinalDronePosition.y)
				{
					// Wait till the drone reaches the final position
					droneTransformComponent.setPosition(position);
					return qsf::action::RESULT_CONTINUE;
				}

				// Set helicopter on ground
				droneTransformComponent.setPosition(mFinalDronePosition);

				// Deactivate rotors
				mDroneEntity.getSafe().getOrCreateComponentSafe<HelicopterComponent>().setFlying(false);

				MinimapHelper::unregisterIcon(*mDroneEntity);

				// Close the door
				mRoadVehicleComponent->openDoor(USED_DOOR_TYPE, false);

				mState = STATE_CLOSE_DOOR;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_DOOR:
			{
				if (!mRoadVehicleComponent->areAllDoorsOfTypeInState(USED_DOOR_TYPE, DoorComponent::DOOR_CLOSED))
				{
					// Wait for closing the door
					return qsf::action::RESULT_CONTINUE;
				}

				// Link the drone to the transporter
				qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
					if (nullptr != droneTransportLogic)
					{
						droneTransportLogic->setIsDroneUnitedWithLeader(true);
					}
				}

				// Link entity
				EntityHelper(mDroneEntity.getSafe()).enterContainer(getEntity(), container::CONTAINERTYPE_SQUAD);
				mRoadVehicleComponent->setSpecialEntity(mDroneEntity->getId());

				// Really make sure the entity is on the right position
				mDroneEntity->getOrCreateComponentSafe<qsf::LinkComponent>().setLocalPosition(mLocalDroneOffset);

				// Make linked entity visible
				mDroneEntity->destroyComponent<qsf::game::HiddenComponent>();

				mState = STATE_FINISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINISH:
			{
				// Own state, because it cannot be done without conflicts in the same tick as "destroyComponent<qsf::game::HiddenComponent>()"

				// Set router component inactive again
				qsf::ai::RouterComponent* routerComponent = mDroneEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->setActive(false);

				// Enable owner movement
				qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(true);
				}

				// Everything done
				return qsf::action::RESULT_DONE;
			}
		}

		// Error, abort action
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
