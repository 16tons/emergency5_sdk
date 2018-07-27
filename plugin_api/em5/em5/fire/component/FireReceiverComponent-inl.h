// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	inline bool FireReceiverComponent::isBurning() const
	{
		// Default implementation returns always false -> it is not burnable at all
		return false;
	}

	inline bool FireReceiverComponent::isBurned() const
	{
		// Default implementation returns always false -> it is not burnable at all
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
