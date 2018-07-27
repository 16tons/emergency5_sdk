// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MovableComponent::~MovableComponent()
	{
		// Nothing to do in here
	}

	inline bool MovableComponent::isMoving() const
	{
		// TODO(fw): The "moving" flag handling is not final yet

		// Use the "moving" flag as override for entities that count as moving although they stand still
		if (mInternalFlags.isSet(MOVING))
			return true;

		// The entity is obviously moving if there is movement
		return glm::dot(mVelocity, mVelocity) > 0.000001f;
	}

	inline const glm::vec3& MovableComponent::getVelocity() const
	{
		// Return linear velocity
		return mVelocity;
	}

	inline const glm::vec3& MovableComponent::getAcceleration() const
	{
		// Return linear acceleration
		return mAcceleration;
	}

	inline const glm::quat& MovableComponent::getAngularAcceleration() const
	{
		// Return angular acceleration
		return mAngularAcceleration;
	}

	inline const Transform& MovableComponent::getTransformBias() const
	{
		return mTransformBias;
	}

	inline bool MovableComponent::hasContainer() const
	{
		return (nullptr != mContainerLinkProxy.getRemoteAnchor());
	}

	inline uint64 MovableComponent::getContainerEntityId() const
	{
		return mContainerLinkProxy.getRemoteAnchorId().mLocalId;
	}

	inline const std::string& MovableComponent::getGroundSpeedCategory() const
	{
		return mGroundSpeedCategory;
	}

	inline void MovableComponent::setGroundSpeedCategory(const std::string& groundSpeedCategory)
	{
		assignAndPromoteChange(mGroundSpeedCategory, groundSpeedCategory, GROUND_SPEED_CATEGORY);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
