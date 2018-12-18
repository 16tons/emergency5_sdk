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
	*    SampleStreamFactory implementation for creating VideoAudioSampleBufferStream streams which operate
	*    upon a single VideoAudioSampleBuffer.
	*
	*  @note
	*    Actually, simultaneous reading of a single VideoAudioSampleBuffer is not supported because the buffer
	*    removed parts that have been read once. So be sure you only have one source playing this stream or
	*    you get chops of the buffer for every source that plays this stream (you really don't want that!)
	*/
	class QSF_API_EXPORT VideoAudioSampleBufferStreamFactory : public AudioBuffer::SampleStreamFactory
	{
	public:
		VideoAudioSampleBufferStreamFactory(AudioSampleBuffer* buffer);
		virtual ~VideoAudioSampleBufferStreamFactory();

		virtual AudioBuffer::SampleStream* createSampleStream();

		virtual void destroySampleStream(AudioBuffer::SampleStream* stream) override;

	protected:
		AudioSampleBuffer* mSampleBuffer;
		uint32 mNumberOfStreamsCreated;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
