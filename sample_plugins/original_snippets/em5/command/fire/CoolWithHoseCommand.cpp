// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/CoolWithHoseCommand.h"
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
	const uint32 CoolWithHoseCommand::PLUGINABLE_ID = qsf::StringHash("em5::CoolWithHoseCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 CoolWithHoseCommand::ACTION_PRIORITY = action::COMMAND_LOW;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CoolWithHoseCommand::CoolWithHoseCommand(qsf::game::CommandManager* commandManager) :
		ExtinguishWithHoseCommand(commandManager, PLUGINABLE_ID)
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
