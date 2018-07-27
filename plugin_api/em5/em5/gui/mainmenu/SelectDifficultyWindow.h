// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/settings/GameSettingsGroup.h"


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
	*    Select difficulty window
	*/
	class SelectDifficultyWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SelectDifficultyWindow(qsf::GuiContext& guiContext);
		virtual ~SelectDifficultyWindow();

		void setAutostartMainEvent(int32 autostartMainEventNumber);


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
		void setupGui();
		void selectDifficulty(GameSettingsGroup::Difficulty difficulty);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int32 mAutostartMainEventNumber;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
