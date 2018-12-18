// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <boost/optional.hpp>
#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Clock class
	*/
	class QSF_API_EXPORT Clock : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const Clock&)> ClockSignal;	///< This Boost signal is emitted when a clock update is triggered


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline Clock();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~Clock();

		/**
		*  @brief
		*    Reset clock state and configuration to default values, e.g. current time is set to 0
		*/
		inline virtual void reset();

		/** Return the current time of this clock */
		inline const Time& getCurrentTime() const;

		/** Manually set the current time */
		inline void setCurrentTime(const Time& time);

		/** Return the time passed since the last clock signal */
		inline const Time& getTimePassed() const;

		/** Manually set the time passed */
		inline void setTimePassed(const Time& timePassed);

		/** Return the number of trigger events (or time steps) so far */
		inline int getSignalCounter() const;

		/** Manually set the signal counter */
		inline void setSignalCounter(int signalCounter);

		/** Return the time speed multiplier */
		inline float getSpeedMultiplier() const;

		/** Set the time speed multiplier; default value is 1.0 */
		inline void setSpeedMultiplier(float multiplier);

		/** Return the fixed time step */
		inline const boost::optional<Time>& getFixedTimeStep() const;

		/** Define fixed time step settings; a fixed time step will cause triggering of clock signals with in fixed time steps */
		inline void setFixedTimeStep(const boost::optional<Time>& fixedTimeStep);

		/** Return the maximum time step */
		inline const boost::optional<Time>& getMaxTimeStep() const;

		/** Define maximum time step settings; when a maximum time step is set, higher incoming time difference are clamped to this maximum */
		inline void setMaxTimeStep(const boost::optional<Time>& maxTimeStep);

		/** Get remaining time that was not used yet for a fixed time step */
		inline Time getRemainingTime() const;

		/**
		*  @brief
		*    Update the clock. Depending on the configuration, this leads to triggering of clock signals
		*
		*  @param[in] timePassed
		*    The time passed since the last update call; this may be expressed in real-time or another clock
		*/
		void updateClock(Time timePassed);

		/**
		*  @brief
		*    Update the clock. Depending on the configuration, this leads to triggering of clock signals
		*
		*  @param[in] baseClock
		*    The base clock to take the passed time from
		*/
		void updateClock(const Clock& baseClock);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void triggerClockSignal(const Time& timePassed);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Current state
		Time	mCurrentTime;				///< The current total time, starting with zero, then summed up time differences
		Time	mTimePassed;				///< Last time difference between two triggered clock signals
		int		mSignalCounter;				///< Number of triggered clock signals so far
		Time	mRemainingTimeForSignals;	///< Used internally to "collect" time until the next fixed time step can be triggered

		// Configuration
		float	mSpeedMultiplier;				///< Time speed multiplier applied to incoming time differences
		boost::optional<Time>	mFixedTimeStep;	///< If valid, clock signals are triggered in fixed time steps, e.g. for game ticks
		boost::optional<Time>	mMaxTimeStep;	///< If valid, incoming time differences get limited to the given maximum (after applying the speed multiplier)


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/clock/Clock-inl.h"
