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
#include <boost/container/flat_map.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StringHash;
	class BinarySerializer;
	namespace game
	{
		class GameCounter;
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
		*    Game counter manager class
		*/
		class QSF_GAME_API_EXPORT GameCounterManager : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			GameCounterManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GameCounterManager();

			/**
			*  @brief
			*    Reset all game counters to zero
			*/
			void resetAllCounters();

			/**
			*  @brief
			*    Register a new game counter
			*/
			GameCounter* registerCounter(const std::string& name);

			/**
			*  @brief
			*    Get a game counter by its ID
			*/
			GameCounter* getCounterById(const StringHash& id) const;

			/**
			*  @brief
			*    Get a game counter by its ID
			*/
			GameCounter& getCounterByIdSafe(const StringHash& id) const;

			/**
			*  @brief
			*    Serialization of the timer
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void destroyAllCounters();


		//[-------------------------------------------------------]
		//[ Private declarations                                  ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_map<uint32, GameCounter*> GameCounterMap;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			GameCounterMap mCounters;	///< Map of all game counters, mapped by their ID (which is the hashed name)


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
