// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LocomotionNode::~LocomotionNode()
	{
		// Nothing to do
	}

	inline uint16 LocomotionNode::getLeftFootBoneId() const
	{
		return mLeftFootBoneId;
	}

	inline uint16 LocomotionNode::getRightFootBoneId() const
	{
		return mRightFootBoneId;
	}

	inline float LocomotionNode::getProgress() const
	{
		return mProgress;
	}

	inline const LocomotionNode::WalkingCycle& LocomotionNode::getWalkingCycle() const
	{
		return mWalkingCycle;
	}

	inline void LocomotionNode::setWalkingCycle(const WalkingCycle& walkingCycle)
	{
		mWalkingCycle = walkingCycle;
	}

	inline const glm::vec2& LocomotionNode::getKneeAngleConstrainMinMax() const
	{
		return mKneeAngleConstrainMinMax;
	}

	inline const glm::vec2& LocomotionNode::getHeightConstrainMinMax() const
	{
		return mHeightConstrainMinMax;
	}

	inline void LocomotionNode::setHeightConstrainMinMax(const glm::vec2& heightConstrainMinMax)
	{
		mHeightConstrainMinMax = heightConstrainMinMax;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
