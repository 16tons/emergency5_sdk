// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/LandHelicopterCommandMode.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/police/SEK/PerformRopeDownFromHelicopterCommand.h"
#include "em5/command/police/SEK/RopingFromHelicopterCommand.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/application/Application.h"
#include "em5/game/Game.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/query/RayMapQuery.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf_logic/targetpoint/TargetPoint.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/RayMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Ray.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 LandHelicopterCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::LandHelicopterCommandMode");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 LandHelicopterCommandMode::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LandHelicopterCommandMode::LandHelicopterCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mCaller(nullptr),
		mMap(nullptr)
	{
		// Nothing here
	}

	LandHelicopterCommandMode::~LandHelicopterCommandMode()
	{
		// Nothing here
	}

	void LandHelicopterCommandMode::initialize(qsf::Entity& caller)
	{
		mCaller = &caller;
		mMap = &mCaller->getMap();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void LandHelicopterCommandMode::startup()
	{
		// Nothing here
	}

	void LandHelicopterCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool LandHelicopterCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		QSF_ASSERT(nullptr != mMap, "em5::LandHelicopterCommandMode: Map pointer is not initialized", QSF_REACT_THROW);

		if (!commandContext.hasTargetPosition())
		{
			return false;
		}

		if (!allowLandingAtTarget(commandContext.getTargetPosition(), commandContext.mTargetEntity))
		{
			return false;
		}

		CollisionHelper collisionHelper(*mMap);

		qsf::EntityVectorScratchBuffer callers;
		mMap->getEntitiesByIds(callerIds, callers, true);

		bool canPushAction = true;
		for (qsf::Entity* selectedEntity : callers)
		{
			// Check if selected entity is a SWAT helicopter at all
			CommandableComponent* commandableComponent = selectedEntity->getComponent<CommandableComponent>();
			if (nullptr == commandableComponent || !commandableComponent->isPoliceSEKHeli())
				continue;

			// Check if target position is blocked for this type of unit
			if (collisionHelper.isInBlockedArea(*selectedEntity, commandContext.getTargetPosition()))
				return true;

			if (canPushAction)
			{
				// Action can only be pushed once
				canPushAction = false;

				// Push action
				qsf::ActionComponent& actionComponent = selectedEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();

				// Create the static target point for the AI to move towards only for non selftargets
				qsf::logic::TargetPoint moveTargetConfig;
				moveTargetConfig.mPosition = commandContext.getTargetPosition();

				actionComponent.pushAction<StartHelicopterAction>(ACTION_PRIORITY);
				actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_DEFAULT);
				actionComponent.pushAction<LandHelicopterAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init();

				// Only play it once when the command is executed for multiple entities at the same time
				if (commandContext.mCurrentCallerIndex == 0)
				{
					DefaultCommandMode::showVisualCommandFeedback(commandContext);
				}
			}
			else
			{
				// In case of multi selection, keep mode active for the next entity
				return false;
			}
		}

		// End command mode
		return true;
	}

	void LandHelicopterCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Do nothing, when the mouse cursor is over an hud element
		EmergencyGui& emergencyGui = EmergencyGui::getInstanceSafe();
		if (GameSettingsGroup::getInstanceSafe().getMouseOverHudElement())
		{
			emergencyGui.resetMouseCursor();
			mDebugDrawProxy.unregister();
		}
		else
		{
			// Check is position under mouse is collision free
			if (allowLandingUnderMousePosition())
			{
				emergencyGui.setMouseCursor("place_person_by_flying_heli");
			}
			else
			{
				emergencyGui.setMouseCursor("place_person_by_flying_heli_no");
			}

			#ifndef RETAIL
				// Debug output at the player's mouse cursor
				mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("RopingFromHelicopter ", EM5_GUI.getGuiContext().getMouseCursorPosition() + glm::vec2(16, 8)));
			#endif
		}

		bool returnToDefaultCommandMode = true;
		for (uint64 selectedEntityId : EM5_GAME.getSelectionManager().getSelectedIdSet())
		{
			qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(selectedEntityId);
			if (nullptr == selectedEntity)
				continue;

			// Build context
			CommandContext commandContext;
			commandContext.mCaller = selectedEntity;
			commandContext.mTargetEntity = nullptr;

			// Call "checkCaller" of the commando for validation
			RopingFromHelicopterCommand* ropingFromHelicopterCommand = (QSFGAME_COMMAND.getCommandManager().getById<RopingFromHelicopterCommand>(RopingFromHelicopterCommand::PLUGINABLE_ID));
			QSF_CHECK(nullptr != ropingFromHelicopterCommand, "LandHelicopterCommandMode::updateCommandMode(): Could not find RopingFromHelicopter command instance", return);
			if (ropingFromHelicopterCommand->checkCaller(*commandContext.mCaller))
			{
				// We have at least one valid caller for roping, keep this command mode active
				returnToDefaultCommandMode = false;
				break;
			}
		}

		if (returnToDefaultCommandMode)
		{
			// No valid
			DefaultCommandMode* defaultCommandMode = new DefaultCommandMode();
			QSFGAME_COMMAND.setCommandMode(*defaultCommandMode);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool LandHelicopterCommandMode::allowLandingUnderMousePosition()
	{
		glm::vec3 targetPosition;
		qsf::Entity* targetEntity = nullptr;
		if (!getTargetUnderMouse(targetPosition, targetEntity))
			return false;

		if (!allowLandingAtTarget(targetPosition, targetEntity))
			return false;

		return true;
	}

	bool LandHelicopterCommandMode::getTargetUnderMouse(glm::vec3& targetPosition, qsf::Entity*& targetEntity)
	{
		// Don't allow the helicopter component marker selection entity as target, it's a visual thing only
		RayMapQuery::IgnoreEntityIds ignoreEntityIds;
		if (nullptr != mCaller)
			HelicopterComponent::ignoreMarkerSelectionEntityId(*mCaller, ignoreEntityIds);

		RayMapQueryResponse rayMapQueryResponse(RayMapQueryResponse::POSITION_RESPONSE);
		const glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();

		RayMapQuery(QSF_MAINMAP).getEntityUnderNormalizedMousePosition(normalizedMousePosition, rayMapQueryResponse, &ignoreEntityIds);

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

	bool LandHelicopterCommandMode::allowLandingAtTarget(const glm::vec3& targetPosition, const qsf::Entity* targetEntity)
	{
		// This is based on "em5::MoveCommand"

		// Make sure the target is terrain, walkable or there is none at all
		if (nullptr != targetEntity)
		{
			if (nullptr == targetEntity->getComponentById("qsf::TerrainComponent"))
			{
				qsf::WalkableComponent* walkableComponent = targetEntity->getComponent<qsf::WalkableComponent>();
				if (nullptr == walkableComponent || !walkableComponent->isActive())
				{
					return false;
				}
			}
		}

		// TODO(fw): We could also check if there is enough space on the ground

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
