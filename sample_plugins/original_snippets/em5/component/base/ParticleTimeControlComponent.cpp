// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/base/ParticleTimeControlComponent.h"
#include "em5/plugin/Jobs.h"

#include <qsf_game/environment/time/TimeOfDayComponent.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Random.h>
#include <qsf/renderer/particles/ParticlesComponent.h>


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
	const uint32 ParticleTimeControlComponent::COMPONENT_ID = qsf::StringHash("em5::ParticleTimeControlComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void ParticleTimeControlComponent::setEarliestOnTime(const qsf::Time& time)
	{
		mEarliestOnTime = time;
	}

	void ParticleTimeControlComponent::setLatestOnTime(const qsf::Time& time)
	{
		mLatestOnTime = time;
	}

	void ParticleTimeControlComponent::setEarliestOffTime(const qsf::Time& time)
	{
		mEarliestOffTime = time;
	}

	void ParticleTimeControlComponent::setLatestOffTime(const qsf::Time& time)
	{
		mLatestOffTime = time;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool ParticleTimeControlComponent::onStartup()
	{
		// Setup on/off times
		mOnTime = getTimeFromTimeRange(mEarliestOnTime, mLatestOnTime);
		mOffTime = getTimeFromTimeRange(mEarliestOffTime, mLatestOffTime);

		return true;
	}

	void ParticleTimeControlComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Serialize base class
		qsf::Component::serialize(serializer);
	}

	void ParticleTimeControlComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isActive())
		{
			onSetActive(true);
		}
	}

	void ParticleTimeControlComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			// TODO(sw) Use another way to get cyclic update calls for the check, so that this component does it work also inside the editor, when the game time gets changed inside the editor context
			if (active && isSimulating())
			{
				if (!mSimulationJobProxy.isValid())
				{
					qsf::jobs::JobConfiguration jobConfiguration;

					// We control only lights depending of the current game time -> call us only each realtime second
					jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
					mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&ParticleTimeControlComponent::updateSimulation, this, false), jobConfiguration);

					// One shot update after getting activated
					updateSimulation(true);
				}
			}
			else
			{
				if (mSimulationJobProxy.isValid())
				{
					// Disable particle
					enableParticle(false);
					mSimulationJobProxy.unregister();
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ParticleTimeControlComponent::updateSimulation(bool forceSet)
	{
		const qsf::game::TimeOfDayComponent* timeOfDayComponent = getEntity().getMap().getCoreEntity().getComponent<qsf::game::TimeOfDayComponent>();
		if (nullptr != timeOfDayComponent)
		{
			// Get the current time of day as time
			const qsf::Time currentTimeOfDay = qsf::Time::fromHours(static_cast<float>(timeOfDayComponent->getHour())) + qsf::Time::fromMinutes(static_cast<float>(timeOfDayComponent->getMinute()));

			// Call update simulation method
			onUpdateSimulation(currentTimeOfDay, forceSet);
		}
	}

	void ParticleTimeControlComponent::onUpdateSimulation(const qsf::Time& currentTimeOfDay, bool forceSet)
	{
		// Check current time of day against the interval when the other lights (sign lights and linked child lights) should be switched on
		bool otherShouldBeOn = isTimeInOnInterval(currentTimeOfDay, mOnTime, mOffTime);

		if (forceSet || mOnStatus != otherShouldBeOn)
		{
			mOnStatus = otherShouldBeOn;

			enableParticle(otherShouldBeOn);
		}
	}

	qsf::Time ParticleTimeControlComponent::getTimeFromTimeRange(const qsf::Time& minimumTime, const qsf::Time& maximumTime) const
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

	bool ParticleTimeControlComponent::isTimeInOnInterval(const qsf::Time& timeToCheck, const qsf::Time& lightsOnTime, const qsf::Time& lightsOffTime)
	{
		if (lightsOffTime < lightsOnTime)
		{
			// The time at which the lights should be switched off points to a time on the "next day"
			// We have two intervals to check the current time against to determine if the lights should be switched on
			// First interval [<OnTime>, 23:59]
			// Second interval [00:00, <OffTime>)
			return timeToCheck >= lightsOnTime || timeToCheck < lightsOffTime;
		}
		else if (lightsOffTime > lightsOnTime)
		{
			// Both time values are on the same day
			// We have only one interval to check: [<OnTime>, <OffTime>)
			return timeToCheck >= lightsOnTime && timeToCheck < lightsOffTime;
		}
		else
		{
			QSF_ERROR("sOnTime and sOffTime must not be the same value", return false);
		}
	}

	void ParticleTimeControlComponent::enableParticle(bool enable)
	{
		qsf::ParticlesComponent* particlesComponent = getEntity().getComponent<qsf::ParticlesComponent>();
		QSF_ASSERT(nullptr != particlesComponent, "The current entity doesn't have a particle component", QSF_REACT_NONE);
		if (nullptr != particlesComponent)
		{
			// When disabling the particle only the emitter gets disabled
			// -> We get a nice fadeout of the particles
			// When no particle elements are left from this particle component instance the component unregister itself automatically from the update job
			// -> No need to disable the particle

			// Enabled/Disable the particle emitter
			particlesComponent->setEmitterEnabled(enable);

			// We want that the particle effect repeats after reaching the end
			// Enable it only when not already active and we want the particle itself be active
			if (!particlesComponent->getRepeatAnimation() && enable)
			{
				particlesComponent->setRepeatAnimation(true);
			}

			// Enable the particle component when not already active and we want the particle itself be active
			if (!particlesComponent->isActive() && enable)
			{
				particlesComponent->setActive(true);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
