// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/SpawnUnitAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/game/player/Player.h"
#include "em5/game/selection/SelectionMarkerManager.h"
#include "em5/game/units/OrderInfoManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/game/Game.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/EActor.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/physics/collision/CollisionHelper.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SpawnUnitAction::ACTION_ID = "em5::SpawnUnitAction";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void SpawnUnitAction::setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, qsf::Entity& spawnPointEntity, const OrderInfo& orderInfo, const Player& player)
	{
		// Use the spawn point's transformation for the spawned unit as well
		const qsf::TransformComponent& spawnPointTransformComponent = spawnPointEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		setupCreatedEntityAsUnitVehicle(newOrderedUnit, spawnPointTransformComponent.getTransform(), orderInfo, player);
	}

	void SpawnUnitAction::setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, const qsf::Transform& transform, const OrderInfo& orderInfo, const Player& player)
	{
		// New vehicle unit was created
		// - Set the initial transform to the given one
		// - Setup some vehicle specific settings
		// - Setup fade in effect
		// - Setup the entity as unit

		// Set position to spawn point
		qsf::TransformComponent& transformComponent = newOrderedUnit.getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::MovableComponent* movableComponent = newOrderedUnit.getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
		{
			movableComponent->warpToPositionAndRotation(transform.getPosition(), transform.getRotation());
		}
		else
		{
			transformComponent.setPositionAndRotation(transform.getPosition(), transform.getRotation());
		}

		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			// Fade in effect
			newOrderedUnit.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::fromSeconds(1.0f));
		}

		setupCreatedEntityAsUnitVehicle(newOrderedUnit, orderInfo, player);
	}

	void SpawnUnitAction::setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, const OrderInfo& orderInfo, const Player& player)
	{
		// Activate lights if we have a helicopter
		HelicopterComponent* helicopterComponent = newOrderedUnit.getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			helicopterComponent->forceFlying(true);
		}

		// Mark the vehicle as not parking
		VehicleComponent* vehicleComponent = newOrderedUnit.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			vehicleComponent->setIsParkingEntity(false);
		}

		// Setup the entity as unit
		setupCreatedEntityAsUnit(newOrderedUnit, orderInfo, player);
	}

	void SpawnUnitAction::registerPersonnelInSpawn(const OrderInfo& orderInfo, const OrderInfo::OrderPair& orderPair, const Player& player)
	{
		// Register a number of X of personal for spawning
		const OrderInfo* personalOrderInfo = EM5_GAME.getOrderInfoManager().findElement(qsf::StringHash(orderPair.first));
		QSF_CHECK(personalOrderInfo != nullptr, "SpawnUnitAction::init() Vehicle has unknown start personal: \"" + orderPair.first + "\" skip this person. Check em5 order info asset \"" + orderInfo.getName() + "\" for details.", return);
		UnitPool& unitPool = player.getUnitPool();
		for (uint32 i = 0; i < orderPair.second; ++i)
		{
			if (unitPool.isUnitAvailableInHQ(*personalOrderInfo))
			{
				unitPool.registerUnitInSpawn(*personalOrderInfo);
			}
		}
	}

	void SpawnUnitAction::fillVehicleWithStartPersonnel(qsf::Entity& vehicleEntity, const OrderInfo& orderInfo, const Player& player, OrderInfo::OrderPairList* orderPairList, uint64 spawnpointEntityId, uint64 userData)
	{
		VehicleComponent* vehicleComponent = vehicleEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			qsf::Map& map = vehicleEntity.getMap();
			MapHelper mapHelper(map);
			const UnitPool& unitPool = player.getUnitPool();
			const OrderInfoManager& orderInfoManager = EM5_GAME.getOrderInfoManager();
			for (const auto& personalPair : orderInfo.getStartPersonnelList())
			{
				// Create a number of X of personnel for every type
				OrderInfo* personalOrderInfo = orderInfoManager.findElement(qsf::StringHash(personalPair.first));
				QSF_CHECK(personalOrderInfo != nullptr, "SpawnUnitAction::fillVehicleWithStartPersonnel() Vehicle has unknown start personnel: \"" + personalPair.first + "\" skip this person. Check em5 order info asset \"" + orderInfo.getName() + "\" for details.", continue);

				// Find number of personnel
				uint32 numberOfPersonnel = personalPair.second;

				// If order configuration has a number of units, take this number
				if (nullptr != orderPairList)
				{
					const auto configurationPairIterator = orderPairList->find(personalPair.first);
					if (configurationPairIterator != orderPairList->end())
					{
						numberOfPersonnel = configurationPairIterator->second;
					}
				}

				for (uint32 i = 0; i < numberOfPersonnel; ++i)
				{
					// Note that the vehicle's personnel is already registered in spawn here
					if (unitPool.isUnitAvailableInSpawn(*personalOrderInfo))
					{
						uint32 prefabAssetId = personalOrderInfo->getPrefab().getLocalAssetId();
						qsf::Entity* newPersonnelUnit = nullptr;
						bool hidden = true;
						if (qsf::isInitialized(prefabAssetId) && prefabAssetId != 0)
						{
							newPersonnelUnit = mapHelper.createObjectByLocalPrefabAssetId(prefabAssetId);
						}
						else
						{
							// The motor boot transporter carried his "personnel" with him in visible state
							newPersonnelUnit = map.getEntityById(vehicleComponent->getSpecialEntity());
							hidden = false;
						}

						QSF_ASSERT(nullptr != newPersonnelUnit, "SpawnUnitAction::fillVehicleWithStartPersonnel() failed to create entity instance", continue);
						if (nullptr != newPersonnelUnit)
						{
							setupCreatedEntityAsUnitPerson(*newPersonnelUnit, &vehicleComponent->getEntity(), *personalOrderInfo, player, hidden);
							sendUnitSpawnedMessage(*newPersonnelUnit, spawnpointEntityId, userData);
						}
					}
				}
			}
		}
	}

	void SpawnUnitAction::setupCreatedEntityAsUnitPerson(qsf::Entity& newOrderedUnit, qsf::Entity* vehicleEntity, const OrderInfo& orderInfo, const Player& player, bool hidden)
	{
		// New person unit was created
		// - Assign the person unit to the vehicle unit to which the person belongs
		// - Setup the entity as unit
		// - Note: This also works with an motorboat, though it is technically a vehicle

		if (nullptr != vehicleEntity)
		{
			// Enter vehicle
			EntityHelper(newOrderedUnit).enterContainer(*vehicleEntity, container::CONTAINERTYPE_SQUAD, hidden);
		}

		// Setup the entity as unit
		setupCreatedEntityAsUnit(newOrderedUnit, orderInfo, player);
	}

	void SpawnUnitAction::setupCreatedEntityAsUnit(qsf::Entity& newOrderedUnit, const OrderInfo& orderInfo, const Player& player)
	{
		// Register the ordering player to the entity
		// In case of assert break point here, we have possible a "qsf::game::CommandableComponent", in this case check the editing of the unit and add a "em5::CommandableComponent"
		CommandableComponent& commandableComponent = newOrderedUnit.getOrCreateComponentSafe<CommandableComponent>();
		commandableComponent.setPlayerId(player.getPlayerIndex());
		commandableComponent.setTeamId(player.getTeamId());
		commandableComponent.setUnitType(orderInfo.getOrderInfoId());
		commandableComponent.setUnitTagsAsString(orderInfo.getUnitTagsAsString());
		commandableComponent.setMiniMapIconType(orderInfo.getMiniMapIconType());

		// Assign random speaker index for command and selection feedback
		const uint32 speakerIndex = qsf::Random::getRandomUint(1, 2);
		commandableComponent.setSpeakerIndex(speakerIndex);

		// Register the unit to the ordering player's unit pool as a unit on map
		UnitPool& unitPool = player.getUnitPool();
		unitPool.registerUnitInMap(orderInfo, newOrderedUnit.getId());
		unitPool.unregisterUnitInSpawn(orderInfo);

		if (!EntityHelper(newOrderedUnit).isEntityHidden())
		{
			// Show ground marker, except if entity is initially inside a vehicle already
			SelectionMarkerManager::getInstanceSafe().showGroundMarker(newOrderedUnit, false);
		}

		if (nullptr != EM5_NETWORK.getMultiplayerHost())
		{
			EntityHelper::setupEntityAsGhost(newOrderedUnit, player.getPlayerIndex());
		}

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_UNIT_SETUP, orderInfo.getOrderInfoId(), newOrderedUnit.getId()));
	}

	void SpawnUnitAction::sendUnitSpawnedMessage(const qsf::Entity& newOrderedUnit, uint64 spawnpointEntityId, uint64 userData)
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_SPAWN_UNIT, newOrderedUnit.getId(), spawnpointEntityId, userData));
	}

	void SpawnUnitAction::sendUnitSpawnedPlayerUpdateSelectionMessage(uint32 playerId, uint64 spawnedUnitEntityId)
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_UNIT_SPAWNED_PLAYER_UPDATE_SELECTION, playerId, spawnedUnitEntityId));
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpawnUnitAction::SpawnUnitAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mOrderInfo(nullptr),
		mUserData(qsf::getUninitialized<uint64>()),
		mPlayer(nullptr),
		mIndex(0),
		mMaxIndex(0)
	{
		// Nothing here
	}

	SpawnUnitAction::~SpawnUnitAction()
	{
		// Nothing here
	}

	void SpawnUnitAction::init(const OrderInfo& orderInfo, const glm::vec3& targetPosition, Player* player, const OrderInfo::OrderPairList& orderPairList, int index, int maxIndex, const glm::quat* targetRotation, uint64 userData)
	{
		mOrderInfo = &orderInfo;
		mTargetPosition = targetPosition;
		if (nullptr != targetRotation)
		{
			mTargetRotation = *targetRotation;
		}
		else
		{
			mTargetRotation.reset();
		}
		mUserData = userData;
		mPlayer = player;
		mOrderPairList = orderPairList;

		mIndex = index;
		mMaxIndex = maxIndex;

		registerUnitWithPersonnelInSpawn();
		mOrderedTimeStamp = EM5_GAME.getSimulationClock().getCurrentTime();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SpawnUnitAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result SpawnUnitAction::updateAction(const qsf::Clock&)
	{
		QSF_CHECK(nullptr != mOrderInfo, "EM5: The order information instance of the spawn unit action is invalid", return qsf::action::RESULT_DONE);
		QSF_CHECK(nullptr != mPlayer, "EM5: The player instance of the spawn unit action is invalid", return qsf::action::RESULT_DONE);

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// TODO(mk) Start open door animation
				mCurrentState = STATE_WAIT_FORSPACE;

				// Fall through by design
			}

			case STATE_WAIT_FORSPACE:
			{
				// Check for collision in the area
				bool isColliding = false;
				{
					// Get collision from prefab, ignore child objects
					qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId(mOrderInfo->getPrefab().getLocalAssetId());
					QSF_CHECK(nullptr != prototype, "EM5: Can't create prefab with asset \"" << mOrderInfo->getPrefab().getLocalAssetId() << "\" skip creating process", return qsf::action::RESULT_DONE);

					qsf::BulletCollisionComponent* collisionComponent = prototype->getComponent<qsf::BulletCollisionComponent>();
					if (nullptr != collisionComponent)
					{
						// Check at the spawn point position
						const qsf::TransformComponent& spawnPointTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
						qsf::Transform transform = spawnPointTransformComponent.getTransform();
						transform.setScale(qsf::Math::GLM_VEC3_UNIT_XYZ);

						isColliding = !qsf::CollisionHelper(getMap()).canBeAddedWithoutCollision(transform, *collisionComponent);

						// Lets ask the old routefinder too
						{
							glm::vec3 anchorPoint, extensions;
							glm::quat rotation;
							collisionComponent->getAsOrientedBoundingBox(anchorPoint, rotation, extensions);

							qsf::ai::CollisionList collisionRange;
							qsf::ai::EM3::Router->FindCollisions(extensions, -1, qsf::ai::ERouterObjectState(transform.getPosition(), glm::mat3_cast(transform.getRotation())), collisionRange, qsf::ai::EOTC_ANYTHING); // call with values for right now

							for (qsf::ai::ECollisionObject* collision : collisionRange)
							{
								// Check type of the collision object
								qsf::ai::EActor* actor = collision->GetObject();
								if (nullptr != actor)
								{
									// Collision with persons and vehicles are not allowed
									if (actor->GetType() == qsf::ai::EAT_VEHICLE || actor->GetType() == qsf::ai::EAT_PERSON)
									{
										isColliding = true;
									}
								}
							}
						}
					}
				}

				if (isColliding)
				{
					// Area is blocked, try again next time
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_WAIT_FORORDERTIME;

				// Fall through by design
			}

			case STATE_WAIT_FORORDERTIME:
			{
				//// Wait till the order time of the unit is over
				// TODO(co) Why is this commented without any comment why it's commented?
				//mOrderTime += clock.getTimePassed().getSeconds();
				//if (mOrderTime < mOrderInfo->getOrderSpeed())
				//{
				//	// Wait till unit can spawn
				//	return qsf::action::RESULT_CONTINUE;
				//}

				mCurrentState = STATE_SPAWNUNIT;

				// Fall through by design
			}

			case STATE_SPAWNUNIT:
			{
				// Find correct prefab and create new entity from it
				mNewSpawnedVehicle = MapHelper(getEntity().getMap()).createObjectByLocalPrefabAssetId(mOrderInfo->getPrefab().getLocalAssetId());
				if (mNewSpawnedVehicle.valid())
				{
#ifndef ENDUSER
					// Just for debugging: Directly spawn at cursor when "Alt" is pressed
					if (QSF_INPUT.getKeyboard().anyMenuPressed())
					{
						qsf::Transform transform = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
						transform.setPosition(mTargetPosition);
						if (mTargetRotation.is_initialized())
						{
							transform.setRotation(*mTargetRotation);
						}
						setupCreatedEntityAsUnitVehicle(mNewSpawnedVehicle.getSafe(), transform, *mOrderInfo, *mPlayer);
					}
					else
#endif
					{
						setupCreatedEntityAsUnitVehicle(mNewSpawnedVehicle.getSafe(), getEntity(), *mOrderInfo, *mPlayer);
					}

					// Fill vehicle with start personnel
					fillVehicleWithStartPersonnel(mNewSpawnedVehicle.getSafe(), *mOrderInfo, *mPlayer, &mOrderPairList, getEntityId(), mUserData);

					// Send message
					sendUnitSpawnedMessage(mNewSpawnedVehicle.getSafe(), getEntityId(), mUserData);
				}

				// We interrupt and delay the next part to give the navigation system time to create the router component with all his necessary settings
				// Next tick we have everything and can send a valid movement target
				mCurrentState = STATE_SPAWNUNIT_2_PART;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SPAWNUNIT_2_PART:
			{
				QSF_CHECK(mNewSpawnedVehicle.valid(), "Spawned vehicle was just destryoed", return qsf::action::RESULT_DONE);

				// For multiple ordered units, find a good target position to avoid pushing all units to the same position
				if (mMaxIndex > 1)
				{
					MoveCommand::computeMultiSelectionTargetPositionForOrdering(mNewSpawnedVehicle.getSafe(), mIndex, mMaxIndex, mTargetPosition);
				}
				else
				{
					// For single entity, find target place with enough room
					MoveCommand::computeAvailableTargetPosition(mNewSpawnedVehicle.getSafe(), mTargetPosition);
				}

				// Move to given position
				qsf::logic::TargetPoint moveTargetConfig;
				moveTargetConfig.mPosition	  = mTargetPosition;
				moveTargetConfig.mOrientation = mTargetRotation;
				moveTargetConfig.mTolerance	  = 1.0f;

				qsf::ActionComponent& actionComponent = mNewSpawnedVehicle->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);

				const qsf::MessageConfiguration message(Messages::EM5_UNIT_REACHED_DESTINATION_AFTER_SPAWN, mOrderInfo->getOrderInfoId());
				actionComponent.pushAction<SignalAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(message);

				// Only select target if the time after the ordering is very short
				if ((EM5_GAME.getSimulationClock().getCurrentTime() - mOrderedTimeStamp) <= qsf::Time::fromSeconds(1.0f))
				{
					// Emit a message for the relevant player's selection manager to decide if the selection should be changed
					sendUnitSpawnedPlayerUpdateSelectionMessage(mPlayer->getPlayerIndex(), mNewSpawnedVehicle->getId());
				}

				mCurrentState = STATE_CLOSEDOORS;

				// Fall through by design
			}

			case STATE_CLOSEDOORS:
			{
				// TODO(mk) Check if this was the last ordered unit
				// - "true" wait till the vehicle is out and close the door
				// - "false" abort, next spawnUnitAction can start
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SpawnUnitAction::registerUnitWithPersonnelInSpawn()
	{
		QSF_CHECK(nullptr != mOrderInfo, "EM5: The order information instance of the spawn unit action is invalid", return);
		QSF_CHECK(nullptr != mPlayer, "EM5: The player instance of the spawn unit action is invalid", return);

		mPlayer->getUnitPool().registerUnitInSpawn(*mOrderInfo);

		// Register childs (only if they are available)
		if (0 == mIndex)
		{
			for (auto personalPair : mOrderPairList)
			{
				registerPersonnelInSpawn(*mOrderInfo, personalPair, *mPlayer);
			}
		}
		else
		{
			for (const auto& personalPair : mOrderInfo->getStartPersonnelList())
			{
				registerPersonnelInSpawn(*mOrderInfo, personalPair, *mPlayer);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
