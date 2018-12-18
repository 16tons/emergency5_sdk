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
	class LoadCampaignDataSource;
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
	*    EMERGENCY 5 ingame load/save window
	*/
	class IngameLoadSaveWindow : public GuiHotReloadSupport
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameLoadSaveWindow(qsf::GuiContext& guiContext);
		virtual ~IngameLoadSaveWindow();
		void setupLoadGame();
		void setupSaveGame();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void loadSelectedSaveGame();
		void deleteSaveGame(bool accept);
		void saveGame();
		void saveCampaignProgressAndScreenshot(bool accept, const LoadCampaignDataSource* loadCampaignDataSource, uint32 selectedItem);
		void saveCampaignProgressAndScreenshot(const std::string& fileName, const std::string& name);
		void setDefaultScreenshotTexture();
		void createScreenshotTexture(const std::string& absolutePath);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string	mSelectedFilename;	// The filename for the selected saved game
		bool		mIsGameSaved;
		bool		mIsLoadWindow;
		uint32		mSelectedItem;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
