// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/CoolWithExtinguisherCommand.h"
#include "em5/action/ActionPriority.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CoolWithExtinguisherCommand::PLUGINABLE_ID = qsf::StringHash("em5::CoolWithExtinguisherCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CoolWithExtinguisherCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CoolWithExtinguisherCommand::CoolWithExtinguisherCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishWithExtinguisherCommand(commandManager, PLUGINABLE_ID)
	{
		mIsCoolCommand = true;

		// Priority is at default value

		// TODO(sw) An extinguisher shoudn't be usable to cool things as by the concept
		// Cool with extinguisher command neither has a gui button, nor does it start a special command mode (triggered by right-click)
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cool_with_extinguisher";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
