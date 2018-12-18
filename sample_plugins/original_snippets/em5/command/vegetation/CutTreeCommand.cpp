// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/vegetation/CutTreeCommand.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/vegetation/CutTreeAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/targetpoint/CutTreeTargetPointProvider.h"
#include "em5/game/targetpoint/CutHedgeTargetPointProvider.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/input/device/KeyboardDevice.h>
#include <qsf/input/InputSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CutTreeCommand::PLUGINABLE_ID = qsf::StringHash("em5::CutTreeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CutTreeCommand::ACTION_PRIORITY = action::COMMAND_LOW;
	const uint32 CutTreeCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CutTreeCommand::CutTreeCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cut_tree";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CutTreeCommand::checkAvailable()
	{
		return true;
	}

	bool CutTreeCommand::checkCaller(qsf::Entity& caller)
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

		// Check if equipment matches
		if (!(entityHelper.checkIsActiveEquipment(equipment::CHAIN_SAW) || entityHelper.checkIsActiveEquipment(equipment::CHAIN_SAW_HISTORICAL)))
		{
			if (!isQueueingCommand(caller))
			{
				return false;
			}
		}

		return true;
	}

	bool CutTreeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr == context.mTargetEntity)
			return false;

		// Is target reserved
		if (!ReserveLogic::checkReservation(*context.mTargetEntity, context.mCaller->getId(), ReserveLogic::MOVETO))
			return false;

		const TreeComponent* treeComponent = context.mTargetEntity->getComponent<TreeComponent>();
		if (nullptr != treeComponent)
		{
			// Check if the tree is already cut
			if (!treeComponent->isIntact())
				return false;
		}
		else
		{
			const HedgeComponent* hedgeComponent = context.mTargetEntity->getComponent<HedgeComponent>();
			if (nullptr != hedgeComponent)
			{
				// Check if already cut
				if (!hedgeComponent->isIntact())
					return false;
			}
			else
			{
				BuryComponent* buryComponent = context.mTargetEntity->getComponent<BuryComponent>();
				if (nullptr != buryComponent && buryComponent->getCutableBySaw() && !buryComponent->getIsHidden())
				{
					// The tree is marked for cutting
					return true;
				}

				// No tree, no hedge, no action
				return false;
			}
		}

		FireComponent* fireComponent = context.mTargetEntity->getComponent<FireComponent>();
		if (nullptr != fireComponent)
		{
			if (fireComponent->isBurning() || fireComponent->isBurned())
			{
				return false;
			}
		}
		// else, entities without fire component are valid

		return true;
	}

	void CutTreeCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::Entity& targetEntity = *context.mTargetEntity;

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const CutTreeAction* cutTreeAction = actionComponent.getAction<CutTreeAction>();
		if (nullptr != cutTreeAction && cutTreeAction->getTargetEntityId() == targetEntity.getId())
		{
			// Don't push the same command twice
			return;
		}

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		if (!isQueueingCommand(*context.mCaller))
		{
			// Clear plan before the actions are pushed
			actionComponent.clearPlan();
		}

		// Define move target point
		uint32 targetPointId = (nullptr != targetEntity.getComponent<TreeComponent>() ? CutTreeTargetPointProvider::TARGET_POINT_ID : CutHedgeTargetPointProvider::TARGET_POINT_ID);
		qsf::Entity* moveTargetEntity = &targetEntity;

#if 0
		// Check if there is a special "cut_target" child defining exactly where to go
		// (fw) This feature is actually not used yet, but code could be useful later on
		qsf::LinkComponent* linkComponent = targetEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
			{
				if (EntityHelper(childLinkComponent->getEntity()).getName() == "cut_target")
				{
					moveTargetEntity = &childLinkComponent->getEntity();
					// TODO(fw): Choose a good target point provider, should be something like "move to exactly the given position" (it's strange that we don't have this already!)
					break;
				}
			}
		}
#endif

		actionComponent.pushAction<MoveAction>(usedActionPriority).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *moveTargetEntity, targetPointId));
		actionComponent.pushAction<CutTreeAction>(usedActionPriority).init(targetEntity);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool CutTreeCommand::isQueueingCommand(qsf::Entity& firefighterEntity)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(firefighterEntity);

		// Check the actions on his plan, but not entering vehicle
		const GetEquipmentAction* getEquipmentAction = actionComponent.getAction<GetEquipmentAction>();
		if (nullptr != getEquipmentAction)
		{
			const std::string& equipmentName = getEquipmentAction->getEquipmentName();
			if (equipmentName == equipment::CHAIN_SAW ||
				equipmentName == equipment::CHAIN_SAW_HISTORICAL)
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
