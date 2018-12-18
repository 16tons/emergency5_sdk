// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MainMenu;
	class BaseLayer;
	class AccountStatus;
	class LoadCampaign;
	class AccountStatusTitle;
	class AccountInfo;
	class AchievementsWindow;
	class StartGame;
	class CreditsWindow;
	class StartCampaign;
	class SelectCampaign;
	class SelectDifficultyWindow;
	class FreeplayWindow;
	class MainEventWindow;
	class MultiplayerWindow;
	class MultiplayerDirectConnectWindow;
	class MultiplayerCreateServerWindow;
	class MultiplayerLobbyWindow;
	class MultiplayerHelpWindow;
	class PreferencesGameWindow;
	class PreferencesGraphicWindow;
	class ExtendedPreferencesGraphicWindow;
	class PreferencesAudioWindow;
	class InputWindow;
	class AccountStatusTitleDataSource;
	class AchievementsWindowDataSource;
	class LoadCampaignDataSource;
	class MainEventDataSource;
	class EndlessGameDataSource;
	class MultiplayerServerDataSource;
	class VideoPlayerWindow;
}


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
	*    EMERGENCY 5 Main menu manager
	*/
	class EM5_API_EXPORT MainMenuManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class ScreenToShow
		{
			DEFAULT,
			CREDITS,
			MAINEVENTS,
			SELECTCAMPAIGN
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainMenuManager();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~MainMenuManager();

		/**
		*  @brief
		*    Load the default EMERGENCY 5 main menu
		*/
		void loadGui();

		/**
		*  @brief
		*    Hide the default EMERGENCY 5 main menu
		*/
		void hideGui();

		/**
		*  @brief
		*    Destroy the EMERGENCY 5 main menu
		*/
		void destroyGui();

		/**
		*  @brief
		*    Select which window gets shown on return to main menu
		*/
		void setScreenToShowOnReturn(ScreenToShow screenToShow);

		void showAccountInfoWindow(bool show);

		/**
		*  @brief
		*    Return the main menu window instance
		*
		*  @return
		*    Reference to the main menu window instance, do not destroy the instance
		*/
		MainMenu& getMainMenuWindow();

		/**
		*  @brief
		*    Return the base layer window instance
		*
		*  @return
		*    Reference to the base layer window instance, do not destroy the instance
		*/
		BaseLayer& getBaseLayer();

		/**
		*  @brief
		*    Return the account status window instance
		*
		*  @return
		*    Reference to the account status window instance, do not destroy the instance
		*/
		AccountStatus& getAccountStatusWindow();

		/**
		*  @brief
		*    Return the account status title window instance
		*
		*  @return
		*    Reference to the account status title window instance, do not destroy the instance
		*/
		AccountStatusTitle& getAccountStatusTitleWindow();

		/**
		*  @brief
		*    Return the achievements window instance
		*
		*  @return
		*    Reference to the achievements window instance, do not destroy the instance
		*/
		AchievementsWindow& getAchievementsWindow();

		/**
		*  @brief
		*    Return the start game window instance
		*
		*  @return
		*    Reference to the start game window instance, do not destroy the instance
		*/
		StartGame& getStartGameWindow();

		/**
		*  @brief
		*    Return the credits window instance
		*
		*  @return
		*    Reference to the credits window instance, do not destroy the instance
		*/
		CreditsWindow& getCreditsWindow();

		/**
		*  @brief
		*    Return the start campaign window instance
		*
		*  @return
		*    Reference to the start campaign window instance, do not destroy the instance
		*/
		StartCampaign& getStartCampaignWindow();

		/**
		*  @brief
		*    Return the select campaign window instance
		*
		*  @return
		*    Reference to the select campaign window instance, do not destroy the instance
		*/
		SelectCampaign& getSelectCampaignWindow();

		/**
		*  @brief
		*    Return the select difficulty window instance
		*
		*  @return
		*    Reference to the select difficulty window instance, do not destroy the instance
		*/
		SelectDifficultyWindow& getSelectDifficultyWindow();

		/**
		*  @brief
		*    Return the freeplay window instance
		*
		*  @return
		*    Reference to the freeplay window instance, do not destroy the instance
		*/
		FreeplayWindow& getFreeplayWindow();

		/**
		*  @brief
		*    Return the large operation window instance
		*
		*  @return
		*    Reference to the large operation window instance, do not destroy the instance
		*/
		MainEventWindow& getMainEventWindow();

		/**
		*  @brief
		*    Return the multiplayer window instance
		*
		*  @return
		*    Reference to the multiplayer window instance, do not destroy the instance
		*/
		MultiplayerWindow& getMultiplayerWindow();

		/**
		*  @brief
		*    Return the multiplayer create server window instance
		*
		*  @return
		*    Reference to the multiplayer create server window instance, do not destroy the instance
		*/
		MultiplayerCreateServerWindow& getMultiplayerCreateServerWindow();

		/**
		*  @brief
		*    Return the multiplayer create server window instance
		*
		*  @return
		*    Reference to the multiplayer create server window instance, do not destroy the instance
		*/
		MultiplayerLobbyWindow& getMultiplayerLobbyWindow();

		/**
		*  @brief
		*    Return the multiplayer direct connect window instance
		*
		*  @return
		*    Reference to the multiplayer direct connect window instance, do not destroy the instance
		*/
		MultiplayerDirectConnectWindow& getMultiplayerDirectConnectWindow();

		/**
		*  @brief
		*    Return the multiplayer help window instance
		*
		*  @return
		*    Reference to the multiplayer help window instance, do not destroy the instance
		*/
		MultiplayerHelpWindow& getMultiplayerHelpWindow();

		/**
		*  @brief
		*    Return the preferences window instance
		*
		*  @return
		*    Reference to the preferences window instance, do not destroy the instance
		*/
		PreferencesGameWindow& getPreferencesGameWindow();
		PreferencesGraphicWindow& getPreferencesGraphicWindow();
		ExtendedPreferencesGraphicWindow& getExtendedPreferencesGraphicWindow();
		PreferencesAudioWindow& getPreferencesAudioWindow();

		/**
		*  @brief
		*    Return the input window instance
		*
		*  @return
		*    Reference to the input window instance, do not destroy the instance
		*/
		InputWindow& getInputWindow();

		/**
		*  @brief
		*    Return the video player window instance
		*
		*  @return
		*    Reference to the video player window instance, do not destroy the instance
		*/
		VideoPlayerWindow& getVideoPlayerWindow();

		// TODO(sw) Why are the datasources accessible/created here? Normally they should be only needed by one document
		//			-> Move them to the using document class if possible (when only used there)

		/**
		*  @brief
		*    Return the datasource of the account status title
		*
		*  @return
		*    Reference to the account status title data source instance, do not destroy the instance
		*/
		AccountStatusTitleDataSource& getAccountStatusTitleDataSource();

		/**
		*  @brief
		*    Return the datasource of the achievements window
		*
		*  @return
		*    Reference to the achievements window data source instance, do not destroy the instance
		*/
		AchievementsWindowDataSource& getAchievementsWindowDataSource();

		/**
		*  @brief
		*    Return the datasource of the campaign score
		*
		*  @return
		*    Reference to the load campaign window data source instance, do not destroy the instance
		*/
		LoadCampaignDataSource& getLoadCampaignDataSource();

		/**
		*  @brief
		*    Return the datasource of the campaign score
		*
		*  @return
		*    Reference to the save campaign window data source instance, do not destroy the instance
		*/
		MainEventDataSource& getMainEventDataSource();

		/**
		*  @brief
		*    Return the datasource of the endless game score
		*
		*  @return
		*    Reference to the save campaign window data source instance, do not destroy the instance
		*/
		EndlessGameDataSource& getEndlessGameDataSource();

		/**
		*  @brief
		*    Return the datasource of the multiplayer server menu
		*
		*  @return
		*    Reference to the multiplayer window data source instance, do not destroy the instance
		*/
		MultiplayerServerDataSource& getMultiplayerServerDataSource();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateAccountStatus();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MainMenu*							mMainMenuWindow;					///< Main menu window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		BaseLayer*							mBaseLayer;							///< Base layer - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AccountStatus*						mAccountStatusWindow;				///< Account status window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AccountStatusTitle*					mAccountStatusTitleWindow;			///< Account status title window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AccountInfo*						mAccountInfoWindow;					///< Account info window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AchievementsWindow*					mAchievementsWindow;				///< Achievements window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		StartGame*							mStartGameWindow;					///< Start game window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		CreditsWindow*						mCreditsWindow;						///< Credits window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		StartCampaign*						mStartCampaignWindow;				///< Start campaign window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SelectCampaign*						mSelectCampaignWindow;				///< Select campaign window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SelectDifficultyWindow*				mSelectDifficultyWindow;			///< Select difficulty window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		FreeplayWindow*						mFreeplayWindow;					///< Freeplay window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MainEventWindow*					mMainEventWindow;					///< Large operation window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerWindow*					mMultiplayerWindow;					///< Multiplayer window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerCreateServerWindow*		mMultiplayerCreateServerWindow;		///< Multiplayer create server window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerLobbyWindow*				mMultiplayerLobbyWindow;			///< Multiplayer lobby window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerHelpWindow*				mMultiplayerHelpWindow;				///< Multiplayer help window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerDirectConnectWindow*		mMultiplayerDirectConnectWindow;	///< Multiplayer direct connect window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		PreferencesGameWindow*				mPreferencesGameWindow;				///< Preferences game window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		PreferencesGraphicWindow*			mPreferencesGraphicWindow;			///< Preferences graphic window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ExtendedPreferencesGraphicWindow*	mExtendedPreferencesGraphicWindow;	///< Extended preferences graphic window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		PreferencesAudioWindow*				mPreferencesAudioWindow;			///< Preferences audio window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		InputWindow*						mInputWindow;						///< Input window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		VideoPlayerWindow*					mVideoPlayerWindow;					///< Video player window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it

		AccountStatusTitleDataSource*	mAccountStatusTitleDataSource;
		AchievementsWindowDataSource*	mAchievementsWindowDataSource;
		LoadCampaignDataSource*			mLoadCampaignDataSource;
		MainEventDataSource*			mMainEventDataSource;
		EndlessGameDataSource*			mEndlessGameDataSource;
		MultiplayerServerDataSource*	mMultiplayerServerDataSource;

		bool							mInitialized;
		ScreenToShow					mScreenToShow;							///< GUI window to show when returning to the main menu (usually from the game)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
