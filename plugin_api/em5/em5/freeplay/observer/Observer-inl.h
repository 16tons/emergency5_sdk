// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& Observer::getName() const
	{
		return mName;
	}

	inline void Observer::setName(const std::string& name)
	{
		mName = name;
	}

	template<typename T>
	T& Observer::createObserver(uint64 entityId)
	{
		return static_cast<T&>(getParentEvent()->createObserverByTypeId(T::GAMELOGIC_TYPE_ID, entityId));
	}

	template<typename T>
	T* Observer::getObserver(uint64 entityId)
	{
		return static_cast<T*>(getParentEvent()->getObserverByTypeId(T::GAMELOGIC_TYPE_ID, entityId));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
