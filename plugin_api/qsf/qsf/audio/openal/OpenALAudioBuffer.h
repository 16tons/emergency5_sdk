// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioBuffer.h"
#include "qsf/time/Time.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <OpenAL/al.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class File;
	namespace openal
	{
		class OpenALStream;
	}
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
		*    OpenAL audio buffer resource class
		*/
		class OpenALAudioBuffer : public AudioBuffer, public FastPodAllocator<OpenALAudioBuffer>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class OpenALAudioManager;	///< Only the OpenAL audio manager is allowed to created OpenAL audio buffer instances


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~OpenALAudioBuffer();

			/**
			*  @brief
			*    Create an OpenAL audio stream
			*
			*  @param[in] openALSource
			*    OpenAL audio source using this stream
			*
			*  @return
			*    The created OpenAL audio stream, a null pointer on error (maybe buffer isn't streamed), destroy the instance if you no longer need it
			*/
			OpenALStream* createOpenALStream(ALuint openALSource) const;

			/**
			*  @brief
			*    Return the OpenAL buffer
			*
			*  @return
			*    The OpenAL buffer, 0 on error
			*/
			inline ALuint getOpenALBuffer() const;

			/**
			*  @brief
			*    Return the audio buffer filename
			*
			*  @return
			*    The audio buffer UTF-8 filename in platform-independent notation
			*
			*  @note
			*    - Only valid if this audio buffer is a stream
			*/
			inline std::string getFilename() const;

			/**
			*  @brief
			*    Open the file the audio buffer is using
			*
			*  @return
			*    The file the audio buffer is using, a null pointer on error, destroy the instance if you no longer need it
			*
			*  @note
			*    - The returned file is already opened for reading, don't forget to destroy the file object if you no longer need it
			*/
			File* openFile() const;

			/**
			*  @brief
			*    Return the audio buffer data
			*
			*  @return
			*    The audio buffer data, can be a null pointer, don't destroy the instance
			*
			*  @note
			*    - If the data is a null pointer, the stream was loaded using a filename, don't destroy the data
			*
			*  @see
			*    - "qsf::openal::OpenALAudioBuffer::getFilename()"
			*/
			inline const uint8* getData() const;

			/**
			*  @brief
			*    Return the audio buffer data size (in bytes)
			*
			*  @return
			*    The audio buffer data size (in bytes)
			*
			*  @note
			*    - If the data is a null pointer, the stream was loaded using a filename
			*
			*  @see
			*    - "qsf::openal::OpenALAudioBuffer::getData()"
			*/
			inline uint32 getDataSize() const;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Helper method for computing length of a buffer in time units
			*/
			static Time computeLengthOfBuffer(ALuint openALBuffer);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioBuffer methods               ]
		//[-------------------------------------------------------]
		public:
			virtual bool loadBuffer(const std::string& filename, bool stream) override;
			virtual bool loadBuffer(const uint8* data, uint32 numberOfBytes, bool stream = false) override;
			virtual bool loadMemoryStreamedBuffer(SampleStreamFactory& sampleStreamFactory) override;
			virtual void unloadBuffer() override;
			inline virtual bool isLoaded() const override;
			inline virtual bool isStreamed() const override;
			virtual bool queryBufferLength(Time& outBufferLength);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] audioManager
			*    Owner audio manager, must stay valid as long as this OpenAL audio buffer instance exists
			*  @param[in] filename
			*    UTF-8 audio filename (supported file formats are API dependent)
			*  @param[in] stream
			*    Stream the file?
			*/
			inline OpenALAudioBuffer(AudioManager& audioManager);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					mStream;					///< Is the buffer a stream?
			ALuint					mOpenALBuffer;				///< Audio buffer
			// For streaming
			std::string				mFilename;					///< UTF-8 filename in platform-independent notation
			const uint8*			mData;						///< Data, can be a null pointer, do not destroy the instance because we just share it
			uint32					mDataSize;					///< Data size (in bytes)
			SampleStreamFactory*	mManualStreamSampleFactory;	///< Factory for manual stream
			Time					mCachedDuration;			///< Duration of the source data, is computed once then cached
			bool					mHasDuration;				///< Flag which tells if the duration property is valid


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/openal/OpenALAudioBuffer-inl.h"
