// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/GetHoseCommand.h"
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
	const uint32 GetHoseCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetHoseCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetHoseCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetHoseCommand::GetHoseCommand(qsf::game::CommandManager* commandManager) :
		em5::GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority, so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "firefighters_firehose-nozzle";	// Use nozzle graphics, as the firehose graphics looks weird (like a grenade)
		mEquipmentName = equipment::HOSE_ROLLED;
		mIconSettings.mTooltip = "EM5_Command_60";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_FIREMAN_FIREHOSE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetHoseCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		return true;
	}

	void GetHoseCommand::execute(const qsf::game::CommandContext& context)
	{
		executeFromVehicle(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
