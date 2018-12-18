// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ContaminationCloudComponent::~ContaminationCloudComponent()
	{
		// Nothing to do in here
	}

	inline ContaminationCloudComponent::State ContaminationCloudComponent::getSimulationState() const
	{
		return mState;
	}

	inline float ContaminationCloudComponent::getMinContaminationDelay() const
	{
		return mMinContaminationDelay;
	}

	inline void ContaminationCloudComponent::setMinContaminationDelay(float contaminationDelay)
	{
		mMinContaminationDelay = contaminationDelay;
	}

	inline float ContaminationCloudComponent::getMaxContaminationDelay() const
	{
		return mMaxContaminationDelay;
	}

	inline void ContaminationCloudComponent::setMaxContaminationDelay(float contaminationDelay)
	{
		mMaxContaminationDelay = contaminationDelay;
	}

	inline float ContaminationCloudComponent::getContaminationRadius() const
	{
		return mContaminationRadius;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
