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
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class AchievementSystem;
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
		*    Steam API (aka "Steamworks") runtime linking
		*
		*  @note
		*    - Also handles the Steam API initialization when "qsf::game::SteamApiRuntimeLinking::isSteamApiAvailable()" is called the first time (Steam API deinitialization then handled as well, of course)
		*    - MS Windows: "steam_api64.dll" (x64) / "steam_api.dll" (x86) must be inside the executable directory, if it's not there the Steam API initialization will fail
		*/
		class QSF_GAME_API_EXPORT SteamApiRuntimeLinking
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class AchievementSystem;	// For now, we only use the Steam API for achievements


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return whether or not the Steam API support is enabled
			*
			*  @return
			*    "true" if the Steam API support is enabled, else "false"
			*
			*  @note
			*    - By default, the Steam API support is disabled
			*    - Ones enabled, the Steam API support can no longer be disabled during runtime
			*/
			inline bool isSteamApiEnabled() const;

			/**
			*  @brief
			*    Set whether or not the Steam API support is enabled
			*
			*  @param[in] enabled
			*    "true" if the Steam API support is enabled, else "false"
			*/
			inline void setSteamApiEnabled(bool enabled);

			/**
			*  @brief
			*    Set whether or not the Steam achievements are enabled; with enabled modifications and/or chats one might not want to reward the player with achievements
			*
			*  @param[in] enabled
			*    "true" if the Steam achievements are enabled, else "false"
			*/
			inline void setSteamAchievementsEnabled(bool enabled);

			/**
			*  @brief
			*    Return whether or not the Steam API is available
			*
			*  @return
			*    "true" if the Steam API is available, else "false"
			*/
			bool isSteamApiAvailable();

			/**
			*  @brief
			*    Tell Steam about all already gained achievements (World of EMERGENCY account over Steam account information)
			*/
			void tellSteamAboutAllAchievements();

			/**
			*  @brief
			*    Return the appID of the current process
			*/
			uint32 getAppId() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Constructor
			*/
			explicit SteamApiRuntimeLinking(AchievementSystem& achievementSystem);

			/**
			*  @brief
			*    Destructor
			*/
			~SteamApiRuntimeLinking();

			/**
			*  @brief
			*    Load the shared libraries
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool loadSharedLibraries();

			/**
			*  @brief
			*    Load the Steam API entry points
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool loadSteamApiEntryPoints();

			/**
			*  @brief
			*    Main update function that is passed to the job manager
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void updateJob(const JobArguments& jobArguments);

			void onQsfGameAchievementCompleted(const MessageParameters& parameters);
			void onQsfGameAchievementUpdated(const MessageParameters& parameters);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AchievementSystem& mAchievementSystem;
			bool			   mSteamApiEnabled;
			bool			   mSteamAchievementsEnabled;	///< "true" if the Steam achievements are enabled, else "false"
			void*			   mSteamApiSharedLibrary;		///< Steam API shared library, can be a null pointer
			bool			   mSteamApiInitialized;		///< Steam API successfully initialized?
			bool			   mInitialized;				///< Already initialized?
			JobProxy		   mJobProxy;					///< Regular job proxy; for updates once a frame
			MessageProxy	   mQsfGameAchievementCompletedMessageProxy;
			MessageProxy	   mQsfGameAchievementUpdatedMessageProxy;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/achievement/SteamApiRuntimeLinking-inl.h"
