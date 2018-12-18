// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline NavigationMapUpdateConfiguration::NavigationMapUpdateConfiguration(Mode mode, unsigned int mapId, uint64 entityId, const Transform& transform, const glm::vec3& boxHalfExtents) :
			mMode(mode),
			mMapId(mapId),
			mEntityId(entityId),
			mTransform(transform),
			mCollisionBoxHalfExtents(boxHalfExtents)
		{}

		inline NavigationMapUpdateConfiguration::NavigationMapUpdateConfiguration() :
			mMode(REMOVE),
			mMapId(getUninitialized(mMapId)),
			mEntityId(getUninitialized(mEntityId))
		{}

	}

	namespace serialization
	{
		// specialization for binary serialization
		template <>
		struct serializer<ai::NavigationMapUpdateConfiguration>
		{
			inline static void serialize(BinarySerializer& serializer, ai::NavigationMapUpdateConfiguration& data)
			{
				serializer.serializeAs<uint32>(data.mMode);
				serializer & data.mMapId;
				serializer & data.mEntityId;
				data.mTransform.serialize(serializer); // (vs) No idea why in this case only this form works
				serializer & data.mCollisionBoxHalfExtents;
			}
		};
	}
}
