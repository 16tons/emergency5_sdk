// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/Command.h"
#include "em5/action/ActionPriority.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	const Command::IconSettings& Command::getIconSettings() const
	{
		return mIconSettings;
	}

	bool Command::highlightTarget() const
	{
		// Default: support command target highlighting
		return true;
	}

	const std::string& Command::getHelpPageName() const
	{
		return mHelpPageName;
	}

	int32 Command::getButtonPriority() const
	{
		return mButtonPriority;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	Command::Command(qsf::game::CommandManager* commandManager, uint32 commandId) :
		qsf::game::Command(commandManager, commandId),
		mButtonPriority(0)
	{
		// Nothing here
	}

	bool Command::checkCallerActionPriorityNoBlocking(qsf::Entity& caller, uint32 maximumPriority) const
	{
		const qsf::ActionComponent* actionComponent = caller.getComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
		{
			// No action component, then every priority is okay
			return true;
		}

		uint32 currentActionPriority = actionComponent->getCurrentPriority();
		// Don't allow other actions if the current action is blocking
		if (currentActionPriority == action::BLOCKING)
			return false;

		// Check if current priority is less or equal to the given maximum priority
		return (currentActionPriority <= maximumPriority);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
