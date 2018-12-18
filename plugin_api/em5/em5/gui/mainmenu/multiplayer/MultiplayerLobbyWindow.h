// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"
#include "em5/network/multiplayer/SessionBase.h"

#include <qsf/math/Color3.h>

#include <array>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace packet
	{
		class TestOpenPortResponse;
	}
	namespace multiplayer
	{
		struct HostEntry;
	}
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


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Multiplayer lobby window
	*/
	class MultiplayerLobbyWindow : public MainMenuWindowBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerLobbyWindow(qsf::GuiContext& guiContext);
		virtual ~MultiplayerLobbyWindow();
		void setupHostLobby(const multiplayer::HostEntry* hostEntry);
		void setupClientLobby(const multiplayer::HostEntry* hostEntry);
		void addTextMessage(const std::string& user, const std::string& message, const qsf::Color3& color = qsf::Color3::WHITE);


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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum MatchmakingServerRegistationState
		{
			MATCHMAKING_SERVER_REGISTRATION_UNKNOWN = 0,		///< Initial uninitialized state
			MATCHMAKING_SERVER_REGISTRATION,					///< Trying to register inside the matchmaking server list
			MATCHMAKING_SERVER_REGISTRATION_SUCCESS_VALIDATION,	///< Successfully registered inside the matchmaking server list, now testing public reachability
			MATCHMAKING_SERVER_REGISTRATION_SUCCESS_CONFIRMED,	///< Successfully registered inside the matchmaking server list, public reachability confirmed
			MATCHMAKING_SERVER_REGISTRATION_SUCCESS_FAILED,		///< Successfully registered inside the matchmaking server list, no public reachability
			MATCHMAKING_SERVER_REGISTRATION_FAILED				///< Failed to register inside the matchmaking server list
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		void setupUserListUIData(uint32 maxPlayerCount, bool forHost);
		void setMatchmakingServerRegistationState(MatchmakingServerRegistationState matchmakingServerRegistationState);
		void addUser(const multiplayer::SessionBase::PlayerInfo& playerInfo);
		void removeUser(const multiplayer::SessionBase::PlayerInfo& playerInfo);

		// Multiplayer host/client signals
		void setPlayerJoined(uint32 playerIndex, const multiplayer::SessionBase::PlayerInfo& playerInfo);
		void setPlayerLeft(uint32 playerIndex, const multiplayer::SessionBase::PlayerInfo& playerInfo);
		void onPlayerChangeReady(uint32 playerIndex, bool isReady);
		void onCurrentStateChanged(multiplayer::SessionBase::State prevState, multiplayer::SessionBase::State newState);
		void onChatMessage(const multiplayer::SessionBase::ChatMessage& message);
		void startConnectionTest();
		void onTestOpenPortResult(const em5::packet::TestOpenPortResponse& response);
		void addTextToStatusArea(const std::string& text, const std::string& tooltipPrefix, bool failedText, bool clearAreaBeforeInserting);
		void goBack();
		void showElement(Rocket::Core::Element* element, bool show) const;
		void fillInfoField();
		void clearChat();
		void onHostLostProxyConnection();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct PlayerInfo
		{
			Rocket::Core::Element* mPlayerElement;
			Rocket::Core::Element* mPlayerNameElement;
			Rocket::Core::ElementText* mPlayerNameTextElement;
			Rocket::Core::Element* mPlayerReadyElement;
			Rocket::Core::Element* mPlayerCheckElement;

			PlayerInfo() :
				mPlayerElement(nullptr),
				mPlayerNameElement(nullptr),
				mPlayerNameTextElement(nullptr),
				mPlayerReadyElement(nullptr),
				mPlayerCheckElement(nullptr)
			{}

		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MatchmakingServerRegistationState	mMatchmakingServerRegistationState;
		const multiplayer::HostEntry*		mServer;
		multiplayer::SessionBase*			mMultiplayerSession;	///< A pointer to the used multiplayer session instance, don't destroy it
		bool								mbClickOnBack;
		bool								mbFirstFocus;
		bool								mIsPVPMode;
		bool								mIsHost;
		boost::signals2::connection			mConnectionTestResultConnection;
		boost::signals2::connection			mHostLostProxyConnection;
		Rocket::Core::Element*				mLobbyStatusElement;
		Rocket::Core::Element*				mPlayerListElement;
		std::vector<PlayerInfo>				mPlayerElementList;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
