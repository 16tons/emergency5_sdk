// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 FreeplayEventFactory::getId() const
	{
		return mName.getHash();
	}

	inline const std::string& FreeplayEventFactory::getEventName() const
	{
		return mName.getName();
	}

	inline bool FreeplayEventFactory::isEnabled() const
	{
		return mIsEnabled;
	}

	inline float FreeplayEventFactory::getTriggerChance() const
	{
		return mTriggerChance;
	}

	inline void FreeplayEventFactory::setTriggerChance(float triggerChance)
	{
		mTriggerChance = triggerChance;
	}

	inline float FreeplayEventFactory::getEventDensity() const
	{
		return mEventDensity;
	}

	inline void FreeplayEventFactory::setEventDensity(float eventDensity)
	{
		mEventDensity = eventDensity;
	}

	inline bool FreeplayEventFactory::getStartEvent() const
	{
		return mStartEvent;
	}

	inline void FreeplayEventFactory::setStartEvent(bool startEvent)
	{
		mStartEvent = startEvent;
	}

	inline uint32 FreeplayEventFactory::getMaxAppearances() const
	{
		return mMaxAppearances;
	}

	inline void FreeplayEventFactory::setMaxAppearances(uint32 maxAppearances)
	{
		mMaxAppearances = maxAppearances;
	}

	inline const std::string& FreeplayEventFactory::getEventTag() const
	{
		return mEventTag;
	}

	inline void FreeplayEventFactory::setEventTag(const std::string& eventTag)
	{
		mEventTag = eventTag;
	}

	inline const FreeplayEventFactory::VariantArray& FreeplayEventFactory::getVariants() const
	{
		return mVariants;
	}

	inline void FreeplayEventFactory::setVariants(const VariantArray& variants)
	{
		mVariants = variants;
	}

	inline bool FreeplayEventFactory::operator ==(const FreeplayEventFactory& other) const
	{
		return (getId() == other.getId());
	}

	inline bool FreeplayEventFactory::operator ==(const std::string& other) const
	{
		return (getEventName() == other);
	}

	inline bool FreeplayEventFactory::operator ==(uint32 other) const
	{
		return (getId() == other);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template<typename T>
	T& FreeplayEventFactory::createAndInitializeEvent()
	{
		// Create a new instance of the given type
		T* eventInstance = new T();

		// Initialize and done
		initializeEventInstance(*eventInstance);
		return *eventInstance;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
