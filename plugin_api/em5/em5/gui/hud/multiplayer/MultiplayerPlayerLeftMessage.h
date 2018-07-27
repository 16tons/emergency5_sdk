// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/SessionBase.h"

#include <qsf/gui/GuiDocument.h>


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
	*    EMERGENCY 5 player left session message HUD element
	*/
	class MultiplayerPlayerLeftMessage : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerPlayerLeftMessage(qsf::GuiContext& guiContext);
		virtual ~MultiplayerPlayerLeftMessage();


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
		/**
		*  @brief
		*    Handler for the multiplayer signal "player left"
		*/
		void onPlayerLeft(uint32 playerIndex, const multiplayer::SessionBase::PlayerInfo& playerInfo);

		/**
		*  @brief
		*    Handler for the multiplayer signal host disconnect
		*/
		void onHostLeft();

		/**
		*  @brief
		*    Handler for the multiplayer signal "state change"
		*/
		void onStateChange(multiplayer::SessionBase::State prevState, multiplayer::SessionBase::State newState);

		/**
		*  @brief
		*    Fill the window with the connection lost information
		*/
		void setupWindowConnectionLost();

		/**
		*  @brief
		*    Fill the window with the player left information
		*/
		void setupWindowPlayerLeft(uint32 playerIndex);

		void disconnectSignals();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::signals2::connection	mPlayerLeftSignalConnection;
		boost::signals2::connection	mConnectionLostSignalConnection;
		boost::signals2::connection	mHostLeftSignalConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
