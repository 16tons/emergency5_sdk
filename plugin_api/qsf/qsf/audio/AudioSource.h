// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioResource.h"
#include "qsf/time/Time.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioBuffer;
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
	*    Abstract audio source resource base class
	*
	*  @note
	*    - For sources with 3D spatialization, do only use one channel buffers because not each audio backend may be capable to use 3D spatialization for buffers with multiple channels
	*/
	class QSF_API_EXPORT AudioSource : public AudioResource
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Audio source flags
		*/
		enum Flags
		{
			NO_MASTER_PITCH		= 1<<0,	///< The master pitch has no influence on this audio source
			AUTOCLEANUP			= 1<<1,	///< Destroy the source automatically when it finished playing (for fire-and-forget sources) Be careful with looping SFX!
		};

		/**
		*  @brief
		*    Audio source attribute
		*/
		enum Attribute
		{
			POSITION_ATTRIBUTE = 0,	///< Position (float3, x/y/z)
			VELOCITY_ATTRIBUTE = 1	///< Velocity in meters per second (float3, x/y/z)
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the audio buffer the audio source is using
		*
		*  @return
		*    The used audio buffer, can be a null pointer, do not destroy the instance
		*/
		inline const AudioBuffer* getAudioBuffer() const;

		/**
		*  @brief
		*    Return the audio source flags
		*
		*  @return
		*    Audio source flags (see "qsf::AudioSource::Flags")
		*/
		inline uint32 getFlags() const;

		/**
		*  @brief
		*    Set the audio source flags
		*
		*  @param[in] flags
		*    Audio source flags (see "qsf::AudioSource::Flags")
		*/
		inline void setFlags(uint32 flags);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AudioSource methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load the audio source with the given audio buffer
		*
		*  @param[in] audioBuffer
		*    Audio buffer the audio source should load, can be a null pointer (in this case just the same as "qsf::AudioSource::unload()")
		*
		*  @return
		*   "true" if all went fine, else "false"
		*/
		inline virtual bool load(const AudioBuffer* audioBuffer);

		/**
		*  @brief
		*    Unload the audio source
		*/
		inline virtual void unload();

		/**
		*  @brief
		*    Play the audio source
		*
		*  @param[in] restart
		*    Restart audio source if it's already playing?
		*
		*  @return
		*    "true" if all went fine, else "false" (maybe no audio buffer is loaded)
		*/
		virtual bool play(bool restart = false) = 0;

		/**
		*  @brief
		*    Check whether the audio source is currently playing or not
		*
		*  @return
		*    "true" if the audio source is playing at the moment, else "false"
		*
		*  @note
		*    - If the audio source is paused this function will return "true" because the audio source is played at the moment - but paused
		*/
		virtual bool isPlaying() const = 0;

		/**
		*  @brief
		*    Pause the audio source playback
		*
		*  @note
		*    - Use "qsf::AudioSource::play()" to continue the playback
		*/
		virtual void pause() = 0;

		/**
		*  @brief
		*    Check whether the audio source is currently paused or not
		*
		*  @return
		*    "true" if the audio source is paused at the moment, else "false"
		*/
		virtual bool isPaused() const = 0;

		/**
		*  @brief
		*    Stop the audio source playback
		*/
		virtual void stop() = 0;

		/**
		*  @brief
		*    Check whether the audio source is currently stopped or not
		*
		*  @return
		*    "true" if the audio source is stopped at the moment, else "false"
		*/
		virtual bool isStopped() const = 0;

		/**
		*  @brief
		*    Return the volume of the audio source
		*
		*  @return
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual float getVolume() const = 0;

		/**
		*  @brief
		*    Set the volume of the audio source
		*
		*  @param[in] volume
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual void setVolume(float volume) = 0;

		/**
		*  @brief
		*    Starts automatic fading of the volume over a given duration
		*/
		virtual void startVolumeFade(float targetVolume, const qsf::Time& duration, bool stopWhenFinished) = 0;

		/**
		*  @brief
		*    Stops any currently running volume fade
		*/
		virtual void cancelVolumeFade() = 0;

		/**
		*  @brief
		*    Return whether the audio source is spatial or not
		*
		*  @return
		*    "true" if audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		*/
		virtual bool isSpatial() const = 0;

		/**
		*  @brief
		*    Set whether the audio source is spatial or not
		*
		*  @param[in] spatial
		*    If "true" the audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		*/
		virtual void setSpatial(bool spatial) = 0;

		/**
		*  @brief
		*    Return whether the audio source is looping or not
		*
		*  @return
		*    "true" if the audio source is looping, else "false"
		*/
		virtual bool isLooping() const = 0;

		/**
		*  @brief
		*    Set whether the audio source is looping or not
		*
		*  @param[in] looping
		*    If "true" the audio source is looping, else "false"
		*/
		virtual void setLooping(bool looping) = 0;

		/**
		*  @brief
		*    Changes the mixing channel of this source
		*
		*  @param[in] channel
		*    Index of the mixing channel to assign to this source. Anyone is valid,
		*    but only a few might actually be used by your project
		*/
		virtual void setMixerChannel(uint32 channel) = 0;

		/**
		*  @brief
		*    Returns the current mixing channel this source is assigned to
		*/
		virtual uint32 getMixerChannel() const = 0;

		/**
		*  @brief
		*    Return the pitch multiplier
		*
		*  @return
		*    Pitch multiplier
		*
		*  @remarks
		*    pitch < 1.0 = slower\n
		*    pitch = 1.0 = normal\n
		*    pitch > 1.0 = faster
		*/
		virtual float getPitch() const = 0;

		/**
		*  @brief
		*    Set the pitch multiplier
		*
		*  @param[in] pitch
		*    Pitch multiplier
		*
		*  @see
		*    - "qsf::AudioSource::getPitch()"
		*/
		virtual void setPitch(float pitch) = 0;

		/**
		*  @brief
		*    Return the reference distance
		*
		*  @return
		*    Reference distance
		*
		*  @remarks
		*    Used to increase or decrease the range of a audio source by decreasing or increasing the attenuation, respectively.
		*/
		virtual float getReferenceDistance() const = 0;

		/**
		*  @brief
		*    Set the reference distance
		*
		*  @param[in] referenceDistance
		*    Reference distance
		*
		*  @note
		*    - See "qsf::AudioSource::getReferenceDistance()"
		*/
		virtual void setReferenceDistance(float referenceDistance) = 0;

		/**
		*  @brief
		*    Return the maximum distance
		*
		*  @return
		*    Maximum distance
		*
		*  @remarks
		*    Defines a distance beyond which the audio source will not be further attenuated by distance.
		*/
		virtual float getMaximumDistance() const = 0;

		/**
		*  @brief
		*    Set the maximum distance
		*
		*  @param[in] maximumDistance
		*    Maximum distance
		*
		*  @see
		*    - "qsf::AudioSource::getMaximumDistance()"
		*/
		virtual void setMaximumDistance(float maximumDistance) = 0;

		/**
		*  @brief
		*    Return the roll off factor
		*
		*  @return
		*    Roll off factor
		*
		*  @remarks
		*    This will scale the distance attenuation over the applicable range.
		*/
		virtual float getRolloffFactor() const = 0;

		/**
		*  @brief
		*    Set the roll off factor
		*
		*  @param[in] rolloffFactor
		*    Roll off factor
		*
		*  @see
		*    - "qsf::AudioSource::getRolloffFactor()"
		*/
		virtual void setRolloffFactor(float rolloffFactor) = 0;

		/**
		*  @brief
		*    Get a audio source attribute value
		*
		*  @param[in] attribute
		*    Audio source attribute to return
		*
		*  @return
		*    Requested audio source attribute value
		*/
		virtual const glm::vec3& getAttribute(Attribute attribute) const = 0;

		/**
		*  @brief
		*    Set a audio source attribute value
		*
		*  @param[in] attribute
		*    Audio source attribute to set
		*  @param[in] value
		*    New audio source attribute value
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool setAttribute(Attribute attribute, const glm::vec3& value) = 0;

		/**
		*  @brief
		*    Looks up the current playback time of the sample if it is playing
		*/
		virtual Time getPlaybackPosition() const = 0;

		/**
		*  @brief
		*    Changes the current playback position on the source
		*/
		virtual void setPlaybackPosition(const Time& time) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] audioManager
		*    Owner audio manager, must stay valid as long as this audio source exists
		*/
		inline explicit AudioSource(AudioManager& audioManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AudioSource();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			   mFlags;			///< Flags (see "qsf::AudioSource::Flags")
		const AudioBuffer* mAudioBuffer;	///< Audio buffer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioSource-inl.h"
