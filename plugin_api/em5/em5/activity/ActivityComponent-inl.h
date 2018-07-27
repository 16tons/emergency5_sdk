// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActivityComponent::ActivityComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mActivityPlayer(nullptr),
		mWasMoveToDespawnActive(false)
	{
		// Nothing here
	}

	inline ActivityComponent::~ActivityComponent()
	{
		// Nothing here
	}

	inline const qsf::AssetProxy& ActivityComponent::getActivityAsset() const
	{
		return mActivityAsset;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
