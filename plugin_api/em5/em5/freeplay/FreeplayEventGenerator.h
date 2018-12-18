// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/time/Time.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
	class FreeplayEventFactory;
	class FreeplayEventTriggerInfo;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 freeplay event generation class
	*/
	class EM5_API_EXPORT FreeplayEventGenerator : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FreeplaySystem;	// May update the last triggered events
		friend class FreeplayGameMode;	// To update number of succeeded and failed events


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::function<bool(uint32)> EventIndexAllowedCallback;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayEventGenerator();

		/**
		*  @brief
		*    Destructor
		*/
		~FreeplayEventGenerator();

		void startup();
		void shutdown();

		void updateEventGeneration(const qsf::Time& timePassed);

		FreeplayEvent* triggerEvent(FreeplayEventFactory& factory, const FreeplayEventTriggerInfo& triggerInfo);

		/**
		*  @brief
		*    Get the maximum event density
		*/
		float getUpperLimit() const;

		/**
		*  @brief
		*    Set the maximum event density that should not be exceeded
		*/
		void setUpperLimit(float density);

		/**
		*  @brief
		*    Set the minimum and maximum wait time between events (in seconds)
		*/
		void setWaitTimeBetweenEvents(float mininumWaitTime, float maximumWaitTime);

		/**
		*  @brief
		*    Set the minimum and maximum start wait time before first event (in seconds)
		*/
		void setStartWaitTime(float mininumWaitTime, float maximumWaitTime);

		/**
		*  @brief
		*    Returns the maximum number of events that can be active at the same time
		*/
		uint32 getUpperEventLimit() const;

		/**
		*  @brief
		*    Set the maximum number of events that can be active at the same time
		*/
		void setUpperEventLimit(uint32 maximumEventCount);

		uint32 getMaxEventLimit() const;

		bool isUsingForcedEventOrder() const;
		void switchToForcedEventOrder(bool useForcedOrder, int32 lastEventIndex = -1);

		int32 getEventFactoryIndexById(uint32 factoryId) const;

		void setTriggerFailReason(const std::string& reason);

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool tryTriggerEvent();
		bool triggerFromFactory(FreeplayEventFactory& factory);
		bool wasFactoryRecentlyTriggered(FreeplayEventFactory& factory, uint32 numberOfRecentFactories) const;
		FreeplayEventFactory* getNextForcedEventFactory();
		void calculateNewWaitTime();

		std::string getEventGenerationDebugText() const;
		void updateDebug();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::deque<std::string> FactoryDeque;
		typedef boost::container::flat_map<uint32, uint32> CounterMap;

		struct TimeInterval
		{
			qsf::Time mMinimum;
			qsf::Time mMaximum;
			qsf::Time getRandomTime() const;
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float		 mUpperDensityLimit;				///< The maximum event density that should not be exceeded
		qsf::Time	 mWaitTimeUntilNextEvent;
		TimeInterval mWaitTimeBetweenEventsAtStart;
		TimeInterval mWaitTimeBetweenEvents;
		uint32		 mCurrentEventLimit;
		uint32		 mMinEventLimit;
		uint32		 mMaxEventLimit;
		bool		 mSelectOnlyEventsMarkedAsStartEvents;

		// Forced order mode
		bool		 mForcedEventOrder;
		int32		 mLastEventIndex;

		// Statistics & information
		const char*	 mLastTriedFactoryCampClassId;
		CounterMap   mTriggeredFactoryCounters;
		FactoryDeque mLastTriggeredFactories;
		uint32		 mNumEventsTriggered;
		uint32		 mNumEventsSucceededInARow;
		uint32		 mNumEventsSucceededSinceCheck;		// Not the total number
		uint32		 mNumEventsFailedSinceCheck;		// Not the total number
		uint32		 mWonEventsForIncrease;
		uint32		 mWonEventsForNotDecrease;			// Minimum number of events which have to be won, to not decrease the max event number when a second event is lost

		// Debug output
		qsf::DebugDrawProxy mDebugDrawProxy;
		std::string			mLastTriggerFailReason;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
