// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ParticleTimeControlComponent::ParticleTimeControlComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mEarliestOnTime(qsf::Time::fromHours(20.0f)),
		mLatestOnTime(qsf::Time::fromHours(21.0f)),
		mEarliestOffTime(qsf::Time::fromHours(7.0f)),
		mLatestOffTime(qsf::Time::fromHours(8.0f)),
		mOnTime(qsf::Time::ZERO),
		mOffTime(qsf::Time::ZERO),
		mOnStatus(false)
	{
	}

	inline ParticleTimeControlComponent::~ParticleTimeControlComponent()
	{
		// Noting here
	}

	inline const qsf::Time ParticleTimeControlComponent::getEarliestOnTime() const
	{
		return mEarliestOnTime;
	}

	inline const qsf::Time ParticleTimeControlComponent::getLatestOnTime() const
	{
		return mLatestOnTime;
	}

	inline const qsf::Time ParticleTimeControlComponent::getEarliestOffTime() const
	{
		return mEarliestOffTime;
	}

	inline const qsf::Time ParticleTimeControlComponent::getLatestOffTime() const
	{
		return mLatestOffTime;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
