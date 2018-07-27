// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline EventTagComponent::EventTagComponent(Prototype* prototype) :
			Component(prototype),
			mDirtyEventTagString(false)
		{
			// Nothing here
		}

		inline EventTagComponent::~EventTagComponent()
		{
			// Nothing here
		}

		inline const EventTagComponent::EventTagArray& EventTagComponent::getEventTags() const
		{
			return mEventTagArray;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
