// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/settings/SettingsGroup.h"
#include "qsf/base/UniqueInstance.h"


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
	*    QSF audio settings group implementation
	*/
	class QSF_API_EXPORT AudioSettingsGroup : public SettingsGroup, public UniqueInstance<AudioSettingsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;					///< "qsf::AudioSettingsGroup" unique pluginable settings group ID
		// Properties: Restart required
		static const uint32 AUDIO_SYSTEM;					///< "AudioSystem" unique class property ID inside the class
		static const uint32 ENABLE_OPENAL_SOFT;				///< "EnableOpenALSoft" unique class property ID inside the class
		// Properties: Runtime change possible
		static const uint32 MIXER_CHANNEL_MASTER_VOLUME;	///< "MixerChannelMasterVolume" unique class property ID inside the class
		static const uint32 MIXER_CHANNEL_SPEECH_VOLUME;	///< "MixerChannelSpeechVolume" unique class property ID inside the class
		static const uint32 MIXER_CHANNEL_SFX_VOLUME;		///< "MixerChannelSFXVolume" unique class property ID inside the class
		static const uint32 MIXER_CHANNEL_MUSIC_VOLUME;		///< "MixerChannelMusicVolume" unique class property ID inside the class
		static const uint32 MIXER_CHANNEL_GUI_VOLUME;		///< "MixerChannelGUIVolume" unique class property ID inside the class
		static const uint32 MIXER_CHANNEL_VIDEO_VOLUME;		///< "MixerChannelVideoVolume" unique class property ID inside the class
		static const uint32 ENABLE_OPENAL_SOFT_HRTF;		///< "EnableOpenALSoftHrtf" unique class property ID inside the class

		/**
		*  @brief
		*    Audio system
		*/
		enum AudioSystem
		{
			NULL_SYSTEM   = 0,	///< Null audio system
			OPENAL_SYSTEM = 1	///< OpenAL audio system
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] settingsGroupManager
		*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
		*/
		explicit AudioSettingsGroup(SettingsGroupManager* settingsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AudioSettingsGroup();

		//[-------------------------------------------------------]
		//[ Restart required                                      ]
		//[-------------------------------------------------------]
		inline AudioSystem getAudioSystem() const;
		inline void setAudioSystem(AudioSystem audioSystem);

		inline bool getEnableOpenALSoft() const;
		inline void setEnableOpenALSoft(bool enableOpenALSoft);

		//[-------------------------------------------------------]
		//[ Runtime change possible                               ]
		//[-------------------------------------------------------]
		float getMixerChannelMasterVolume() const;
		void setMixerChannelMasterVolume(float newVolume);

		float getMixerChannelSpeechVolume() const;
		void setMixerChannelSpeechVolume(float newVolume);

		float getMixerChannelSFXVolume() const;
		void setMixerChannelSFXVolume(float newVolume);

		float getMixerChannelMusicVolume() const;
		void setMixerChannelMusicVolume(float newVolume);

		float getMixerChannelGUIVolume() const;
		void setMixerChannelGUIVolume(float newVolume);

		float getMixerChannelVideoVolume() const;
		void setMixerChannelVideoVolume(float newVolume);

		// TODO(sw) Runtime change only possible when not using OpenAL32.dll from creative-labs (memory layout seems to be broken in this case, an invalid device pointer is passed to the extension method inside OpenAL soft)
		inline bool getEnableOpenALSoftHrtf() const;
		inline void setEnableOpenALSoftHrtf(bool enableOpenALSoftHrtf);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		float getMixerChannelVolume(uint32 mixerChannelIndex) const;
		void setMixerChannelVolumeAndPromote(uint32 mixerChannelIndex, uint32 properyId, float newVolume);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AudioSystem mAudioSystem;			///< Audio system to use
		bool		mEnableOpenALSoft;		///< Should OpenAL soft as OpenAL implementation be used
		float		mMixerChannelMasterVolume;
		float		mMixerChannelSpeechVolume;
		float		mMixerChannelSFXVolume;
		float		mMixerChannelMusicVolume;
		float		mMixerChannelGUIVolume;
		float		mMixerChannelVideoVolume;
		bool		mEnableOpenALSoftHrtf;	///< Enables HRTF (Head-Related Transfer Function: http://kcat.strangesoft.net/openal-extensions/SOFT_HRTF.txt) Needs ALC_SOFT_HRTF extension


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
#include "qsf/settings/AudioSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AudioSettingsGroup)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::AudioSettingsGroup::AudioSystem)
