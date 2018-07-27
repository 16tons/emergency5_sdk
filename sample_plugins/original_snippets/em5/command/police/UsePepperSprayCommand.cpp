// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/UsePepperSprayCommand.h"
#include "em5/action/police/UsePepperSprayAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/targetpoint/UsePepperSprayTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UsePepperSprayCommand::PLUGINABLE_ID = qsf::StringHash("em5::UsePepperSprayCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UsePepperSprayCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UsePepperSprayCommand::UsePepperSprayCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_pepper_spray";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UsePepperSprayCommand::checkAvailable()
	{
		return true;
	}

	bool UsePepperSprayCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_PEPPERSPRAY))
			return false;

		return true;
	}

	bool UsePepperSprayCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// Check for the person component
		const PersonComponent* personComponent = context.mTargetEntity->getComponent<PersonComponent>();
		if (nullptr == personComponent)
			return false;

		// don't spray the usual non-suspects
		EntityHelper targetEntityHelper(*context.mTargetEntity);
		if (targetEntityHelper.isCivilPerson())
			return false;
		if (targetEntityHelper.isCommandable())	// Extra check for policedog
			return false;
		if (targetEntityHelper.isSquadPerson())
			return false;
		if (targetEntityHelper.isPersonInjured())
			return false;
		if (targetEntityHelper.isArrested())
			return false;
		if (targetEntityHelper.isPersonBuried())
			return false;
		if (targetEntityHelper.isPersonContaminated())
			return false;

		return true;
	}

	void UsePepperSprayCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		// Get the transform component for the position
		actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachObjectGoal(*context.mCaller, *context.mTargetEntity, UsePepperSprayTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<UsePepperSprayAction>(ACTION_PRIORITY).init(*context.mTargetEntity);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
