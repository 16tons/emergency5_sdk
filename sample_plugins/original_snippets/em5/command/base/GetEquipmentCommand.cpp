// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/GetEquipmentCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/equipment/DropEquipmentAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/game/targetpoint/GetEquipmentTargetPointProvider.h"
#include "em5/game/player/Player.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/time/Time.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetEquipmentCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetEquipmentCommand::GetEquipmentCommand(qsf::game::CommandManager* commandManager, uint32 pluginableId) :
		Command(commandManager, pluginableId)
	{
		// Priority is at default value

		// No icon settings are set, nor does it start a special command mode (triggered by right-click)
	}

	const std::string& GetEquipmentCommand::getEquipmentName()
	{
		return mEquipmentName;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetEquipmentCommand::checkAvailable()
	{
		return true;
	}

	bool GetEquipmentCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Hands are full
		if (entityHelper.isCarrying())
			return false;

		// Already got that equipment
		if (entityHelper.checkIsActiveEquipment(mEquipmentName))
			return false;

		return true;
	}

	void GetEquipmentCommand::execute(const qsf::game::CommandContext& context)
	{
		// Simple implementation for direct switching to equipment
		startAction(context);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void GetEquipmentCommand::executeFromVehicle(const qsf::game::CommandContext& context)
	{
		// Find a vehicle component
		const RoadVehicleComponent* roadVehicleComponent = findVehicleComponent(context);
		if (nullptr != roadVehicleComponent)
		{
			startAction(context, roadVehicleComponent->getEntity());
		}
		else
		{
			// If vehicle is too far away or not available, tell the player about it
			const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);
			if (em5CommandContext.mIsAutomatism == false)
			{
				uint32 playerIndex = (nullptr != em5CommandContext.mPlayer ? em5CommandContext.mPlayer->getPlayerIndex() : qsf::getUninitialized<uint32>());
				HintHelper::showHint("ID_HINT_FIREFIGHTER_NORW", playerIndex, qsf::Time::fromSeconds(30.0f));

				#ifndef ENDUSER
					// Draw information that no fire truck was found
					QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("There is no fire truck near to me!",
						qsf::EntityHelper(*context.mCaller).getPosition(), qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
				#endif
			}
		}
	}

	void GetEquipmentCommand::startAction(const qsf::game::CommandContext& context, const qsf::Entity& targetEntity) const
	{
		// TODO(mk) Add check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();
		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(targetEntity, true);
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, targetEntity, GetEquipmentTargetPointProvider::TARGET_POINT_ID));

		if (mEquipmentName.empty())
			actionComponent.pushAction<DropEquipmentAction>(action::BLOCKING).init(qsf::Time::ZERO);
		else
			actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING).init(targetEntity, mEquipmentName);
	}

	void GetEquipmentCommand::startAction(const qsf::game::CommandContext& context) const
	{
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		if (mEquipmentName.empty())
			actionComponent.pushAction<DropEquipmentAction>(action::BLOCKING).init(qsf::Time::ZERO);
		else
			actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING).init(mEquipmentName);
	}

	RoadVehicleComponent* GetEquipmentCommand::findVehicleComponent(const qsf::game::CommandContext& context) const
	{
		return EntityHelper(*context.mCaller).getReachableRoadVehicleWithEquipment(mEquipmentName);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
