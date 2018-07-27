// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <vector>


namespace qsf
{
	namespace ai
	{
		class NavigationComponent;
		class DynamicCollisionLocalPlanner;

		namespace steering
		{
			class SteeringControlPoint;

			// Helper function that tries to find a collision free position along the path provided.
			// It uses the local planner for the collision checks and starts at the end of the path.
			// If the final position is free than nothing needs to be done, otherwise the path is cut at the position free position.
			// A new steering control point is inserted there and later points are erased.
			void tweakPathToEndInCollisionFreePosition(const NavigationComponent& navigator, std::vector<SteeringControlPoint>& path, DynamicCollisionLocalPlanner& collisionTester);
		}
	}
}
