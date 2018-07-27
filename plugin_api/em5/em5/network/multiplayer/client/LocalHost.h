// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/client/HostBase.h"

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
		namespace client
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    EMERGENCY 5 multiplayer local host (a special client side multiplayer host instance for testing the de/serialisation parts of the MP communication)
			*    This instance hosts the game player and the command recorder
			*/
			class EM5_API_EXPORT LocalHost : public client::HostBase
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
				explicit LocalHost(const std::string& dataPath);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~LocalHost();


			//[-------------------------------------------------------]
			//[ Public virtual em5::multiplayer::client::HostBase methods ]
			//[-------------------------------------------------------]
			public:
				virtual bool connect(const std::string& hostOrIpAddress, uint16 port) override;
				virtual bool sendData(const qsf::game::BitStream& bitStream) override;


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			private:
				/**
				*  @brief
				*    "Handler" for the non ingame operation messages send by the client (e.g. the join request)
				*/
				bool handleNonIngameOperationMessages(const qsf::game::BitStream& bitStream);

				void onRealtimeGameClockUpdate(const qsf::JobArguments& jobArguments);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::unique_ptr<DataPlayer>		mGamePlayer;		///< The game player instance for playing back any recorded multiplay game data
				std::unique_ptr<DataRecorder>	mCommandRecorder;	///< The command recorder instance for recording any commands initiated by the client
				qsf::JobProxy					mRealtimeGameClockUpdateJobProxy;
				qsf::Time						mTimePassedSinceStart;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
