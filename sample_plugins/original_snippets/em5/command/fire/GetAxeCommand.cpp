// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/GetAxeCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GetAxeCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetAxeCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetAxeCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetAxeCommand::GetAxeCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "firefighters_axe";
		mEquipmentName = equipment::AXE;
		mIconSettings.mTooltip = "EM5_Command_63";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_FIREMAN_AXE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetAxeCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		return true;
	}

	void GetAxeCommand::execute(const qsf::game::CommandContext& context)
	{
		executeFromVehicle(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
