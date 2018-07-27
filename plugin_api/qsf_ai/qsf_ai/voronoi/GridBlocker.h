// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/voronoi/GridConfiguration.h"
#include "qsf_ai/voronoi/HeightRestriction.h"

#include <qsf/platform/PlatformTypes.h>
#include <qsf/math/PlanarProjector.h>

#include <glm/fwd.hpp>

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>


namespace qsf
{
	class CollisionComponent;
	class GroundMapQuery;

	namespace ai
	{
		// The result of a request to enter something in a grid.
		// Returns whether the entity could be entered into the grid or was completely outside the range.
		namespace gridEntry
		{
			enum Result
			{
				SUCCESS,
				OUTSIDE_BORDERS,
			};
		}

		/**
		* A grid blocker manages adding obstacles of different shapes to a grid.
		* It encapsulates the algorithmic details to determine which cells are actually blocked.
		* We support oriented and axis aligned bounding boxes, spheres and polygons as obstacle shapes.
		* Since the distance grid it works on is a templated class itself the blocker is also templated.
		* To be open for using it with grids for another purpose in the future it is written with the whole grid as a template parameter.
		* You can filter the obstacles for height by providing a window which needs to contain at least some part of the obstacle to be considered.
		* As soon as the obstacle is not completely outside this window the whole obstacle is used at the moment.
		*/
		template <typename Grid>
		class GridBlocker : public boost::noncopyable
		{
		public:
			// You may optionally pass a ground map query so that the real terrain heights are used
			GridBlocker(const GridConfiguration& config, Grid& grid, const HeightRestriction& heightRestriction, GroundMapQuery* query = nullptr);

			gridEntry::Result addSphereBlocker(const glm::vec3& midpoint, UnsignedFloat radius);

			// The min and max are determined automatically from corner1 and corner2's values
			gridEntry::Result addAxisAlignedBoundingBoxBlocker(const glm::vec3& corner1, const glm::vec3& corner2);

			// The position is expected to be in the middle of the ground plane like defined for our entity positions.
			// The extensions define the side length and need to be added to the position in a different way for the axis defining the ground plane than the height axis.
			gridEntry::Result addOrientedBoundingBoxBlocker(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& extensions);

			// Extracts the collision geometry from the component and delegates to the fitting add method.
			gridEntry::Result addBlockerFromCollisionComponent(const CollisionComponent& collision);

			// Sets each cell at the outer border as blocked by the id passed.
			// This is required for technical reasons to create paths on the outskirts of the Voronoi diagram.
			// This is typically not a result of a real physical object so an id needs to be selected with care to avoid an accidental clash with a real obstacle.
			// A proposal is to use the restricted core entity ID zero which typically has a MapProperties component that introduces such outer boundaries.
			void addOuterBorderObstacle();
		private:
			// internal helper function to get the position at the ground for specific cell coordinates
			glm::vec3 getBasePositionAt(unsigned int cellX, unsigned int cellY);

			const GridConfiguration mGridConfig;
			Grid& mGrid;

			// Converts a bounding box in world coordinates into a bounding box in grid coordinates.
			// It intersects the bounding box with the legal space defined by the grid configuration.
			// The minimum is the first entry of the returned pair while the second entry holds indices one past the end of the bounding box.
			// The returned value is to be used like a pair of STL iterators which means never actually access the second (end) value.
			typedef std::pair<GridCoordinates, GridCoordinates> CellBoundaries;
			CellBoundaries calculateIntersectionWithGrid(const glm::vec3& minimum, const glm::vec3& maximum) const;

			// Defines an optional window for the height value that is used as a filter to whether obstacles are considered or ignored.
			// If used only obstacles who are not completely outside the window [mHeightRestriction.first, mHeightRestriction.second] are used.
			// First is minimum, second maximum height value.
			HeightRestriction mHeightRestriction;

			// Optional ground query to use during the grid blocking phase if we want to use the terrain height dynamically
			GroundMapQuery* mGroundQuery;

			const PlanarProjector<float>& mGroundPlaneProjector;
		};
	}
}

#include "qsf_ai/voronoi/GridBlocker-inl.h"
