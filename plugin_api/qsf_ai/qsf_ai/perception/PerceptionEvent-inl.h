// Copyright (C) 2012-2017 Promotion Software GmbH


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
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline PerceptionEvent::PerceptionEvent() :
			mEventId(0),
			mState(PERCEPTION_BEGAN),
			mEntityId(0),
			mPerceivedBy(0)
		{
		}

		inline PerceptionEvent::PerceptionEvent(uint32 eventId,uint64 entityId,uint64 perceivedBy,PerceptionState state) :
			mEventId(eventId),
			mState(state),
			mEntityId(entityId),
			mPerceivedBy(perceivedBy)
		{
		}

		inline PerceptionEvent PerceptionEvent::createWithSwappedEntities() const
		{
			return PerceptionEvent(mEventId,mPerceivedBy,mEntityId,mState);
		}

		inline uint32 PerceptionEvent::getEventId() const
		{
			return mEventId;
		}

		inline PerceptionState PerceptionEvent::getPerceptionState() const
		{
			return mState;
		}

		inline uint64 PerceptionEvent::getEntityId() const
		{
			return mEntityId;
		}

		inline uint64 PerceptionEvent::getPerceivedByEntityId() const
		{
			return mPerceivedBy;
		}

		inline uint64 PerceptionEvent::getEntityIdOtherThan(uint64 entityId) const
		{
			return mEntityId==entityId ? mPerceivedBy : mEntityId;
		}

		inline void PerceptionEvent::serialize(BinarySerializer& serializer)
		{
			serializer & mEntityId;
			serializer & mPerceivedBy;
			serializer & mEventId;
			serializer.serializeAs<unsigned char>(mState);
		}

	}
	// to support the array-serializer in StlTypeSerialization
	namespace serialization
	{
		template<>
		struct serializer<ai::PerceptionEvent>
		{
			inline static void serialize(BinarySerializer& serializer, ai::PerceptionEvent& value)
			{
				value.serialize(serializer);
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
