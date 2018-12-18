// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/SessionBase.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/math/Color3.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT ChatWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ChatWindow(qsf::GuiContext& guiContext);
		virtual ~ChatWindow();

		/**
		*  @brief
		*    Add a message to the chat window
		*/
		void addTextMessage(const std::string& user, const std::string& message, const qsf::Color3& color = qsf::Color3::WHITE);

		void showChatWindow();
		void blurInputField();
		void disableChatWindow();	// Calls "disconnectFromMultiplayerSession()" and suppresses chat window show requests
		void disconnectFromMultiplayerSession();


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
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Block game input
		*/
		void blockInput();

		/**
		*  @brief
		*    Unblock game input
		*/
		void unblockInput();

		/**
		*  @brief
		*    Listener function for incoming messages
		*/
		void onChatMessage(const multiplayer::SessionBase::ChatMessage& message);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		multiplayer::SessionBase*		   mMultiplayerSession;	///< A pointer to the used multiplayer session instance, don't destroy it
		bool							   mChatWindowDisabled;
		boost::signals2::scoped_connection mChatMessageConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
