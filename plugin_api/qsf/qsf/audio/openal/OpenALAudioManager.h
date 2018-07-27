// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioManager.h"
#include "qsf/time/Time.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include <ogg/os_types.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace openal
	{
		namespace detail
		{
			class BackgroundStreamingContext;
		}
		class OpenALAudioBuffer;
		class OpenALAudioSource;
		class OpenALStreamOgg;
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
		*    OpenAL audio backend class
		*/
		class OpenALAudioManager : public AudioManager
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend OpenALAudioSource;	// Needs to register/unregister itself
			friend OpenALStreamOgg;		// Needs to register/unregister itself


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			static const Time CACHE_DECAY_DELAY;


		//[-------------------------------------------------------]
		//[ Public structures                                     ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Read from memory structure for the custom OpenAL file callbacks
			*/
			struct MemoryData
			{
				const uint8* data;		///< Current data position, always valid, do not destroy the data
				const uint8* dataEnd;	///< Data end position, always valid, do not destroy the data
			};


		//[-------------------------------------------------------]
		//[ Custom static OpenAL file callbacks                   ]
		//[-------------------------------------------------------]
		public:
			// From "qsf::File"
			static size_t read_func(void* buffer, size_t size, size_t nmemb, void* dataSource);
			static int seek_func(void* dataSource, ogg_int64_t offset, int whence);
			static int close_func(void* dataSource);
			static long tell_func(void* dataSource);
			// From memory
			static size_t read_func_mem(void* buffer, size_t size, size_t nmemb, void* dataSource);
			static int seek_func_mem(void* dataSource, ogg_int64_t offset, int whence);
			static int close_func_mem(void* dataSource);
			static long tell_func_mem(void* dataSource);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			OpenALAudioManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OpenALAudioManager();

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Startup the OpenAL audio manager
			*/
			virtual bool startup() override;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioManager methods              ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getDescription() const override;
			virtual void getFormatList(std::vector<Format>& formats) const override;
			virtual float getVolume() const override;
			virtual void setVolume(float volume) override;
			virtual float getPitch() const override;
			virtual void setPitch(float pitch) override;
			virtual float getDopplerFactor() const override;
			virtual void setDopplerFactor(float factor) override;
			virtual void pauseAllActiveAudioSources() override;
			virtual void resumeAllActiveAudioSources() override;
			virtual void update() override;
			//[-------------------------------------------------------]
			//[ Create audio buffer/source                            ]
			//[-------------------------------------------------------]
			virtual AudioBuffer* createAudioBuffer(const std::string& filename, bool stream = false) override;
			virtual AudioBuffer* createAudioBuffer() override;
			virtual void destroyAudioBuffer(AudioBuffer& audioBuffer) override;
			virtual AudioSource* createAudioSourceWithAudioBuffer(AudioBuffer* audioBuffer = nullptr, bool isSpatial = false) override;
			virtual AudioSource* createAudioSource(const std::string& filename, bool stream = false, bool isSpatial = false) override;
			virtual void destroyAudioSource(AudioSource& audioSource) override;
			virtual bool queryDurationOfAudioAsset(const std::string& audioFile, Time& outDuration) override;
			//[-------------------------------------------------------]
			//[ Listener                                              ]
			//[-------------------------------------------------------]
			virtual const glm::vec3& getListenerAttribute(ListenerAttribute listenerAttribute) const override;
			virtual bool setListenerAttribute(ListenerAttribute listenerAttribute, const glm::vec3& value) override;


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Basically the same as alGetError(), but it will additionally log the error
			*/
			static ALenum checkForOpenALError();

			/**
			*  @brief
			*    Basically the same as alGetError(), but it will additionally log the error with the given message and an text representation of the OpenAL error
			*
			*  @param[in] errorMessage
			*    The message which should be logged when an error was found. Must be an valid C-string
			*/
			static ALenum checkAndLogOpenALError(const char* errorMessage);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::AudioManager methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void notifyChannelVolumeChanged(uint32 channelIndex, float newVolume) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Add an active audio source to the audio manager
			*
			*  @param[in] openALAudioSource
			*    OpenAL audio source to add
			*
			*  @return
			*    "true" if all went fine and the audio source was added to the audio manager, else "false"
			*
			*  @note
			*    - The audio source itself is only added to the audio managers list of sources
			*/
			bool addActiveAudioSource(OpenALAudioSource& openALAudioSource);

			/**
			*  @brief
			*    Remove an active audio source from the audio manager
			*
			*  @param[in] openALAudioSource
			*    OpenAL audio source to remove
			*
			*  @return
			*    "true" if all went fine and the audio source was removed from the audio manager, else "false" (maybe the source isn't in the audio manager)
			*
			*  @note
			*    - The audio source itself isn't destroyed, it is just removed from the audio managers list of sources
			*/
			bool removeActiveAudioSource(OpenALAudioSource& openALAudioSource);

			bool addActiveStreamOgg(OpenALStreamOgg& openAlStreamOgg);
			bool removeActiveStreamOgg(OpenALStreamOgg& openAlStreamOgg, bool isCalledFromThread);

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Shutdown the OpenAL audio manager
			*/
			void shutdown();


		//[-------------------------------------------------------]
		//[ Private types                                         ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Internal data stored for cached audio buffers
			*/
			struct CachedAudioBuffer
			{
				OpenALAudioBuffer* mAudioBuffer;
				std::vector<AudioSource*> mAudioSources;
				Time mDeleteAfter;

				inline CachedAudioBuffer() :
					mAudioBuffer(nullptr)
				{
					// Nothing to do in here
				}

				inline explicit CachedAudioBuffer(OpenALAudioBuffer* audioBuffer) :
					mAudioBuffer(audioBuffer)
				{
					// Nothing to do in here
				}
			};


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string						mDeviceName;								///< ASCII name of the device to use, if empty string the "preferred device" will be selected
			ALCdevice*						mDevice;									///< Device, can be a null pointer
			ALCcontext*						mContext;									///< Context, can be a null pointer
			uint32							mEaxLevel;									///< EAX level
			float							mPitch;										///< Master pitch
			glm::vec3						mListenerAttributes[LISTENER_ATTRIBUTES];	///< Listener attributes
			std::vector<OpenALAudioSource*> mActiveOpenALAudioSources;					///< Current active audio sources of this audio manager
			std::vector<OpenALAudioSource*>		mAudioSources;							///< Audio sources of this audio manager
			std::vector<OpenALAudioBuffer*>		mExplicitAudioBuffers;					///< Explicitly created audio buffers of this audio manager
			boost::container::flat_map<std::pair<uint32, bool>, CachedAudioBuffer> mCachedAudioBuffersMap;	///< Map of implicitly created audio buffers of this audio manager
			std::queue<std::pair<uint32, bool>>	mUnusedCachedAudioBuffers;
			detail::BackgroundStreamingContext&	mBackgroundStreamingContext;
			std::vector<OpenALAudioSource*>		mPausedAudioSources;					///< Holds a list of all audio sources paused by pauseAllActiveAudioSources


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf
