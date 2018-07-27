// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/base/GetUninitialized.h>


namespace qsf
{
	namespace ai
	{
		inline ObstructingCollision::ObstructingCollision() :
			mEvadeToLeft(true),
			mEntityId(getUninitialized(mEntityId)),
			mAreaId(getUninitialized(mAreaId)),
			mSegmentId(getUninitialized(mSegmentId))
		{}

		inline ObstructingCollision::ObstructingCollision(uint64 entityId, unsigned int areaId, unsigned int segmentId, bool evadeToLeft) :
			mEvadeToLeft(evadeToLeft),
			mEntityId(entityId),
			mAreaId(areaId),
			mSegmentId(segmentId)
		{}
	}

	namespace serialization
	{
		// specialization for binary serialization
		template <>
		struct serializer<ai::ObstructingCollision>
		{
			inline static void serialize(BinarySerializer& serializer, ai::ObstructingCollision& data)
			{
				serializer & data.mEntityId;
				serializer & data.mAreaId;
				serializer & data.mSegmentId;
				serializer & data.mMaxLateralFreeSpace;
				serializer & data.mEvadeToLeft;
				for (unsigned int vertex = 0; vertex < ai::ObstructingCollision::NUM_COLLISION_VERTICES; ++vertex)
					serializer & data.mCollisionBoxVertices[vertex];
			}
		};
	}
}
