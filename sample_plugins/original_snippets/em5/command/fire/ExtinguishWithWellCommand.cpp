// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishWithWellCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/fire/FireHelper.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/fire/ConnectHydrantAction.h"
#include "em5/action/fire/DisconnectHydrantAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/objects/HistoricWellComponent.h"
#include "em5/game/targetpoint/ExtinguishTargetPointProvider.h"
#include "em5/game/targetpoint/HydrantTargetPointProvider.h"
#include "em5/game/player/Player.h"
#include "em5/logic/HintHelper.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithWellCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExtinguishWithWellCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithWellCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishWithWellCommand::ExtinguishWithWellCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishCoolBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";

		mIsActive = true;
	}

	ExtinguishWithWellCommand::ExtinguishWithWellCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		ExtinguishCoolBaseCommand(commandManager, commandId)
	{
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";

		mIsActive = true;
	}

	void ExtinguishWithWellCommand::setActive(bool active)
	{
		mIsActive = active;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExtinguishWithWellCommand::checkAvailable()
	{
		return mIsActive;
	}

	bool ExtinguishWithWellCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			if (isNoQueueingCommand(caller))
			{
				return false;
			}
		}

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		return true;
	}

	bool ExtinguishWithWellCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// For performance reasons safe the result of the search in a list and use it as lookup table
		static std::map<uint64, bool> historicWellInRange;	// target entityId to bool (true if well is in range)

		auto searchResult = historicWellInRange.find(context.mTargetEntity->getId());
		if (searchResult == historicWellInRange.end())
		{
			// Not in list, insert new
			std::vector<HistoricWellComponent*> historicWellComponents;
			const qsf::TransformComponent* targetTransformComponent = context.mTargetEntity->getComponent<qsf::TransformComponent>();
			const float searchRadius = FireSpecsGroup::getInstanceSafe().getRangeHoseMax() + 1.f ;
			qsf::ComponentMapQuery(context.mTargetEntity->getMap()).getInstancesInCircle(targetTransformComponent->getPosition(), searchRadius, historicWellComponents);

			historicWellInRange.insert(std::pair<uint64, bool>(context.mTargetEntity->getId(), !historicWellComponents.empty()));

			// Check for well in range
			if (historicWellComponents.empty())
				return false;
		}
		else if (searchResult->second == false)
		{
			// Target is not in range to a known well
			return false;
		}

		return checkTarget(context);
	}

	void ExtinguishWithWellCommand::execute(const qsf::game::CommandContext& context)
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

		if (isNoQueueingCommand(*context.mCaller))
		{
			actionComponent.clearPlan();
		}

		bool searchNewTarget = true;
		const qsf::Entity* oldHydrantEntity = FireHelper::getConnectedHydrantFromEntity(*context.mCaller);
		if (nullptr != oldHydrantEntity)
		{
			// Check if the target is in range of the hydrant
			const FireSpecsGroup& fireSpecsGroup = FireSpecsGroup::getInstanceSafe();
			const glm::vec3& targetPosition = EntityHelper(*context.mTargetEntity).getPosition();
			if (FireHelper::isHydrantInRange(*oldHydrantEntity, targetPosition, fireSpecsGroup.getRangeHoseMax()))	// Remarks, here is a slightly different to the ExtinguishWithHose class
			{
				// We can keep the old hydrant
				actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, ExtinguishTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<ExtinguishFireAction>(ACTION_PRIORITY).init(*context.mTargetEntity, fireSpecsGroup.getPowerHose(), fireSpecsGroup.getCoolingHose());
				searchNewTarget = false;
			}
			else
			{
				// Disconnect from old hydrant
				actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *oldHydrantEntity, HydrantTargetPointProvider::TARGET_POINT_ID));
				actionComponent.pushAction<TurnToAction>(action::BLOCKING).init(oldHydrantEntity->getId());
				actionComponent.pushAction<DisconnectHydrantAction>(action::BLOCKING).init(*oldHydrantEntity);
			}
		}

		if (searchNewTarget)
		{
			const bool foundHydrant = connectToFreeHydrant(*context.mCaller, *context.mTargetEntity);

			if (!foundHydrant)
			{
				const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
				if (em5CommandContext.mIsAutomatism == false)
				{
					uint32 playerIndex = (nullptr != em5CommandContext.mPlayer ? em5CommandContext.mPlayer->getPlayerIndex() : qsf::getUninitialized<uint32>());
					HintHelper::showHint("EM2017_ME200_GUARD_NOHYDRANT", playerIndex, qsf::Time::fromSeconds(30.0f));
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool ExtinguishWithWellCommand::connectToFreeHydrant(qsf::Entity& caller, qsf::Entity& target) const
	{
		const FireSpecsGroup& fireSpecsGroup = FireSpecsGroup::getInstanceSafe();
		bool foundAtLeastOneInRange = false;
		const qsf::Entity* newHydrantEntity = FireHelper::findFreeHydrantNearEntity(caller, target, foundAtLeastOneInRange);
		if (nullptr != newHydrantEntity)
		{
			// Get the action component without cleaning the plan!
			qsf::ActionComponent& actionComponent = getActionComponent(caller);

			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(caller, *newHydrantEntity, HydrantTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<TurnToAction>(action::BLOCKING).init(newHydrantEntity->getId());
			actionComponent.pushAction<ConnectHydrantAction>(action::BLOCKING).init(*newHydrantEntity);
			actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(caller, target, ExtinguishTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<ExtinguishFireAction>(ACTION_PRIORITY).init(target, fireSpecsGroup.getPowerHose(), fireSpecsGroup.getCoolingHose());

			// Everything alright
			return true;
		}

		// Boo, failure!
		return false;
	}

	bool ExtinguishWithWellCommand::isNoQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction)
		{
			const std::string& equipmentName = getEquipmentAction->getEquipmentName();
			if (equipmentName == equipment::HOSE_ROLLED || equipmentName == equipment::HOSE_NOZZLE ||
				equipmentName == equipment::HOSE_ROLLED_HISTORICAL || equipmentName == equipment::HOSE_NOZZLE_HISTORICAL)
			{
				return false;
			}
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
