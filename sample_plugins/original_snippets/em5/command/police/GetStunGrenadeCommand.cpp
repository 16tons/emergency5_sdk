// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/GetStunGrenadeCommand.h"
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
	const uint32 GetStunGrenadeCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetStunGrenadeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetStunGrenadeCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetStunGrenadeCommand::GetStunGrenadeCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		mButtonPriority = -50;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_stungrenade";
		mEquipmentName = equipment::POLICE_STUN_GRENADE;
		mIconSettings.mTooltip = "EM5_Command_58";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_SEK_FLASHBANG";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetStunGrenadeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
