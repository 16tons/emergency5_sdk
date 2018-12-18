// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/CoolWithWellCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/action/ActionPriority.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CoolWithWellCommand::PLUGINABLE_ID = qsf::StringHash("em5::CoolWithWellCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CoolWithWellCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CoolWithWellCommand::CoolWithWellCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishWithWellCommand(commandManager, PLUGINABLE_ID)
	{
		mIsCoolCommand = true;

		// Priority is at default value

		// Cool with extinguisher command neither has a gui button, nor does it start a special command mode (triggered by right-click)
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mStartCommandMode = false;

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "cool_with_hose";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
