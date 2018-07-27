// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/GetPepperSprayCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"

#include <qsf_game/command/CommandContext.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GetPepperSprayCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetPepperSprayCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetPepperSprayCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetPepperSprayCommand::GetPepperSprayCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		mButtonPriority = -50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_pepperspray";
		mEquipmentName = equipment::POLICE_PEPPERSPRAY;
		mIconSettings.mTooltip = "EM5_Command_35";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_EQUIP_POLICE_PEPPERSPRAY";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetPepperSprayCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
