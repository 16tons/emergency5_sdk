// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioSource.h"
#include "qsf/job/JobProxy.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <OpenAL/al.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
		*    OpenAL audio source resource class
		*/
		class OpenALAudioSource : public AudioSource, public FastPodAllocator<OpenALAudioSource>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class OpenALAudioManager;	///< Only the OpenAL audio manager is allowed to created OpenAL audio source instances


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~OpenALAudioSource();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioSource methods               ]
		//[-------------------------------------------------------]
		public:
			virtual bool load(const AudioBuffer* audioBuffer = nullptr) override;
			virtual void unload() override;
			virtual bool play(bool restart = false) override;
			virtual bool isPlaying() const override;
			virtual void pause() override;
			virtual bool isPaused() const override;
			virtual void stop() override;
			virtual bool isStopped() const override;
			virtual float getVolume() const override;
			virtual void setVolume(float volume) override;
			virtual uint32 getMixerChannel() const override;
			virtual void setMixerChannel(uint32 channel) override;
			inline virtual bool isSpatial() const override;
			virtual void setSpatial(bool spatial) override;
			inline virtual bool isLooping() const override;
			virtual void setLooping(bool looping) override;
			inline virtual float getPitch() const override;
			virtual void setPitch(float pitch) override;
			inline virtual float getReferenceDistance() const override;
			virtual void setReferenceDistance(float referenceDistance) override;
			inline virtual float getMaximumDistance() const override;
			virtual void setMaximumDistance(float maximumDistance) override;
			inline virtual float getRolloffFactor() const override;
			virtual void setRolloffFactor(float rolloffFactor) override;
			virtual const glm::vec3& getAttribute(Attribute attribute) const override;
			virtual bool setAttribute(Attribute attribute, const glm::vec3& value) override;
			virtual Time getPlaybackPosition() const override;
			virtual void setPlaybackPosition(const Time& time) override;
			virtual void startVolumeFade(float targetVolume, const Time& duration, bool stopWhenFinished) override;
			virtual void cancelVolumeFade() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] audioManager
			*    Owner audio manager, must stay valid as long as this audio source instance exists
			*/
			explicit OpenALAudioSource(AudioManager& audioManager, bool isSpatial);

			/**
			*  @brief
			*    Update the source
			*
			*  @note
			*    - This must only be done for currently active sources
			*/
			void update();

			/**
			*  @brief
			*    Modified the volume of the source to match the "real" volume which
			*    is combined from the source volume and the mixer channel volume
			*/
			void updateSourceVolume();

			/**
			*  @brief
			*    Used to perform automatic volume fades
			*/
			void updateVolumeFade(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ALuint		  mOpenALSource;		///< Audio source, 0 if no OpenAL stream is currently used
			OpenALStream* mOpenALStream;		///< Audio source stream if streamed buffer is used, else a null pointer, we're responsible for destroying the instance if we no longer need it
			bool		  mSpatial;				///< "true" if audio source is spatial (3D), else "false" (2D)
			bool		  mSpatialInitialized;	///< Initially "false", gets set after spatial member was explicitly initialized once
			bool		  mLooping;				///< Is the audio source looping?
			float		  mPitch;				///< Pitch
			float		  mReferenceDistance;	///< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively
			float		  mMaximumDistance;		///< Defines a distance beyond which the source will not be further attenuated by distance
			float		  mRolloffFactor;		///< This will scale the distance attenuation over the applicable range
			glm::vec3	  mPosition;			///< Audio position
			glm::vec3	  mVelocity;			///< Audio velocity
			uint32		  mMixerChannel;		///< Assigned mixer channel
			float		  mVolume;				///< Raw volume of the source, without any mixer volume changes applied
			JobProxy	  mFadeUpdateProxy;		///< Job proxy for automatic volume fading
			float		  mFadeVolumeSource;
			float		  mFadeVolumeTarget;
			Time		  mFadeDuration;
			Time		  mFadeTime;
			bool		  mFadeStopWhenFinished;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // openal
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/openal/OpenALAudioSource-inl.h"
