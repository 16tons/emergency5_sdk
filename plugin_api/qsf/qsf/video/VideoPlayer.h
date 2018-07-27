// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobArguments.h"
#include "qsf/job/JobProxy.h"

// TODO(oi) Get rid of this header somehow, but seems to be not easy when the TexturePtr should be used for ref counting the resource
#include <OgreTextureManager.h>
#undef ERROR // OGRE includes an OS header resulting in the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition

#include <boost/signals2.hpp>

#include <thread>
#include <condition_variable>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class VideoCodec;
	class VideoBuffer;
	class AudioSampleBuffer;
	class File;
	class AudioBuffer;
	class AudioSource;
	class VideoAudioSampleBufferStreamFactory;
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
	*    Self-contained video player
	*/
	class QSF_API_EXPORT VideoPlayer
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void ()>						 PlaybackFinished;	///< This Boost signal is emitted when the video playback has been finished (meaning not aborted premature)
		boost::signals2::signal<void (const qsf::JobArguments&)> SyncVideo;			///< This Boost signal is emitted after syncing of video frames, job arguments as parameter


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VideoPlayer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~VideoPlayer();

		/**
		*  @brief
		*    Change the video that is played by this player
		*
		*  @param[in] shaderYuvConversion
		*    "true" if the YUV conversion is done by a shader instead of the CPU, else "false"
		*/
		void setVideoByFilename(const std::string& videoPath, bool shaderYuvConversion = false);

		/**
		*  @brief
		*    Retrieve the name of the texture which is synchronized with video
		*    playback. Use the texture manager to get a reference to the texture or simply
		*    assign this texture name to whatever drawcall should render the video.
		*
		*  @remarks
		*    The texture lifetime is subject to the video player. In the case that the
		*    video is changed, a new texture is created. This is needed to keep the texture in sync with
		*    the source video dimensions. Also, when the player is destructed, the texture is automatically
		*    removed. You might still reference it but it will not be used again.
		*/
		std::string getVideoTextureName() const;

		/**
		*  @brief
		*    Provide direct access to the underlying video buffer to get statistics etc. directly from it
		*/
		VideoBuffer* getVideoBuffer() { return mVideoBuffer;}

		/**
		*  @brief
		*    Pause video/audio playback
		*/
		void pause();

		/**
		*  @brief
		*    Resume video/audio playback
		*
		*  @note
		*    - By default, the video is already playing
		*/
		void play();

		/**
		*  @brief
		*    Return if the player is currently playing the video and updating the video texture
		*/
		bool isPlaying() const { return mIsPlaying && !mIsPaused; }

		/**
		*  @brief
		*    Stop video/audio playback
		*/
		void stop();

		/**
		*  @brief
		*    Get current position in side the video (in seconds)
		*/
		float getCurrentTime();

		/**
		*  @brief
		*    Seeks into a specific time in the video
		*
		*  @param[in] time
		*    Absolute audio time where to seek to
		*/
		void seek(float time);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Does syncing of video frames and the video texture
		*/
		void syncVideo(const JobArguments& jobArgs);

		/**
		*  @brief
		*    Decodes parts of the video stream to fill up the video buffer
		*/
		void asynchronousDecoding();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const float PRECACHING_TIME;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef std::lock_guard<std::mutex> MutexLock;

		bool					mIsPlaying;				///< Flag if the video should be advanced in sync calls
		bool					mIsPaused;				///< Flag if the video should be paused
		bool					mShaderYuvConversion;	///< "true" in case YUV conversion is done by a shader instead by the CPU, else "false"
		Ogre::TexturePtr		mVideoTexture;			///< Video texture surface which gets updated with video frames, RGBA in case YUV conversion is not via shader, Y if done via shader
		Ogre::TexturePtr		mVideoTextureU;			///< U (YUV) video texture surface which gets updated with video frames, only used in case YUV conversion is done via shader
		Ogre::TexturePtr		mVideoTextureV;			///< V (YUV) video texture surface which gets updated with video frames, only used in case YUV conversion is done via shader
		VideoBuffer*			mVideoBuffer;			///< Buffer for video frames
		AudioSampleBuffer*		mAudioSampleBuffer;
		VideoAudioSampleBufferStreamFactory* mStreamFactory;
		AudioBuffer*			mAudioBuffer;
		AudioSource*			mAudioSource;

		VideoCodec*				mCodec;				///< Codec used to decode the played video
		File*					mVideoFile;			///< File which is the source for the video stream
		std::istream*			mVideoFileStream;	///< Stream wrapper which is passed to the codec and wraps mVideoFile
		float					mVideoTime;			///< Current playhead time
		uint32					mLastSyncedFrame;	///< The last frame which was synced to the video texture, used for change detection
		JobProxy				mVideoUpdateProxy;	///< Proxy to register the sync callback with the realtime job manager

		std::mutex						mMutex;						///< Mutex for data synchronization
		std::unique_ptr<std::thread>	mDecodeThread;				///< A pointer to the thread object
		std::condition_variable			mContinueDecodingCondition;	///< Conditional variable used to signal the thread to do more decoding
		bool							mStopDecoding;
		float							mAudioPlaybackStartTime;
		float							mLastAudioSourcePlaybackTime;
		float							mAudioPlaybackAdvanceEstimation;
		bool							mFirstFrame;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
