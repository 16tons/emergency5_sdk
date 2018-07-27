// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/ingamemenu/IngameEventFailedWindow.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MainEvent;
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
	*    Window to show when a main event fails
	*/
	class IngameMainEventFailedWindow : public IngameEventFailedWindow
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameMainEventFailedWindow(qsf::GuiContext& guiContext);
		~IngameMainEventFailedWindow();

		void setMainEvent(MainEvent* mainEvent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::IngameEventFailedWindow methods]
	//[-------------------------------------------------------]
	protected:
		virtual void triggerRepeatEvent();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MainEvent* mMainEvent;		///< The main event that opened this dialog, may be a null pointer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
