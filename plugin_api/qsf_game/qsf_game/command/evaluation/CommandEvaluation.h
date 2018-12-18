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
#include <boost/container/flat_set.hpp>


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
		class CommandManager;
		class CommandDelegate;
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
		*    Command evaluation class
		*/
		class QSF_GAME_API_EXPORT CommandEvaluation : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64> EntityIdSet;
			typedef std::vector<Entity*> EntityArray;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			CommandEvaluation();

			/**
			*  @brief
			*    Process command evaluation
			*
			*  @remarks
			*    This includes choosing a command for the given selection and target, and executing it.
			*/
			bool chooseAndExecuteCommand(CommandContext& context, const EntityIdSet& callerIds) const;

			/**
			*  @brief
			*    Process command evaluation with a fixed command
			*
			*  @remarks
			*    This includes checking the command for the given selection and target, and executing it.
			*/
			bool checkAndExecuteCommand(const Command& command, CommandContext& context, const EntityIdSet& callerIds) const;

			/**
			*  @brief
			*    Processing of a whole command
			*
			*  @remarks
			*    Can called by automatism.
			*/
			bool processCommand(Command& command, EntityArray& callers, CommandContext& context) const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Utility functions
			bool checkAndProcessCommand(CommandContext& context, const EntityIdSet& callerIds, const Command* toBeCheckedCommand) const;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
