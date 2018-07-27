// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool BuildingClippingManager::isBuildingClippingEnabled() const
	{
		return mBuildingClippingEnabled;
	}

	inline void BuildingClippingManager::setVisibilityCheckEnabled(bool visibilityCheckEnabled)
	{
		mVisibilityCheckEnabled = visibilityCheckEnabled;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
