// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/platform/PlatformTypes.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


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
		*    Static command evaluation helper class
		*/
		class QSF_GAME_API_EXPORT CommandEvaluationHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64> EntityIdSet;
			typedef std::vector<Entity*> EntityArray;

			/** Storage of the set of valid caller entities for a given command */
			struct CommandInfo
			{
				Command* mCommand;
				EntityArray mCallers;
			};
			typedef boost::container::flat_map<uint32, CommandInfo> CommandInfoMap;
			typedef std::vector<CommandInfo> CommandInfoArray;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			// Command context related
			static void writeAllowSelfExecution(const EntityArray& callers, CommandContext& context);

			// Checks & filtering
			static void getPossibleCommandsForSelectionIdSet(const EntityIdSet& idSet, CommandInfoArray& outCommands);
			static bool doCheckCallerFiltering(Command& command, EntityArray& callers, const CommandContext& context);
			static bool doCheckContextFiltering(Command& command, EntityArray& callers, CommandContext& context);

			// Command excution
			static bool executeCommand(Command& command, const EntityArray& callers, CommandContext& context);

			/**
			*  @brief
			*    Returns an CommandInfo object with the command which is executable in the given context
			*
			*  @returns
			*    A CommandInfo object on success.
			*/
			static const boost::optional<CommandInfo> getCommandForContext(CommandContext& commandContext, const EntityIdSet& idSet, const Command* toBeCheckedCommand);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Utility functions
			static bool compareCommandInfosByPriority(const CommandInfo& commandInfo1, const CommandInfo& commandInfo2);
			static void getEntityArrayFromIdSet(const EntityIdSet& idSet, EntityArray& outEntities);

			static void buildCommandInfoMapForCallers(const EntityArray& callers, CommandInfoMap& outCommandInfoMap);
			static void getCommandInfoArrayFromMap(const CommandInfoMap& commandInfoMap, CommandInfoArray& outCommands);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
