// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EventIdComponent::~EventIdComponent()
	{
		// Nothing here
	}

	inline FreeplayEvent* EventIdComponent::getFreeplayEvent() const
	{
		return mFreeplayEvent.get();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
