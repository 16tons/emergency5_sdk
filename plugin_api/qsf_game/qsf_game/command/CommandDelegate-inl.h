// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CommandDelegate::CommandDelegate()
		{
			// Nothing here
		}

		inline CommandDelegate::~CommandDelegate()
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::CommandDelegate methods     ]
		//[-------------------------------------------------------]
		inline bool CommandDelegate::areCommandsAllowed()
		{
			// Default implementation: No filtering
			return true;
		}

		inline bool CommandDelegate::isCommandAllowed(const Command& command)
		{
			// Default implementation: No filtering
			return true;
		}

		inline bool CommandDelegate::isCallerAllowed(Entity& caller, const qsf::game::CommandContext& context)
		{
			// Default implementation: No filtering
			return true;
		}

		inline bool CommandDelegate::isTargetAllowed(const CommandContext& context)
		{
			// Default implementation: No filtering
			return true;
		}

		inline bool CommandDelegate::preCommandExecution(const Command& command, const std::vector<Entity*>& callers, CommandContext& context)
		{
			// Default implementation: No filtering or changes to the context
			return true;
		}

		inline bool CommandDelegate::preCommandExecution(const Command& command, CommandContext& context)
		{
			// Default implementation: No filtering or changes to the context
			return true;
		}

		inline void CommandDelegate::postCommandExecution(const Command& command, const CommandContext& context)
		{
			// Default implementation is empty
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
