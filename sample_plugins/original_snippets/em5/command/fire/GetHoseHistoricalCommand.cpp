// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/GetHoseHistoricalCommand.h"
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
	const uint32 GetHoseHistoricalCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetHoseHistoricalCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetHoseHistoricalCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetHoseHistoricalCommand::GetHoseHistoricalCommand(qsf::game::CommandManager* commandManager) :
		em5::GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Second lowest button priority, so the button is always the second place at right (with hotkey 'r')
		mButtonPriority = -40;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "firefighters_historical_firehose_nozzle";	// Here Use nozzle graphics
		mEquipmentName = equipment::HOSE_ROLLED_HISTORICAL;
		mIconSettings.mTooltip = "EM5_Command_76";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_FIREMAN_FIREHOSE";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetHoseHistoricalCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		return true;
	}

	void GetHoseHistoricalCommand::execute(const qsf::game::CommandContext& context)
	{
		executeFromVehicle(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
