// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/action/base/UnloadPersonFromVehicleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Math.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ExitVehicleAction::ACTION_ID = "em5::ExitVehicleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitVehicleAction::ExitVehicleAction() :
		ExitVehicleAction(ACTION_ID)
	{
		// Nothing here
	}

	ExitVehicleAction::ExitVehicleAction(uint32 actionId) :
		Action(actionId),
		mCurrentState(STATE_INIT),
		mDoorType(DoorComponent::DOORTYPE_CODRIVER),
		mSelectPersonsAfterExitVehicle(false),
		mExitPersonId(qsf::getUninitialized<uint64>()),
		mUnloadUnfiltered(false)
	{
		// Nothing here
	}

	ExitVehicleAction::~ExitVehicleAction()
	{
		// Nothing here
	}

	void ExitVehicleAction::init(uint64 entityId)
	{
		mExitPersonId = entityId;
	}

	void ExitVehicleAction::initForUnloadUnfiltered()
	{
		mUnloadUnfiltered = true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ExitVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serializeAs<uint16>(mDoorType);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ExitVehicleAction::onStartup()
	{
		mVehicleComponent = getEntity().getComponent<VehicleComponent>();
		if (!mVehicleComponent.valid())
			return false;

		EntityHelper entityHelper(getEntity());
		if (entityHelper.isHelicopter() && entityHelper.isFlying())
			return false;

		return true;
	}

	qsf::action::Result ExitVehicleAction::updateAction(const qsf::Clock& clock)
	{
		if (!mVehicleComponent.valid())
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				findCorrectVehicleDoor();

				// Open the door
				mVehicleComponent->openDoor(mDoorType, true);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_OPEN_DOOR;
			}

			case STATE_OPEN_DOOR:
			{
				if (!mVehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_OPEN, 0.7f))
				{
					if (mVehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSED))
					{
						// In rare cases, the door stays closed... so start again
						mCurrentState = STATE_INIT;
					}

					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_EXIT;
			}

			case STATE_EXIT:
			{
				exitEntityFromVehicle();

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_WAIT_FOR_PERSONAL;
			}

			case STATE_WAIT_FOR_PERSONAL:
			{
				if (mWaitTillPersonsFadedIn > qsf::Time::ZERO)
				{
					mWaitTillPersonsFadedIn -= clock.getTimePassed();
					return qsf::action::RESULT_CONTINUE;
				}

				// Check if we still should select the persons to exit
				if (mSelectPersonsAfterExitVehicle)
				{
					// Double check if the conditions are the same after exiting the vehicle (time is passed, everything can changed)
					SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
					mSelectPersonsAfterExitVehicle = selectionManager.isIdSelected(getEntityId()) && selectionManager.getSelectionSize() == 1;

					if (mSelectPersonsAfterExitVehicle)
					{
						qsf::EntityVectorScratchBuffer entitiesToSelect;
						for (qsf::Entity* entity : mEntitiesToExit)
						{
							if (nullptr != entity && EntityHelper(*entity).getContainerEntity() == nullptr)
							{
								// Ignore civilians (relevant e.g. for ME310)
								if (nullptr != entity->getComponent<CommandableComponent>())
								{
									entitiesToSelect.push_back(entity);
								}
							}
						}

						if (!entitiesToSelect.empty())
						{
							// Select only all previous exited entities
							selectionManager.clearSelection();
							for (qsf::Entity* entity : entitiesToSelect)
							{
								selectionManager.addIdToSelection(entity->getId());
							}
							QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
						}
					}
				}

				// Close the door again
				mVehicleComponent->openDoor(mDoorType, false);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_CLOSE_DOOR;
			}

			case STATE_CLOSE_DOOR:
			{
				if (!mVehicleComponent->areAllDoorsOfTypeInState(mDoorType, DoorComponent::DOOR_CLOSED, 0.7f))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Done
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ExitVehicleAction::findCorrectVehicleDoor()
	{
		mDoorType = mVehicleComponent->getMainDoorType();
	}

	void ExitVehicleAction::exitEntityFromVehicle()
	{
		qsf::Entity& entity = getEntity();
		{
			CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
			Player* player = nullptr;
			if (nullptr != commandableComponent)
			{
				player = EM5_PLAYERS.getPlayerByIndex(commandableComponent->getPlayerId());
			}

			if (!getMap().getCoreEntity().getOrCreateComponentSafe<GameComponent>().isInTutorial() && (nullptr != player && player->isPlayerLocal()))
			{
				// In case only the vehicle is selected and all persons are leaving the vehicle, select the personal after this
				SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
				mSelectPersonsAfterExitVehicle = selectionManager.isIdSelected(getEntityId()) && selectionManager.getSelectionSize() == 1;
			}
			else
			{
				// Don't do automatic selection when the unit is not from the local player
				mSelectPersonsAfterExitVehicle = false;
			}
		}

		// Collect the entities to exit, if filter is set, it is only one entity
		if (mUnloadUnfiltered)
		{
			ExitVehicleCommand::getEntitiesToExitUnfiltered(entity, mEntitiesToExit);
		}
		else
		{
			ExitVehicleCommand::getEntitiesToExitWithFilter(entity, mEntitiesToExit, mExitPersonId);
		}

		// Get the door position
		qsf::Transform placeTransform;
		getPlaceTransform(placeTransform);

		// Exit the entities now
		qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
		qsf::Time spawnDelay = qsf::Time::ZERO;
		int index = 0;
		for (qsf::Entity* containedEntity : mEntitiesToExit)
		{
			float placeOffset = getPlaceOffset(index);
			glm::vec3 newPosition = placeTransform.getPosition() + (transformComponent->getRotation() * qsf::CoordinateSystem::getIn() * placeOffset);

			// Push extra action for all the entity details
			qsf::ActionComponent* actionComponent = containedEntity->getOrCreateComponent<qsf::ActionComponent>();
			actionComponent->pushAction<UnloadPersonFromVehicleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(getEntity(), newPosition, placeTransform.getRotation(), spawnDelay);

			spawnDelay += qsf::Time::fromSeconds(0.5f);	// Increase the spawnDelay for everyone after the first

			index++;
		}
		mWaitTillPersonsFadedIn = spawnDelay + qsf::Time::fromSeconds(0.2f);
	}

	void ExitVehicleAction::getPlaceTransform(qsf::Transform& outTransform)
	{
		// Get the door position
		mVehicleComponent->getFirstDoorTransformByType(mDoorType, outTransform);

		// For leaving the vehicle, we rotate at 180°
		outTransform.setRotation(outTransform.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>()));
	}

	float ExitVehicleAction::getPlaceOffset(int index)
	{
		if (UnitHelper(getEntity()).isFireFighterRW() && index <= 2)
		{
			// Place position is optimized for best access to the vehicle equipment doors
			float placePositionOffset[3] = { -1.25, -2.75, -3.75 };
			return placePositionOffset[index];
		}
		else
		{
			// Default for most vehicles
			return (index == 0 ? 0.0f : index * -1.5f);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
