// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AudioSettingsGroup::AudioSystem AudioSettingsGroup::getAudioSystem() const
	{
		return mAudioSystem;
	}

	inline void AudioSettingsGroup::setAudioSystem(AudioSystem audioSystem)
	{
		assignAndPromoteChange(mAudioSystem, audioSystem, AUDIO_SYSTEM);
	}

	inline bool AudioSettingsGroup::getEnableOpenALSoft() const
	{
		return mEnableOpenALSoft;
	}

	inline void AudioSettingsGroup::setEnableOpenALSoft(bool enableOpenALSoft)
	{
		assignAndPromoteChange(mEnableOpenALSoft, enableOpenALSoft, ENABLE_OPENAL_SOFT);
	}

	inline bool AudioSettingsGroup::getEnableOpenALSoftHrtf() const
	{
		return mEnableOpenALSoftHrtf;
	}

	inline void AudioSettingsGroup::setEnableOpenALSoftHrtf(bool enableOpenALSoftHrtf)
	{
		assignAndPromoteChange(mEnableOpenALSoftHrtf, enableOpenALSoftHrtf, ENABLE_OPENAL_SOFT_HRTF);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
