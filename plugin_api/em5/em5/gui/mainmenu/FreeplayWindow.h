// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/game/highscore/HighscoreManager.h"
#include "em5/map/MapIdentifier.h"

#include <qsf/job/JobProxy.h>
#include <qsf/time/DateTime.h>


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
	*    Freeplay window
	*/
	class FreeplayWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit FreeplayWindow(qsf::GuiContext& guiContext);


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
		virtual void onDocumentPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		void addMapButton(const MapIdentifier& mapIdentifier, const std::string& text, const std::string& image);
		void setInfo();
		void startMap();

		qsf::DateTime getYoungestDate(const boost::container::flat_set<HighscoreEntry>& set);

		void blinkCampaignButton();
		void onBlink(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct MapButtonData
		{
			MapIdentifier mMapIdentifier;
			std::string   mPreviewImage;
		};
		typedef std::vector<MapButtonData> MapButtonArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int			   mSelectedIndex;
		MapButtonArray mMapButtons;
		int			   mOriginalMapNameInformationFontSize;

		// Blinking
		float		   mBlinkPhase;
		qsf::JobProxy  mBlinkJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
