// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StatusOverlayComponent::~StatusOverlayComponent()
	{
		// Nothing to do in here
	}

	inline uint64 StatusOverlayComponent::getIconFlags() const
	{
		return mStatusIconFlags.getStorage();
	}

	inline const StatusOverlayComponent::BarMap& StatusOverlayComponent::getBarValues() const
	{
		return mBarMap;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
