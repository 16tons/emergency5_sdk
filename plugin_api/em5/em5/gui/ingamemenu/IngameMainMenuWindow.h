// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class IngameLoadSaveWindow;
	class IngameExitDialogWindow;
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
	*    EMERGENCY 5 in-game main menu window
	*/
	class IngameMainMenuWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameMainMenuWindow(qsf::GuiContext& guiContext);
		virtual ~IngameMainMenuWindow();

		void setInTutorial(bool inTutorial);
		void showMenu();
		void hideMenu();
		bool menuIsHide();

		void blockShow(bool blocked);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPostShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();
		void closeMenu();
		void restartEvent(bool accepted);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		IngameLoadSaveWindow*	mIngameLoadSaveWindow;		///< Ingame load/save window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		IngameExitDialogWindow*	mIngameExitDialogWindow;	///< Ingame exit game dialog - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		bool					mInTutorial;				///< Indicates if currently tutorial is active
		bool					mCurrentInputBlockedState;	///< Holds the input blocked state before the window was shown
		bool					mMenuWasClose;
		bool					mBlockShow;					///< Indicates if showing via showMenu should be blocked or not, this is needed for the tutorial

		Rocket::Core::Element* mIngameMenuElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
