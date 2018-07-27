// Copyright (C) 2012-2017 Promotion Software GmbH


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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
