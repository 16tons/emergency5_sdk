// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/voronoi/GridConfiguration.h"
#include "qsf_ai/voronoi/DistanceGrid.h"
#include "qsf_ai/voronoi/HeightRestriction.h"

#include <qsf/math/PlanarProjector.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#include <set>


namespace qsf
{
	class Map;
	class GroundMapQuery;
	class BulletCollisionComponent;

	namespace ai
	{
		namespace voronoi
		{
			/**
			* Class for use during the voronoi grid creation.
			* This implements the step of entering the obstacles into the grid by using the collisions available inside the bullet physics world.
			* Subsequent raycast like collision tests are used to determine whether there is a collision at all.
			* These are conducted at the resolution of the grid with objects of the size of a single grid cell.
			*/
			class QSF_AI_API_EXPORT ContactTestGridBlocker : public boost::noncopyable
			{
			public:
				//@{
				// Access the test object collision flags statically before starting the simulation here.
				static void setBulletTestObjectCollisionMask(short mask);
				static short getBulletTestObjectCollisionMask();
				//@}

				// The constructor already does all the work and the grid is changed as a result of creating an object with it.
				// The avoid collision filter is used to see which objects actually pose an obstacle.
				// The require collision filter is used to positively select collision to define the areas where entities may move.
				// The collision test is symmetric inside bullet and we assume the own collision flags for the test object to be all set.
				ContactTestGridBlocker(DistanceGrid& grid, const GridConfiguration& gridConfig, const HeightRestriction& restriction, Map& map,
					int walkableLevel, bool testAtTerrainHeight, short avoidCollisionFilter, short requiredCollisionFilter);

				// Sets each cell at the outer border as blocked by the address passed.
				// This is required for technical reasons to create paths on the outskirts of the voronoi diagram.
				// This is typically not a result of a real physical object so an address needs to be selected so that we avoid an accidental clash with a real obstacle.
				// Currently the address of this object is just used here
				void addOuterBorderObstacle();

				// filter cells which are not on out walkable level
				void filterWalkables();

				// filter cells which are not inside water
				void filterWater();

				// Main function that enters all collisions known to bullet into the grid
				// Optionally pass a set with all the collisions components found
				void enterAllBulletCollisions(std::set<const BulletCollisionComponent*>* collisions);

			private:
				// helper function calculating the bottom base position at a given point.
				glm::vec3 getBasePositionAt(unsigned int cellX, unsigned int cellY) const;

				// map and terrain access data
				Map& mMap; // map should be const but isn't for interface reaso9ns of the ground map query
				const PlanarProjector<float>& mGroundPlaneProjector;
				int mWalkableLevel;
				bool mTestAtWalkable;

				// bullet access data
				short mAvoidCollisionFilter;
				short mRequireCollisionFilter;
				btCollisionObject mCollisionTestObject;
				btBoxShape mCollisionTestShape;

				// the AI grid configuration and data
				DistanceGrid& mGrid;
				const GridConfiguration& mGridConfig;
				const HeightRestriction mHeightRestriction;

				// static collision mask of the test object that is used for voronoi checks.
				static short sBulletTestObjectCollisionMask;
			};
		}
	}
}
