// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/UniqueInstance.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RenderWindow;
	class GuiContext;
}
namespace em5
{
	class MainMenuManager;
	class StartMenuManager;
	class IngameHud;
	class GenericDebugGui;
	class LogoScreen;
	class LoadingScreen;
	class DatagridPictureFormatter;
	class DatagridCheckboxFormatter;
	class ExpandButtonFormatter;
	class ButtonFormatter;
	class ImageFormatter;
	class LoadCampaignFormatter;
	class BuyUpgradeTitleAndCostFormatter;
	class BuyUpgradeIconFormatter;
	class UpDownFormatter;
	class MultiplayerWindowNameFormatter;
	class MultiplayerWindowLockFormatter;
	class AchievementIconFormatter;
	class AchievementNameFormatter;
	class AchievementDescriptionFormatter;
	class AchievementPointsFormatter;
	class TitleNameFormatter;
	class TitleIconFormatter;
	class Tooltip;
	class ToolTipManager;
	class MessageBox;
	class ContentAuthenticationWindow;
	class CloudSynchronisationWindow;
	class MouseCursorManager;
	class PatchUpdateProgressDialog;
}
namespace Rocket
{
	namespace Core
	{
		class ElementDocument;
	}
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
	*    EMERGENCY 5 GUI instance to handle the game-specific user interface
	*/
	class EM5_API_EXPORT EmergencyGui : public qsf::UniqueInstance<EmergencyGui>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] renderWindow
		*    Render window to assign the GUI context to
		*/
		explicit EmergencyGui(qsf::RenderWindow& renderWindow);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EmergencyGui();

		/**
		*  @brief
		*    Load and startup menus
		*/
		void startupMenus();

		/**
		*  @brief
		*    Load the given libRocket document as an alternative to the EMERGENCY 5 main menu
		*
		*  @param[in] guiDocumentName
		*    UTF-8 virtual filename of the document to load
		*
		*  @remarks
		*    Use this for testing a GUI document in development.
		*/
		void startupGenericDebugGui(const std::string& guiDocumentName);

		/**
		*  @brief
		*    Show the logo screen
		*/
		void showLogoScreen();

		/**
		*  @brief
		*    Show the main menu, create one if no one exist yet
		*
		*  @remarks
		*    Use this for a dummy main menu
		*/
		void showMainMenu();

		/**
		*  @brief
		*    Hide the main menu
		*
		*  @remarks
		*    Use this for a dummy main menu
		*/
		void hideMainMenu();

		/**
		*  @brief
		*    Show the start menu, create one if no one exist yet
		*/
		void showStartMenu(bool installed = true);

		/**
		*  @brief
		*    Hide the start menu
		*/
		void hideStartMenu();

		/**
		*  @brief
		*    Return the LoadingScreen instance
		*
		*  @return
		*    Reference to the LoadingScreen instance, do not destroy the instance
		*/
		LoadingScreen& getLoadingScreen();

		/**
		*  @brief
		*    Return whether the loading screen is currently shown
		*/
		bool isLoadingScreenVisible() const;

		/**
		*  @brief
		*    Show a loading screen, create one if no one exist yet
		*/
		void showLoadingScreen();

		/**
		*  @brief
		*    Hide the loading screen
		*/
		void hideLoadingScreen();

		/**
		*  @brief
		*    Show the ingame HUD
		*/
		void showIngameHud();

		/**
		*  @brief
		*    Hide the ingame HUD
		*/
		void hideIngameHud();

		/**
		*  @brief
		*    Hide tooltip
		*/
		void hideTooltip();

		/**
		*  @brief
		*    Show tooltip
		*/
		void showTooltip();

		/**
		*  @brief
		*    Return the GUI context
		*
		*  @return
		*    Reference to the GUI context, do not destroy the instance
		*/
		inline qsf::GuiContext& getGuiContext() const;

		/**
		*  @brief
		*    Return the main menu manager instance
		*
		*  @return
		*    Reference to the main menu manager instance, do not destroy the instance
		*/
		MainMenuManager& getMainMenuManager();

		/**
		*  @brief
		*    Return the start menu manager instance
		*
		*  @return
		*    Reference to the start menu manager instance, do not destroy the instance
		*/
		inline StartMenuManager& getStartMenuManager() const;

		/**
		*  @brief
		*    Return the in-game HUD instance
		*
		*  @return
		*    Reference to the in-game HUD instance, do not destroy the instance
		*/
		inline IngameHud& getIngameHud() const;
		inline IngameHud* tryGetIngameHud() const;

		/**
		*  @brief
		*    Return the patch update progress dialog
		*/
		inline PatchUpdateProgressDialog& getPatchUpdateProgressDialog() const;

		/**
		*  @brief
		*    Show or hide software mouse cursor
		*/
		void showMouseCursor(bool show);

		/**
		*  @brief
		*    Change current mouse cursor
		*/
		void setMouseCursor(const std::string& cursorName);

		/**
		*  @brief
		*    Resets the current mouse cursor to default
		*/
		void resetMouseCursor();

		/**
		*  @brief
		*    Open a standard message box with two buttons as modal window
		*/
		void showModalMessageBox(const std::string& title, const std::string& text, const std::string& buttonTrueText, const std::string& buttonFalseText, const boost::function<void(bool)>& callback);

		/**
		*  @brief
		*    Open a standard message box with two buttons as modal window which close after "autoFalseTime" with a false command
		*/
		void showModalMessageBox(const std::string& title, const std::string& text, const std::string& buttonTrueText, const std::string& buttonFalseText, float autoFalseTime, const boost::function<void(bool)>& callback);

		/**
		*  @brief
		*    Open a standard message box with one button as modal window, with callback
		*/
		void showModalMessageBox(const std::string& title, const std::string& text, const std::string& buttonText, const boost::function<void(bool)>& callback);

		/**
		*  @brief
		*    Open a standard message box with one button as modal window, no callback
		*/
		void showModalMessageBox(const std::string& title, const std::string& text, const std::string& buttonText);

		/**
		*  @brief
		*    Abort the modal dialog
		*/
		void abortModalMessageBox();

		void setContentAuthenticationWindowVisible(bool visible) const;
		void setCloudSynchronisationWindowVisible(bool visible) const;

		/**
		*  @brief
		*    Shows the patch update progress dialog window
		*/
		void showPatchUpdateProgressDialog();

		inline ToolTipManager& getToolTipManager() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onViewportDimensionsChanged(int width, int height);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GuiContext*				 mGuiContext;						///< QSF GUI context instance, always valid, we're responsible for destroying the instance in case we no longer need it
		boost::signals2::scoped_connection mViewportDimensionChangedConnection;

		MainMenuManager*				 mMainMenuManager;					///< Main menu manager instance; always valid, we're responsible for destroying the instance in case we no longer need it
		StartMenuManager*				 mStartMenuManager;					///< Start menu manager instance; always valid, we're responsible for destroying the instance in case we no longer need it
		IngameHud*						 mIngameHud;						///< In-game HUD instance; always valid, we're responsible for destroying the instance in case we no longer need it

		LogoScreen*						 mLogoScreen;
		LoadingScreen*					 mLoadingScreen;					///< Loading screen

		ExpandButtonFormatter*			 mExpandButtonFormatter;			///< Not a test, this formatter should be used for the data tree structure in libRocket
		ButtonFormatter*				 mButtonFormatter;					///< Button formatter
		ImageFormatter*					 mImageFormatter;					///< Image formatter
		LoadCampaignFormatter*			 mLoadCampaignFormatter;			///< Load campaign formatter
		MultiplayerWindowNameFormatter*	 mMultiplayerWindowNameFormatter;	///< Multiplayer window name formatter
		MultiplayerWindowLockFormatter*	 mMultiplayerWindowLockFormatter;	///< Multiplayer window lock formatter
		AchievementIconFormatter*		 mAchievementIconFormatter;			///< Achievement lock formatter
		AchievementNameFormatter*		 mAchievementNameFormatter;
		AchievementDescriptionFormatter* mAchievementDescriptionFormatter;
		AchievementPointsFormatter*		 mAchievementPointsFormatter;
		TitleNameFormatter*				 mTitleNameFormatter;
		TitleIconFormatter*				 mTitleIconFormatter;
		UpDownFormatter*				 mUpDownFormatter;
		BuyUpgradeIconFormatter*		 mBuyUpgradeIconFormatter;			///< Buy-/Upgrade window icon formatter
		BuyUpgradeTitleAndCostFormatter* mBuyUpgradeTitleAndCostFormatter;	///< Buy-/Upgrade window icon formatter
		PatchUpdateProgressDialog*		 mPatchUpdateProgressDialog;		///< Separate GUI window instance for patch progress dialog

		ToolTipManager*					 mToolTipManager;					///< Manager for tool tips; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MessageBox*						 mMessageBox;						///< Message box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ContentAuthenticationWindow*	 mContentAuthenticationWindow;
		CloudSynchronisationWindow*		 mCloudSynchronisationWindow;

		// libRocket data formatter for tables (e.g. modification window)
		DatagridPictureFormatter*		 mDatagridPictureFormatter;
		DatagridCheckboxFormatter*		 mDatagridCheckboxFormatter;

		// Generic debug UI
		GenericDebugGui*				 mGenericDebugGui;					///< Generic GUI instance, only valid in case the generic UI is used, we're responsible for destroying the instance

		MouseCursorManager*				 mMouseCursorManager;				///< Manager for handling mouse cursors


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/EmergencyGui-inl.h"
