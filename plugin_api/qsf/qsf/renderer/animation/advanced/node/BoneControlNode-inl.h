// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoneControlNode::~BoneControlNode()
	{
		// Nothing to do
	}

	inline uint16 BoneControlNode::getBoneId() const
	{
		return mBoneId;
	}

	inline void BoneControlNode::setBoneId(uint16 boneId)
	{
		mBoneId = boneId;
	}

	inline const glm::vec3& BoneControlNode::getPosition() const
	{
		return mPosition;
	}

	inline void BoneControlNode::setPosition(const glm::vec3& position)
	{
		mPosition = position;
	}

	inline const glm::quat& BoneControlNode::getRotation() const
	{
		return mRotation;
	}

	inline void BoneControlNode::setRotation(const glm::quat& rotation)
	{
		mRotation = rotation;
	}

	inline const BoneControlNode::ControlBitmask& BoneControlNode::getControlType() const
	{
		return mControlType;
	}

	inline void BoneControlNode::setControlType(const BoneControlNode::ControlBitmask& controlType)
	{
		mControlType = controlType;
	}

	inline float BoneControlNode::getWeight() const
	{
		return mWeight;
	}

	inline void BoneControlNode::setWeight(float weight)
	{
		mWeight = weight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
