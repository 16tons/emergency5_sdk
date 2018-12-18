// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Transform::Transform() :
		mScale(1.0f, 1.0f, 1.0f)
	{
		// Nothing to do in here
	}

	inline Transform::Transform(const Transform& source) :
		mPosition(source.mPosition),
		mRotation(source.mRotation),
		mScale(source.mScale)
	{
		// Nothing to do in here
	}

	inline Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale) :
		mPosition(position),
		mRotation(rotation),
		mScale(scale)
	{
		// Nothing to do in here
	}

	inline Transform::Transform(const glm::mat4x4& transformMatrix)
	{
		setMatrix(transformMatrix);
	}

	inline Transform& Transform::operator=(const Transform& source)
	{
		// Copy data
		mPosition = source.mPosition;
		mRotation = source.mRotation;
		mScale	  = source.mScale;

		// Return a reference to this instance
		return *this;
	}

	inline bool Transform::operator==(const Transform& other) const
	{
		return (mPosition == other.mPosition && mRotation == other.mRotation && mScale == other.mScale);
	}

	inline bool Transform::operator!=(const Transform& other) const
	{
		return (mPosition != other.mPosition || mRotation != other.mRotation || mScale != other.mScale);
	}

	inline const glm::vec3& Transform::getPosition() const
	{
		return mPosition;
	}

	inline void Transform::setPosition(const glm::vec3& position)
	{
		// Set new position
		mPosition = position;
	}

	inline const glm::quat& Transform::getRotation() const
	{
		return mRotation;
	}

	inline void Transform::setRotation(const glm::quat& rotation)
	{
		// Set new rotation
		mRotation = rotation;
	}

	inline const glm::vec3& Transform::getScale() const
	{
		return mScale;
	}

	inline void Transform::setScale(const glm::vec3& scale)
	{
		// Set new scale
		mScale = scale;
	}

	inline void Transform::setPositionAndRotation(const glm::vec3& position, const glm::quat& rotation)
	{
		// Set new position and rotation
		mPosition = position;
		mRotation = rotation;
	}

	inline void Transform::fromLocalTransform(const Transform& localTransform, const Transform& referenceTransform)
	{
		mPosition = referenceTransform.mRotation * (localTransform.mPosition * referenceTransform.mScale) + referenceTransform.mPosition;
		mRotation = referenceTransform.mRotation * localTransform.mRotation;
		mScale    = localTransform.mScale * referenceTransform.mScale;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
