// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/GuiHotReloadSupport.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MainEvent;
}
namespace Rocket
{
	namespace Core
	{
		class ElementText;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{

	// TODO(co) Code style violations, beside this, you might want to keep the global namespace clean
	enum subtitles
	{
		Freeplay_Challenge,
		Freeplay_Endless,
		MainEvent_Title,
		Multiplayer_COOP,
		Multiplayer_Against
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 in-game options menu. Change audio and graphics settings
	*/
	class IngameOnlineHighscoreWindow : public GuiHotReloadSupport
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameOnlineHighscoreWindow(qsf::GuiContext& guiContext);
		virtual ~IngameOnlineHighscoreWindow();

		void setIsEndScreen(bool isEndSreen);
		void setMainEvent(MainEvent* mainEvent);

		void setSubtitle(subtitles tile);
		void selectUser(uint32 entryId);

		/**
		*  @brief
		*    Use this function to show the online highscore window
		*
		*  @param[in] stopApplication
		*    If stopApplication true, the appliction will shutdown after the next button is clicked
		*/
		void showIngameOnlineHighscoreWindow(bool stopApplication = false);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


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
		void selectUserInDatagrid();
		void setupGui();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mIsEndScreen;
		MainEvent*	mMainEvent;			///< Holds a pointer to the main event, which has shown this dialog, can be a nullptr
		uint32		mEntryId;
		bool		mStopApplication;	///< If this member true the appliction will stop if the player click the next button

		Rocket::Core::ElementText* mPlayerRankText;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
