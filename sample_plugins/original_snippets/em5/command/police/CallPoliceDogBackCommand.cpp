// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/CallPoliceDogBackCommand.h"
#include "em5/action/ActionPriority.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CallPoliceDogBackCommand::PLUGINABLE_ID = qsf::StringHash("em5::CallPoliceDogBackCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CallPoliceDogBackCommand::ACTION_PRIORITY = action::BLOCKING;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CallPoliceDogBackCommand::CallPoliceDogBackCommand(qsf::game::CommandManager* commandManager) :
		CancelDogSearchCommand(commandManager, PLUGINABLE_ID)
	{
		// Second lowest button priority (SendDogForSearchCommand is lower), so the button is always the second place at right (with hotkey 'r')
		// -> Identical to "em5::CancelDogSearchCommand"
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "send_dog_cancel";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cancel_dog_search";
		mIconSettings.mTooltip = "EM5_Command_84";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_POLICEDOG_CALL";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
