// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

#include <iosfwd> // For the forward declarations of iostreams
#include <memory> // For std::unique_ptr
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class BitStream;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 multiplayer data player. Plays back any data recorded by the data recorder
		*/
		class EM5_API_EXPORT DataPlayer : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::function<void(const qsf::game::BitStream& bitStream)> SendDataCallback;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] updateJobId
			*    The job id to use for the playback of the data
			*  @param[in] fileName
			*    The filename where the player will read the recorded data from
			*  @param[in] sendDataCallback
			*    An callback, which gets called, when the player wants to send some data to the client
			*/
			explicit DataPlayer(const uint32 updateJobId, const std::string& fileName, const SendDataCallback& sendDataCallback);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DataPlayer();

			/**
			*  @brief
			*    Gets called, when the player should start its work
			*/
			void startup();


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct GameRecord
			{
				int64				mTime;			///< The time, in milliseconds, when the data was recorded
				size_t				mExtraBitCount;	///< Indicates how many extra bits are inside the recorded data, which doesn't use a full byte
				std::vector<uint8>	mData;			///< The recorded data
			};


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onUpdateTick(const qsf::JobArguments& jobArguments);
			bool readNextGameRecordFromFile();
			void sendDataToClient();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::unique_ptr<std::istream>	mPlayerStream;		///< The (file)stream from which the data is read, to always valid
			std::string						mFilename;			///< The filename which the player uses to read data from
			GameRecord						mNextGameRecord;	///< Holds the data of the next recorded data
			SendDataCallback				mSendDataCallback;
			const uint32					mUpdateJobId;

			qsf::JobProxy					mPlayDataJobProxy;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
