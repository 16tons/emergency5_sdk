// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetProxy.h"
#include "qsf/time/Time.h"
#include "qsf/job/JobProxy.h"
#include "qsf/job/JobArguments.h"
#include "qsf/base/WeakPtr.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class AudioSource;
	class AudioPlaybackManagementComponent;
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
	*    The audio proxy resembles the playback of an audio asset
	*
	*  @remarks
	*    You can keep a instance of the audio proxy around when you want to
	*    occasionally trigger a one-shot sound or event if you want to
	*    keep a self-cleaning "handle" for a playing SFX which you want
	*    to manually modify while its playing (e.g. changing the volume, pitch
	*    or other attributes).
	*
	*    The audio proxy defines a "chained interface" which means that you can
	*    configure a proxy step-by-step.
	*    Examples:
	*
	*    Playing a managed 3D sound on a entity which is automatically removed when it finished playing:
	*      AudioProxy().asset(AssetProxy("/my/cool/sfx.ogg")).contextEntity(mEntity).play().detach();
	*
	*    Playing a managed 2D sound for GUI:
	*      AudioProxy().asset(AssetProxy("/my/cool/button/click.ogg")).absolute().channel(qsf::AudioManager::MIXERCHANNEL_GUI).play().detach();
	*
	*    Starting a looped SFX and stopping it later:
	*      AudioProxy mySfx;
	*      mySfx.asset(AssetProxy("/my/cool/looped/sound.ogg")).spatial().channel(qsf::AudioManager::MIXERCHANNEL_SFX);
	*      mySfx.play();
	*      // later...
	*      mySfx.stop();
	*/
	class QSF_API_EXPORT AudioProxy : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AudioProxy();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioProxy();

		/**
		*  @brief
		*    Switches the proxy to another asset. If there is currently a
		*    SFX instance handled by the proxy, it will be stopped and destroyed.
		*/
		AudioProxy& asset(const AssetProxy& sfxAsset);

		/**
		*  @brief
		*    Switches the proxy to another source file. If there is currently a
		*    SFX instance handled by the proxy, it will be stopped and destroyed.
		*/
		AudioProxy& sourceFile(const std::string& sfxFilename);

		/**
		*  @brief
		*    Switches the context entity. If there is currently a SFX instance handled
		*    by the proxy, it will be re-attached to the new entity. It will not stop playing
		*    and it will not be destroyed.
		*/
		AudioProxy& contextEntity(Entity& contextEntity);

		/**
		*  @brief
		*    Changes the SFX to be context-free. If there is currently a SFX playing, it
		*    will be detached from the previous context entity if there was any. It will
		*    then be free and you can control it manually.
		*/
		AudioProxy& contextFree();

		/**
		*  @brief
		*    Checks if there is any audio source playing currently.
		*
		*  @return
		*    Will return "false" if there either is no currently managed source or the
		*    managed source has stopped playing. Else it will return "true".
		*/
		bool isPlaying() const;

		/**
		*  @brief
		*    Checks if there is any audio source currently paused.
		*
		*  @return
		*    Will return "false" if there either is no currently managed source or the
		*    managed source is not paused playing. Else it will return "true".
		*/
		bool isPaused() const;

		/**
		*  @brief
		*    Checks if there is any audio source currently stopped.
		*
		*  @return
		*    Will return "true" if there is no currently managed source or the
		*    managed source is stopped playing. Else it will return "false".
		*/
		bool isStopped() const;

		/**
		*  @brief
		*    Plays a SFX with the current configuration. If there still is a SFX managed by the proxy,
		*    it will be "continued" if it has been paused or just keep playing. To restart the SFX, @see restart()
		*    To launch a completely new SFX, detach the previous SFX first (@see detach())
		*/
		AudioProxy& play();

		/**
		*  @brief
		*    Plays a SFX with the current configuration after a delay
		*
		*  @param[in] delay
		*    delay time after which we call AudioProxy::play()
		*/
		AudioProxy& playDelayed(const Time& delay);

		/**
		*  @brief
		*   Restarts the currently playing SFX
		*/
		AudioProxy& restart();

		/**
		*  @brief
		*    Pause the audio source playback
		*/
		AudioProxy& pause();

		/**
		*  @brief
		*    Stops playback of the previous started SFX
		*/
		AudioProxy& stop();

		/**
		*  @brief
		*    Changes the currently playing source to looping mode.
		*    Don't detach a source that is looping! It will never be collected.
		*/
		AudioProxy& loop();

		/**
		*  @brief
		*    Changes the currently playing source to one-shot mode. In one-shot
		*    mode, the source is only played once. It is safe to detach from
		*    one-shot SFX
		*/
		AudioProxy& oneShot();

		/**
		*  @brief
		*    Changes the volume of the currently played SFX
		*/
		AudioProxy& volume(float newVolume);

		float getVolume() const;

		/**
		*  @brief
		*    Changes the pitch of the currently played SFX
		*/
		AudioProxy& pitch(float newPitch);

		/**
		*  @brief
		*    Changes the position of playback source. For spatial sounds, this is an absolute
		*    position in world space. For non-spatial sounds, the position is in listener space, i.e.
		*    always relative to the listener (it will move and rotate with it). So this is perfect to
		*    align 3D sources in the world or to do panning of 2D stereo/mono sounds
		*/
		AudioProxy& position(const glm::vec3& position);

		/**
		*  @brief
		*    Changes the roll-off factor of the sound which is used to attenuate the sound based on the distance
		*    to the listener
		*/
		AudioProxy& rollOffFactor(float factor);

		/**
		*  @brief
		*    Changes the reference listener distance of the sound. This is the distance where
		*    the source volume would normally drop to half before being attenuated by the
		*    rolloff factor or maximum distance
		*/
		AudioProxy& referenceDistance(float distance);

		/**
		*  @brief
		*    Changes the maximum listener distance at which the source will no longer be attenuated
		*/
		AudioProxy& maximumDistance(float maximumDistance);

		/**
		*  @brief
		*    Switches the source to be spatial. This means it is placed in 3D space
		*    relative to the listener.
		*/
		AudioProxy& spatial();

		/**
		*  @brief
		*    Switches source to absolute (i.e. 2D) playback mode
		*/
		AudioProxy& absolute();

		/**
		*  @brief
		*    Changes the mixer channel the SFX is routed to
		*/
		AudioProxy& channel(uint32 channelIndex);

		/**
		*  @brief
		*    Detaches the previously started source. This means you will no longer have access
		*    to the source and it will continue to play and when done, it will be automatically
		*    destroyed.
		*/
		AudioProxy& detach();

		/**
		*  @brief
		*    Indicates that the next playback buffer should be streamed.
		*
		*  @warn
		*    Switching an already playing audio proxy to "streamed" forces it to restart playback.
		*/
		AudioProxy& stream();

		/**
		*  @brief
		*    Indicates that the next playback buffer should be loaded into memory.
		*
		*  @warn
		*    Switching an already playing audio proxy to "in-memory" forces it to restart playback.
		*/
		AudioProxy& inMemory();

		/**
		*  @brief
		*    Starts to fade the audio source volume over time
		*
		*  @see cancelVolumeFade
		*
		*  @remarks
		*    There is only ever one fade active. Instructing a new fade will
		*    automatically cancel the previous one!
		*    This call also only has effect when the audio source is already playing.
		*
		*  @param[in] targetVolume
		*    Target volume when fade finished
		*
		*  @param[in] duration
		*    The duration of the fade
		*
		*  @param[in] stopWhenFinished
		*    Wether the source should be stopped automatically when the fade finishes
		*/
		AudioProxy& startVolumeFade(float targetVolume, const Time& duration, bool stopWhenFinished);

		/**
		*  @brief
		*    Cancels any currently running volume fade
		*
		*  @remarks
		*    You won't get any errors when there is no fade active. Just assume that after calling this,
		*    there is no fade active whether or not there was one active before.
		*    This call obviously has only any effect when the source is playing and fading.
		*
		*  @see startVolumeFade
		*/
		AudioProxy& cancelVolumeFade();

		/**
		*  @brief
		*    Lets you access the underlying audio source which is currently playing.
		*
		*  @return
		*    Returns the internally managed reference to the audio source. It can be nullptr when
		*    there is no instance managed currently!
		*
		*  @note
		*    Handle with care! Don't destroy it, don't store it! The source belongs to the proxy.
		*/
		AudioSource* getCurrentlyManagedAudioSource();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AudioProxy methods             ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Gets called when a user of this class calls play() or playDelayed()
		*/
		virtual void onPlay();

		/**
		*  @brief
		*    Gets called when a user of this class calls stop()
		*/
		virtual void onStop();

		/**
		*  @brief
		*    Gets called when a user of this class calls contextFree() or contextEntity()
		*/
		virtual void onContextChanged(Entity* entity);

		/**
		*  @brief
		*    Gets called when a user of this class calls detach()
		*/
		virtual void onDetach();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		const Time& getDelayTime() const;
		const uint32 getChannel() const;
		const bool isSpartial() const;
		const bool isAbsolute() const;
		const bool isLooping() const;
		const AssetProxy& getAssetProxy();
		const std::string& getFileName() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void destroyCurrentlyManagedSource();
		void setContext(Entity* contextEntity);
		void setStreamed(bool stream);

		void updateDelay(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WeakPtr<AudioPlaybackManagementComponent> mPlaybackHandler;
		AssetProxy	mSfxAsset;
		std::string	mSfxFilename;

		/**
		*  @brief
		*    Reference of the source which was played previously. Used to
		*    perform source-context-sensitive operations like volume changes.
		*/
		AudioSource* mCurrentlyManagedSource;

		bool		 mStream;				///< "true" if the audio is streamed, else "false"
		float		 mVolume;				///< Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		uint32		 mChannel;				///< Mixing channel index (anyone is valid)
		bool		 mSpatial;				///< "true" if audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		bool		 mLooping;				///< "true" if the audio source is looping, else "false"
		float		 mPitch;				///< Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster)
		float		 mReferenceDistance;	///< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively
		float		 mMaximumDistance;		///< Defines a distance beyond which the source will not be further attenuated by distance
		float		 mRolloffFactor;		///< This will scale the distance attenuation over the applicable range
		glm::vec3	 mPosition;				///< Manual spatial position
		Time		 mDelay;				///< Time for delayed playing
		Time		 mTimePassed;			///< Time passed since update delay started
		JobProxy	 mDelayUpdate;			///< Job proxy for delay


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
