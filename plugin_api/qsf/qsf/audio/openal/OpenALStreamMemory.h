// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/openal/OpenALStream.h"
#include "qsf/audio/AudioBuffer.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <list>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class File;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    OpenAL memory stream class
		*/
		class OpenALStreamMemory : public OpenALStream, public FastPodAllocator<OpenALStreamMemory>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] openALSource
			*    OpenAL audio source using this stream
			*  @param[in] openALAudioBuffer
			*    OpenAL audio buffer this stream is using
			*/
			OpenALStreamMemory(ALuint openALSource, const OpenALAudioBuffer& openALAudioBuffer, AudioBuffer::SampleStreamFactory& sampleStreamFactory);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OpenALStreamMemory();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::openal::OpenALStream methods      ]
		//[-------------------------------------------------------]
		public:
			virtual bool init() override;
			virtual bool isInitialized() const override;
			virtual void deInit() override;
			virtual bool update() override;
			virtual Time getActiveBufferBaseTime() const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Open the stream
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool openStream();

			/**
			*  @brief
			*    Fill an OpenAL buffer
			*
			*  @param[in] openALBuffer
			*    OpenAL buffer to fill
			*
			*  @return
			*    "true" if all went fine, else "false" (end of stream reached?)
			*/
			bool fillOpenALBuffer(ALuint openALBuffer);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const uint32 SCRATCH_BUFFER_SIZE = 1 << 16;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AudioBuffer::SampleStreamFactory&	mSampleStreamFactory;
			AudioBuffer::SampleStream*			mSampleStream;

			ALuint		 mOpenALBuffers[2];			///< Back and front OpenAL buffer
			Time		 mBuffersBaseTime[2];		///< Base times of the front and back buffers, respectively
			Time		 mTotalTimeOfSamplesRead;	///< Total time of actually read samples
			uint8*		 mScratchBuffer;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf
