// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActiveToggleComponent::ActiveToggleComponent(Prototype* prototype) :
		Component(prototype),
		mMultiplier(1.0f),
		mTimeOn(100.0f),
		mTimeOff(100.0f),
		mTime(0.0f),
		mActiveState(true)
	{
		// Nothing to do in here
	}

	inline ActiveToggleComponent::~ActiveToggleComponent()
	{
		// Nothing to do in here
	}

	inline float ActiveToggleComponent::getMultiplier() const
	{
		return mMultiplier;
	}

	inline void ActiveToggleComponent::setMultiplier(float multiplier)
	{
		assignAndPromoteChange(mMultiplier, multiplier, MULTIPLIER);
	}

	inline float ActiveToggleComponent::getTimeOn() const
	{
		return mTimeOn;
	}

	inline void ActiveToggleComponent::setTimeOn(float timeOn)
	{
		assignAndPromoteChange(mTimeOn, timeOn, TIME_ON);
	}

	inline float ActiveToggleComponent::getTimeOff() const
	{
		return mTimeOff;
	}

	inline void ActiveToggleComponent::setTimeOff(float timeOff)
	{
		assignAndPromoteChange(mTimeOff, timeOff, TIME_OFF);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
