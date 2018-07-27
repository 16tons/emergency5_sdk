// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/network/matchmaking/MatchmakingTypes.h"
#include "em5/network/multiplayer/server/Host.h"

#include <Rocket/Core/String.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Controls
	{
		class ElementFormControlSelect;
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
	*    Multiplayer create server window
	*/
	class MultiplayerCreateServerWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerCreateServerWindow(qsf::GuiContext& guiContext);


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


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		bool createMultiplayerHostingSession();
		void addMapToList(Rocket::Controls::ElementFormControlSelect& selectControl, const std::string& mapName, const MapIdentifier& mapIdentifier);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		multiplayer::HostEntry mHostEntry;
		Rocket::Core::String   mPassword;
		Rocket::Core::String   mUsername;
		bool				   mClearServerNameOnce;
		bool				   mClearPasswordOnce;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
