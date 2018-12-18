// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline float DampedSpring::getValue() const
	{
		return mValue;
	}

	inline void DampedSpring::setValue(float value)
	{
		mValue = value;
	}

	inline float DampedSpring::getSpringConstant() const
	{
		return mSpringConstant;
	}

	inline void DampedSpring::setSpringConstant(float springConstant)
	{
		mSpringConstant = springConstant;
	}

	inline float DampedSpring::getDampingConstant() const
	{
		return mDampingConstant;
	}

	inline void DampedSpring::setDampingConstant(float dampingConstant)
	{
		mDampingConstant = dampingConstant;
	}

	inline bool DampedSpring::isStoppingAtTarget() const
	{
		return mStoppingAtTarget;
	}

	inline void DampedSpring::setStoppingAtTarget(bool stoppingAtTarget)
	{
		mStoppingAtTarget = stoppingAtTarget;
	}

	inline bool DampedSpring::hasStopped()
	{
		return mSpeed == 0.0f;
	}

	inline float DampedSpring::getMinimalSpeed() const
	{
		return mMinimalSpeed;
	}

	inline void DampedSpring::setMinimalSpeed(float minimalSpeed)
	{
		mMinimalSpeed = minimalSpeed;
	}

	inline float DampedSpring::getSpeed() const
	{
		return mSpeed;
	}

	inline void DampedSpring::setSpeed(float speed)
	{
		mSpeed = speed;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
