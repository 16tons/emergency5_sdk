// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/historic/GetSwordCommand.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/fire/FireHoseCheckAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"

#include <qsf_game/command/CommandContext.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GetSwordCommand::PLUGINABLE_ID = qsf::StringHash("em5::GetSwordCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GetSwordCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GetSwordCommand::GetSwordCommand(qsf::game::CommandManager* commandManager) :
		GetEquipmentCommand(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "guard_medieval_sword";
		mEquipmentName = equipment::POLICE_SWORD;
		mIconSettings.mTooltip = "EM5_Command_86";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_GUARDSMAN_ARREST";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool GetSwordCommand::checkContext(const qsf::game::CommandContext& context)
	{
		return (nullptr == context.mTargetEntity && !context.mTargetPosition.is_initialized());
	}

	void GetSwordCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		// The firehose check here is to support a guardsman, connected with a historic well (hydrant). We give an invalid (zero) position to force the disconnecting.
		actionComponent.pushAction<FireHoseCheckAction>(ACTION_PRIORITY).init(glm::vec3(), true);
		actionComponent.pushAction<GetEquipmentAction>(action::BLOCKING).init(mEquipmentName);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
