// Copyright (C) 2012-2018 Promotion Software GmbH

namespace em5
{

	inline InhabitantSpawnComponent::InhabitantSpawnComponent(qsf::Prototype* prototype) :
		SpawnPointComponent(prototype),
		mDestinationEntityId(qsf::getUninitialized<uint64>()),
		mTimeBegin(11*60+50),
		mTimeEnd(12*60+10),
		mTime(12*60)
	{
		mList.push_back(std::string());
	}

	inline InhabitantSpawnComponent::~InhabitantSpawnComponent()
	{
	}

	inline uint64 InhabitantSpawnComponent::getDestinationEntityId() const
	{
		return mDestinationEntityId;
	}

	inline void InhabitantSpawnComponent::setDestinationEntityId(uint64 entityId)
	{
		assignAndPromoteChange(mDestinationEntityId, entityId, DESTINATION_ENTITY_ID);
	}

	inline const std::string& InhabitantSpawnComponent::getPerson() const
	{
		return mList[0];
	}

	inline void InhabitantSpawnComponent::setPerson(const std::string& personName)
	{
		if (mList[0] != personName)
		{
			mList[0] = personName;
			promotePropertyChange(PERSON);
		}
	}

	inline glm::vec2 InhabitantSpawnComponent::getTimeBegin() const
	{
		return minuteOfDayToFloatTime(mTimeBegin);
	}

	inline void InhabitantSpawnComponent::setTimeBegin(const glm::vec2& time)
	{
		if (isValidFloatTime(time))
		{
			const int newTime(floatTimeToToMinuteOfDay(time));
			if (mTimeBegin != newTime)
			{
				mTimeBegin = newTime;
				updateSpawnTime();
				promotePropertyChange(TIME_BEGIN);
			}
		}
	}

	inline glm::vec2 InhabitantSpawnComponent::getTimeEnd() const
	{
		return minuteOfDayToFloatTime(mTimeEnd);
	}

	inline void InhabitantSpawnComponent::setTimeEnd(const glm::vec2& time)
	{
		if (isValidFloatTime(time))
		{
			const int newTime(floatTimeToToMinuteOfDay(time));
			if (mTimeEnd != newTime)
			{
				mTimeEnd = newTime;
				updateSpawnTime();
				promotePropertyChange(TIME_END);
			}
		}
	}

	inline bool InhabitantSpawnComponent::allowSpawn(int currentMinuteOfDay) const
	{
		if (!mInhabitant.valid())
		{
			if (mTime<mTimeEnd)
				return (currentMinuteOfDay>=mTime && currentMinuteOfDay<mTimeEnd);
			else
				return (currentMinuteOfDay>=mTime || currentMinuteOfDay<mTimeEnd);
		}
		return false;
	}

	inline bool InhabitantSpawnComponent::isValidFloatTime(const glm::vec2& floatTime)
	{
		return floatTime.x>=0.0f && floatTime.y>=0.0f && floatTime.x<24.0f && floatTime.y<60.0f;
	}

	inline int InhabitantSpawnComponent::floatTimeToToMinuteOfDay(const glm::vec2& floatTime)
	{
		return static_cast<int>(floatTime.x)*60+static_cast<int>(floatTime.y);
	}

	inline glm::vec2 InhabitantSpawnComponent::minuteOfDayToFloatTime(int minuteOfDay)
	{
		return glm::vec2(static_cast<float>(minuteOfDay/60), static_cast<float>(minuteOfDay%60));
	}

}
