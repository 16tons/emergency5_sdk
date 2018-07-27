// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WaterCannonComponent::WaterCannonComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mCannonEntityId(qsf::getUninitialized<uint64>()),
		mFinalYaw(0),
		mFinalPitch(0),
		mIsCannonInFinalRotation(true),
		mIsOnBoat(false)
	{
		// Nothing to do in here
	}

	inline WaterCannonComponent::~WaterCannonComponent()
	{
		// Nothing to do in here
	}

	inline uint64 WaterCannonComponent::getCannonEntityId() const
	{
		return mCannonEntityId;
	}

	inline void WaterCannonComponent::setIsOnBoat(bool isOnBoat)
	{
		mIsOnBoat = isOnBoat;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
