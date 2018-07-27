// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/GetPistolCommand.h"
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
	const uint32 GetPistolCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetPistolCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetPistolCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetPistolCommand::GetPistolCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mButtonPriority = -30;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_pistol";
		mEquipmentName = equipment::POLICE_PISTOL;	// TODO(mk) support SEK_PISTOL here, currently the bone is not right and the icon in the inventory
		mIconSettings.mTooltip = "EM5_Command_51";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_SEK_HANDGUN";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetPistolCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}

	void GetPistolCommand::execute(const qsf::game::CommandContext& context)
	{
		GetEquipmentCommand::execute(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
