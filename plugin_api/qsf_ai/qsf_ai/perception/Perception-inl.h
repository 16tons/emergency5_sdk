// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{


		//[-------------------------------------------------------]
		//[ Local methods                                         ]
		//[-------------------------------------------------------]
		// to support the array-serializer in StlTypeSerialization
		template<>
		struct serializer<ai::Perception>
		{
			inline static void serialize(BinarySerializer& serializer, ai::Perception& value)
			{
				value.serialize(serializer);
			}
		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // serialization


	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline Perception::Perception() :
			mEntityID(0),
			mEntity(0),
			mState(PERCEPTION_BEGAN)
		{
		}

		inline Perception::Perception(Entity& collider) :
			mEntityID(collider.getId()),
			mEntity(&collider),
			mState(PERCEPTION_BEGAN)
		{
		}

		inline uint64 Perception::getEntityID() const
		{
			return mEntityID;
		}

		inline PerceptionState Perception::getState() const
		{
			return mState;
		}

		inline void Perception::setState(PerceptionState state)
		{
			mState = state;
		}

		inline void Perception::serialize(BinarySerializer& serializer)
		{
			serializer & mEntityID;
			serializer.serializeAs<unsigned char>(mState);
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		inline Entity* Perception::getEntity() const
		{
			return mEntity;
		}

		inline void Perception::invalidate()
		{
			mEntityID = 0;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
