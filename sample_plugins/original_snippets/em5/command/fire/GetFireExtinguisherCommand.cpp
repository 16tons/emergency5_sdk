// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/GetFireExtinguisherCommand.h"
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
	const uint32 GetFireExtinguisherCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetFireExtinguisherCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetFireExtinguisherCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetFireExtinguisherCommand::GetFireExtinguisherCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Lowest button priority, so the button is always at the right (with hotkey 'e')
		mButtonPriority = -50;

		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "firefighters_extinguisher";
		mEquipmentName = equipment::FIRE_EXTINGUISHER;
		mIconSettings.mTooltip = "EM5_Command_59";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_FIREMAN_EXTINGUISHER";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetFireExtinguisherCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		return true;
	}

	void GetFireExtinguisherCommand::execute(const qsf::game::CommandContext& context)
	{
		executeFromVehicle(context);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5