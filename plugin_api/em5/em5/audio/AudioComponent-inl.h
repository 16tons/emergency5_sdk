// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AudioComponent::AudioComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mAudioSet(AUDIOSET_NONE),
		mRandomWaitIntervalStart(30.0f),
		mRandomWaitIntervalEnd(60.0f),
		mRandomProbability(1.0f)
	{
		// Nothing to do in here
	}

	inline AudioComponent::~AudioComponent()
	{
		// Nothing to do in here
	}

	inline AudioComponent::AudioSet AudioComponent::getAudioSet() const
	{
		return mAudioSet;
	}

	inline void AudioComponent::setAudioSet(AudioComponent::AudioSet audioSet)
	{
		mAudioSet = audioSet;
	}

	inline float AudioComponent::getRandomWaitIntervalStart() const
	{
		return mRandomWaitIntervalStart;
	}

	inline void AudioComponent::setRandomWaitIntervalStart(float randomProbabilityStart)
	{
		mRandomWaitIntervalStart = randomProbabilityStart;
	}

	inline float AudioComponent::getRandomWaitIntervalEnd() const
	{
		return mRandomWaitIntervalEnd;
	}

	inline void AudioComponent::setRandomWaitIntervalEnd(float randomWaitIntervalEnd)
	{
		mRandomWaitIntervalEnd = randomWaitIntervalEnd;
	}

	inline float AudioComponent::getRandomProbability() const
	{
		return mRandomProbability;
	}

	inline void AudioComponent::setRandomProbability(float randomProbability)
	{
		mRandomProbability = randomProbability;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
