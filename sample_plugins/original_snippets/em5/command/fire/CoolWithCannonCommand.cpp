// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/CoolWithCannonCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CoolWithCannonCommand::PLUGINABLE_ID = qsf::StringHash("em5::CoolWithCannonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CoolWithCannonCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CoolWithCannonCommand::CoolWithCannonCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishWithCannonCommand(commandManager, PLUGINABLE_ID)
	{
		mIsCoolCommand = true;

		// Priority is at default value

		// Cool with extinguisher command neither has a gui button, nor does it start a special command mode (triggered by right-click)
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cool_with_cannon";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool CoolWithCannonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (!ExtinguishWithCannonCommand::checkContext(context))
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
