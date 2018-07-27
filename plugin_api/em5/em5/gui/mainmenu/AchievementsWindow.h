// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class AchievementsWindowDataSource;
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
	*    Achievements window
	*/
	class AchievementsWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit AchievementsWindow(qsf::GuiContext& guiContext);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainMenuWindowBase methods     ]
	//[-------------------------------------------------------]
	protected:
		virtual void onGoingBack() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;
		virtual bool onDocumentPreShow() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();

		/**
		*  @brief
		*    Setup all achievements
		*/
		void setupAbstract();

		/**
		*  @brief
		*    Setup all general achievements
		*/
		void setupGeneral();

		/**
		*  @brief
		*    Setup all main event achievements
		*/
		void setupMainEvent();

		/**
		*  @brief
		*    Setup all freeplay endless achievements
		*/
		void setupEndless();

		/**
		*  @brief
		*    Setup all freeplay challenge achievements
		*/
		void setupChallenge();

		/**
		*  @brief
		*    Setup all multiplayer COOP achievements
		*/
		void setupCOOP();

		/**
		*  @brief
		*    Setup all multiplayer against achievements
		*/
		void setupAgainst();

		/**
		*  @brief
		*    Deselect all main menu buttons
		*/
		void deselectButtons();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AchievementsWindowDataSource*	mAchievementsWindowDataSource; //< Holds the achievement window datasouce; Do not free this pointer
		uint32							mAchievementCount;
		uint32							mAchievementUnlocked;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
