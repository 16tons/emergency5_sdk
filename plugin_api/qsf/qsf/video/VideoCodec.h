// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/audio/AudioBuffer.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class VideoBuffer;
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
	*    Abstract video codec base class
	*/
	class QSF_API_EXPORT VideoCodec
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum LoadingError
		{
			LOADINGERROR_OK = 0,
			LOADINGERROR_COULD_NOT_OPEN_FILE,
			LOADINGERROR_UNEXPECTED_EOF,
			LOADINGERROR_INVALID_THEORA_HEADER,
			LOADINGERROR_INVALID_VORBIS_HEADER,
			LOADINGERROR_HEADERS_NOT_FOUND
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Intermediate factory for theora codecs, destroy the instance if you no longer need it
		*/
		static VideoCodec* createTheoraCodec();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VideoCodec();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VideoCodec();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::VideoManager methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Reset the streaming to be able to reuse the codec for another video
		*/
		virtual void reset() = 0;

		/**
		*  @brief
		*    Start video streaming
		*
		*  @param[in] videoBuffer
		*    Video frames are buffered in this video buffer. Memory ownership is kept at the caller
		*    but the buffer has to stay valid as long as the codec runs
		*  @param[in] audioBuffer
		*    Audio samples are buffered in this audio buffer. Memory ownership is kept at the caller
		*    but the buffer has to stay valid as long as the codec runs
		*  @param[in] inputDataStream
		*    Stream which supplies the data for the video/audio decoder. Does not have to be seekable but
		*    to figure out total length or calculate file position, seeks are required to calculate file size.
		*    Memory ownership for the stream is kept at the caller but it has to stay valid as long as the codec runs
		*  @param[in] precachingTime
		*    Preloads amounts of video frames and audio sample so that this much time is covered by them. The
		*    call will block until this much data is loaded
		*
		*  @return
		*    When the streaming starts up well, the method will return LOADINGERROR_OK. If there was a error,
		*    you can test for several reasons (@see LoadingError)
		*/
		virtual LoadingError startVideoStreaming(VideoBuffer* videoBuffer, AudioSampleBuffer* audioBuffer, std::istream* inputDataStream, float precachingTime = 0.0f);

		/**
		*  @brief
		*    Progresses streaming for a single "block". This does not mean that e.g. a full frame is decoded in this
		*    call. To preload a known amount of data use precache() which takes a time target to preload to
		*
		*  @param[in] ignoreVideo
		*    Does not update video stream
		*  @param[in] ignoreAudio
		*    Does not update audio stream
		*  @param[in] enableVideoFrameskip
		*    Does decode the video stream but never fetches frames into the video buffer, just updates it with empty frames if one would be available
		*
		*  @note
		*    Use the "ignore" flags if you only want to update streaming for a specific stream, e.g. if you want to
		*    bring the streams closer into sync if one stream progresses faster than the other. This is internally done
		*    by the "precache" methods.
		*/
		virtual bool updateStreaming(bool ignoreVideo = false, bool ignoreAudio = false, bool enableVideoFrameskip = false) = 0;

		/**
		*  @brief
		*    Precaches audio and video streams up to the given point in time
		*
		*  @param[in] time
		*    Absolute video time up to which the video and audio buffers should be filled
		*/
		void precache(float time);

		/**
		*  @brief
		*    Precaches audio and video streams up to the given points in time
		*
		*  @param[in] videoTime
		*    Absolute video time up to which the video buffer should be filled
		*  @param[in] audioTime
		*    Absolute audio time up to which the audio buffer should be filled
		*  @param[in] enableFrameskip
		*    Does skip all video frames to speed up precaching (empty frames are added to the video buffer)
		*/
		void precache(float videoTime, float audioTime, bool enableFrameskip);

		/**
		*  @brief
		*    Seeks into a specific time in the video
		*
		*  @param[in] time
		*    Absolute audio time where to seek to
		*
		*  @remarks
		*    In order to seek, the data stream has to be seekable. We need to know the
		*    size of the stream in order to stream to the roughly correct position in the
		*    data stream to find the nearest keyframe
		*    TODO(oi) Seeking currently has undefined behavior on the Audio and VideoBuffer! New keyframes/samples
		*    will be added blindly without taking into account the buffer status and actual stream lengths (in terms of time)
		*    DON'T USE SEEKING FOR NOW UNTIL THE IMPLEMENTATION OF EACH CODEC HAS BEEN CHECKED TO SUPPORT IT.
		*/
		virtual void seek(float time) = 0;

		/**
		*  @brief
		*    Retrieve current time position in the video stream
		*/
		virtual float getVideoPosition() = 0;

		/**
		*  @brief
		*    Retrieve current time position in the audio stream
		*/
		virtual float getAudioPosition() = 0;

		/**
		*  @brief
		*    Retrieve current read cursor position (as percentage) in the data stream
		*
		*  @note
		*    This requires a seek to figure out the stream size
		*/
		virtual float getFilePosition() = 0;

		/**
		*  @brief
		*    Return if the codec is currently streaming either audio or video
		*/
		virtual bool isStreaming() const = 0;


	protected:
		/**
		*  @brief
		*    Video streaming start implementation which has to be implemented by each individual codec.
		*    The audio/video buffers are valid depending on whether the user wants audio and video to be decoded.
		*/
		virtual LoadingError startVideoStreamingImpl(std::istream* inputDataStream) = 0;


	protected:
		VideoBuffer* mVideoBuffer;
		AudioSampleBuffer* mAudioBuffer;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
