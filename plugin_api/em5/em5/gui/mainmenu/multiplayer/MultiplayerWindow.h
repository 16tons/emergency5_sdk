// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/network/multiplayer/client/Client.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		struct HostEntry;
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
	*    Multiplayer window
	*/
	class MultiplayerWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerWindow(qsf::GuiContext& guiContext);
		virtual ~MultiplayerWindow();
		void updateDataGrid();


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
		void setupGui();
		void onCurrentStateChanged(multiplayer::SessionBase::State prevState, multiplayer::SessionBase::State newState);
		void onAutoUpdateServerList();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const multiplayer::HostEntry*	mSelectedServer;	///< The server for the selected entry
		multiplayer::client::Client*	mMultiplayerClient;	///< A pointer to the used multiplayer session instance, don't destroy it
		qsf::JobProxy					mRefreshUpdater;	///< Refresh the server list in certain intervals


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
