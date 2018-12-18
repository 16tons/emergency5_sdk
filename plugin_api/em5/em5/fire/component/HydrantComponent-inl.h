// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HydrantComponent::HydrantComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mHydrantType(FIXED_HYDRANT)
	{
		// Nothing to do in here
	}

	inline HydrantComponent::~HydrantComponent()
	{
		// Nothing to do in here
	}

	inline HydrantComponent::HydrantType HydrantComponent::getHydrantType() const
	{
		return mHydrantType;
	}

	inline void HydrantComponent::setHydrantType(HydrantType hydrantType)
	{
		mHydrantType = hydrantType;
	}

	inline bool HydrantComponent::isReserved() const
	{
		return mInternalFlags.isSet(RESERVE_HYDRANT);
	}

	inline void HydrantComponent::setReserved(bool reserved)
	{
		mInternalFlags.set(RESERVE_HYDRANT, reserved);
	}

	inline bool HydrantComponent::isConnected() const
	{
		return mConnectedFireman.valid();
	}

	inline qsf::Entity* HydrantComponent::getConnectedFireman() const
	{
		return mConnectedFireman.get();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
