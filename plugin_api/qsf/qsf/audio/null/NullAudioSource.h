// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioSource.h"
#include "qsf/base/manager/FastPodAllocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace null
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Null audio source resource class
		*/
		class NullAudioSource : public AudioSource, public FastPodAllocator<NullAudioSource>
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class NullAudioManager;	///< Only the null audio manager is allowed to created null audio source instances


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~NullAudioSource();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::AudioSource methods               ]
		//[-------------------------------------------------------]
		public:
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
			explicit NullAudioSource(AudioManager& audioManager, bool isSpatial);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // null
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/null/NullAudioSource-inl.h"
