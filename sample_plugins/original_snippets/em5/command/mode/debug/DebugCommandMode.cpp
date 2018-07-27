// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/DebugCommandMode.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/EM5Helper.h"

#include <qsf/gui/GuiContext.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	DebugCommandMode::DebugCommandMode(uint32 commandId) :
		qsf::game::CommandMode(commandId)
	{
		// Nothing here
	}

	DebugCommandMode::~DebugCommandMode()
	{
		// Nothing here
	}

	FreeplayEvent* DebugCommandMode::getCurrentFreeplayEvent() const
	{
		return EM5_FREEPLAY.getEventManager().getLastEvent();
	}

	glm::vec2 DebugCommandMode::getMousePosition() const
	{
		return EM5_GUI.getGuiContext().getMouseCursorPosition();
	}

	glm::vec2 DebugCommandMode::getNormalizedMousePosition() const
	{
		return EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
