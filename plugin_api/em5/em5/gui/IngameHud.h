// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GuiContext;
}
namespace em5
{
	class BaseHudLayer;
	class ChatWindow;
	class MessageWindow;
	class WarblerFeedWindow;
	class SupervisorWindow;
	class MiniMapWindow;
	class UnitOverviewWindow;
	class ObjectiveWindow;
	class SelectionWindowCommandable;
	class SelectionWindowVehicle;
	class SelectionWindowMulti;
	class SEKDroneScanWindow;
	class FullscreenVideoWindow;
	class DebugGuiWindow;
	class DebugSelectionInfoWindow;
	class DebugEquipmentBox;
	class DebugEventBox;
	class DebugEventDetailBox;
	class DebugTriggerWindow;
	class DebugUpgradeBox;
	class DebugWeatherBox;
	class ObjectiveWindow;
	class DebugOrderBox;
	class DebugLogBox;
	class Tooltip;
	class CommandWindow;
	class IngameEventFailedWindow;
	class IngameMainMenuWindow;
	class IngameMainEventFailedWindow;
	class IngameMainEventSuccessWindow;
	class AiNavigationDebugBox;
	class AiPerceptionDebugBox;
	class AiReactionDebugBox;
	class VehicleOrderBar;
	class VehicleSelectionWindow;
	class VehicleOrderWindow;
	class MenuBar;
	class BuyUpgradeWindow;
	class FireSpreadSimulationWindow;
	class StatusOverlayWindowManager;
	class BuildingOverlayWindowManager;
	class CommandTargetManager;
	class BuildingClippingManager;
	class HelpWindow;
	class BuyUpgradeDataSource;
	class IngamePromotionWindow;
	class FreeplayFailedWindow;
	class IngameFreeplayEndWindow;
	class IngameOnlineHighscoreWindow;
	class AchievementUnlockedWindow;
	class MultiplayerScoreOverview;
	class IngameMultiplayerCOOPEndWindow;
	class IngameMultiplayerPvPEndWindow;
	class MultiplayerPlayerLeftMessage;
	class MultiplayerVoteWindow;
	class MultiplayerVoteFailedWindow;
	class HqExtendedMessage;
	class IngameHighscoreDatasource;
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
	*    EMERGENCY 5 in-game HUD
	*/
	class EM5_API_EXPORT IngameHud : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		IngameHud();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~IngameHud();

		/**
		*  @brief
		*    Load the default EMERGENCY 5 in-game HUD
		*/
		void loadGui();

		/**
		*  @brief
		*    Destroy the EMERGENCY 5 in-game HUD
		*/
		void destroyGui();

		/**
		*  @brief
		*    Show the default EMERGENCY 5 in-game HUD; not every GUI will be hidden, used for win/failed
		*/
		void showGui();

		/**
		*  @brief
		*    Hide the default EMERGENCY 5 in-game HUD; not every GUI will be hidden, used for win/failed
		*/
		void hideGui();

		/**
		*  @brief
		*    If any end screen visible this method returns true
		*/
		bool isAnyEndscreenVisible();

		/**
		*  @brief
		*    Return the chat window instance
		*
		*  @return
		*    Reference to the chat window instance, do not destroy the instance
		*/
		inline ChatWindow& getChatWindow() const;

		/**
		*  @brief
		*    Return the message window instance
		*
		*  @return
		*    Pointer to the message window instance, may be a null pointer, do not destroy the instance
		*/
		inline MessageWindow* getMessageWindow() const;

		/**
		*  @brief
		*    Return the warbler feed window instance
		*
		*  @return
		*    Pointer to the warbler feed window instance, may be a null pointer, do not destroy the instance
		*/
		inline WarblerFeedWindow* getWarblerFeedWindow() const;
		WarblerFeedWindow& getOrCreateWarblerFeedWindow();

		/**
		*  @brief
		*    Return the supervisor window instance
		*
		*  @return
		*    Reference to the supervisor window instance, do not destroy the instance
		*/
		inline SupervisorWindow& getSupervisorWindow() const;

		/**
		*  @brief
		*    Return the mini-map window instance
		*
		*  @return
		*    Pointer to the mini-map window instance, may be null pointer, do not destroy the instance
		*/
		inline MiniMapWindow* getMiniMapWindow() const;

		/**
		*  @brief
		*    Return the unit overview window instance
		*
		*  @return
		*    Pointer to the unit overview window instance, do not destroy the instance
		*/
		inline UnitOverviewWindow& getUnitOverviewWindow() const;

		/**
		*  @brief
		*    Return the SEK drone scan window instance
		*
		*  @return
		*    Reference to the SEK drone scan window instance, do not destroy the instance
		*/
		inline SEKDroneScanWindow& getSEKDroneScanWindow() const;

		/**
		*  @brief
		*    Return the base HUD layer
		*
		*  @return
		*    Reference to the base HUD layer, do not destroy the instance
		*/
		inline BaseHudLayer& getBaseHudLayer() const;

		/**
		*  @brief
		*    Return the fullscreen video window instance
		*
		*  @return
		*    Reference to the video window instance, do not destroy the instance
		*/
		inline FullscreenVideoWindow& getFullscreenVideoWindow() const;

		/**
		*  @brief
		*    Return the commando box instance
		*
		*  @return
		*    Reference to the commando box instance, do not destroy the instance
		*/
		DebugSelectionInfoWindow& getDebugSelectionInfoWindow();

		/**
		*  @brief
		*    Return the equipment box instance
		*
		*  @return
		*    Reference to the equipment box instance, do not destroy the instance
		*/
		DebugEquipmentBox& getDebugEquipmentBox();

		/**
		*  @brief
		*    Return the order box instance
		*
		*  @return
		*    Reference to the order box instance, do not destroy the instance
		*/
		DebugOrderBox& getDebugOrderBox();

		/**
		*  @brief
		*    Return the debug log box instance
		*
		*  @return
		*    Reference to the debug log box instance, do not destroy the instance
		*/
		DebugLogBox& getDebugLogBox();

		/**
		*  @brief
		*    Return the event box instance
		*
		*  @return
		*    Reference to the event box instance, do not destroy the instance
		*/
		DebugEventBox& getDebugEventBox();

		/**
		*  @brief
		*    Return the event detail box instance
		*
		*  @return
		*    Reference to the event box detail instance, do not destroy the instance
		*/
		DebugEventDetailBox& getDebugEventDetailBox();

		DebugGuiWindow& getDebugGuiWindow();

		/**
		*  @brief
		*    Return the debug trigger window instance. Here we can place functions we need to test specific functionalities.
		*
		*  @return
		*    Reference to the debug trigger window instance, do not destroy the instance
		*/
		DebugTriggerWindow& getDebugTriggerWindow();

		/**
		*  @brief
		*    Return the debug upgrade box instance. Here we can place functions we need to test specific functionalities.
		*
		*  @return
		*    Reference to the debug upgrade box instance, do not destroy the instance
		*/
		DebugUpgradeBox& getDebugUpgradeBox();

		/**
		*  @brief
		*    Return the debug weather box instance. Here we can place functions we need to test specific functionalities.
		*
		*  @return
		*    Reference to the debug weather box instance, do not destroy the instance
		*/
		DebugWeatherBox& getDebugWeatherBox();

		/**
		*  @brief
		*    Return the ingame event failed window instance
		*
		*  @return
		*    Reference to the ingame event failed window instance, do not destroy the instance
		*/
		IngameEventFailedWindow& getIngameEventFailedWindow();

		/**
		*  @brief
		*    Return the ingame main menu window instance
		*
		*  @return
		*    Reference to the ingame main menu window instance, do not destroy the instance
		*/
		IngameMainMenuWindow& getIngameMainMenuWindow();

		/**
		*  @brief
		*    Return the freeplay failed window instance
		*
		*  @return
		*    Reference to the freeplay failed window instance, do not destroy the instance
		*/
		FreeplayFailedWindow& getFreeplayFailedWindow();

		/**
		*  @brief
		*    Return the freeplay end window instance
		*
		*  @return
		*    Reference to the freeplay end window instance, do not destroy the instance
		*/
		IngameFreeplayEndWindow& getIngameFreeplayEndWindow();

		/**
		*  @brief
		*    Return the IngameMainEventFailedWindow instance
		*
		*  @return
		*    Reference to the IngameMainEventFailedWindow instance, do not destroy the instance
		*/
		IngameMainEventFailedWindow& getIngameMainEventFailedWindow();

		/**
		*  @brief
		*    Return the IngameMainEventSuccessWindow instance
		*
		*  @return
		*    Reference to the IngameMainEventSuccessWindow instance, do not destroy the instance
		*/
		IngameMainEventSuccessWindow& getIngameMainEventSuccessWindow();

		/**
		*  @brief
		*    Return the IngamePromotionWindow instance
		*
		*  @return
		*    Reference to the IngamePromotionWindow instance, do not destroy the instance
		*/
		IngamePromotionWindow& getIngamePromotionWindow();

		/**
		*  @brief
		*    Return the online highscore window instance
		*
		*  @return
		*    Reference to the online highscore window instance, do not destroy the instance
		*/
		IngameOnlineHighscoreWindow& getIngameOnlineHighscoreWindow();

		/**
		*  @brief
		*    Return the achievement unlocked window instance
		*
		*  @return
		*    Reference to the online achievement unlocked window  instance, do not destroy the instance
		*/
		AchievementUnlockedWindow& getAchievementUnlockedWindow();

		/**
		*  @brief
		*    Return the multiplayer score overview window instance
		*
		*  @return
		*    Reference to the online multiplayer score overview window  instance, do not destroy the instance
		*/
		MultiplayerScoreOverview& getMultiplayerScoreOverview();

		/**
		*  @brief
		*    Return the AI navigation debug box instance
		*
		*  @return
		*    Reference to the AI navigation debug box, do not destroy the instance
		*/
		AiNavigationDebugBox& getAiNavigationDebugBox();

		/**
		*  @brief
		*    Return the AI perception debug box instance
		*
		*  @return
		*    Reference to the AI perception debug box, do not destroy the instance
		*/
		AiPerceptionDebugBox& getAiPerceptionDebugBox();

		/**
		*  @brief
		*    Return the AI reaction debug box instance
		*
		*  @return
		*    Reference to the AI reaction debug box, do not destroy the instance
		*/
		AiReactionDebugBox& getAiReactionDebugBox();

		/**
		*  @brief
		*    Return the objective window instance
		*
		*  @return
		*    Reference to the objective window, do not destroy the instance
		*/
		ObjectiveWindow& getObjectiveWindow();

		/**
		*  @brief
		*    Return the vehicle order bar window instance
		*
		*  @return
		*    Pointer to the vehicle order bar window, may be a null pointer, do not destroy the instance
		*/
		inline VehicleOrderBar* getVehicleOrderBar() const;

		/**
		*  @brief
		*    Return the vehicle selection window instance
		*
		*  @return
		*    Reference to the vehicle selection window, do not destroy the instance
		*/
		inline VehicleSelectionWindow& getVehicleSelectionWindow() const;

		/**
		*  @brief
		*    Return the vehicle order window instance
		*
		*  @return
		*    Reference to the vehicle order window, do not destroy the instance
		*/
		inline VehicleOrderWindow& getVehicleOrderWindow() const;

		/**
		*  @brief
		*    Return the datasource of the campaign score
		*
		*  @return
		*    Reference to the load campaign window data source instance, do not destroy the instance
		*/
		inline IngameHighscoreDatasource& getIngameHighscoreDatasource() const;

		/**
		*  @brief
		*    Return the IngameMultiplayerCOOPEndWindow instance
		*
		*  @return
		*    Reference to the IngameMultiplayerCOOPEndWindow instance, do not destroy the instance
		*/
		IngameMultiplayerCOOPEndWindow& getIngameMultiplayerCOOPEndWindow();

		/**
		*  @brief
		*    Return the IngameMultiplayerCOOPEndWindow instance
		*
		*  @return
		*    Reference to the IngameMultiplayerCOOPEndWindow instance, do not destroy the instance
		*/
		IngameMultiplayerPvPEndWindow& getIngameMultiplayerPvPEndWindow();

		/**
		*  @brief
		*    Return the MultiplayerPlayerLeftMessage instance
		*
		*  @return
		*    Reference to the MultiplayerPlayerLeftMessage instance, do not destroy the instance
		*/
		MultiplayerPlayerLeftMessage& getMultiplayerPlayerLeftMessage();

		/**
		*  @brief
		*    Return the MultiplayerVoteWindow instance
		*
		*  @return
		*    Reference to the MultiplayerVoteWindow instance, do not destroy the instance
		*/
		MultiplayerVoteWindow& getMultiplayerVoteWindow();

		/**
		*  @brief
		*    Return the MultiplayerVoteFailedWindow instance
		*
		*  @return
		*    Reference to the MultiplayerVoteFailedWindow instance, do not destroy the instance
		*/
		MultiplayerVoteFailedWindow& getMultiplayerVoteFailedWindow();

		/**
		*  @brief
		*    Return the HqExtendedMessage instance
		*
		*  @return
		*    Reference to the HqExtendedMessage instance, do not destroy the instance
		*/
		HqExtendedMessage& getHqExtendedMessage();

		/**
		*  @brief
		*    Return the command window instance
		*
		*  @return
		*    Reference to the command window, do not destroy the instance
		*/
		CommandWindow& getCommandWindow();

		/**
		*  @brief
		*    Return the help window instance
		*
		*  @return
		*    Reference to the help window, do not destroy the instance
		*/
		HelpWindow& getHelpWindow();

		/**
		*  @brief
		*    Manage which selection window to display
		*
		*  @note
		*    We have many different selection windows, so it makes sense to have a helper that decides which window to display
		*/
		void manageSelectionWindows();

		/**
		*  @brief
		*    Hide all selection windows
		*/
		void hideAllSelectionWindows();

		/**
		*  @brief
		*    Return the buy and upgrade menu window instance
		*
		*  @return
		*    Reference to the buy upgrade menu window, do not destroy the instance
		*/
		BuyUpgradeWindow& getBuyUpgradeWindow();

		/**
		*  @brief
		*    Return the menu bar instance
		*
		*  @return
		*    Pointer to the menu bar, do not destroy the instance
		*/
		inline MenuBar* getMenuBar() const;

		/**
		*  @brief
		*    Return the buy-/upgrade datasource
		*
		*  @return
		*    Reference to the Buy Upgrade datasource, do not destroy the instance
		*/
		inline BuyUpgradeDataSource& getBuyUpgradeDataSouce() const;

		StatusOverlayWindowManager& getStatusOverlayWindowManager();
		BuildingOverlayWindowManager& getBuildingOverlayWindowManager();
		CommandTargetManager& getCommandTargetManager();
		BuildingClippingManager& getBuildingClippingManager();

		//[-------------------------------------------------------]
		//[ Selection window                                      ]
		//[-------------------------------------------------------]
		SelectionWindowCommandable& getSelectionWindowCommandable();
		SelectionWindowVehicle& getSelectionWindowVehicle();
		SelectionWindowMulti& getSelectionWindowMulti();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSelectionChange(const qsf::MessageParameters& parameters);

		void loadGameHudItems();
		void loadFireSpreadSimulationHudItems();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GuiContext&				mGuiContext;					///< Used GUI context, do not destroy the instance
		BaseHudLayer*					mBaseHudLayer;					///< Base HUD Layer - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ChatWindow*						mChatWindow;					///< Chat window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MessageWindow*					mMessageWindow;					///< Message window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		WarblerFeedWindow*				mWarblerFeedWindow;				///< Warbler feed window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SupervisorWindow*				mSupervisorWindow;				///< Supervisor window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		FullscreenVideoWindow*			mFullscreenVideoWindow;			///< Fullscreen Video window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		CommandWindow*					mCommandWindow;					///< Command window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameEventFailedWindow*		mIngameEventFailedWindow;		///< Event failed window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameMainMenuWindow*			mIngameMainMenuWindow;			///< Ingame main menu window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameMainEventFailedWindow*	mIngameMainEventFailedWindow;	///< Ingame main event failed  window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameMainEventSuccessWindow*	mIngameMainEventSuccessWindow;	///< Ingame main event success window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MiniMapWindow*					mMiniMapWindow;					///< Mini-map window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		UnitOverviewWindow*				mUnitOverviewWindow;			///< Unit overview window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ObjectiveWindow*				mObjectiveWindow;				///< Objective window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SEKDroneScanWindow*				mSEKDroneScanWindow;			///< SEK scan window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		VehicleOrderBar*				mVehicleOrderBar;				///< Vehicle order bar - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		VehicleSelectionWindow*			mVehicleSelectionWindow;		///< Vehicle selection window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		VehicleOrderWindow*				mVehicleOrderWindow;			///< Vehicle order window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MenuBar*						mMenuBar;						///< Menu bar - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		BuyUpgradeWindow*				mBuyUpgradeWindow;
		HelpWindow*						mHelpWindow;					///< Helper window - GUI document instance;
		IngamePromotionWindow*			mIngamePromotionWindow;			///< Ingame promotion window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		FreeplayFailedWindow*			mFreeplayFailedWindow;			///< Freeplay failed window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameFreeplayEndWindow*		mIngameFreeplayEndWindow;		///< Freeplay end window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameOnlineHighscoreWindow*	mIngameOnlineHighscoreWindow;   ///< Online highscore window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AchievementUnlockedWindow*		mAchievementUnlockedWindow;		///< Achievement unlocked window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerScoreOverview*		mMultiplayerScoreOverview;		///< Multiplayer score overview - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameMultiplayerCOOPEndWindow* mIngameMultiplayerCOOPEndWindow;///< Multiplayer COOP end window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameMultiplayerPvPEndWindow*	mIngameMultiplayerPvPEndWindow; ///< Multiplayer PvP end window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerPlayerLeftMessage*	mMultiplayerPlayerLeftMessage;	///< Multiplayer player left message window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerVoteWindow*			mMultiplayerVoteWindow;			///< Multiplayer vote message window- GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MultiplayerVoteFailedWindow*	mMultiplayerVoteFailedWindow;	///< Multiplayer vote failed window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		HqExtendedMessage*				mHqExtendedMessage;				///< HQ extended message - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it

		// Selection window
		SelectionWindowCommandable*		mSelectionWindowCommandable;	///< Selection window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SelectionWindowVehicle*			mSelectionWindowVehicle;		///< Selection window for vehicles - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		SelectionWindowMulti*			mSelectionWindowMulti;			///< Selection window for multi-selection - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it

		IngameHighscoreDatasource*		mIngameHighscoreDatasource;

		// Debug
		DebugGuiWindow*					mDebugGuiWindow;				///< Debug GUI window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugSelectionInfoWindow*		mDebugSelectionInfoWindow;		///< Debug selection info window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugEquipmentBox*				mDebugEquipmentBox;				///< Debug equipment box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugEventBox*					mDebugEventBox;					///< Debug event box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugEventDetailBox*			mDebugEventDetailBox;			///< Debug event detail box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugLogBox*					mDebugLogBox;					///< Debug log box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugOrderBox*					mDebugOrderBox;					///< Debug order box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugTriggerWindow*				mDebugTriggerWindow;			///< Debug trigger window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugUpgradeBox*				mDebugUpgradeBox;				///< Debug upgrade box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		DebugWeatherBox*				mDebugWeatherBox;				///< Debug weather box - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AiNavigationDebugBox*			mAiNavigationDebugBox;			///< Debug AI window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AiPerceptionDebugBox*			mAiPerceptionDebugBox;			///< Debug AI window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AiReactionDebugBox*				mAiReactionDebugBox;			///< Debug AI window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it

		// Fire spread simulation
		FireSpreadSimulationWindow*		mFireSpreadSimulationWindow;	///< Fire spread simulation window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it

		// Managers
		StatusOverlayWindowManager*		mStatusOverlayWindowManager;
		BuildingOverlayWindowManager*	mBuildingOverlayWindowManager;
		CommandTargetManager*			mCommandTargetManager;
		BuildingClippingManager*		mBuildingClippingManager;

		// Datasouce
		BuyUpgradeDataSource* mBuyUpgradeDataSource;

		// Message proxy
		qsf::MessageProxy	mMessageProxy;
		qsf::JobProxy		mUpdateJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/IngameHud-inl.h"
