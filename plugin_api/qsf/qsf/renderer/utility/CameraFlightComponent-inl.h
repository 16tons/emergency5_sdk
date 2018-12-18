// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CameraFlightComponent::CameraFlightComponent(Prototype* prototype) :
		Component(prototype),
		mAcceleration(1000.0f),
		mDeceleration(1000.0f),
		mMaximumSpeed(100000.0f),
		mMaximumZoomout(500.0f),
		mJumpDirectly(true),
		mTargetZoomout(0.0f),
		mCurrentMoveSpeed(0.0f),
		mInterpolateRotation(false)
	{
		// Nothing to do in here
	}

	inline CameraFlightComponent::~CameraFlightComponent()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
