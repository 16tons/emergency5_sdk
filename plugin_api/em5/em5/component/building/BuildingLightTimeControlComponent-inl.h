// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BuildingLightTimeControlComponent::BuildingLightTimeControlComponent(qsf::Prototype* prototype) :
		LightTimeControlComponent(prototype),
		mWindowLightEarliestOnTime(qsf::Time::fromHours(20.0f)),
		mWindowLightLatestOnTime(qsf::Time::fromHours(21.0f)),
		mWindowLightEarliestOffTime(qsf::Time::fromHours(7.0f)),
		mWindowLightLatestOffTime(qsf::Time::fromHours(8.0f)),
		mWindowLightOnTime(qsf::Time::ZERO),
		mWindowLightOffTime(qsf::Time::ZERO),
		mWindowLightIsOnStatus(false)
	{
		// Noting here
	}

	inline BuildingLightTimeControlComponent::~BuildingLightTimeControlComponent()
	{
		// Noting here
	}

	inline const qsf::AssetProxy& BuildingLightTimeControlComponent::getWindowEmissiveMaterial() const
	{
		return mWindowEmissiveMaterial;
	}

	inline void BuildingLightTimeControlComponent::setWindowEmissiveMaterial(const qsf::AssetProxy& material)
	{
		mWindowEmissiveMaterial = material;
	}

	inline const qsf::Time BuildingLightTimeControlComponent::getWindowLightEarliestOnTime() const
	{
		return mWindowLightEarliestOnTime;
	}

	inline void BuildingLightTimeControlComponent::setWindowLightEarliestOnTime(const qsf::Time& time)
	{
		mWindowLightEarliestOnTime = time;
	}

	inline const qsf::Time BuildingLightTimeControlComponent::getWindowLightLatestOnTime() const
	{
		return mWindowLightLatestOnTime;
	}

	inline void BuildingLightTimeControlComponent::setWindowLightLatestOnTime(const qsf::Time& time)
	{
		mWindowLightLatestOnTime = time;
	}

	inline const qsf::Time BuildingLightTimeControlComponent::getWindowLightEarliestOffTime() const
	{
		return mWindowLightEarliestOffTime;
	}

	inline void BuildingLightTimeControlComponent::setWindowLightEarliestOffTime(const qsf::Time& time)
	{
		mWindowLightEarliestOffTime = time;
	}

	inline const qsf::Time BuildingLightTimeControlComponent::getWindowLightLatestOffTime() const
	{
		return mWindowLightLatestOffTime;
	}

	inline void BuildingLightTimeControlComponent::setWindowLightLatestOffTime(const qsf::Time& time)
	{
		mWindowLightLatestOffTime = time;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
