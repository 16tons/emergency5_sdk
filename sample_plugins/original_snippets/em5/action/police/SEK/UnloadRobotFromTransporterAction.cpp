// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SEK/UnloadRobotFromTransporterAction.h"
#include "em5/action/police/SEK/WaitForRobotAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/logic/local/police/SEKRobotLogic.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/message/MessageSystem.h>
#include <qsf/math/Math.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementations                                ]
	//[-------------------------------------------------------]
	namespace detail
	{
		qsf::Entity* getFirstLinkEntity(VehicleComponent* vehicleComponent)
		{
			if (nullptr == vehicleComponent)
				return nullptr;

			boost::container::flat_map<qsf::LinkAnchorId, qsf::ContainerLink*>::const_iterator iterator = vehicleComponent->getLinks().begin();
			if (iterator == vehicleComponent->getLinks().end())
				return nullptr;

			return &((*iterator).second->getTargetEntity());
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UnloadRobotFromTransporterAction::ACTION_ID = "em5::UnloadRobotFromTransporterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadRobotFromTransporterAction::UnloadRobotFromTransporterAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mDoorType(DoorComponent::DOORTYPE_TRUNK)
	{
		// Nothing here
	}

	UnloadRobotFromTransporterAction::~UnloadRobotFromTransporterAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UnloadRobotFromTransporterAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serializeAs<uint16>(mDoorType);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool UnloadRobotFromTransporterAction::onStartup()
	{
		// Check if transport car is currently loaded with robot
		const VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
		return (nullptr != vehicleComponent) ? !vehicleComponent->getLinks().empty() : false;
	}

	qsf::action::Result UnloadRobotFromTransporterAction::updateAction(const qsf::Clock&)
	{
		// Get the most important objects first
		VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Open the door
				vehicleComponent->openDoor(mDoorType, true);

				// Play ramp sound effect
				AudioProxy audioProxy;
				AudioHelper::playSEKRoboterRamp(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_OPEN_DOOR;
			}

			case STATE_OPEN_DOOR:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_OPEN))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_EXIT;
				// No break here by intent
			}

			case STATE_EXIT:
			{
				// Exit the robot inside the vehicle now
				qsf::Entity* sekRobotEntity = exitAndPlaceRobot(*vehicleComponent);
				if (nullptr != sekRobotEntity)
				{
					// Deselect transporter, and select robot
					EM5_GAME.getSelectionManager().removeIdFromSelection(getEntityId());
					EM5_GAME.getSelectionManager().addIdToSelection(sekRobotEntity->getId());
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));

					// Push waiting action
					getEntity().getComponentSafe<qsf::ActionComponent>().pushAction<WaitForRobotAction>(action::Priority::BLOCKING).init(sekRobotEntity->getId());

					// Start fade effect
					sekRobotEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
				}

				// Close the door again
				vehicleComponent->openDoor(mDoorType, false);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_CLOSE_DOOR;
			}

			case STATE_CLOSE_DOOR:
			{
				if (!vehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSED))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				// Done
				return qsf::action::RESULT_DONE;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* UnloadRobotFromTransporterAction::exitAndPlaceRobot(VehicleComponent& vehicleComponent)
	{
		const qsf::Map& map = getMap();

		// Get the door position
		// TODO(db): Use a target point here
		glm::vec3 placePosition = EntityHelper(getEntity()).getPosition();	// Only as fallback
		glm::quat placeRotation = qsf::Math::GLM_QUAT_IDENTITY;
		{
			auto driverDoorIds = vehicleComponent.getVehicleDoorIdsByType(mDoorType);
			if (!driverDoorIds.empty())
			{
				const qsf::Entity* doorEntity = map.getEntityById(driverDoorIds[0]);
				if (nullptr != doorEntity)
				{
					const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
					if (nullptr != doorComponent)
					{
						placePosition = doorComponent->getDoorOffsetWorldSpacePosition();
						placeRotation = doorComponent->getDoorRotationWorldSpaceRotation();

						// For leaving the vehicle, we rotate at 180°
						placeRotation = placeRotation * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());
					}
				}
			}
		}

		// Get the sek robot entity
		qsf::Entity* sekRobotEntity = detail::getFirstLinkEntity(&vehicleComponent);
		if (nullptr != sekRobotEntity)
		{
			// Remove the robot from the container
			EntityHelper(*sekRobotEntity).leaveContainer();

			// TODO(db) Add find free place function here
			{
				qsf::MovableComponent* robotMovableComponent = sekRobotEntity->getComponent<qsf::MovableComponent>();
				QSF_ASSERT(nullptr != robotMovableComponent, "The sek robot has no movable component", QSF_REACT_NONE);
				if (nullptr != robotMovableComponent)
				{
					// Robot exit vehicle
					robotMovableComponent->warpToPositionAndRotation(placePosition, placeRotation);
				}
			}

			// Add logic to remember the connected transporter
			SEKRobotLogic& sekRobotLogic = sekRobotEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>().getOrCreateGameLogicSafe<SEKRobotLogic>();
			sekRobotLogic.setConnectedTransporterId(getEntityId());

			// We have to force startup again to set the messages with the right parameters
			sekRobotLogic.startup();
		}

		return sekRobotEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
