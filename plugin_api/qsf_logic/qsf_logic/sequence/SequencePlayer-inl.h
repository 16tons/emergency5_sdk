// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SequencePlayer::SequencePlayer() :
			mCurrentTime(0.0f),
			mPlaybackSpeed(1.0f),
			mIsLooping(true),
			mCurrentRunningSequence(nullptr),
			mOwnerComponent(nullptr)
		{
			// Nothing to do in here
		}

		inline SequencePlayer::~SequencePlayer()
		{
			mUpdateJobProxy.unregister();
		}

		inline PlaySequenceComponent* SequencePlayer::getOwnerComponent() const
		{
			return mOwnerComponent;
		}

		inline void SequencePlayer::setOwnerComponent(PlaySequenceComponent* component)
		{
			mOwnerComponent = component;
		}

		inline bool SequencePlayer::isPlaying() const
		{
			return mUpdateJobProxy.isValid();
		}

		inline float SequencePlayer::getPlaybackSpeed() const
		{
			return mPlaybackSpeed;
		}

		inline void SequencePlayer::setPlaybackSpeed(float newSpeed)
		{
			mPlaybackSpeed = newSpeed;
		}

		inline bool SequencePlayer::getIsLooping() const
		{
			return mIsLooping;
		}

		inline void SequencePlayer::setIsLooping(bool isLooping)
		{
			mIsLooping = isLooping;
		}

		inline Sequence* SequencePlayer::getSequence() const
		{
			return mCurrentRunningSequence;
		}

		inline void SequencePlayer::modifyTime(float newTime)
		{
			mCurrentTime = newTime;
		}

		inline float SequencePlayer::getTime() const
		{
			return mCurrentTime;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
