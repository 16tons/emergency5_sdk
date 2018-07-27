// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/Perception.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A PerceptionEvent is used to communicate perception-information,
		*    usually from sensor-components to reaction-components.
		*    It contains information about the perceived entity, the perceiver, the perception's state
		*    and an ID to give the event a name.
		*/
		class QSF_AI_API_EXPORT PerceptionEvent
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			PerceptionEvent(); // necessary for std::vector::resize used by the vector-serializer
			PerceptionEvent(uint32 eventId,uint64 entityId,uint64 perceivedBy,PerceptionState state);

			// Access to the event's attributes.
			//@{
			uint32 getEventId() const;
			PerceptionState getPerceptionState() const;
			uint64 getEntityId() const;
			uint64 getPerceivedByEntityId() const;

			Entity* getEntity() const;
			Entity* getPerceivedByEntity() const;
			//@}

			// returns either mEntityId or mPerceivedBy, depending on the id passed.
			// Returns 0 if neither mEntityId nor mPerceivedBy match.
			uint64 getEntityIdOtherThan(uint64 entityId) const;

			// Returns the distance between the perceiver and the perceived entity,
			// if possible (which means both entities have a TransformComponent attached).
			// Returns -1 if it is not possible to calculate the distance.
			//@{
			float getDistance() const;
			float getDistanceSquare() const;
			//@}

			// Calculates a position between both objects (perceiver and perceived entity) +/- some gap.
			glm::vec3 getPositionBetween(float gap) const;

			// Returns a perception event where the entity and the perceived entity are swapped.
			// Useful for forwarding events to the perceived entity and back.
			PerceptionEvent createWithSwappedEntities() const;

			// Serialize this event.
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mEventId; // the event's ID
			PerceptionState mState; // the state the event is in. Did the perception of the entity just being, is it continuing or did it just end?
			uint64 mEntityId; // the perceived entity's ID
			uint64 mPerceivedBy; // the perceiving entity's ID
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionEvent-inl.h"
