// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishWithHoseCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/fire/FireHelper.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/fire/ExtinguishFireAction.h"
#include "em5/action/fire/ConnectHydrantAction.h"
#include "em5/action/fire/ChangeMaskAction.h"
#include "em5/action/fire/DisconnectHydrantAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/EquipmentAssets.h"
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
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithHoseCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExtinguishWithHoseCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishWithHoseCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishWithHoseCommand::ExtinguishWithHoseCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishCoolBaseCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}

	ExtinguishWithHoseCommand::ExtinguishWithHoseCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		ExtinguishCoolBaseCommand(commandManager, commandId)
	{
		// Nothing here
		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "extinguish_fire";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExtinguishWithHoseCommand::checkAvailable()
	{
		return true;
	}

	bool ExtinguishWithHoseCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
		{
			if (!isQueueingCommand(caller))
			{
				return false;
			}
		}

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Handle case firefighter has not always an rolled hose with him
		if (!(entityHelper.checkIsActiveEquipment(equipment::HOSE_ROLLED) ||
			entityHelper.checkIsActiveEquipment(equipment::HOSE_NOZZLE) ||
			entityHelper.checkIsActiveEquipment(equipment::HOSE_ROLLED_HISTORICAL) ||
			entityHelper.checkIsActiveEquipment(equipment::HOSE_NOZZLE_HISTORICAL)))
		{
			if (!isQueueingCommand(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool ExtinguishWithHoseCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return checkTarget(context);
	}

	void ExtinguishWithHoseCommand::execute(const qsf::game::CommandContext& context)
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

		if (!isQueueingCommand(*context.mCaller))
		{
			actionComponent.clearPlan();
		}

		bool searchNewTarget = true;
		const qsf::Entity* oldHydrantEntity = FireHelper::getConnectedHydrantFromEntity(*context.mCaller);
		if (nullptr != oldHydrantEntity)
		{
			// Check if the target is in range of the hydrant
			const FireSpecsGroup& fireSpecsGroup = FireSpecsGroup::getInstanceSafe();

			// Get maximum extinguish distance
			// -> Increase the maximum range to avoid problems with targets at the border of the normal range. We don't take it too serious here.
			// (also see "ExtinguishFireAction::onStartup" where the same is done)
			const float maxExtinguishRange = FireHelper::getExtinguishRangeFromEntity(*context.mCaller) * 1.4f;
			qsf::Entity* effectEntity = FireHelper::getEffectTargetInRange(*context.mCaller, *context.mTargetEntity, maxExtinguishRange);

			// Get position from effect entity we want to extinguish, if we have one (see EM_2017 bug 1450)
			const glm::vec3& targetPosition = EntityHelper((nullptr == effectEntity) ? *context.mTargetEntity : *effectEntity).getPosition();
			if (FireHelper::isHydrantInRange(*oldHydrantEntity, targetPosition, fireSpecsGroup.getRangeHydrant()))
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
				actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(oldHydrantEntity->getId());
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
					HintHelper::showHint("ID_HINT_FIREFIGHTER_NOHYDRANT", playerIndex, qsf::Time::fromSeconds(30.0f));
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool ExtinguishWithHoseCommand::connectToFreeHydrant(qsf::Entity& caller, qsf::Entity& target) const
	{
		const FireSpecsGroup& fireSpecsGroup = FireSpecsGroup::getInstanceSafe();
		bool foundAtLeastOneInRange = false;
		const qsf::Entity* newHydrantEntity = FireHelper::findFreeHydrantNearEntity(caller, target, foundAtLeastOneInRange);
		if (nullptr == newHydrantEntity)
			return false;	// Boo, failure!

		// Get the action component without cleaning the plan!
		qsf::ActionComponent& actionComponent = getActionComponent(caller);

		// The change mask action will not perform for every unit
		actionComponent.pushAction<ChangeMaskAction>(ACTION_PRIORITY).init(ChangeMaskAction::ADD_MASK);

		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(caller, *newHydrantEntity, HydrantTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<TurnToAction>(ACTION_PRIORITY).init(newHydrantEntity->getId());
		actionComponent.pushAction<ConnectHydrantAction>(action::BLOCKING).init(*newHydrantEntity);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(caller, target, ExtinguishTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<ExtinguishFireAction>(ACTION_PRIORITY).init(target, fireSpecsGroup.getPowerHose(), fireSpecsGroup.getCoolingHose());

		// Everything alright
		return true;
	}

	bool ExtinguishWithHoseCommand::isQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction)
		{
			const std::string& equipmentName = getEquipmentAction->getEquipmentName();
			if (equipmentName == equipment::HOSE_ROLLED ||
				equipmentName == equipment::HOSE_NOZZLE ||
				equipmentName == equipment::HOSE_ROLLED_HISTORICAL ||
				equipmentName == equipment::HOSE_NOZZLE_HISTORICAL)
			{
				return true;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
