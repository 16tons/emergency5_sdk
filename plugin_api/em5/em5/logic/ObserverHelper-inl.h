// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T>
	T& ObserverHelper::createObserver(uint64 entityId) const
	{
		return static_cast<T&>(mEvent.createObserverByTypeId(T::GAMELOGIC_TYPE_ID, entityId));
	}

	template<typename T>
	T& ObserverHelper::createGeneralObserver() const
	{
		return static_cast<T&>(mEvent.createGeneralObserverByTypeId(T::GAMELOGIC_TYPE_ID));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
