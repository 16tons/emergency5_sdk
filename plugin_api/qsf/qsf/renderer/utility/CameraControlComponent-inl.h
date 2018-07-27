// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CameraControlComponent::CameraControlComponent(Prototype* prototype) :
		Component(prototype),
		mLocked(false),
		mTargetDistance(0.0f),
		mPitch(0.0f)
	{
		// Nothing to do in here
	}

	inline CameraControlComponent::~CameraControlComponent()
	{
		// Nothing to do in here
	}

	inline float CameraControlComponent::getTargetDistance() const
	{
		return mTargetDistance;
	}

	inline void CameraControlComponent::setTargetDistanceWithoutMoving(float distance)
	{
		mTargetDistance = distance;
	}

	inline bool CameraControlComponent::isLocked() const
	{
		return mLocked;
	}

	inline void CameraControlComponent::setLocked(bool locked)
	{
		assignAndPromoteChange(mLocked, locked, LOCKED);
	}

	inline float CameraControlComponent::getPitch() const
	{
		return mPitch;
	}

	inline void CameraControlComponent::setPitch(float pitchDegrees)
	{
		mPitch = pitchDegrees;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
