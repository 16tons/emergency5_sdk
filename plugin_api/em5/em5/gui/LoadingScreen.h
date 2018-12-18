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

#include <boost/signals2/signal.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ProgressBar;
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
	*    Loading screen GUI window
	*/
	class LoadingScreen : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit LoadingScreen(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LoadingScreen();

		/**
		*  @brief
		*    Set the progress bar percentage
		*
		*  @param percentage
		*    Percentage between 0.0 and 1.0
		*/
		void setPercentage(float percentage);

		/**
		*  @brief
		*    Set background image
		*
		*  @param image
		*    Filename of the image file
		*/
		void setBackgroundImage(const std::string& image);

		/**
		*  @brief
		*    Enable and disable progress bar
		*
		*  @param enable
		*    Enable flag
		*/
		void enableProgressbar(bool enable);


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
		*    Handler for the multiplayer host signal "ready for game"
		*/
		void onReadyForGame();
		void showMultiplayerStartGameButton(bool show);
		void showMultiplayerReadyInfo(bool show);
		void showErrorMessage(bool show);

		/**
		*  @brief
		*    Handler for the multiplayer signal "player left"
		*/
		void onPlayerLeft(uint32 playerIndex, const multiplayer::SessionBase::PlayerInfo& playerInfo);

		/**
		*  @brief
		*    Handler for the multiplayer signal "state change"
		*/
		void onStateChange(multiplayer::SessionBase::State prevState, multiplayer::SessionBase::State newState);

		void disconnectSignals();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ProgressBar*				mProgressBar;	///< Must be valid
		bool						mProgressBarEnabled;
		bool						mIsConnected;	///< Is true if the multiplayer connection active
		boost::signals2::connection	mPlayerLeftSignalConnection;
		boost::signals2::connection	mConnectionLostSignalConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
