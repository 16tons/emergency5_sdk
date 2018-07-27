// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/OrderUnitCommandMode.h"
#include "em5/command/mode/DefaultCommandMode.h" // For new pick entity under mouse position implementation
#include "em5/command/CommandContext.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/SpawnUnitAction.h"
#include "em5/application/Application.h"
#include "em5/audio/AudioFeedbackHelper.h"
#include "em5/component/polygon/WaterDeepTypePolygonComponent.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/game/Game.h"
#include "em5/game/player/Player.h"
#include "em5/game/units/UnitPool.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/gui/hud/vehicle/VehicleOrderWindow.h"
#include "em5/gui/hud/vehicle/VehicleSelectionWindow.h"
#include "em5/gui/hud/vehicle/VehicleOrderBar.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/map/query/RayMapQuery.h"
#include "em5/plugin/Messages.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/em2012/terrain.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Detail implementation                                 ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
		// With this wait time we become a small but noticeable delay when the entity under the mouse changes
		static const qsf::Time WAIT_TIME_BETWEEN_CHECKS = qsf::Time::fromSeconds(0.1f);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 OrderUnitCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::OrderUnitCommandMode");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 OrderUnitCommandMode::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	SpawnUnitAction* OrderUnitCommandMode::orderUnit(const OrderInfo& orderInfo, Player& player, const OrderInfo::OrderPairList& orderPairList, const glm::vec3& targetPosition, const glm::quat* targetRotation)
	{
		// Ask unit pool if this kind of unit is still available for the player
		if (player.getUnitPool().isUnitAvailableInHQ(orderInfo))
		{
			const UnitSpawnPointComponent* unitSpawnPointComponent = findUnitSpawnPoint(orderInfo, UnitSpawnPointComponent::UNITSPAWNPOINTTYPE_ENTRY, targetPosition);
			if (nullptr != unitSpawnPointComponent)
			{
				// Push a "spawnUnitAction" to the spawn point, this handles all the details about animation, finding space in the ordering area, pushing starting actions
				qsf::ActionComponent* unitSpawnPointActionComponent = unitSpawnPointComponent->getEntity().getOrCreateComponent<qsf::ActionComponent>();
				SpawnUnitAction& spawnUnitAction = unitSpawnPointActionComponent->pushAction<SpawnUnitAction>(ACTION_PRIORITY);
				spawnUnitAction.init(orderInfo, targetPosition, &player, orderPairList, 1, 1, targetRotation);
				return &spawnUnitAction;
			}
		}

		// Error!
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	OrderUnitCommandMode::OrderUnitCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mOrderInfo(nullptr),
		mOrderOnlyInThisAreaEntity(nullptr),
		mNumberToOrder(0),
		mPlayAudioForOrderUnit(false),
		mElapsedWaitTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	OrderUnitCommandMode::~OrderUnitCommandMode()
	{
		// Nothing here
	}

	void OrderUnitCommandMode::initialize(const OrderInfo& orderInfo, uint32 numberToOrder, OrderInfo::OrderPairList orderPairList)
	{
		// Set the order information at startup
		mOrderInfo = &orderInfo;
		mNumberToOrder = numberToOrder;
		mOrderPairList = orderPairList;
	}

	void OrderUnitCommandMode::setOrderOnlyInThisAreaEntity(qsf::Entity* orderOnlyInThisAreaEntity)
	{
		mOrderOnlyInThisAreaEntity = orderOnlyInThisAreaEntity;
	}

	const OrderInfo* OrderUnitCommandMode::getOrderInfo() const
	{
		return mOrderInfo;
	}

	uint32 OrderUnitCommandMode::getNumberToOrder() const
	{
		return mNumberToOrder;
	}

	void OrderUnitCommandMode::setNumberToOrder(uint32 numberToOrder)
	{
		mNumberToOrder = numberToOrder;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void OrderUnitCommandMode::startup()
	{
		// Clear the selection
		EM5_GAME.getSelectionManager().clearSelection();
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));

		showAllHarborIcons(true);

		// Reset mouse cursor
		EM5_APPLICATION.getGui().resetMouseCursor();
	}

	void OrderUnitCommandMode::shutdown()
	{
		showAllHarborIcons(false);
		mDebugDrawProxy.unregister();
		mOrderInfo = nullptr;
	}

	bool OrderUnitCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		QSF_CHECK(nullptr != mOrderInfo, "OrderUnitCommandMode::executeCommandContext(): Can't find order info for unit, check order info asset again. Command abort.", return false);

		// If we have a area restriction entity set, check if the target entity is the restriction entity
		if (nullptr != mOrderOnlyInThisAreaEntity)
		{
			if (commandContext.mTargetEntity == mOrderOnlyInThisAreaEntity)
			{
				// We use the position of area entity for the target point, but only if there's a transform component
				const qsf::TransformComponent* transformComponent = mOrderOnlyInThisAreaEntity->getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					commandContext.mTargetPosition = transformComponent->getPosition();
				}
			}
			else
			{
				// The player tried to send a unit in an area outside of the restricted area -> cancel the request
				return false;
			}
		}
		else if (!allowOrderingAtTarget(commandContext.getTargetPosition(), commandContext.mTargetEntity))
		{
			return false;
		}

		// Reset mouse cursor
		EM5_APPLICATION.getGui().resetMouseCursor();

		CommandContext& em5CommandContext = static_cast<CommandContext&>(commandContext);
		Player* player = em5CommandContext.mPlayer;
		QSF_CHECK(nullptr != player, "OrderUnitCommandMode::executeCommandContext(): command context without a valid player, that should not happen", QSF_REACT_THROW);

		mPlayAudioForOrderUnit = false;
		for (uint32 i = 0; i < mNumberToOrder; ++i)
		{
			// Ask unit pool if this kind of unit is still available for the player
			if (player->getUnitPool().isUnitAvailableInHQ(*mOrderInfo))
			{
				QSF_ASSERT(em5CommandContext.mTargetPosition.is_initialized(), "Command context target position must always be provided", QSF_REACT_NONE);
				const UnitSpawnPointComponent* unitSpawnPointComponent = findUnitSpawnPoint(*mOrderInfo, UnitSpawnPointComponent::UNITSPAWNPOINTTYPE_ENTRY, *em5CommandContext.mTargetPosition);

				QSF_CHECK(nullptr != unitSpawnPointComponent, "No spawn point found for unit " << mOrderInfo->getName(), continue)
				triggerUnitSpawn(*unitSpawnPointComponent, i, em5CommandContext);
			}
		}

		// Hide the interface
		{
			IngameHud& ingameHud = EM5_GUI.getIngameHud();

			VehicleOrderBar* vehicleOrderBar = ingameHud.getVehicleOrderBar();
			if (nullptr != vehicleOrderBar)
				vehicleOrderBar->deselectAll();

			ingameHud.getVehicleSelectionWindow().resetCurrentDepartment();
			ingameHud.getVehicleOrderWindow().hide();
		}

		// Only play audio, when the command is not executed by automatism and comes from local player
		if (mPlayAudioForOrderUnit && !em5CommandContext.mIsAutomatism)
		{
			DefaultCommandMode::showVisualCommandFeedback(commandContext);
			AudioFeedbackHelper::playCommandResponse(mOrderInfo);
		}

		return true;
	}

	void OrderUnitCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		EmergencyGui& emergencyGui = EM5_APPLICATION.getGui();

		// Do nothing, when the mouse cursor is over an HUD element
		if (GameSettingsGroup::getInstanceSafe().getMouseOverHudElement())
		{
			emergencyGui.resetMouseCursor();
			mDebugDrawProxy.unregister();
			return;
		}

		mElapsedWaitTime += clock.getTimePassed();

		// Don't do the checks on each update tick
		if (mElapsedWaitTime >= ::detail::WAIT_TIME_BETWEEN_CHECKS)
		{
			mElapsedWaitTime = qsf::Time::ZERO;

			const qsf::GuiContext& guiContext = EM5_GUI.getGuiContext();

			bool isOrderableHere = false;
			{
				// Per default, we don't check the target position - only exception is when we have a target area set
				if (nullptr != mOrderOnlyInThisAreaEntity)
				{
					// We have a restriction entity set so check if the mouse is over this entity
					RayMapQueryResponse rayMapQueryResponse(RayMapQueryResponse::EXCLUDE_TERRAIN);
					RayMapQuery(QSF_MAINMAP).getEntityUnderNormalizedMousePosition(guiContext.getNormalizedMouseCursorPosition(), rayMapQueryResponse);

					isOrderableHere = (mOrderOnlyInThisAreaEntity == rayMapQueryResponse.entity.get());
				}
				else
				{
					isOrderableHere = allowOrderingUnderMousePosition();
				}
			}
			emergencyGui.setMouseCursor(isOrderableHere ? "orderable" : "not_orderable");

			#ifndef RETAIL
				// Debug output at the player's mouse cursor
				mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Order " + mOrderInfo->getName(), guiContext.getMouseCursorPosition() + glm::vec2(16.0f, 8.0f)));
			#endif
		}
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	UnitSpawnPointComponent* OrderUnitCommandMode::findUnitSpawnPoint(const OrderInfo& orderInfo, UnitSpawnPointComponent::UnitSpawnPointType unitSpawnPointType, const glm::vec3& targetPosition)
	{
		const uint32 unitTypeId = qsf::StringHash(orderInfo.getName());
		return UnitSpawnPointComponent::findUnitSpawnPoint(unitTypeId, unitSpawnPointType, QSF_MAINMAP, targetPosition);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool OrderUnitCommandMode::allowOrderingUnderMousePosition()
	{
		glm::vec3 targetPosition;
		qsf::Entity* targetEntity = nullptr;
		if (!getTargetUnderMouse(targetPosition, targetEntity))
			return false;

		if (!allowOrderingAtTarget(targetPosition, targetEntity))
			return false;

		return true;
	}

	bool OrderUnitCommandMode::getTargetUnderMouse(glm::vec3& targetPosition, qsf::Entity*& targetEntity)
	{
		RayMapQueryResponse rayMapQueryResponse(RayMapQueryResponse::POSITION_RESPONSE);
		const glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();

		RayMapQuery(QSF_MAINMAP).getEntityUnderNormalizedMousePosition(normalizedMousePosition, rayMapQueryResponse);

		if (rayMapQueryResponse.entity.valid())
		{
			// Correct height
			qsf::GroundMapQuery(QSF_MAINMAP).getHeightAtXZPosition(rayMapQueryResponse.position.x, rayMapQueryResponse.position.z, rayMapQueryResponse.position.y);

			targetPosition = rayMapQueryResponse.position;
			targetEntity = rayMapQueryResponse.entity.get();
			return true;
		}
		return false;
	}

	bool OrderUnitCommandMode::allowOrderingAtTarget(const glm::vec3& targetPosition, const qsf::Entity* targetEntity)
	{
		// This is based on "em5::MoveCommand"

		// Make sure the target is terrain, walkable or there is none at all
		bool isShip = mOrderInfo->hasUnitTag(qsf::StringHash("Ship")) || mOrderInfo->hasUnitTag(qsf::StringHash("Boat"));
		bool isHeli = mOrderInfo->hasUnitTag(qsf::StringHash("Heli"));

		if (nullptr != targetEntity)
		{
			if (nullptr == targetEntity->getComponentById("qsf::TerrainComponent"))
			{
				if (isShip)
				{
					bool isWaterDeep = false;

					for (WaterDeepTypePolygonComponent* deepWaterPolygonComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<WaterDeepTypePolygonComponent>())
					{
						if (deepWaterPolygonComponent->getWaterDeepType() == WaterDeepTypePolygonComponent::DEEP_WATER &&
							deepWaterPolygonComponent->isPositionInside(targetPosition))
						{
							isWaterDeep = true;
							break;
						}
					}

					if (!isWaterDeep)
					{
						return false;
					}
				}
				else if (!isHeli)
				{
					qsf::WalkableComponent* walkableComponent = targetEntity->getComponent<qsf::WalkableComponent>();
					if (nullptr == walkableComponent || !walkableComponent->isActive())
					{
						return false;
					}
				}
			}
		}

		// Check if the terrain accept the vehicle
		{
			// The order info has detailed tags about the unit
			qsf::ai::EObjectTerrainClass terrainClass = qsf::ai::EOTC_SQUADCAR;
			if (isHeli)
			{
				terrainClass = qsf::ai::EOTC_AIRPLANE;
			}
			else if (isShip)
			{
				terrainClass = qsf::ai::EOTC_BOAT;
			}

			// Speed == 0 on this position means that the vehicle can't drive there
			if (qsf::ai::ERouteFinder::Instance()->GetSpeedFactor(terrainClass, targetPosition) <= 0)
			{
				return false;
			}
		}

		// TODO(mk): We could also check if there is enough space on the ground

		return true;
	}

	void OrderUnitCommandMode::triggerUnitSpawn(const UnitSpawnPointComponent& unitSpawnPointComponent, uint32 index, CommandContext& commandContext)
	{
		// Game design decided: It's better to take the order configuration built via ordering GUI for all ordered units, not just the first one
		OrderInfo::OrderPairList orderPairList = mOrderPairList;

		qsf::MessageParameters parameters;
		parameters.setParameter("commandContext", &commandContext);
		parameters.setParameter("orderInfoId", mOrderInfo->getOrderInfoId());
		// TODO(sw) We have to use here a type which is registered at camp and can hold the value of a pointer
		//          Because em5::OrderInfo::OrderPairList = std::map<std::string, int> is not registered at camp which is used internally by the qsf::MessageParameters::setParameter method
		parameters.setParameter("orderPairList", reinterpret_cast<uint_ptr>(&orderPairList));

		bool result = false;
		QSF_MESSAGE.emitMessageWithResponse(qsf::MessageConfiguration(Messages::EM5_PRE_ORDER_UNIT), result, parameters);
		if (result)
		{
			// TODO(mk) Add here the target position optimization for multiple sent entities for online part

			// The above message is only handled in multiplay context at client side -> when player is local allow playing the response
			if (commandContext.mPlayer->isPlayerLocal())
			{
				mPlayAudioForOrderUnit = true;
			}

			// The message receiver canceled the operation
		}
		else
		{
			// Push a "spawnUnitAction" to the spawn point, this handles all the details about animation, finding space in the ordering area, pushing starting actions
			qsf::ActionComponent* unitSpawnPointActionComponent = unitSpawnPointComponent.getEntity().getOrCreateComponent<qsf::ActionComponent>();
			unitSpawnPointActionComponent->pushAction<SpawnUnitAction>(ACTION_PRIORITY).init(*mOrderInfo, *commandContext.mTargetPosition, commandContext.mPlayer, orderPairList, index, mNumberToOrder, commandContext.mTargetRotation.get_ptr(), commandContext.mUserData);

			if (commandContext.mPlayer->isPlayerLocal())
			{
				mPlayAudioForOrderUnit = true;

				// Send the order unit message only for the local player
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, mOrderInfo->getOrderInfoId()));
			}
		}
	}

	void OrderUnitCommandMode::showAllHarborIcons(bool show)
	{
		// TODO(co) This special functionality feels wrong in the more generic class. Florian Wendel mentioned that it would also
		// be possible to implement a new message in here and react in special classes on it. Since lack of time until EMERGENCY 2016
		// release this solution has to do the trick for now. A more generic approach is highly recommended.
		EM5_GAME.getMiniMapIconRegistrationLogic().showAllHarborIcons(show && nullptr != mOrderInfo && mOrderInfo->getName() == "TFMB");
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
