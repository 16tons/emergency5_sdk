// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BridgeComponent::BridgeComponent(Prototype* prototype) :
		Component(prototype),
		mBridgeId(0)
	{
		// Nothing here
	}

	inline BridgeComponent::~BridgeComponent()
	{
		// Nothing here
	}

	inline uint32 BridgeComponent::getBridgeId() const
	{
		return mBridgeId;
	}

	inline void BridgeComponent::setBridgeId(uint32 bridgeId)
	{
		mBridgeId = bridgeId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
