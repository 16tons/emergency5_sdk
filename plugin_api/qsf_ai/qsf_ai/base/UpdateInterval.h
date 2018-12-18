// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** UpdateInterval is a tiny helper class to manage an
	* interval and a countdown. Internal values are meant to be in seconds.
	* Used by the ReactionComponent and the sensors.
	*/
	class UpdateInterval
	{
	public:
		// Tag string IDs to be used with a JSON serializer
		//@{
		static const char* UPDATE_INTERVAL_TAG;
		static const char* UPDATE_INTERVAL_JITTER_TAG;
		//@}

		// default values for the properties, also used by the CAMP system.
		//@{
		static const UnsignedFloat DEFAULT_UPDATE_INTERVAL;
		static const UnsignedFloat DEFAULT_UPDATE_INTERVAL_JITTER;
		//@}
	public:
		UpdateInterval(UnsignedFloat interval=DEFAULT_UPDATE_INTERVAL,UnsignedFloat intervalJitter=DEFAULT_UPDATE_INTERVAL_JITTER);

		// Binary (de)serialization method.
		void serialize(BinarySerializer& serializer);

		// Get / set the interval to which the timer is resetted if resetTimer is called.
		//@{
		UnsignedFloat getInterval() const;
		void setInterval(UnsignedFloat interval);
		//@}

		// Get / set an optional random jitter which is added to the timer value when it is resetted.
		// The final value to be added is a random number between 0 and this value.
		//@{
		UnsignedFloat getIntervalJitter() const;
		void setIntervalJitter(UnsignedFloat intervalJitter);
		//@}

		// Returns true if the timer-attribute is smaller than 0.
		bool isItTimeToUpdate() const;

		// Resets the timer-attribute to the interval's value plus an optional random jitter.
		void resetTimer();

		// Decreases the timer-attribute by secondsPassed.
		void updateTimer(float secondsPassed);

	private:
		UnsignedFloat mInterval;
		UnsignedFloat mIntervalJitter;
		float mTimer; // remaining time until the next triggering

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UpdateInterval-inl.h"
