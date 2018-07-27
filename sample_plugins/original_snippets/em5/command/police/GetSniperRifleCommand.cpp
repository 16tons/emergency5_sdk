// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/GetSniperRifleCommand.h"
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
	const uint32 GetSniperRifleCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetSniperRifleCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetSniperRifleCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetSniperRifleCommand::GetSniperRifleCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority, so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_sniperrifle";
		mEquipmentName = equipment::POLICE_SNIPERRIFLE;
		mIconSettings.mTooltip = "EM5_Command_52";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_SEK_RIFLE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetSniperRifleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}

	void GetSniperRifleCommand::execute(const qsf::game::CommandContext& context)
	{
		GetEquipmentCommand::execute(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
