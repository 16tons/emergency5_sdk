// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioResource.h"

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Time;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Abstract audio buffer resource base class
	*/
	class QSF_API_EXPORT AudioBuffer : public AudioResource
	{


	//[-------------------------------------------------------]
	//[ Public types                                          ]
	//[-------------------------------------------------------]
	public:

		/**
		*  @brief
		*    Different formats of samples
		*/
		enum SampleFormat
		{
			SF_MONO_8BIT,		///<  8-bit mono sample,   size=1b
			SF_MONO_16BIT,		///< 16-bit mono sample,   size=2b
			SF_STEREO_8BIT,		///<  8-bit stereo sample, size=2b (Order: 1b Left, 1b Right)
			SF_STEREO_16BIT		///< 16-bit stereo sample, size=4b (Order: 2b Left, 2b Right)
		};

		/**
		*  @brief
		*    Simple memory stream interface for manually streamed audio buffers, allows
		*    easy queueing of new sample data
		*/
		class SampleStream
		{
		public:
			virtual ~SampleStream() {};
			/**
			*  Read any number of samples into the passed limited-size buffer
			*/
			virtual uint32 readSamples(uint8* buffer, uint32 bufferSize, SampleFormat& outSampleFormat, uint32& outSampleFrequencyInSamplesPerSecond) = 0;
		};

		/**
		*  @brief
		*    Serves as a factory method for SampleStreams. When using manual sample streams,
		*    a new actual SampleStream has to be created for each source which is playing a
		*    manually streamed audio buffer.
		*
		*  @note
		*    The best approach would be to have a single master buffer, which will retain sample data
		*    until it is being read by all SampleStream instances created for that buffer. SampleStream
		*    instances will be destroyed when a source stops playing, but not when it is paused. It is best used
		*    when you can control how the AudioBuffer is actually played
		*/
		class SampleStreamFactory
		{
		public:
			virtual SampleStream* createSampleStream() = 0;
			virtual void destroySampleStream(SampleStream* stream) = 0;
		};


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AudioBuffer methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load the audio buffer with the given data from a file
		*
		*  @param[in] filename
		*    UTF-8 audio filename (supported file formats are API dependent)
		*  @param[in] stream
		*    Stream the file?
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Not each audio buffer can be streamed, use "qsf::AudioBuffer::isStreamed()" to check whether streaming is used
		*/
		virtual bool loadBuffer(const std::string& filename, bool stream) = 0;

		/**
		*  @brief
		*    Load the audio buffer with the given data from memory
		*
		*  @param[in] data
		*    Audio data, can be a null pointer
		*  @param[in] numberOfBytes
		*    Number of data bytes, must be valid
		*  @param[in] stream
		*    Stream the data?
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - If "stream" is "true", "data" must stay valid!
		*    - Not each audio buffer can be streamed, use "qsf::AudioBuffer::isStreamed()" to check whether streaming is used
		*/
		virtual bool loadBuffer(const uint8* data, uint32 numberOfBytes, bool stream = false) = 0;

		/**
		*  @brief
		*    "loads" a buffer implementation which allows streaming from arbitrary supplied memory
		*    TODO(oi) Would be practical to supply a sample format here!
		*
		*  @param[in] sampleStreamFactory
		*    Reference to a sample stream factory which is queried for a new stream for each
		*    source which is playing this buffer. The stream is queried for samples when the buffer runs out
		*    of samples.
		*    Has to stay valid as long as the buffer exists or as long as the
		*    buffer shall stream from the manual sample stream (buffer might load a file etc.)
		*
		*  @return
		*    "true" when the buffer is now in manual streamed mode or "false" if an error occurred
		*/
		virtual bool loadMemoryStreamedBuffer(SampleStreamFactory& sampleStreamFactory) = 0;

		/**
		*  @brief
		*    Unload the audio buffer
		*/
		virtual void unloadBuffer() = 0;

		/**
		*  @brief
		*    Return whether the audio buffer is loaded or not
		*
		*  @return
		*    "true" if the audio buffer is loaded, else "false"
		*/
		virtual bool isLoaded() const = 0;

		/**
		*  @brief
		*    Return whether the audio buffer is streamed or not
		*
		*  @return
		*    "true" if the audio buffer is streamed, else "false"
		*/
		virtual bool isStreamed() const = 0;

		/**
		*  @brief
		*    Queries the length of the buffer in time units
		*
		*  @return
		*    Returns only "true" if the length of the buffer is available. For some streamed buffers, the
		*    length may be not known before the end is reached so these will return "false" and leave the
		*    "outLength" parameter untouched.
		*/
		virtual bool queryBufferLength(Time& outLength) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] audioManager
		*    Owner audio manager, must stay valid as long as this audio buffer instance exists
		*/
		inline explicit AudioBuffer(AudioManager& audioManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AudioBuffer();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioBuffer-inl.h"
