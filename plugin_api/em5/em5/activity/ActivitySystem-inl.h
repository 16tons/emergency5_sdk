// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Activity* ActivitySystem::getActivityByAsset(const qsf::AssetProxy& assetProxy)
	{
		Activity* activity = mActivityManager.findElement(assetProxy.getLocalAssetId());
		if (nullptr == activity)
		{
			activity = loadActivityByAsset(assetProxy);
		}
		return activity;
	}

	inline ActivityStepFactory& ActivitySystem::getActivityStepFactory() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mActivityStepFactory, "The activity step factory instance is invalid", QSF_REACT_THROW);
		return *mActivityStepFactory;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* ActivitySystem::getName() const
	{
		return "Activity system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
