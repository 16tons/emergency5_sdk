// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionEvent.h"

#include <qsf/serialization/binary/StlTypeSerialization.h>	// TODO(fw): Neededhere?


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		typedef std::vector<PerceptionEvent> PerceptionEventContainer;


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A PerceptionEventQueue holds any number of PerceptionEvents
		*    in the order at which they were pushed into it.
		*    It also allows to iterate over its queued events selectively.
		*
		*    PerceptionEventQueues are used
		*    - by the ReactionSystem to hold global events
		*    - by the ReactionComponents to hold events local to an entity
		*    - to query a sensor's PerceptionEvents
		*/
		class PerceptionEventQueue
		{
		public:


			//[-------------------------------------------------------]
			//[ Subclasses                                            ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    A PerceptionEventIterator simplifies iterating over a
			*    PerceptionQueue's events by only returning events with a
			*    specific ID.
			*/
			class PerceptionEventIterator : public std::iterator<std::input_iterator_tag,PerceptionEvent*>
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				PerceptionEventIterator();
				PerceptionEventIterator(uint32 eventId,const PerceptionEventContainer& eventContainer);

				const PerceptionEvent& operator*() const;
				const PerceptionEvent* operator->() const;
				bool operator==(const PerceptionEventIterator& rho) const;
				bool operator!=(const PerceptionEventIterator& rho) const;
				operator bool() const;
				PerceptionEventIterator& operator++();
				const PerceptionEvent* getPerceptionEvent() const;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				const PerceptionEvent* mPerceptionEvent;
				uint32 mIndex;
				uint32 mEventId;
				const PerceptionEventContainer* mEventContainer;

			}; // PerceptionEventQueue


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			// Clears the queue. The internal ReactionComponent's and ReactionSystem's queues are cleared automatically after their update cycles.
			void clear();

			// Adds an event to the queue.
			void push(const PerceptionEvent& perceptionEvent);

			// Returns a reference to the internal event-container for manual iteration.
			const PerceptionEventContainer& getEvents() const;

			// Returns the number of events currently queued.
			uint32 getNumEvents() const;

			// Methods to create the custom iterators to loop over the events.
			// Note: eventId has only been filled with a default value of 0 (= loop through all events) to
			// allow using c++ range loops (e.g. for (auto& i : evaluationParameters) ).
			//@{
			PerceptionEventIterator begin(uint32 eventId=0) const;
			PerceptionEventIterator end() const;
			//@}

			// Serialize this PerceptionEventQueue. All events are (re)stored.
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PerceptionEventContainer mEvents; // internal event-container.
			//std::unordered_map<uint32,PerceptionEventContainer> events; // after all this is most likely less efficient than a simple vector
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionEventQueue-inl.h"
