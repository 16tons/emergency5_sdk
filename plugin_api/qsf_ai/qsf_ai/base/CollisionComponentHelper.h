// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	class CollisionComponent;

	namespace ai
	{
		/**
		* Helpers function for converting the data from collision components to the format we need to use it in the AI.
		* Calculates extents in several directions and writes the result to the reference parameters.
		* Forward and Lateral extent are measured as distance to the midpoint on one dimension in the movement plane.
		* The max extent is the worst distance in the movement plane that is relevant for example when rotating in place.
		*/
		void calculate2DExtents(const CollisionComponent& collision, UnsignedFloat& lateralExtent, UnsignedFloat& forwardExtent, UnsignedFloat& maxExtent);

		/**
		* Helper function to calculate the rough XZ size of a collision component for use with the AI sensor perception system.
		* It's rather cheap and simply returns a very rough number to decide wether a collider is pretty small or pretty large.
		*/
		float getRoughSizeOfCollisionComponent(const CollisionComponent& collisionComponent);
	}
}
