// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinearRotationComponent::LinearRotationComponent(Prototype* prototype) :
		Component(prototype),
		mMultiplier(1.0f),
		mListening(true)
	{
		// Nothing to do in here
	}

	inline LinearRotationComponent::~LinearRotationComponent()
	{
		// Nothing to do in here
	}

	inline const glm::quat& LinearRotationComponent::getRotation() const
	{
		return mRotation;
	}

	inline float LinearRotationComponent::getMultiplier() const
	{
		return mMultiplier;
	}

	inline void LinearRotationComponent::setMultiplier(float multiplier)
	{
		assignAndPromoteChange(mMultiplier, multiplier, MULTIPLIER);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
