// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TransformComponent::TransformComponent(Prototype* prototype) :
		Component(prototype),
		mAnimationTransform(&mTransform)
	{
		// Nothing to do in here
	}

	inline TransformComponent::~TransformComponent()
	{
		// Nothing to do in here
	}

	inline const glm::vec3& TransformComponent::getPosition() const
	{
		return mTransform.getPosition();
	}

	inline const glm::quat& TransformComponent::getRotation() const
	{
		return mTransform.getRotation();
	}

	inline const glm::vec3& TransformComponent::getScale() const
	{
		return mTransform.getScale();
	}

	inline Transform& TransformComponent::getTransform()
	{
		return mTransform;
	}

	inline const Transform& TransformComponent::getTransform() const
	{
		return mTransform;
	}

	inline const Transform& TransformComponent::getAnimationTransform() const
	{
		return *mAnimationTransform;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
