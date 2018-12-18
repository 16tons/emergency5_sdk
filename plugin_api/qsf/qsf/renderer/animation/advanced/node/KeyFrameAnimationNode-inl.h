// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline KeyFrameAnimationNode::~KeyFrameAnimationNode()
	{
		// Nothing to do
	}

	inline const AssetProxy& KeyFrameAnimationNode::getAnimationProxy() const
	{
		return mAnimationProxy;
	}

	inline const std::string& KeyFrameAnimationNode::getAnimationName() const
	{
		return mAnimationName;
	}

	inline float KeyFrameAnimationNode::getProgress() const
	{
		return mProgress;
	}

	inline float KeyFrameAnimationNode::getProgressAsTime() const
	{
		return mProgress * mTimeLength;
	}

	inline float KeyFrameAnimationNode::getTimeLength() const
	{
		return mTimeLength;
	}

	inline bool KeyFrameAnimationNode::getRetarget() const
	{
		return mRetarget;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
