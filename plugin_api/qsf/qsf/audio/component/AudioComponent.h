// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/job/JobProxy.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AudioSource;
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
	*    Audio component class
	*/
	class QSF_API_EXPORT AudioComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "qsf::AudioComponent" unique component ID
		static const uint32 ACTIVE;				///< "Active" unique class property ID inside the class
		static const uint32 AUDIO;				///< "Audio" unique class property ID inside the class
		static const uint32 STREAM;				///< "Stream" unique class property ID inside the class
		static const uint32 VOLUME;				///< "Volume" unique class property ID inside the class
		static const uint32 CHANNEL;			///< "Channel" unique class property ID inside the class
		static const uint32 SPATIAL;			///< "Spatial" unique class property ID inside the class
		static const uint32 LOOPING;			///< "Looping" unique class property ID inside the class
		static const uint32 PITCH;				///< "Pitch" unique class property ID inside the class
		static const uint32 REFERENCE_DISTANCE;	///< "ReferenceDistance" unique class property ID inside the class
		static const uint32 MAXIMUM_DISTANCE;	///< "MaximumDistance" unique class property ID inside the class
		static const uint32 ROLLOFF_FACTOR;		///< "RolloffFactor" unique class property ID inside the class
		static const uint32 AUTO_RESTART_TIME_MINIMUM;	///< "AutoRestartTimeMinimum" unique class property ID inside the class
		static const uint32 AUTO_RESTART_TIME_MAXIMUM;	///< "AutoRestartTimeMaximum" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit AudioComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~AudioComponent();

		/**
		*  @brief
		*    Return the audio source
		*
		*  @return
		*    The audio source, can be a null pointer, don't destroy the instance
		*
		*  @note
		*    - Don't use the audio source directly if it can be avoided
		*/
		inline AudioSource* getAudioSource() const;

		/**
		*  @brief
		*    Return if playback is paused or not
		*
		*  @return
		*    True when paused otherwise false
		*/
		inline bool isPlayBackPaused() const;

		/**
		*  @brief
		*    Sets if playback should be paused or not
		*
		*  @param[in] paused
		*    True when playback should be paused otherwise false
		*/
		inline void setPlayBackPaused(bool paused);

		/**
		*  @brief
		*    Return the audio asset to use
		*
		*  @return
		*    The audio asset to use (e.g. "sample/audio/animal/squirrel"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getAudio() const;

		/**
		*  @brief
		*    Set the audio asset to use
		*
		*  @param[in] audio
		*    The audio asset to use (e.g. "sample/audio/animal/squirrel")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAudio(const AssetProxy& audio);

		/**
		*  @brief
		*    Return whether or not the audio is streamed
		*
		*  @return
		*    "true" if the audio is streamed, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool isStream() const;

		/**
		*  @brief
		*    Set whether or not the audio is streamed
		*
		*  @param[in] stream
		*    "true" if the audio is streamed, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setStream(bool stream);

		/**
		*  @brief
		*    Return the volume
		*
		*  @return
		*    The volume, value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getVolume() const;

		/**
		*  @brief
		*    Set the volume
		*
		*  @param[in] volume
		*    The volume, value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setVolume(float volume);

		/**
		*  @brief
		*    Return the mixing channel
		*
		*  @return
		*    The channel index
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getChannel() const;

		/**
		*  @brief
		*    Set the mixing channel
		*
		*  @param[in] volume
		*    The mixing channel index, anyone is valid but few might actually be used by your project
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setChannel(uint32 channel);

		/**
		*  @brief
		*    Return whether the audio source is spatial or not
		*
		*  @return
		*    "true" if audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool isSpatial() const;

		/**
		*  @brief
		*    Set whether the audio source is spatial or not
		*
		*  @param[in] spatial
		*    If "true" the audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSpatial(bool spatial);

		/**
		*  @brief
		*    Return whether the audio source is looping or not
		*
		*  @return
		*    "true" if the audio source is looping, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool isLooping() const;

		/**
		*  @brief
		*    Set whether the audio source is looping or not
		*
		*  @param[in] looping
		*    If "true" the audio source is looping, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLooping(bool looping);

		/**
		*  @brief
		*    Return the pitch multiplier
		*
		*  @return
		*    The pitch multiplier, pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getPitch() const;

		/**
		*  @brief
		*    Set the pitch multiplier
		*
		*  @param[in] pitch
		*    The pitch multiplier, pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setPitch(float pitch);

		/**
		*  @brief
		*    Return the reference distance
		*
		*  @return
		*    The reference distance, used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getReferenceDistance() const;

		/**
		*  @brief
		*    Set the reference distance
		*
		*  @param[in] referenceDistance
		*    The reference distance, used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setReferenceDistance(float referenceDistance);

		/**
		*  @brief
		*    Return the maximum distance
		*
		*  @return
		*    The maximum distance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMaximumDistance() const;

		/**
		*  @brief
		*    Set the maximum distance
		*
		*  @param[in] maximumDistance
		*    The maximum distance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMaximumDistance(float maximumDistance);

		/**
		*  @brief
		*    Return the rolloff dactor
		*
		*  @return
		*    The rolloff factor, this will scale the distance attenuation over the applicable range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRolloffFactor() const;

		/**
		*  @brief
		*    Set the rolloff factor
		*
		*  @param[in] rolloffFactor
		*    The rolloff factor, this will scale the distance attenuation over the applicable range
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRolloffFactor(float rolloffFactor);

		/**
		*  @brief
		*    Return the minimum time in seconds until audio gets restarted automatically
		*
		*  @return
		*    The minimum time in seconds until audio gets restarted automatically
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		float getAutoRestartTimeMinimum() const;

		/**
		*  @brief
		*    Set the minimum time in seconds until audio gets restarted automatically
		*
		*  @param[in] seconds
		*    The minimum time in seconds until audio gets restarted automatically
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAutoRestartTimeMinimum(float seconds);

		/**
		*  @brief
		*    Return the maximum time in seconds until audio gets restarted automatically
		*
		*  @return
		*    Maximum time in seconds until audio gets restarted automatically; must be set to a value higher than zero for the restart to be active at all
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		float getAutoRestartTimeMaximum() const;

		/**
		*  @brief
		*    Set the maximum time in seconds until audio gets restarted automatically
		*
		*  @param[in] seconds
		*    Maximum time in seconds until audio gets restarted automatically; must be set to a value higher than zero for the restart to be active at all
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAutoRestartTimeMaximum(float seconds);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetActive(bool active) override;
		virtual void onSetSimulating(bool simulating) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set the audio source instance to use
		*
		*  @note
		*   - Do only call this method if the audio component is currently running
		*/
		void setAudioSourceInstance();

		/**
		*  @brief
		*    Destroy the used audio source instance
		*/
		void destroyAudioSourceInstance();

		void monitorAudio();
		void restartAudio();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		AudioSource* mAudioSource;				///< Audio source instance, can be a null pointer, we're responsible for destroying the instance
		JobProxy	 mMonitorJob;
		JobProxy	 mRestarterJob;
		bool		 mPlayBackPaused;			///< Indicates if audio playback is paused or not (default false)

		// Connected to the CAMP reflection system
		AssetProxy	 mAudio;					///< The audio asset to use (e.g. "sample/audio/animal/squirrel")
		bool		 mStream;					///< "true" if the audio is streamed, else "false"
		float		 mVolume;					///< Volume (value from 0.0-1.0 -> 0.0 = silence, 1.0 = full volume)
		uint32		 mChannel;					///< Mixing channel index (anyone is valid)
		bool		 mSpatial;					///< "true" if audio source is spatial (3D, do only use one channel audio files), else "false" (2D)
		bool		 mLooping;					///< "true" if the audio source is looping, else "false"
		float		 mPitch;					///< Pitch multiplier (pitch<1.0=slower/pitch=1.0=normal/pitch>1.0=faster)
		float		 mReferenceDistance;		///< Used to increase or decrease the range of a source by decreasing or increasing the attenuation, respectively
		float		 mMaximumDistance;			///< Defines a distance beyond which the source will not be further attenuated by distance
		float		 mRolloffFactor;			///< This will scale the distance attenuation over the applicable range
		float		 mAutoRestartTimeMinimum;	///< Minimum time in seconds until audio gets restarted automatically
		float		 mAutoRestartTimeMaximum;	///< Maximum time in seconds until audio gets restarted automatically; must be set to a value higher than zero for the restart to be active at all


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/audio/component/AudioComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AudioComponent)
