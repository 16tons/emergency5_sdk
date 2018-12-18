// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline TimeOfDayComponent::TimeOfDayComponent(Prototype* prototype) :
			Component(prototype),
			mTime(Time::fromHours(12.0f)),
			mState(DAYTIME),
			mDayBegin(Time::fromHours(8.0f)),
			mDayEnd(Time::fromHours(20.0f)),
			mDaySpeedFactor(60.0f / 2.5f),		// Day: 1 hour in-game = 2.5 minutes real-time
			mNightSpeedFactor(60.0f / 2.0f),	// Night: 1 hour in-game = 2.0 minutes real-time
			mSequence(nullptr),
			mSequenceEvaluationTime(0.0f),
			mIsInSequenceUpdate(false)
		{
			// Nothing to do in here
		}

		inline TimeOfDayComponent::~TimeOfDayComponent()
		{
			// Nothing to do in here
		}

		inline Time TimeOfDayComponent::getTime() const
		{
			return mTime;
		}

		inline uint32 TimeOfDayComponent::getHour() const
		{
			return static_cast<uint32>(mTime.getMinutes() + 0.001f) / 60;
		}

		inline uint32 TimeOfDayComponent::getMinute() const
		{
			return static_cast<uint32>(mTime.getMinutes() + 0.001f) % 60;
		}

		inline TimeOfDayComponent::State TimeOfDayComponent::getState() const
		{
			return mState;
		}

		inline Time TimeOfDayComponent::getDayBegin() const
		{
			return mDayBegin;
		}

		inline Time TimeOfDayComponent::getDayEnd() const
		{
			return mDayEnd;
		}

		inline const AssetProxy& TimeOfDayComponent::getSequenceAsset() const
		{
			return mSequenceAsset;
		}

		inline void TimeOfDayComponent::setIsInSequence(bool isInSequenceUpdate)
		{
			mIsInSequenceUpdate = isInSequenceUpdate;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
