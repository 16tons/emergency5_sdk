// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AudioComponent::~AudioComponent()
	{
		// Nothing to do in here
	}

	inline AudioSource* AudioComponent::getAudioSource() const
	{
		return mAudioSource;
	}

	inline bool AudioComponent::isPlayBackPaused() const
	{
		return mPlayBackPaused;
	}

	inline void AudioComponent::setPlayBackPaused(bool paused)
	{
		mPlayBackPaused = paused;
	}

	inline const AssetProxy& AudioComponent::getAudio() const
	{
		return mAudio;
	}

	inline bool AudioComponent::isStream() const
	{
		return mStream;
	}

	inline float AudioComponent::getVolume() const
	{
		return mVolume;
	}

	inline uint32 AudioComponent::getChannel() const
	{
		return mChannel;
	}

	inline bool AudioComponent::isSpatial() const
	{
		return mSpatial;
	}

	inline bool AudioComponent::isLooping() const
	{
		return mLooping;
	}

	inline float AudioComponent::getPitch() const
	{
		return mPitch;
	}

	inline float AudioComponent::getReferenceDistance() const
	{
		return mReferenceDistance;
	}

	inline float AudioComponent::getMaximumDistance() const
	{
		return mMaximumDistance;
	}

	inline float AudioComponent::getRolloffFactor() const
	{
		return mRolloffFactor;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
