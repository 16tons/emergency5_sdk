// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/GetHandcuffsCommand.h"
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
	const uint32 GetHandcuffsCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetHandcuffsCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetHandcuffsCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetHandcuffsCommand::GetHandcuffsCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mButtonPriority = -20;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_handcuffs";
		mEquipmentName = equipment::POLICE_HANDCUFFS_OPEN;
		mIconSettings.mTooltip = "EM5_Command_36";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_POLICEMAN_CUFFS";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetHandcuffsCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
