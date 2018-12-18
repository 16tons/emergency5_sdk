// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const AudioManager::Statistics& AudioManager::getStatistics() const
	{
		return mStatistics;
	}

	inline AudioMixer& AudioManager::getAudioMixer()
	{
		return *mAudioMixer;
	}

	inline const AudioMixer& AudioManager::getAudioMixer() const
	{
		return *mAudioMixer;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
