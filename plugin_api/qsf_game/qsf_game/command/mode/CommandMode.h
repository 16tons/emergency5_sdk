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
	class Clock;
	namespace game
	{
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
		*    Command mode base class
		*
		*  @todo
		*    - TODO(mk) Think about adding this class to reflection system, cause we have different command modes from different projects
		*/
		class QSF_GAME_API_EXPORT CommandMode : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64> EntityIdSet;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			CommandMode(uint32 commandId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CommandMode();

			/**
			*  @brief
			*    Get command mode ID, which is the command mode's PLUGINABLE_ID
			*/
			uint32 getId() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::CommandMode methods         ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Startup
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void startup();

			/**
			*  @brief
			*    Shutdown
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void shutdown();

			/**
			*  @brief
			*    Process command execution
			*/
			virtual bool executeCommandContext(CommandContext& commandContext, const EntityIdSet& callerIds);

			/**
			*  @brief
			*    Regular update
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void updateCommandMode(const Clock& clock);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mCommandModeId;	///< ID of the concrete command mode instance; should always be the class's PLUGINABLE_ID


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
