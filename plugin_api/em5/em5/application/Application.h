// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/application/GameApplication.h>

#include <qsf/base/UniqueInstance.h>
#include <qsf/debug/DebugDrawProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class EmergencyGui;
	class Game;
	class NetworkManager;
	class PlayerProgress;
	class AccountManager;
	class ContentAuthentication;
}
namespace qsf
{
	class Map;
	struct LogMessage;
	namespace game
	{
		class StoreManager;
	}
}
#ifdef QSF_PROFILING
	namespace qsf
	{
		class ProfilingOverlay;
	}
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 application implementation
	*/
	class EM5_API_EXPORT Application : public qsf::game::GameApplication, public qsf::UniqueInstance<Application>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class StartMenuInstalled;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 APPLICATION_ID;			///< "em5::Application" unique ID
		static const std::string LAUNCHER_EXE_PATH;	///< "/bin/em5_launcher.exe"

		struct StartupSettings
		{
			bool		mMapAlreadyLoaded;
			std::string mMapLocalAssetName;
			uint32		mMainEventNumber;
			std::string mGuiDocument;
			bool		mStartMultiplayerHost;	///< "true" = create multiplayer host session, "false" = create a multiplayer client with a local host
			bool		mSkipStartMenu;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Application();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Application();

		/**
		*  @brief
		*    Get application name sent to the patch server
		*/
		std::string getUpdaterApplicationName() const;

		/**
		*  @brief
		*    Return the customized application parameters
		*
		*  @return
		*    Customized application-specific parameters as a generic Boost property tree
		*/
		inline const boost::property_tree::ptree& getCustomizedParametersPTree() const;

		//[-------------------------------------------------------]
		//[ Managers and sub-systems                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the EMERGENCY 5 game instance
		*
		*  @return
		*    Reference to the EM5 game instance, do not destroy the instance
		*/
		inline Game& getGame() const;

		/**
		*  @brief
		*    Return the EMERGENCY 5 GUI instance
		*
		*  @return
		*    Reference to the EMERGENCY 5 GUI instance, do not destroy the instance
		*/
		inline EmergencyGui& getGui() const;

		/**
		*  @brief
		*    Return the EMERGENCY 5 GUI instance
		*
		*  @return
		*    Pointer to the EMERGENCY 5 GUI instance, null pointer if there's no GUI instance, do not destroy the instance
		*/
		inline EmergencyGui* tryGetGui() const;

		/**
		*  @brief
		*    Return the EM5 network manager instance
		*
		*  @return
		*    Reference to the EM5 network manager instance, do not destroy the instance
		*/
		inline NetworkManager& getNetworkManager() const;

		/**
		*  @brief
		*    Return the EM5 player progress instance
		*
		*  @return
		*    Reference to the EM5 player progress instance, do not destroy the instance
		*/
		inline PlayerProgress& getPlayerProgress() const;

		/**
		*  @brief
		*    Return the EM5 account manager instance
		*
		*  @return
		*    Reference to the EM5 account manager instance, do not destroy the instance
		*/
		inline AccountManager& getAccountManager() const;

		/**
		*  @brief
		*    Return the EM5 content authentication instance
		*
		*  @return
		*    Reference to the EM5 content authentication instance, do not destroy the instance
		*/
		inline ContentAuthentication& getContentAuthentication() const;

		/**
		*  @brief
		*    Return the store manager instance
		*
		*  @return
		*    Reference to the store manager instance, do not destroy the instance
		*/
		inline qsf::game::StoreManager& getStoreManager() const;

		//[-------------------------------------------------------]
		//[ Application state                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return whether the application is in start menu ("launcher") right now
		*/
		inline bool isInStartMenu() const;

		/**
		*  @brief
		*    Switch to the logo screen
		*/
		void triggerGotoLogoScreen();

		/**
		*  @brief
		*    Switch to the main menu
		*/
		void triggerGotoMainMenu();

		/**
		*  @brief
		*    Switch to the game
		*
		*  @note
		*    - Make sure to set everything needed inside "EM5_GAME.getGameSessionConfiguration()" before calling this
		*/
		void triggerStartGame();

		/**
		*  @brief
		*    Switch from game to the main menu
		*/
		void triggerStopGame();

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		// Cheats
		inline bool areCheatsEnabled() const;
		void enableCheats();

		/**
		*  @brief
		*    Check whether the user may submit online highscores or gain achievements
		*/
		bool areRewardsEnabled() const;

		/**
		*  @brief
		*    Get md5 checksum from em5_launcher.exe
		*/
		std::string getLauncherChecksum() const;

		/**
		*  @brief
		*    Return the "SkipStartMenu" value from the startup settings
		*/
		bool getSkipStartMenu() const;

		void showDebugVersionInformation(bool show);

#ifdef QSF_PROFILING
		/**
		*  @brief
		*    Toggle visibility of job profiling overlay
		*/
		void toggleProfilingOverlay();
#endif


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Application methods               ]
	//[-------------------------------------------------------]
	public:
		virtual uint32 getId() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Application methods            ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual int onRun() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::WindowApplication methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void getWindowSize(bool& fullscreen, uint32& width, uint32& height) override;
		virtual void onRenderWindowFirstPreShow() override;
		virtual void setDefaultWindowIcon() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::game::GameApplication methods  ]
	//[-------------------------------------------------------]
	protected:
		virtual void onVersionJsonLoaded(const boost::property_tree::ptree& pTree) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool runLoopContent();

		void setupLanguage();
		void showMainMenu();
		void stopMainMenuBackgroundMusic();
		void showStopGameScreen();

		/**
		*  @brief
		*    Start the game
		*
		*  @note
		*    - Make sure to set everything needed inside "EM5_GAME.getGameSessionConfiguration()" before calling this
		*/
		void startGame();

		/**
		*  @brief
		*    Stop the game
		*/
		void stopGame();

		/**
		*  @brief
		*    Application update loop body
		*/
		void updateApplication();

		/**
		*  @brief
		*    Set loading screen percentage
		*/
		void setLoadingScreenPercentage(float percentage);

		/**
		*  @brief
		*    Map loading progress callback
		*
		*  @param[in] progress
		*    Map loading progress callback (0...1 -> start...finished)
		*/
		void onMapLoadingProgress(float progress);

		// Uncommented
		bool calculateLauncherChecksum();
		std::string selectLoadingScreenImage() const;

		void onLauncherFinished();

		void mountMods(const std::string& installationVersion);

		// For configuration by "em5::StartMenuInstalled"
		void disableEM20Project();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum Instruction
		{
			INSTRUCTION_NONE = 0,				///< No instruction
			INSTRUCTION_GOTO_LOGO_SCREEN,		///< Go to logo screen
			INSTRUCTION_GOTO_MAIN_MENU,			///< Go to main menu
			INSTRUCTION_START_GAME,				///< Start the game, parameter string gives the map file name
			INSTRUCTION_STOP_GAME,				///< Stop the game and go to main menu
			INSTRUCTION_EXIT_APPLICATION		///< Exit the application
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			mIsInStartMenu;			///< If "true", we are still in the launcher / start menu
		std::string		mUpdaterAppNamePrefix;	///< Prefix for application name sent to the patch server
		std::string		mOldLanguage;			///< Old language identifier previously set
		qsf::Time		mLastLoadingScreenPercentageUpdate;

		// Instructions
		Instruction		mCurrentInstruction;	///< Instruction to be executed by the application in the next update loop (most of the time, this is INSTRUCTION_NONE)
		std::string		mInstructionParameter;	///< Parameter string for the current instruction; meaning depends on the instruction

		// Settings
		StartupSettings				mStartupSettings;
		boost::property_tree::ptree mCustomizedParametersPTree;

		// Managers and sub-systems
		Game*					 mGame;					///< EM5 game instance, always valid, we have to destroy the instance in case we no longer need it
		EmergencyGui*			 mGui;					///< EM5 GUI instance, always valid, we have to destroy the instance in case we no longer need it
		NetworkManager&			 mNetworkManager;		///< EM5 network manager instance, always valid, we have to destroy the instance in case we no longer need it
		PlayerProgress&			 mPlayerProgress;		///< Holds information on local player's game progress
		AccountManager&			 mAccountManager;		///< Holds all information about the user account
		ContentAuthentication&	 mContentAuthentication;
		qsf::game::StoreManager& mStoreManager;			///< Store manager instance, always valid, we have to destroy the instance in case we no longer need it
		qsf::Map*				 mMainMenuMap;

		// Checksum
		std::string		mLauncherChecksum;	///< Md5 checksum from em5_launcher.exe

		// Others
		bool			mShowModResetWarning;

		// Debug & profiling
		qsf::DebugDrawProxy mVersionDrawProxy;
		bool mCheatsEnabled;
		#ifdef QSF_PROFILING
			qsf::ProfilingOverlay& mProfilingOverlay;	// TODO(fw): Hmm, maybe it's not the best place here
		#endif


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/application/Application-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::Application)
