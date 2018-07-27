// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/OperationMessages.h"

#include <qsf/audio/AudioProxy.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <utility> // For std::pair


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class Map;
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
		namespace client
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Client side audio manager which handles audio update messages
			*/
			class EM5_API_EXPORT AudioManager : public boost::noncopyable
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				AudioManager();

				/**
				*  @brief
				*    Reads audio update data from the given stream
				*
				*  @param[in] bitStream
				*    The bit stream from which the data is read
				*
				*  @return
				*    On success "true" otherwise "false"
				*/
				bool readAudioUpdateDataFromStream(const qsf::game::BitStream& bitStream);

				/**
				*  @brief
				*    Is called each tick to apply the read data on a per tick base
				*/
				void interpolate(const qsf::Clock& clock);

				void setMap(qsf::Map& map);

				void shutdown();


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
			public:
				void handleAudioUpdateData(const AudioUpdateData& data);
				void applyPlayAudio(qsf::AudioProxy& audioProxy, const AudioUpdateData& data);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				qsf::Map*	mCurrentMap;
				std::vector<std::pair<int32, std::vector<AudioUpdateData>>> mAudioUpdateHistory;
				std::unordered_map<uint32, qsf::AudioProxy>	mActiveAudio;	///< Contains a list of audio proxyies, which wasn't detached on start


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // client
	} // multiplayer
} // em5
