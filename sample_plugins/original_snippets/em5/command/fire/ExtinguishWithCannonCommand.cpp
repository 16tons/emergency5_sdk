// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishWithCannonCommand.h"
#include "em5/component/polygon/WaterDeepTypePolygonComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/game/targetpoint/ExtinguishTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
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
	const uint32 ExtinguishWithCannonCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExtinguishWithCannonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithCannonCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishWithCannonCommand::ExtinguishWithCannonCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishCoolBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}

	ExtinguishWithCannonCommand::ExtinguishWithCannonCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		ExtinguishCoolBaseCommand(commandManager, commandId)
	{
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExtinguishWithCannonCommand::checkAvailable()
	{
		return true;
	}

	bool ExtinguishWithCannonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadVehicle())
			return false;

		// Check if entity is on fire or not
		if (entityHelper.isBurning())
			return false;

		return true;
	}

	bool ExtinguishWithCannonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (!checkTarget(context))
			return false;

		// TODO(tl): Hack to prevent ships from cooling unreachable targets
		if (EntityHelper(*context.mCaller).isShip())
		{
			if (context.mCaller != nullptr && context.mTargetEntity != nullptr)
			{
				bool response = true;
				QSF_MESSAGE.emitMessageWithResponse(qsf::MessageConfiguration(Messages::EM5_COMMAND_EXTINGUISH_BY_BOAT_CHECK_TARGET, context.mCaller->getId(), context.mTargetEntity->getId()), response);
				if (!response)
				{
					// Filtered out by message
					return false;
				}
			}

			const glm::vec3& targetPosition = EntityHelper(*context.mTargetEntity).getPosition();
			const float extinguishRangeTLF = FireSpecsGroup::getInstanceSafe().getRangeFireboatMax() + 15.0f; // Added an offset to avoid the inaccuracy between object position and fire receiver position

			bool unreachable = true;
			for (WaterDeepTypePolygonComponent* deepWaterPolygonComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<WaterDeepTypePolygonComponent>())
			{
				// Ignore all polygons that are not deep water
				if (deepWaterPolygonComponent->getWaterDeepType() == WaterDeepTypePolygonComponent::DEEP_WATER)
				{
					// Inside polygon?
					if (deepWaterPolygonComponent->isPositionInside(targetPosition))
					{
						unreachable = false;
						break;
					}

					// Calculate distance to polygon
					float distanceSquared = 0.0f;
					{
						// For this calculation, we use the polygon's local coordinate system
						const qsf::Transform& transform = deepWaterPolygonComponent->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
						const glm::vec3 localTargetPosition = transform.vec3PositionWorldToLocal(targetPosition);

						glm::vec3 closestPoint;
						deepWaterPolygonComponent->getClosestSegmentIndexToPoint(localTargetPosition, &closestPoint);
						closestPoint.y = localTargetPosition.y;

						distanceSquared = glm::distance2(closestPoint, localTargetPosition);
					}

					// If the target is away from deep water we should not try to extinguish it
					if (distanceSquared < extinguishRangeTLF * extinguishRangeTLF)
					{
						unreachable = false;
						break;
					}
				}
			}

			if (unreachable)
				return false;
		}

		return true;
	}

	void ExtinguishWithCannonCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const ExtinguishFireAction* extinguishFireAction = actionComponent.getAction<ExtinguishFireAction>();
		if (nullptr != extinguishFireAction && extinguishFireAction->getTargetEntityId() == context.mTargetEntity->getId())
		{
			// Don't push the same command twice
			return;
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		qsf::ai::NavigationComponent* navigationComponent = context.mCaller->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent && navigationComponent->isActive())
		{
			// Move only if its possible (with connected firefighters the component is deactivated)
			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, ExtinguishTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS);
		}

		// TODO(mk) Find fire Power by vehicle type, or read it from the cannoncomponent
		float extinguishPower = FireSpecsGroup::getInstanceSafe().getPowerTLF();
		float coolingPower = FireSpecsGroup::getInstanceSafe().getCoolingTLF();

		if (UnitHelper(*context.mCaller).isFireFighterFLB())
		{
			extinguishPower = FireSpecsGroup::getInstanceSafe().getPowerFireboat();
			coolingPower = FireSpecsGroup::getInstanceSafe().getCoolingFireboat();
		}


		actionComponent.pushAction<ExtinguishFireAction>(ACTION_PRIORITY).init(*context.mTargetEntity, extinguishPower, coolingPower);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
