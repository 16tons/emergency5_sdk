// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LightTimeControlComponent::LightTimeControlComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mShouldBeSimulated(false),
		mLightsEarliestOnTime(qsf::Time::fromHours(20.0f)),
		mLightsLatestOnTime(qsf::Time::fromHours(21.0f)),
		mLightsEarliestOffTime(qsf::Time::fromHours(7.0f)),
		mLightsLatestOffTime(qsf::Time::fromHours(8.0f)),
		mLightsOnTime(qsf::Time::ZERO),
		mLightsOffTime(qsf::Time::ZERO),
		mLightsAreOnStatus(false),
		mLightEntityId(qsf::getUninitialized<uint64>())
	{
		// Noting here
	}

	inline LightTimeControlComponent::~LightTimeControlComponent()
	{
		// Noting here
	}

	inline const qsf::AssetProxy& LightTimeControlComponent::getLightsEmissiveMaterial() const
	{
		return mLightsEmissiveMaterial;
	}

	inline void LightTimeControlComponent::setLightsEmissiveMaterial(const qsf::AssetProxy& material)
	{
		mLightsEmissiveMaterial = material;
	}

	inline const qsf::Time LightTimeControlComponent::getLightsEarliestOnTime() const
	{
		return mLightsEarliestOnTime;
	}

	inline void LightTimeControlComponent::setLightsEarliestOnTime(const qsf::Time& time)
	{
		mLightsEarliestOnTime = time;
	}

	inline const qsf::Time LightTimeControlComponent::getLightsLatestOnTime() const
	{
		return mLightsLatestOnTime;
	}

	inline void LightTimeControlComponent::setLightsLatestOnTime(const qsf::Time& time)
	{
		mLightsLatestOnTime = time;
	}

	inline const qsf::Time LightTimeControlComponent::getLightsEarliestOffTime() const
	{
		return mLightsEarliestOffTime;
	}

	inline void LightTimeControlComponent::setLightsEarliestOffTime(const qsf::Time& time)
	{
		mLightsEarliestOffTime = time;
	}

	inline const qsf::Time LightTimeControlComponent::getLightsLatestOffTime() const
	{
		return mLightsLatestOffTime;
	}

	inline void LightTimeControlComponent::setLightsLatestOffTime(const qsf::Time& time)
	{
		mLightsLatestOffTime = time;
	}

	inline uint64 LightTimeControlComponent::getLightEntityId() const
	{
		return mLightEntityId;
	}

	inline void LightTimeControlComponent::setLightEntityId(uint64 entityId)
	{
		mLightEntityId = entityId;
	}

	inline void LightTimeControlComponent::updateLightState(const qsf::Time& currentTimeOfDay, bool forceSet)
	{
		// Call virtual method
		onUpdateLightState(currentTimeOfDay, forceSet);
	}

	inline bool LightTimeControlComponent::shouldBeSimulated() const
	{
		return mShouldBeSimulated;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
