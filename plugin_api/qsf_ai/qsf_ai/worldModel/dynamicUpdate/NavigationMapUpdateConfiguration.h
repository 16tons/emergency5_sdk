// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>
#include <qsf/math/Transform.h>


namespace qsf
{
	namespace ai
	{
		/**
		* A navigation map update configuration holds all the data to describe an update incident for one navigation map.
		* It describes the dimension and position of the entity to be integrated and to which map it relates.
		* Only boxes are allowed as dynamic collisions.
		*/
		class NavigationMapUpdateConfiguration
		{
		public:
			// The mode of the update, is an obstacle added or removed?
			enum Mode
			{
				ADD, // add a new obstacle
				REMOVE, // remove an old obstacle
			};

			NavigationMapUpdateConfiguration(Mode mode, unsigned int mapId, uint64 entityId, const Transform& transform, const glm::vec3& boxHalfExtents);
			NavigationMapUpdateConfiguration(); // default constructor needed for serialization, sets everything to dummy values

			// Public data because this is mainly a struct like data collection
			Mode mMode; // The mode this task in run in
			unsigned int mMapId; // The map to change
			uint64 mEntityId; // The entity ID of the collision
			Transform mTransform; // The transform of the collision box in question
			glm::vec3 mCollisionBoxHalfExtents; // The half extents of the collision box in question.
		};
	}
}

#include "qsf_ai/worldModel/dynamicUpdate/NavigationMapUpdateConfiguration-inl.h"
