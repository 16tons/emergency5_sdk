// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace game
	{
		class Command;
		class CommandContext;
		class CommandMode;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Command delegate base class
		*/
		class QSF_GAME_API_EXPORT CommandDelegate : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline CommandDelegate();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CommandDelegate();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::CommandDelegate methods     ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Callback to determine whether commands are currently allowed at all
			*/
			inline virtual bool areCommandsAllowed();

			/**
			*  @brief
			*    Callback to determine whether a certain commands is currently allowed to be checked and executed
			*/
			inline virtual bool isCommandAllowed(const Command& command);

			/**
			*  @brief
			*    Callback to determine whether a certain caller entity is currently allowed to be given commands
			*/
			inline virtual bool isCallerAllowed(Entity& caller, const qsf::game::CommandContext& context);

			/**
			*  @brief
			*    Callback to determine whether the target given as part of the context is currently a valid command target
			*/
			inline virtual bool isTargetAllowed(const CommandContext& context);

			/**
			*  @brief
			*    Callback to determine whether the complete command context is currently allowed to be executed
			*
			*  @remarks
			*    You may also manipulate the context here - but use this with care!
			*/
			inline virtual bool preCommandExecution(const Command& command, const std::vector<Entity*>& callers, CommandContext& context);

			/**
			*  @brief
			*    Callback to determine whether the complete command context is currently allowed to be executed
			*
			*  @remarks
			*    You may also manipulate the context here - but use this with care!
			*/
			inline virtual bool preCommandExecution(const Command& command, CommandContext& context);

			/**
			*  @brief
			*    Callback to inform the delegate when a command was just executed
			*/
			inline virtual void postCommandExecution(const Command& command, const CommandContext& context);

			/**
			*  @brief
			*    Return the CommandMode which should be used as default
			*
			*  @remarks
			*    the returned pointer must be valid. The ownership of the returned instance goes to the caller
			*/
			virtual CommandMode* getDefaultCommandMode();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/command/CommandDelegate-inl.h"
