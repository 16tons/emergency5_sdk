// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FreeplayEvent::~FreeplayEvent()
	{
		// Nothing here
	}

	inline void FreeplayEvent::setGreatJob(bool greatJob)
	{
		mGreatJob = greatJob;
	}

	inline uint32 FreeplayEvent::getId() const
	{
		return mId;
	}

	inline const std::string& FreeplayEvent::getEventName() const
	{
		return mEventName;
	}

	inline void FreeplayEvent::setEventName(const std::string& name)
	{
		mEventName = name;
	}

	inline const qsf::NamedIdentifier& FreeplayEvent::getEventFactoryName() const
	{
		return mEventFactoryName;
	}

	inline void FreeplayEvent::setEventFactoryName(const qsf::NamedIdentifier& factoryName)
	{
		mEventFactoryName = factoryName;
	}

	inline const ObjectiveList& FreeplayEvent::getObjectiveList() const
	{
		return mObjectives;
	}

	inline qsf::Map& FreeplayEvent::getMap()
	{
		return mMap;
	}

	inline const qsf::Map& FreeplayEvent::getMap() const
	{
		return mMap;
	}

	const FreeplayEvent::RecordedHintSystemMessages& FreeplayEvent::getRecordedHintSystemMessages() const
	{
		return mRecordedHintSystemMessages;
	}

	inline FreeplayEvent::State FreeplayEvent::getState() const
	{
		return mState;
	}

	inline void FreeplayEvent::setCanWinEvent(bool canWinEvent)
	{
		mCanWinEvent = canWinEvent;
	}

	inline bool FreeplayEvent::getCanWinEvent() const
	{
		return mCanWinEvent;
	}

	inline void FreeplayEvent::setCanLoseEvent(bool canLoseEvent)
	{
		mCanLoseEvent = canLoseEvent;
	}

	inline bool FreeplayEvent::getCanLoseEvent() const
	{
		return mCanLoseEvent;
	}

	inline qsf::Time FreeplayEvent::getCurrentLifeTime() const
	{
		return mLifeTime;
	}

	inline qsf::Time FreeplayEvent::getCurrentRunningTime() const
	{
		return mRunningTime;
	}

	inline uint32 FreeplayEvent::getNormalPointGain() const
	{
		return mNormalPointGain;
	}

	inline void FreeplayEvent::setNormalPointGain(uint32 pointGain)
	{
		mNormalPointGain = pointGain;
	}

	inline uint32 FreeplayEvent::getNormalCreditGain() const
	{
		return mNormalCreditGain;
	}

	inline void FreeplayEvent::setNormalCreditGain(uint32 creditGain)
	{
		mNormalCreditGain = creditGain;
	}

	inline uint32 FreeplayEvent::getMultiplayerPointGain() const
	{
		return mMultiplayerPointGain;
	}

	inline void FreeplayEvent::setMultiplayerPointGain(uint32 pointGain)
	{
		mMultiplayerPointGain = pointGain;
	}

	inline float FreeplayEvent::getEventDensityIncrease() const
	{
		return mEventDensityIncrease;
	}

	inline void FreeplayEvent::setEventDensityIncrease(float eventDensityIncrease)
	{
		mEventDensityIncrease = eventDensityIncrease;
	}

	inline const FreeplayEvent::EntityIdArray& FreeplayEvent::getEntityIds() const
	{
		return mEntityIds;
	}

	inline float FreeplayEvent::getEventDensity() const
	{
		return (FreeplayEvent::STATE_HIDDEN == getState()) ? mEventDensity : 0.0f;
	}

	inline void FreeplayEvent::setEventDensity(float density)
	{
		mEventDensity = density;
	}

	inline uint32 FreeplayEvent::getPlayerIndex() const
	{
		return mEventResult.mPlayerIndex;
	}

	inline bool FreeplayEvent::isEventLocationEntitySet() const
	{
		return mEventLocationEntity.valid();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	inline bool FreeplayEvent::onStartup()
	{
		// Default implementation simply returns "true"
		return true;
	}

	inline void FreeplayEvent::onShutdown()
	{
		// Default implementation is empty
	}

	inline void FreeplayEvent::onRun()
	{
		// Default implementation is empty
	}

	inline bool FreeplayEvent::onSuccess(EventResult& eventResult)
	{
		// Always inform the freeplay system about finish
		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline ObjectiveList& FreeplayEvent::getObjectives()
	{
		return mObjectives;
	}

	template<typename T>
	T& FreeplayEvent::createObserver(uint64 entityId, const std::string& name)
	{
		return static_cast<T&>(createObserverByTypeId(T::GAMELOGIC_TYPE_ID, entityId, name));
	}

	inline Observer& FreeplayEvent::createGeneralObserverByTypeId(uint32 typeId, const std::string& name)
	{
		return createObserverByTypeId(typeId, 0, name);	// The core entity ID is always 0
	}

	template<typename T>
	T& FreeplayEvent::createGeneralObserver(const std::string& name)
	{
		return static_cast<T&>(createGeneralObserverByTypeId(T::GAMELOGIC_TYPE_ID, name));
	}

	inline bool FreeplayEvent::isMiniEvent() const
	{
		return getIsMiniEvent();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
