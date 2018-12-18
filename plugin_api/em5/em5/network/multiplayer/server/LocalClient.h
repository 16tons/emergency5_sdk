// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/server/ClientBase.h"
#include "em5/network/multiplayer/OperationMessages.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/job/JobProxy.h>

#include <memory>	// For std::unique_ptr
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		class DataRecorder;
		class DataPlayer;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer local client (a special server side multiplayer client instance for testing the de/serialisation parts of the MP communication)
			*    This instance hosts the game recorder and the command player
			*/
			class EM5_API_EXPORT LocalClient : public ClientBase
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] dataPath
				*    The path where the player will find the recorded data file (if any), and where the recorder will save the recorded data
				*/
				explicit LocalClient(const std::string& dataPath);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~LocalClient();

				void onConnected();


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::client::ClientBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual void disconnect() override;
				virtual void disconnect(bool force) override;
				virtual uint64 getId() const override;
				virtual bool sendData(const qsf::Clock& simulationClock, const qsf::game::BitStream& bitStream) override;
				virtual bool sendData(const qsf::game::BitStream& bitStream) override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				/**
				*  @brief
				*    "Handler" for the non ingame operation messages send by the host (e.g. the response to the join request)
				*/
				void onHandleNonIngameOperationMessages(const qsf::JobArguments& jobArguments);

				void onRealtimeGameClockUpdate(const qsf::JobArguments& jobArguments);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::unique_ptr<DataRecorder>	mGameRecorder;								///< The game recorder instance for recording any data send from the host to the client
				std::unique_ptr<DataPlayer>		mCommandPlayer;								///< The command player instance for playing back any recorded commands
				OperationMessage				mNonIngameOperationMessage;					///< The operation message which was send by the host outside of the game
				qsf::game::BitStream			mNonIngameOperationMessageData;				///< Contains the non ingame operation message data
				qsf::JobProxy					mHandleNonIngameOperationMessagesJobProxy;	///< The jobproxy for handling non ingame operation messages
				qsf::JobProxy					mRealtimeGameClockUpdateJobProxy;
				qsf::Time						mTimePassedSinceStart;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
