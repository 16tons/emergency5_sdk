// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioResource;
	class AudioSource;
	class AudioBuffer;
	class AudioMixer;
	class Time;
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
	*    Abstract audio manager base class
	*
	*  @note
	*    - Does not unload unused resources automatically by default
	*    - The audio manager and related classes are based on codes from the open-source engine PixelLight (MIT license, www.pixellight.org), for more backend like FMOD Ex see PixelLight
	*
	*  @todo
	*    - TODO(co) Audio is a good multithreading candidate: Load and stream audio in a separate thread
	*/
	class QSF_API_EXPORT AudioManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class AudioResource;	///< Audio resources automatically register and unregister themselves
		friend class AudioMixer;	///< Audio mixer notifies when channel volumes change


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Listener attribute
		*/
		enum ListenerAttribute
		{
			LISTENER_POSITION	= 0,	///< Listener position (float3, x/y/z)
			LISTENER_VELOCITY	= 1,	///< Listener velocity in meters per second (float3, x/y/z)
			LISTENER_FORWARD	= 2,	///< Listener forward unit length orientation vector (float3, x/y/z)
			LISTENER_UPWARD		= 3,	///< Listener upwards facing unit length orientation vector (float3, x/y/z)
			LISTENER_ATTRIBUTES	= 4		///< Number of listener attributes
		};

		/**
		*  @brief
		*    Default hard-codec mixer channels
		*    TODO(oi) Mixer channel configuration should be project-dependent and not hardcoded.
		*/
		enum DefaultMixerChannels
		{
			MIXERCHANNEL_MASTER = 0,	///< Master channel, everything routes finally into this channel
			MIXERCHANNEL_SPEECH = 1,	///< Speech channel, routing into the master
			MIXERCHANNEL_SFX    = 2,	///< SFX channel, routing into the master
			MIXERCHANNEL_MUSIC  = 3,	///< Music channel, routing into the master
			MIXERCHANNEL_GUI    = 4,	///< GUI SFX channel, routing into the master
			MIXERCHANNEL_VIDEO  = 5		///< Video-audio channel, routing into the master
		};


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Audio statistics
		*/
		struct Statistics
		{
			uint32 numberOfAudioBuffers;	///< Number of audio buffers
			uint32 numberOfAudioSources;	///< Number of audio sources
			uint32 numberOfActiveSources;	///< Number of currently active audio sources
		};

		/**
		*  @brief
		*    Audio format
		*/
		struct Format
		{
			std::string format;			///< UTF-8 audio format, e.g. "ogg"
			std::string description;	///< UTF-8 audio format description
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioManager();

		/**
		*  @brief
		*    Return the audio statistics
		*
		*  @return
		*    The audio statistics, do not destroy the instance
		*/
		inline const Statistics& getStatistics() const;

		/**
		*  @brief
		*    Access to the audio mixer for mixing channel management
		*/
		inline AudioMixer& getAudioMixer();

		/**
		*  @brief
		*    Access to the audio mixer for mixing channel management
		*/
		inline const AudioMixer& getAudioMixer() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::AudioManager methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the audio manager description
		*
		*  @return
		*    UTF-8 audio manager description
		*/
		virtual std::string getDescription() const = 0;

		/**
		*  @brief
		*    Get a list of all known audio formats
		*
		*  @param[out] formats
		*    List to receive the known audio formats, the given list is not cleared before the formats are added
		*/
		virtual void getFormatList(std::vector<Format>& formats) const = 0;

		/**
		*  @brief
		*    Get the master volume
		*
		*  @return
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual float getVolume() const = 0;

		/**
		*  @brief
		*    Set the master volume
		*
		*  @param[in] volume
		*    Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		*/
		virtual void setVolume(float volume) = 0;

		/**
		*  @brief
		*    Return the master pitch multiplier
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
		*    Set the master pitch multiplier
		*
		*  @param[in] pitch
		*    Pitch multiplier
		*
		*  @see
		*    - "qsf::AudioManager::getPitch()"
		*/
		virtual void setPitch(float pitch) = 0;

		/**
		*  @brief
		*    Get the doppler factor
		*
		*  @return
		*    Doppler factor
		*
		*  @remarks
		*    If velocities are applied to the listener object or to any source object, then
		*    doppler shift will be applied to the audio. The following formula is used to
		*    calculate doppler shift:\n
		*    f’ = f*(DV – DF*vl)/(DV + DF*vs)\n
		*    DV = doppler velocity\n
		*    DF = doppler factor\n
		*    vl = listener velocity (scalar value along source-to-listener vector)\n
		*    vs = source velocity (scalar value along source-to-listener vector)\n
		*    f = frequency of sample\n
		*    f’ = Doppler shifted frequency\n
		*    The doppler velocity (340) represents the speed of audio.\n
		*    The doppler factor is used to exaggerate or de-emphasize the doppler shift.
		*/
		virtual float getDopplerFactor() const = 0;

		/**
		*  @brief
		*    Set the doppler factor
		*
		*  @param[in] factor
		*    Doppler factor
		*
		*  @see
		*    - "qsf::AudioManager::getDopplerFactor()"
		*/
		virtual void setDopplerFactor(float factor) = 0;

		/**
		*  @brief
		*    Pauses all currently active audio sources
		*/
		virtual void pauseAllActiveAudioSources() = 0;

		/**
		*  @brief
		*    Resume all currently active audio sources
		*/
		virtual void resumeAllActiveAudioSources() = 0;

		/**
		*  @brief
		*    Update the audio manager
		*/
		virtual void update() = 0;

		//[-------------------------------------------------------]
		//[ Create audio buffer/source                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create a new audio buffer
		*
		*  @param[in] filename
		*    UTF-8 audio filename, if empty, only create the audio buffer without loading it (supported file formats are API dependent)
		*  @param[in] stream
		*   Stream the file? (recommended for large files!)
		*
		*  @return
		*    The created audio buffer, a null pointer on error, don't destroy the instance
		*
		*  @note
		*    - This method will always create a new audio buffer, use "qsf::AudioManager::createCachedAudioSource(...)" instead whenever possible
		*    - Not each audio buffer can be streamed, use "qsf::AudioBuffer::isStreamed()" to check whether streaming is used
		*/
		virtual AudioBuffer* createAudioBuffer(const std::string& filename, bool stream = false) = 0;
		virtual AudioBuffer* createAudioBuffer() = 0;

		/**
		*  @brief
		*    Destroys a audio buffer instance
		*
		*  @param[in] audioBuffer
		*    Reference to the audio buffer which should be destroyed. Don't use the reference anymore after
		*    destroying it as it will be gone!
		*/
		virtual void destroyAudioBuffer(AudioBuffer& audioBuffer) = 0;

		/**
		*  @brief
		*    Create an audio source with the specified audio buffer
		*
		*  @param[in] audioBuffer
		*    Audio buffer to load, a null pointer if load no audio buffer by default
		*  @param[in] isSpatial
		*    Indicates if the source is spatial (3D sound)
		*
		*  @return
		*    The created audio source, a null pointer on error, don't destroy the instance yourself! Instead, call destroyAudioSource()
		*
		*  @note
		*    - Only use this method if you need a unique mutable AudioBuffer instance, otherwise use "qsf::AudioManager::createAudioSource(...)" instead
		*/
		virtual AudioSource* createAudioSourceWithAudioBuffer(AudioBuffer* audioBuffer = nullptr, bool isSpatial = false) = 0;

		/**
		*  @brief
		*    Create an audio source, potentially sharing and caching the underlying audio buffer
		*
		*  @param[in] filename
		*    UTF-8 audio filename
		*  @param[in] stream
		*   Stream the file? (recommended for large files!)
		*  @param[in] isSpatial
		*    Indicates if the source is spatial (3D sound)
		*
		*  @return
		*    The created audio buffer, a null pointer on error, don't destroy the instance
		*
		*  @note
		*    - This method will add all created audio buffers to an internal cache and will reuse the same audio buffer object if called again with the same filename/stream parameters
		*    - Not each audio buffer can be streamed, use "qsf::AudioBuffer::isStreamed()" to check whether streaming is used
		*
		*  @remarks
		*    The simplest creation of a audio source ready for playback would be:
		*    @code
		*    AudioSource* audioSource = audioManager->createAudioSource("MyAudio.ogg");
		*    @endcode
		*/
		virtual AudioSource* createAudioSource(const std::string& filename, bool stream = false, bool isSpatial = false) = 0;

		/**
		*  @brief
		*    Destroys a audio source instance
		*
		*  @param[in] audioSource
		*    Reference to the audio source which should be destroyed. Don't use the reference anymore after
		*    destroying it as it will be gone!
		*/
		virtual void destroyAudioSource(AudioSource& audioSource) = 0;

		/**
		*  @brief
		*    Queries the playback duration of any audio file
		*
		*  @param[in] audioFile
		*    Any reference to a file which resembles a valid audio asset (i.e. you could create an AudioBuffer from it)
		*  @param[out] outDuration
		*    Reference to a qsf::Time where the duration of the asset is stored in case it can be retrieved from the asset.
		*
		*  @return
		*    Returns "true" when the outDuration could be filled out or "false" if there was an error, e.g. the passed asset is no audio asset or
		*    the length could not be determined from the data of the asset.
		*/
		virtual bool queryDurationOfAudioAsset(const std::string& audioFile, Time& outDuration) = 0;

		//[-------------------------------------------------------]
		//[ Listener                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a listener attribute value
		*
		*  @param[in] listenerAttribute
		*    Listener attribute to return
		*
		*  @return
		*    Requested listener attribute value
		*/
		virtual const glm::vec3& getListenerAttribute(ListenerAttribute listenerAttribute) const = 0;

		/**
		*  @brief
		*    Set a listener attribute value
		*
		*  @param[in] listenerAttribute
		*    Listener attribute to set
		*  @param[in] value
		*    New listener attribute value
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		virtual bool setListenerAttribute(ListenerAttribute listenerAttribute, const glm::vec3& value) = 0;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the audio manager
		*/
		virtual bool startup() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		AudioManager();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AudioManager methods           ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Notifies the implementation of the audio manager that a channel volume has changed
		*
		*  @remarks
		*    When channels are chained, this is called for every channel which volume is changing,
		*    not only the actually changed channel, so no need to do recursive resolves in here.
		*/
		virtual void notifyChannelVolumeChanged(uint32 channelIndex, float newVolume) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Statistics	mStatistics;	///< Audio statistics
		AudioMixer*	mAudioMixer;	///< Personal audio mixer


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add a audio resource to the audio manager
		*
		*  @param[in] audioResource
		*    Audio resource to add
		*
		*  @note
		*    - The audio resource itself is only added to the audio manager's list of resources
		*/
		void addAudioResource(AudioResource& audioResource);

		/**
		*  @brief
		*    Remove a audio resource from the audio manager
		*
		*  @param[in] audioResource
		*    Audio resource to remove
		*
		*  @note
		*    - The audio resource itself isn't destroyed, it is just removed from the audio manager's list of resources
		*/
		void removeAudioResource(AudioResource& audioResource);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/AudioManager-inl.h"
