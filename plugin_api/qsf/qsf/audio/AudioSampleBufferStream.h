// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioBuffer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioSampleBuffer;
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
	*    SampleStream implementation for streaming audio samples from a AudioSampleBuffer
	*/
	class QSF_API_EXPORT VideoAudioSampleBufferStream : public AudioBuffer::SampleStream
	{
	public:
		VideoAudioSampleBufferStream(AudioSampleBuffer* sampleBuffer);
		virtual uint32 readSamples(uint8* buffer, uint32 bufferSize, AudioBuffer::SampleFormat& outSampleFormat, uint32& outSampleFrequencyInSamplesPerSecond) override;

	protected:
		AudioSampleBuffer* mSampleBuffer;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
