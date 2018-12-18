// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <boost/noncopyable.hpp>

#include <iosfwd> // For the forward declarations of iostreams
#include <memory> // For std::unique_ptr
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Time;
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
		*    EMERGENCY 5 multiplayer data recorder. Records any data which the host/client would send to the client(s)/host
		*/
		class EM5_API_EXPORT DataRecorder : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] fileName
			*    The filename where the DataRecorder will write the recorded data to
			*/
			explicit DataRecorder(const std::string& fileName);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~DataRecorder();

			/**
			*  @brief
			*    Record the packet, which the hosts sends to the client(s)
			*
			*  @param[in] timePassedSinceStart
			*    The time when the packet was send to the client
			*
			*  @param[in] bitStream
			*    The bit stream which contains the packet data
			*/
			void recordData(const qsf::Time& timePassedSinceStart, const qsf::game::BitStream& bitStream);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::unique_ptr<std::ostream>	mRecorderStream;	///< The (file)stream to which the data is written, to always valid
			std::string						mFilename;			///< The filename which the recorder uses to write data to
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
