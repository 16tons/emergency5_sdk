// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/DLK/DLKCoolCommand.h"
#include "em5/action/fire/DLK/DLKDeployAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/map/EntityHelper.h"
#include "em5/command/CommandContext.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DLKCoolCommand::PLUGINABLE_ID = qsf::StringHash("em5::DLKCoolCommand");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKCoolCommand::DLKCoolCommand(qsf::game::CommandManager* commandManager) :
		DLKExtinguishCommand(commandManager)
	{
		mIsCoolCommand = true;

		// Cool with extinguisher command neither has a gui button, nor does it start a special command mode (triggered by right-click)
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cool_with_cannon";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DLKCoolCommand::checkAvailable()
	{
		return true;
	}

	bool DLKCoolCommand::checkCaller(qsf::Entity& caller)
	{
		return checkCallerActionPriority(caller, action::COMMAND_STD);
	}

	bool DLKCoolCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Check in base class
		if (!checkTarget(context))
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
