// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/timer/GameTimer.h"

#include <qsf/base/manager/ElementManager.h>

#include <vector>


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
		*    Game timer manager
		*/
		class QSF_GAME_API_EXPORT GameTimerManager : private ElementManager<uint32, GameTimer>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			GameTimerManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GameTimerManager();

			/**
			*  @brief
			*    Add a new game timer
			*
			*  @param[in] messageConfiguration
			*    Defines the message that is emitted when the game timer fires
			*  @param[in] waitTime
			*    Simulation time until the timer fires
			*
			*  @return
			*    The newly created game timer's unique ID
			*/
			uint32 addTimer(const MessageConfiguration& messageConfiguration, Time waitTime);

			/**
			*  @brief
			*    Invalidate and remove a game timer
			*
			*  @param[in] timerId
			*    The game timer's unique ID
			*
			*  @return
			*    "true" if the game timer was removed, "false" if no game timer with this ID was found
			*/
			bool removeTimerById(uint32 timerId);

			/**
			*  @brief
			*    Remove all game timers
			*/
			void removeAllTimers();

			/**
			*  @brief
			*    Get a game timer by its unique ID
			*
			*  @return
			*    The game timer, or "nullptr" if the timer could not be found
			*/
			GameTimer* findTimerById(uint32 timerId) const;

			/**
			*  @brief
			*    Get a game timer by its message configuration
			*
			*  @param[in] messageConfiguration
			*    The message configuration associated with the game timer
			*
			*  @return
			*    The game timer, or "nullptr" if the timer could not be found
			*/
			GameTimer* findTimerByMessage(const MessageConfiguration& messageConfiguration) const;

			/**
			*  @brief
			*    Trigger a game timer now
			*
			*  @notes
			*    - This won't do anything if the timer is inactive or the timer ID not valid
			*/
			void triggerTimerById(uint32 timerId) const;

			/**
			*  @brief
			*    Update all timers
			*
			*  @param[in] timePassed
			*    The time delta since the last update
			*/
			void updateTimers(const Time& timePassed);

			/**
			*  @brief
			*    Serialization of the game timer manager
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void performGarbageCollection();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<uint32> mTimersToRemove;	///< List of all timer unique IDs that were invalidated and have to be removed at the next garbage collection


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
