// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioBuffer.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <mutex>
#include <thread>
#include <vector>


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
	*    FIFO buffer for storing audio samples
	*
	*  @note
	*    Queuing/Dequeuing is thread-safe
	*/
	class QSF_API_EXPORT AudioSampleBuffer
	{


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Single chunk of samples, basically just a piece of memory
		*    which is known to contain audio samples
		*/
		struct SampleChunk : public FastPodAllocator<SampleChunk>
		{
			SampleChunk(uint32 l, const uint8* copyfrom) :
				length(l),
				data(new uint8[l])
			{
				memcpy(data, copyfrom, length);
			}

			~SampleChunk()
			{
				delete [] data;
			}

			uint32 length;
			uint8* data;
		};

		/**
		*  @brief
		*    List of sample chunks, used for queueing
		*/
		typedef std::vector<SampleChunk*> SampleChunks;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		AudioSampleBuffer();

		/**
		*  @brief
		*    Destructor
		*/
		~AudioSampleBuffer();

		/**
		*  @brief
		*    Clears the buffer and sets new sample properties on the buffer
		*
		*  @param[in] frequency
		*    Sample frequency in HZ
		*  @param[in] channels
		*    Number of channels, interleaved for each sample
		*/
		void clear(uint32 frequency, AudioBuffer::SampleFormat sampleFormat);

		/**
		*  @brief
		*    Returns the sample format which is covered by this buffer
		*/
		AudioBuffer::SampleFormat getSampleFormat() const;

		/**
		*  @brief
		*    Returns the sample frequency in Hz
		*/
		uint32 getSampleFrequency() const;

		/**
		*  @brief
		*    Returns the number of samples which have totally been queued (so this
		*    value is monotonically increasing, even if samples get dequeued). The sample
		*    number is converted into seconds by dividing by the frequency
		*/
		float getLengthInSec() const;

		void setLengthInSec(float seconds);

		/**
		*  @brief
		*    Queues new sample data
		*
		*  @remarks
		*    The size of the sample chunk has to be divisible by
		*    the size of a single sample, i.e. the buffer cannot contain
		*    partial samples.
		*/
		void queueSamples(uint32 length, const uint8* data);

		/**
		*  @brief
		*    Dequeues as much samples as will fit into the target buffer.
		*
		*  @return
		*    Returns the number of bytes actually copied into the target buffer
		*
		*  @remarks
		*    Only whole samples are copied to the buffer so no
		*    partial samples are present e.g. at the end of the buffer.
		*/
		uint32 dequeueSamples(uint32 bufferSize, uint8* targetBuffer);


	private:
		SampleChunks mSampleChunks;					///< Queue of chunks
		uint32 mNextChunkReadCursor;				///< Read cursor pointing into the data of the front chunk in the queue to allow reading partial chunks
		uint32 mFrequency;							///< Sample frequency in Hz
		AudioBuffer::SampleFormat mSampleFormat;	///< Format of every sample
		uint32 mSampleSize;							///< Cached size of a single sample based on the sample format
		uint32 mTotalSamplesQueued;					///< Number of samples queued totally
		std::mutex mAccessMutex;					///< Mutex for access synchronization of queuing/dequeuing


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
