// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/settings/GameSettingsGroup.h"

#include <qsf/settings/RendererSettingsGroup.h>
#include <qsf/settings/CompositingSettingsGroup.h>


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
	*    Preferences window
	*/
	class ExtendedPreferencesGraphicWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum ShadowQuality
		{
			OFF = 0,
			LOW,
			MEDIUM,
			HIGH,
			ULTRA
		};

		struct CustomGraphicSettings
		{
			qsf::compositing::CompositingSettingsGroup::Quality compositorQuality;
			qsf::compositing::CompositingSettingsGroup::Quality lightingQuality;
			qsf::RendererSettingsGroup::MapQuality				mapQuality;
			float												population;
			ShadowQuality										shadowQuality;
			qsf::compositing::CompositingSettingsGroup::Quality textureDetails;
			qsf::RendererSettingsGroup::TextureFilteringQuality textureFiltering;
			qsf::compositing::CompositingSettingsGroup::Quality environmentDetails;
			bool												depthOfField;
			bool												fxaa;
			qsf::compositing::CompositingSettingsGroup::Quality effects;

			CustomGraphicSettings() :
				compositorQuality(qsf::compositing::CompositingSettingsGroup::Quality::MEDIUM_QUALITY),
				lightingQuality(qsf::compositing::CompositingSettingsGroup::Quality::MEDIUM_QUALITY),
				mapQuality(qsf::RendererSettingsGroup::MapQuality::MEDIUM_MAP_QUALITY),
				population(0.5f),
				shadowQuality(MEDIUM),
				textureDetails(qsf::compositing::CompositingSettingsGroup::Quality::MEDIUM_QUALITY),
				textureFiltering(qsf::RendererSettingsGroup::TextureFilteringQuality::BILINEAR_TEXTURE_FILTERING),
				environmentDetails(qsf::compositing::CompositingSettingsGroup::Quality::MEDIUM_QUALITY),
				depthOfField(false),
				fxaa(false),
				effects(qsf::compositing::CompositingSettingsGroup::Quality::MEDIUM_QUALITY)
			{
				// Nothing to do here
			}

		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ExtendedPreferencesGraphicWindow(qsf::GuiContext& guiContext);
		void inMainMenu(bool menu);
		CustomGraphicSettings& getCustomGraphicSettings();
		void loadSavedSettings();
		void initWindow();


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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void saveSettingsInStruct();
		void setSelectionElement(std::string elementID, qsf::compositing::CompositingSettingsGroup::Quality value);
		void setSelectionElement(std::string elementID, GameSettingsGroup::Quality value);
		void setSelectionElement(std::string elementID, int value);
		void setSelectionElement(std::string elementID, bool value);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		CustomGraphicSettings mCustomGraphicSettings;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
