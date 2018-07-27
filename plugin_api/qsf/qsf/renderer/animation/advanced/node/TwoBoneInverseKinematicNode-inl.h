// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TwoBoneInverseKinematicNode::~TwoBoneInverseKinematicNode()
	{
		// Nothing to do
	}

	inline uint16 TwoBoneInverseKinematicNode::getBoneId() const
	{
		return mBoneId;
	}

	inline void TwoBoneInverseKinematicNode::setBoneId(uint16 boneId)
	{
		mBoneId = boneId;
	}

	inline const glm::vec3& TwoBoneInverseKinematicNode::getTargetPosition() const
	{
		return mTargetPosition;
	}

	inline void TwoBoneInverseKinematicNode::setTargetPosition(const glm::vec3& targetPosition)
	{
		mTargetPosition = targetPosition;
	}

	inline const glm::vec2& TwoBoneInverseKinematicNode::getAlphaAngleConstrainMinMax() const
	{
		return mAlphaAngleConstrainMinMax;
	}

	inline void TwoBoneInverseKinematicNode::setAlphaAngleConstrainMinMax(const glm::vec2& alphaAngleConstrainMinMax)
	{
		mAlphaAngleConstrainMinMax = alphaAngleConstrainMinMax;
	}

	inline float TwoBoneInverseKinematicNode::getWeight() const
	{
		return mWeight;
	}

	inline void TwoBoneInverseKinematicNode::setWeight(float weight)
	{
		mWeight = weight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
