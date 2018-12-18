// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/job/JobProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
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
		*    Game base class
		*/
		class QSF_GAME_API_EXPORT Game : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Messages
			static const uint32 GAME_SPEED_CHANGE_MESSAGE_ID;

			// Input handlers
			static const uint32 GAME_INPUT_HANDLER_ID;
			static const uint32 DEBUG_INPUT_HANDLER;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			Game();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Game();

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Check whether or not the game is currently running
			*/
			inline bool isRunning() const;

			/**
			*  @brief
			*    Startup the game instance
			*/
			void startup();

			/**
			*  @brief
			*    Shutdown the game instance
			*/
			void shutdown();

			//[-------------------------------------------------------]
			//[ Game speed                                            ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Get the current game speed
			*/
			inline float getGameSpeed() const;

			/**
			*  @brief
			*    Set the game speed; default is 1.0f, pause the game with 0.0f
			*
			*  @param[in] gameSpeed
			*    The game speed; normal speed is 1.0f, pause the game with 0.0f
			*/
			void setGameSpeed(float gameSpeed);

			/**
			*  @brief
			*    Set the game speed
			*
			*  @param[in] gameSpeed
			*    The game speed; normal speed is 1.0f, pause the game with 0.0f
			*  @param[in] force
			*    If "true", the game speed will be set, emitting the respective message, even in case there is no change at all
			*/
			void setGameSpeed(float gameSpeed, bool force);

			/**
			*  @brief
			*    Pauses the game (stores the current game speed)
			*
			*  @param[in] multiplayerClientStarvationPause
			*    Is this a game pause due to multiplayer client starvation?
			*/
			void pauseGame(bool multiplayerClientStarvationPause = false);

			/**
			*  @brief
			*    Resumes the game (restores the game speed)
			*/
			void resumeGame();

			/**
			*  @brief
			*    Is this a game pause due to multiplayer client starvation?
			*/
			inline bool isMultiplayerClientStarvationPause() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::game::Game methods             ]
		//[-------------------------------------------------------]
		private:
			virtual void onStartup();
			virtual void onShutdown();
			virtual void onUpdateGame(const Clock& realtimeClock);
			virtual void onGameSpeedChange(float newGameSpeed, float oldGameSpeed);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Called as job
			void updateGame(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			float	 mGameSpeed;				///< Game speed: a multiplier for game time progress relative to real-time progress; default is 1.0f for "normal" game speed


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Game state
			bool	mIsRunning;				///< If "true", the game is in running state
			bool	mIsInPauseMode;
			float	mGameSpeedBeforePause;	///< Game speed value before pausing, gets restored when the game gets resumed
			bool	mMultiplayerClientStarvationPause;

			// Proxy object for scheduling the update process
			JobProxy mUpdateJobProxy;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/game/Game-inl.h"
