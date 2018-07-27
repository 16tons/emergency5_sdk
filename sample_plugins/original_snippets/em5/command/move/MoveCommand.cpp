// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/polygon/WaterDeepTypePolygonComponent.h"
#include "em5/game/player/Player.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/ActorFactory.h>
#include <qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/placement/PlacementComponent.h>
#include <qsf/component/polygon/LiquidPolygonComponent.h>
#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace detail
	{

		float getSearchRadiusForTargetPosition(qsf::Entity& entity)
		{
			EntityHelper entityHelper(entity);
			if (entityHelper.isEntityPerson())
			{
				return 1.5f;
			}
			else if (entityHelper.isHelicopter())
			{
				return 10.0f;
			}
			else if (entityHelper.isShip())
			{
				return 12.0f;
			}
			else
			{
				return 6.0f;
			}
		}

		void getDirectionAndLeftVector(const glm::vec3& input, glm::vec3& dir, glm::vec3& left)
		{
			dir = glm::normalize(glm::vec3(input.x, 0.0f, input.z));
			left = qsf::ai::perp(dir);
		}

		bool isAcceptableTarget(qsf::Entity& targetEntity, qsf::Entity& caller)
		{
			if (nullptr != targetEntity.getComponentById("qsf::TerrainComponent"))
			{
				// Terrain is fine
				return true;
			}

			qsf::WalkableComponent* walkableComponent = targetEntity.getComponent<qsf::WalkableComponent>();
			if (nullptr != walkableComponent && walkableComponent->isActive())
			{
				// Active walkable is fine
				return true;
			}

			if (nullptr != targetEntity.getComponent<qsf::LiquidPolygonComponent>())
			{
				// Water is fine for certain callers
				CommandableComponent* commandableComponent = caller.getComponent<CommandableComponent>();
				if (nullptr != commandableComponent && commandableComponent->canSwim())
				{
					// Boats, divers, etc. can move over water
					return true;
				}
				else if (nullptr != caller.getComponent<HelicopterComponent>())
				{
					// Helicopters can move over (above) water as well
					return true;
				}
			}

			// Everything else is not okay as move target
			return false;
		}

	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MoveCommand::PLUGINABLE_ID = qsf::StringHash("em5::MoveCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 MoveCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MoveCommand::MoveCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		mPriority = 25;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "move";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool MoveCommand::checkAvailable()
	{
		return true;
	}

	bool MoveCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, action::COMMAND_STD))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (entityHelper.isSquadPerson())
		{
			if (!entityHelper.isSquadPersonInValidState())
			{
				return false;
			}
		}

		// TODO(mk) We can move this check here to the em5::MoveAction::checkAllowUpdate(). In best case we make this function virtual and avoid moving if the result is "false"
		// Ask (VS) for details about turning of the steering at runtime. Maybe ai::NavigationComponent::setActive(false) is enough
		// Check for game logic with special meaning
		qsf::GameLogicComponent* targetGameLogicComponent = caller.getOrCreateComponent<qsf::GameLogicComponent>();
		if (nullptr != targetGameLogicComponent)
		{
			ReserveEnterVehicleLogic* reserveEnterVehicleLogic = targetGameLogicComponent->getGameLogic<ReserveEnterVehicleLogic>();
			ReserveWaitForFinishLogic* reserveWaitForFinishLogic = targetGameLogicComponent->getGameLogic<ReserveWaitForFinishLogic>();
			if (nullptr != reserveEnterVehicleLogic || nullptr != reserveWaitForFinishLogic)
			{
				// Don't start moving, some logic is waiting for this entity
				return false;
			}
		}

		// Check if movement is allowed
		qsf::ai::NavigationComponent* navigationComponent = caller.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr == navigationComponent)
			return false;

		if (!navigationComponent->isActive())
			return false;

		// Don't move destroyed entities
		if (entityHelper.isBurning() || entityHelper.isBurned())
			return false;

		return true;
	}

	bool MoveCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Caller and target must not be identical
		if (context.mCaller == context.mTargetEntity)
			return false;

		// We need a valid target position
		if (!context.mTargetPosition.is_initialized())
			return false;

		// Make sure the target is acceptable as move target
		if (nullptr != context.mTargetEntity)
		{
			if (!detail::isAcceptableTarget(*context.mTargetEntity, *context.mCaller))
				return false;
		}

		// Make sure the target position is not blocked by polygons
		if (isTargetPositionBlockedByPolygons(*context.mCaller, *context.mTargetPosition))
			return false;

		return true;
	}

	void MoveCommand::execute(const qsf::game::CommandContext& context_)
	{
		const CommandContext& context = static_cast<const CommandContext&>(context_);
		QSF_CHECK(nullptr != context.mCaller, "No caller for move command", return);

#ifndef ENDUSER
		// With Alt key pressed, directly warp to target position first
		if (QSF_INPUT.getKeyboard().anyMenuPressed() && context.mPlayer->isPlayerLocal())
		{
			qsf::MovableComponent* movableComponent = context.mCaller->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				movableComponent->warpToPositionAndRotation(*context.mTargetPosition, movableComponent->getRotation());
			}
			else
			{
				context.mCaller->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(*context.mTargetPosition);
			}
		}
#endif

		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// The chain order feature is currently in testing
		bool allowChainOrder = false;
#define ALLOW_CHAINORDER
#ifdef ALLOW_CHAINORDER
		if (QSF_INPUT.getKeyboard().anyShiftPressed() && context.mPlayer->isPlayerLocal())
		{
			allowChainOrder = true;
		}
#endif
		if (!allowChainOrder)
		{
			actionComponent.clearPlan();
		}

		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(*context.mTargetPosition);

		// Try to start with helicopter
		HelicopterComponent* helicopterComponent = context.mCaller->getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			// With chain orders, push start helicopter only once
			if (!allowChainOrder || nullptr == actionComponent.getAction<StartHelicopterAction>())
			{
				actionComponent.pushAction<StartHelicopterAction>(ACTION_PRIORITY);
			}
		}

		// Figure out the movement mode. By default, moving in emergency mode by a move command.
		uint32 movementModeId = MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS;

		// Divers carrying persons move backwards
		CommandableComponent* commandableComponent = context.mCaller->getComponent<CommandableComponent>();
		if (commandableComponent && commandableComponent->isFireFighterDiver())
		{
			const qsf::GameLogicComponent* gameLogicComponent = context.mCaller->getComponent<qsf::GameLogicComponent>();
			if (nullptr != gameLogicComponent)
			{
				CarryPersonLogic* carryPersonLogic = gameLogicComponent->getGameLogic<CarryPersonLogic>();
				if (carryPersonLogic && qsf::isInitialized(carryPersonLogic->getCarryPerson()))
				{
					movementModeId = MovementModes::MOVEMENT_MODE_WALK_BACKWARDS;
				}
			}
		}

		// Moving backwards when pulling persons
		if (EntityHelper(*context.mCaller).isPullingPerson())
		{
			movementModeId = MovementModes::MOVEMENT_MODE_WALK_BACKWARDS;
		}

		// Create the static target point for the AI to move towards
		qsf::logic::TargetPoint moveTargetConfig;
		moveTargetConfig.mPosition = *context.mTargetPosition;
		moveTargetConfig.mTolerance = 1.0f;

		bool heliCanLand = (nullptr != helicopterComponent && helicopterComponent->getCanLandOnGround());

		// Optimize target positions but only when the context allows it
		if (!context.mTargetPositionSealed)
		{
			// Find helicopter landing position
			if (heliCanLand)
			{
				// Find target place with enough room on floor
				float height;
				if (qsf::GroundMapQuery(QSF_MAINMAP).getHeightAt(moveTargetConfig.mPosition, height))
				{
					moveTargetConfig.mPosition.y = height;
				}
				if (!computeAvailableTargetPosition(*context.mCaller, moveTargetConfig.mPosition, 2.0f))
				{
					if (!computeAvailableTargetPosition(*context.mCaller, moveTargetConfig.mPosition, 6.0f))
					{
						computeAvailableTargetPosition(*context.mCaller, moveTargetConfig.mPosition, 12.0f);
					}
				}
			}
			// On multi-selections modify target positions
			else if (context.mCallers.size() > 1)
			{
				computeMultiSelectionTargetPosition(*context.mCaller, context.mCallers, moveTargetConfig.mPosition);
			}
			// Single selection
			else
			{
				// Find target place with enough room
				computeAvailableTargetPosition(*context.mCaller, moveTargetConfig.mPosition);
			}
		}

		// Now push the action
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), movementModeId);

		if (heliCanLand)
		{
			// Special check if SEK heli is empty
			CommandableComponent* commandableComponent = context.mCaller->getComponent<CommandableComponent>();
			if (commandableComponent->isPoliceSEKHeli())
			{
				heliCanLand = EntityHelper(*context.mCaller).isContainerEmpty();
			}
		}

		// Try to land with helicopters
		if (heliCanLand)
		{
			LandHelicopterAction* landHelicopterAction = actionComponent.getAction<LandHelicopterAction>();
			if (allowChainOrder && nullptr != landHelicopterAction)
			{
				// With chain orders, land helicopter only once -> delete old land action, push new land action to the end
				actionComponent.removeAction(*landHelicopterAction);
			}

			actionComponent.pushAction<LandHelicopterAction>(ACTION_PRIORITY);
		}

		// Only play it once when the command is executed for multiple entities at the same time
		if (context.mCurrentCallerIndex == 0)
		{
			DefaultCommandMode::showVisualCommandFeedback(context);
		}
	}

	bool MoveCommand::highlightTarget() const
	{
		// No command target highlighting please
		return false;
	}

	void MoveCommand::computeMultiSelectionTargetPosition(qsf::Entity& caller, const std::vector<qsf::Entity*> callers, glm::vec3& position)
	{
		qsf::ai::RouterComponent* routerComponent = caller.getComponent<qsf::ai::RouterComponent>();
		if (nullptr == routerComponent)
			return;

		// Find the biggest object, count and determine the selection center
		glm::vec3 selectionCenter(0.0f, 0.0f, 0.0f);
		int countObjects = 0;
		float maximumSize = 0.0f;
		int ownerindex = -1;

		for (qsf::Entity* entity : callers)
		{
			if (entity == &caller)
			{
				ownerindex = countObjects;
			}

			qsf::ai::RouterComponent* entityRouterComponent = entity->getComponent<qsf::ai::RouterComponent>();
			if (nullptr != entityRouterComponent)
			{
				qsf::ai::EGenericObject* gObject = entityRouterComponent->getEGenericObject();
				if (nullptr != gObject)
				{
					++countObjects;

					EntityHelper entityHelper(*entity);
					float brxy = gObject->GetBoundingRadiusXZ();
					brxy *= entityHelper.isEntityPerson() ? 2.0f : 1.5f;
					if (brxy > maximumSize)
						maximumSize = brxy;

					selectionCenter += entityHelper.getPosition();
				}
			}
		}

		// Get the center of the selected group
		selectionCenter /= (float)countObjects;

		// Left some space between the objects
		maximumSize *= 1.5f;

		int numRows = (int)sqrt((double)countObjects);
		int numCols = numRows;
		if (numRows * numRows < countObjects)
		{
			++numCols;
		}

		// Determine the new center
		int midPointX = 0;
		int midPointY = 0;
		for (int i = 0; i < countObjects; ++i)
		{
			midPointX += ((i % numCols) - (numCols/2));
			midPointY += ((i / numCols) - (numRows/2));
		}

		glm::vec3 dir, left;
		glm::vec3 result = position;
		detail::getDirectionAndLeftVector(result - selectionCenter, dir, left);

		// TODO(co) In English, please: Rest of the comments

		// index im Zielraster bestimmen, Minimum für quadratische Weglänge (bzw. Dauer) aller Einheiten suchen
		QSF_CHECK(ownerindex >= 0, "Invalid owner index", return);

		int zielindex = ownerindex;
		if (countObjects < 8)	// da n! Permutationen zu testen sind, hier beschränken und ggf. simple Zuordnung machen
		{
			float minlen = FLT_MAX;
			std::vector<int> v(countObjects);
			for (int i = 0; i < countObjects; ++i)
			{
				v[i] = i;
			}

			// alle Permutationen testen
			do
			{
				// Gesamtlänge berechnen
				float len = 0.0f;

				for (size_t i = 0; i < callers.size(); ++i)
				{
					qsf::Entity& entity = *callers[i];

					glm::vec3 target = result;
					target -= dir * ((((v[i] / numCols) - (numRows/2)) - ((float)midPointY)/((float)countObjects)) * maximumSize);
					target += left * ((((v[i] % numCols) - (numCols/2)) - ((float)midPointX)/((float)countObjects)) * maximumSize);

					const glm::vec3& pos = EntityHelper(entity).getPosition();

					len += (target.x-pos.x)*(target.x-pos.x) + (target.z-pos.z)*(target.z-pos.z);
				}

				// auswerten
				if (len < minlen)
				{
					minlen = len;
					zielindex = v[ownerindex];
				}
			}
			while (std::next_permutation(v.begin(), v.end()));
		}

		// Positionsoffset bestimmen
		float groupTargetOffsetX = (((zielindex % numCols) - (numCols/2.0f)) - ((float)midPointX)/((float)countObjects)) * maximumSize;
		float groupTargetOffsetY = (((((float)zielindex) / ((float)numCols)) - (numRows/2.0f)) - ((float)midPointY)/((float)countObjects)) * maximumSize;

		// Compute the target point depending on the given target and the current selection group

		// Calculate target position
		result -= dir * groupTargetOffsetY;
		result += left * groupTargetOffsetX;	// order from right to left

		// Apply placement for the resulting position
		{
			qsf::PlacementComponent* placementComponent = caller.getComponent<qsf::PlacementComponent>();
			if (nullptr != placementComponent)
			{
				qsf::TransformComponent* transformComponent = caller.getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					placementComponent->applyPlacement(result, transformComponent->getRotation());
				}
			}
		}

		const float searchRadius = detail::getSearchRadiusForTargetPosition(caller);
		qsf::ai::EGenericObject* eGenericObject = routerComponent->getEGenericObject();
		qsf::ai::ERouteFinder& router = *qsf::ai::ERouteFinder::Instance();
		const qsf::ai::ERouterFlagsSet fapFlags = qsf::ai::ERF_IGNORE_MOVING_OBJECTS;

		if (!router.FindAvailablePosition(eGenericObject, result, position, searchRadius, fapFlags))
		{
			// no available position found -> try again on original position
			result = position;

			if (!router.FindAvailablePosition(eGenericObject, result, searchRadius, fapFlags))
			{
				result = position;
			}
		}

		position = result;
	}

	void MoveCommand::computeMultiSelectionTargetPositionForOrdering(qsf::Entity& caller, int index_, int numOrder_, glm::vec3& position)
	{
		// This function is used if we order a number of units of the same type. That's why we save a lot of calculating of the very similar function computeMultiSelectionTargetPosition()
		qsf::ai::RouterComponent* routerComponent = caller.getComponent<qsf::ai::RouterComponent>();
		if (nullptr == routerComponent)
			return;

		qsf::ai::EGenericObject* gObject = routerComponent->getEGenericObject();
		if (nullptr == gObject)
			return;

		float size = gObject->GetBoundingRadiusXZ() * 1.5f;

		// Zwischenraum zwischen Objekten lassen
		size *= 1.5f;

		int numRows = (int)sqrt((double)numOrder_);
		int numCols = numRows;
		if (numRows*numRows < numOrder_)
			++numCols;

		// neuer Mittelpunkt bestimmen
		int midPointX = 0;
		int midPointY = 0;
		for (int i = 0; i < numOrder_; ++i)
		{
			midPointX += ((i % numCols) - (numCols/2));
			midPointY += ((i / numCols) - (numRows/2));
		}

		// Positionsoffset bestimmen
		glm::vec3 result = position;
		result.x += (((index_ % numCols) - (numCols/2.0f)) - ((float)midPointX)/((float)numOrder_)) * size;
		result.z += (((((float)index_) / ((float)numCols)) - (numRows/2.0f)) - ((float)midPointY)/((float)numOrder_)) * size;

		// Apply placement for the resulting position
		{
			qsf::PlacementComponent* placementComponent = caller.getComponent<qsf::PlacementComponent>();
			if (nullptr != placementComponent)
			{
				qsf::TransformComponent* transformComponent = caller.getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					placementComponent->applyPlacement(result, transformComponent->getRotation());
				}
			}
		}

		const float searchRadius = detail::getSearchRadiusForTargetPosition(caller);
		qsf::ai::ERouteFinder& router = *qsf::ai::ERouteFinder::Instance();
		const qsf::ai::ERouterFlagsSet fapFlags = qsf::ai::ERF_IGNORE_MOVING_OBJECTS;

		if (!router.FindAvailablePosition(gObject, result, position, searchRadius, fapFlags))
		{
			// no available position found -> try again on original position
			result = position;

			if (!router.FindAvailablePosition(gObject, result, searchRadius, fapFlags))
			{
				result = position;
			}
		}

		position = result;
	}

	bool MoveCommand::computeAvailableTargetPosition(qsf::Entity& caller, glm::vec3& position)
	{
		return computeAvailableTargetPosition(caller, position, detail::getSearchRadiusForTargetPosition(caller));
	}

	bool MoveCommand::computeAvailableTargetPosition(qsf::Entity& caller, glm::vec3& position, const float searchRadius)
	{
		qsf::ai::RouterComponent* routerComponent = caller.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != routerComponent)
		{
			qsf::ai::EGenericObject* eGenericObject = routerComponent->getEGenericObject();
			const qsf::ai::ERouterFlagsSet fapFlags = qsf::ai::ERF_IGNORE_MOVING_OBJECTS;

			return qsf::ai::ERouteFinder::Instance()->FindAvailablePosition(eGenericObject, position, searchRadius, fapFlags);
		}

		return false;
	}

	bool MoveCommand::isTargetPositionBlockedByPolygons(qsf::Entity& caller, const glm::vec3& position)
	{
		qsf::ai::RouterComponent* routerComponent = caller.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != routerComponent)
		{
			glm::vec3 pos(position);
			// slightly up the floor, to prevent problems with collision editing not exactly on the terrain
			pos.y += 0.2f;
			qsf::ai::EGenericObject* eGenericObject = routerComponent->getEGenericObject();
			if (qsf::ai::ERouteFinder::Instance()->GetSpeedFactor(eGenericObject, pos) <= 0)
				return true;

			EntityHelper entityHelper(caller);
			if ((entityHelper.isShip() || entityHelper.isBoat()) && !qsf::ai::ERouteFinder::Instance()->IsInWater(position))
				return true;

			// TODO(tl): Checking the right depth because the ai is not able to check it
			if (entityHelper.isShip())
			{
				bool isWaterDeep = false;

				for (WaterDeepTypePolygonComponent* deepWaterPolygonComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<WaterDeepTypePolygonComponent>())
				{
					if (deepWaterPolygonComponent->getWaterDeepType() == WaterDeepTypePolygonComponent::DEEP_WATER &&
						deepWaterPolygonComponent->isPositionInside(position))
					{
						isWaterDeep = true;
						break;
					}
				}

				if (!isWaterDeep)
					return true;
			}
		}

		{
			qsf::MessageConfiguration msg(Messages::EM5_MOVEMENT_BLOCKED_TO_POSITION, caller.getId());
			qsf::MessageParameters params;
			params.setParameter("caller", caller.getId());
			params.setParameter("position", position);
			bool response = false;

			// (oi) Response is only valid when emit returns true
			if (QSF_MESSAGE.emitMessageWithResponse(msg, response, params))
			{
				return response;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
