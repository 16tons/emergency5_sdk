// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/math/Color3.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		struct GameModeUpdateData;
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
	*    EMERGENCY 5 multiplayer PvP end sreen
	*/
	class IngameMultiplayerPvPEndWindow : public qsf::GuiDocument
	{


		// This struct stores the player informations
		struct PlayerInformation
		{
			std::string name;
			uint32		score;
			qsf::Color3 color;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameMultiplayerPvPEndWindow(qsf::GuiContext& guiContext);

		~IngameMultiplayerPvPEndWindow();

		void setEventCount(uint32 failedEvents, uint32 succeededEvents);

		void incrementFailedEvents();
		void incrementSuccessEvents();


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


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onGameModeDataChanged(const multiplayer::GameModeUpdateData& gameModeData);

		/**
		*  @brief
		*    Upload scores function for the results
		*/
		void publish(bool state);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<PlayerInformation> mPlayerList;

		uint32				mFailedEvents;
		uint32				mSucceededEvents;
		uint32				mPlayerHighscore;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
