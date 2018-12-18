// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool PersonComponent::getPersonFlags(PersonComponent::PersonFlags flag) const
	{
		return mPersonFlags.isSet(flag);
	}

	inline void PersonComponent::setPersonFlags(PersonComponent::PersonFlags flag, bool value)
	{
		mPersonFlags.set(flag, value);
	}

	inline bool PersonComponent::getGangsterFlag() const
	{
		return mPersonFlags.isSet(FLAG_MARKED_AS_GANGSTER);
	}

	inline bool PersonComponent::getHandcuffedFlag() const
	{
		return mPersonFlags.isSet(FLAG_HANDCUFFED);
	}

	inline PersonAnimationController& PersonComponent::getPersonAnimationController()
	{
		QSF_ASSERT(nullptr != mPersonAnimationController, "The person animation controller instance is invalid, don't try to retrieve it if person component is not running", QSF_REACT_THROW);
		return *mPersonAnimationController;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
