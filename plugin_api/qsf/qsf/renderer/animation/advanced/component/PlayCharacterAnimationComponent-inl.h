// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlayCharacterAnimationComponent::PlayCharacterAnimationComponent(Prototype* prototype) :
		PlaySkeletalAnimationComponent(prototype),
		mBoneLookAtNode(nullptr),
		mLocomotionNode(nullptr),
		mHeadMaximalDistance(10000000000.0f),	// Just using a big float number because inf and max are a little problematic
		mHeadMovementSpringConstant(100.0f),
		mHeadAnimationInfluence(1.0f),
		mHeadRotationConstrain(glm::pi<float>() * 0.33f, glm::pi<float>() * 0.25f, glm::pi<float>()),
		mHeadMovingToDefaultAngle(glm::pi<float>() * 0.66f),
		mHeadLookAtEntityId(getUninitialized<uint64>())
	{
		// Nothing to do in here
	}

	inline PlayCharacterAnimationComponent::~PlayCharacterAnimationComponent()
	{
		// Nothing to do
	}

	inline const std::string& PlayCharacterAnimationComponent::getHeadBone() const
	{
		return mHeadBone;
	}

	inline uint64 PlayCharacterAnimationComponent::getHeadLookAtEntityId() const
	{
		return mHeadLookAtEntityId;
	}

	inline bool PlayCharacterAnimationComponent::isHeadLookAtActive() const
	{
		return mHeadLookAtEntity.valid();
	}

	inline float PlayCharacterAnimationComponent::getHeadMovementSpringConstant() const
	{
		return mHeadMovementSpringConstant;
	}

	inline float PlayCharacterAnimationComponent::getHeadAnimationInfluence() const
	{
		return mHeadAnimationInfluence;
	}

	inline glm::vec3 PlayCharacterAnimationComponent::getHeadRotationConstrain() const
	{
		return mHeadRotationConstrain;
	}

	inline float PlayCharacterAnimationComponent::getHeadMovingToDefaultAngle() const
	{
		return mHeadMovingToDefaultAngle;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
