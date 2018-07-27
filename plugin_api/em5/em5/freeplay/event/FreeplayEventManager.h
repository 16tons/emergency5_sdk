// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/idgenerator/ConsecutiveIdGenerator.h>

#include <boost/container/flat_map.hpp>


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
	*    EMERGENCY 5 freeplay event factory manager
	*/
	class EM5_API_EXPORT FreeplayEventManager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplaySystem;			// May insert and remove freeplay events
	friend class FreeplayEventGenerator;	// May destroy events


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<uint32> EventIdArray;
		typedef boost::container::flat_map<uint32, FreeplayEvent*> EventMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		FreeplayEventManager();

		/**
		*  @brief
		*    Destructor
		*/
		~FreeplayEventManager();

		void clear();

		void startup();
		void shutdown();

		uint32 getNumberOfActiveEvents() const;

		void updateEvents(const qsf::Time& timePassed);

		FreeplayEvent* getEventById(uint32 eventId) const;

		/**
		*  @brief
		*    Get the map of the current events
		*/
		const EventMap& getCurrentEvents() const;

		/**
		*  @brief
		*    Get the last triggered event
		*/
		FreeplayEvent* getLastEvent() const;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void insertEvent(FreeplayEvent& freeplayEvent);
		void onEventFinished(FreeplayEvent& freeplayEvent, FreeplayEvent::EventResult& eventResult);
		void onObjectiveGetsPoints(FreeplayEvent& freeplayEvent, FreeplayEvent::ObjectiveResult& objectiveResult);
		void onAbortEvent(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Destroy all events to be removed
		*/
		void flushEventsToBeRemoved();

		/**
		*  @brief
		*    Destroy the given event
		*/
		void destroyEvent(FreeplayEvent& freeplayEvent);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Create freeplay event by CAMP class ID
		*/
		static FreeplayEvent* createFreeplayEventById(uint32 campId);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<FreeplayEvent*> EventArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EventMap		mEvents;			///< Active event instances, mapped by their event IDs (i.e. in order of triggering)
		EventArray		mEventsToBeRemoved;	///< Array of event instances to be removed

		qsf::ConsecutiveIdGenerator<uint32> mIdGenerator;	///< Freeplay event instance ID generator


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
