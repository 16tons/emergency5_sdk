// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Node::Node() :
		mScale(1.0f, 1.0f, 1.0f),
		mRadius(1.0f),
		mStatus(true),
		mAutoConnect(false),
		mTag(0)
	{
		// Nothing to do in here
	}

	inline Node::Node(const glm::vec3& position) :
		mPosition(position),
		mScale(1.0f, 1.0f, 1.0f),
		mRadius(1.0f),
		mStatus(true),
		mAutoConnect(false),
		mTag(0)
	{
		// Nothing to do in here
	}

	inline Node::Node(const glm::vec3& position, float radius) :
		mPosition(position),
		mScale(1.0f, 1.0f, 1.0f),
		mRadius(radius),
		mStatus(true),
		mAutoConnect(false),
		mTag(0)
	{
		// Nothing to do in here
	}

	inline Node::Node(const Node& source) :
		mPosition(source.mPosition),
		mRotation(source.mRotation),
		mScale(source.mScale),
		mRadius(source.mRadius),
		mStatus(source.mStatus),
		mAutoConnect(source.mAutoConnect),
		mTag(source.mTag)
	{
		// Nothing to do in here
	}

	inline const glm::vec3& Node::getPosition() const
	{
		return mPosition;
	}

	inline void Node::setPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	inline const glm::quat& Node::getRotation() const
	{
		return mRotation;
	}

	inline void Node::setRotation(const glm::quat& rotation)
	{
		// Ensure that the given quaternion is always normalized, else funny transform issues will occur (tracking down such bugs can be time consuming)
		mRotation = glm::normalize(rotation);
	}

	inline const glm::vec3& Node::getScale() const
	{
		return mScale;
	}

	inline void Node::setScale(const glm::vec3& scale)
	{
		mScale = scale;
	}

	inline Transform Node::getTransform() const
	{
		return Transform(mPosition, mRotation, mScale);
	}

	inline void Node::setTransform(const Transform& transform)
	{
		mRotation = transform.getRotation();
		mPosition = transform.getPosition();
		mScale = transform.getScale();
	}

	inline float Node::getRadius() const
	{
		return mRadius;
	}

	inline void Node::setRadius(float radius)
	{
		mRadius = radius;
	}

	inline bool Node::getStatus() const
	{
		return mStatus;
	}

	inline void Node::setStatus(bool status)
	{
		mStatus = status;
	}

	inline void Node::setAutoConnect(bool enable)
	{
		mAutoConnect = enable;
	}

	inline bool Node::shouldAutoConnect() const
	{
		return mAutoConnect;
	}

	inline int Node::getTag() const
	{
		return mTag;
	}

	inline void Node::setTag(int tag)
	{
		mTag = tag;
	}

	inline Node& Node::operator =(const Node& source)
	{
		mPosition = source.mPosition;
		mRotation = source.mRotation;
		mScale = source.mScale;
		mRadius = source.mRadius;
		mStatus = source.mStatus;
		mAutoConnect = source.mAutoConnect;
		mTag = source.mTag;
		return *this;
	}

	inline bool Node::operator ==(const Node& source) const
	{
		return (mPosition == source.mPosition && mRotation == source.mRotation && mScale == source.mScale && mRadius == source.mRadius && mStatus == source.mStatus && mTag == source.mTag && mAutoConnect == source.mAutoConnect);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}
