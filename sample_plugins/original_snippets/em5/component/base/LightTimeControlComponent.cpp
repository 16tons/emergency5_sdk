// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/base/LightTimeControlComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/environment/time/TimeOfDayComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Random.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace detail
	{
		static const uint32 DAY_IN_TOTAL_MINUTES = 24 * 60;
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 LightTimeControlComponent::COMPONENT_ID = qsf::StringHash("em5::LightTimeControlComponent");


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool LightTimeControlComponent::onStartup()
	{
		// Setup on/off times
		mLightsOnTime = getTimeFromTimeRange(mLightsEarliestOnTime, mLightsLatestOnTime);
		mLightsOffTime = getTimeFromTimeRange(mLightsEarliestOffTime, mLightsLatestOffTime);

		return true;
	}

	void LightTimeControlComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Serialize base class
		qsf::Component::serialize(serializer);
	}

	void LightTimeControlComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isActive())
		{
			onSetActive(true);
		}
	}

	void LightTimeControlComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (active && isSimulating())
			{
				if (!mShouldBeSimulated)
				{
					mShouldBeSimulated = true;

					// One shot update after getting activated
					updateLightState(qsf::game::TimeOfDayComponent::getCurrentTimeOfDay(), true);
				}
			}
			else if (mShouldBeSimulated)
			{
				mShouldBeSimulated = false;

				// Disable all lights
				switchOffAllLights();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::LightTimeControlComponent methods ]
	//[-------------------------------------------------------]
	void LightTimeControlComponent::onUpdateLightState(const qsf::Time& currentTimeOfDay, bool forceSet)
	{
		// Check current time of day against the interval when the other lights (sign lights and linked child lights) should be switched on
		const bool otherLightsShouldBeOn = (isActive() && isTimeInOnInterval(currentTimeOfDay, mLightsOnTime, mLightsOffTime));
		if (forceSet || mLightsAreOnStatus != otherLightsShouldBeOn)
		{
			mLightsAreOnStatus = otherLightsShouldBeOn;
			enableEmissiveMap(mLightsEmissiveMaterial, otherLightsShouldBeOn);
			enableChildLights(otherLightsShouldBeOn);
		}
	}

	void LightTimeControlComponent::enableChildLights(bool enable)
	{
		if (qsf::isInitialized(mLightEntityId))
		{
			// Toggle linked lights on/off
			qsf::Entity* lightEntity = getEntity().getMap().getEntityById(mLightEntityId);
			if (nullptr != lightEntity)
			{
				EntityHelper(*lightEntity).setLightActive(enable);
			}
		}
	}

	void LightTimeControlComponent::switchOffAllLights()
	{
		mLightsAreOnStatus = false;
		enableEmissiveMap(mLightsEmissiveMaterial, false);
		enableChildLights(false);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	void LightTimeControlComponent::enableEmissiveMap(const qsf::AssetProxy& material, bool enable)
	{
		if (material.isValid() && isRunning())
		{
			// Set material properties
			QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(getEntity(), "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(enable), material.getGlobalAssetId());
		}
	}

	qsf::Time LightTimeControlComponent::getTimeFromTimeRange(const qsf::Time& minimumTime, const qsf::Time& maximumTime)
	{
		// We only want a time between (0:00 - 23:59) and an accuracy till minutes
		// We get the times in total minutes
		const uint32 minimumTimeInTotalMinutes = static_cast<uint32>(minimumTime.getMinutes());
		uint32 maximumTimeInTotalMinutes = static_cast<uint32>(maximumTime.getMinutes());

		if (minimumTimeInTotalMinutes > maximumTimeInTotalMinutes)
		{
			// The minimum time is smaller then the maximum time -> the maximum time spans into the next day
			// To get a proper time difference add 24h to the latest time
			maximumTimeInTotalMinutes += detail::DAY_IN_TOTAL_MINUTES;
		}

		// We want only value which contains hours and minutes
		const uint32 timeDiffInMinutes = maximumTimeInTotalMinutes - minimumTimeInTotalMinutes;
		const uint32 addedTimeInMinutes = qsf::Random::getRandomUint(0, timeDiffInMinutes);

		// Calculate the new time
		uint32 selectedTimeInTotalMinutes = minimumTimeInTotalMinutes + addedTimeInMinutes;

		// Check if the calculated time is >= 24h -> subtract 24h to get the real time (0:00 - 23:59)
		if (selectedTimeInTotalMinutes >= detail::DAY_IN_TOTAL_MINUTES)
		{
			selectedTimeInTotalMinutes -= detail::DAY_IN_TOTAL_MINUTES;
		}

		return qsf::Time::fromMinutes(static_cast<float>(selectedTimeInTotalMinutes));
	}

	bool LightTimeControlComponent::isTimeInOnInterval(const qsf::Time& timeToCheck, const qsf::Time& lightsOnTime, const qsf::Time& lightsOffTime)
	{
		if (lightsOffTime < lightsOnTime)
		{
			// The time at which the lights should be switched off points to a time on the "next day"
			// We have two intervals to check the current time against to determine if the lights should be switched on
			// First interval [<LightOnTime>, 23:59]
			// Second interval [00:00, <LightOffTime>)
			return timeToCheck >= lightsOnTime || timeToCheck < lightsOffTime;
		}
		else if (lightsOffTime > lightsOnTime)
		{
			// Both time values are on the same day
			// We have only one interval to check: [<LightOnTime>, <LightOffTime>)
			return timeToCheck >= lightsOnTime && timeToCheck < lightsOffTime;
		}
		else
		{
			QSF_ERROR("LightsOnTime and LightsOffTime must not be the same value", return false);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
