// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/math/GlmHelper.h>
#include <qsf/math/GlmBulletConversion.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/Segment.h>
#include <qsf/math/CoordinateSystem.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <bullet/LinearMath/btAabbUtil2.h>

namespace qsf
{
	namespace ai
	{
		template <typename Grid>
		GridBlocker<Grid>::GridBlocker(const GridConfiguration& config, Grid& grid, const HeightRestriction& heightRestriction, GroundMapQuery* query) :
			mGridConfig(config),
			mGrid(grid),
			mHeightRestriction(heightRestriction),
			mGroundQuery(query),
			mGroundPlaneProjector(CoordinateSystem::getTopViewProjector())
		{}

		template <typename Grid>
		std::pair<GridCoordinates, GridCoordinates> GridBlocker<Grid>::calculateIntersectionWithGrid(const glm::vec3& minimum, const glm::vec3& maximum) const
		{
			const GridCoordinates begin = mGridConfig.convertPosition(getMaximum(minimum, mGridConfig.mOrigin));
			// TODO(vs) hardcoded Y here - I was unable to write a shorter version that covers all usecases correctly
			// If the obstacle is out of the range completely we need to return (0, 0) as past the end coordinates
			const GridCoordinates end = (maximum.x < mGridConfig.mOrigin.x || maximum.z < mGridConfig.mOrigin.z) ? GridCoordinates(0, 0) :
				getMinimum(mGridConfig.convertPosition(maximum) + GridCoordinates(1, 1), mGridConfig.mDimensions);
			return std::make_pair(begin, end);
		}

		template <typename Grid>
		glm::vec3 GridBlocker<Grid>::getBasePositionAt(unsigned int cellX, unsigned int cellY)
		{
			glm::vec3 cellMidPoint = mGridConfig.convertPositionToMidpoint(GridCoordinates(cellX, cellY));
			if (!mGroundQuery)
				return cellMidPoint; // we can't correct it so we just use the plain values calculated

			const glm::vec2 midPoint2D = mGroundPlaneProjector.dropValue(cellMidPoint);
			float realHeight = getUninitialized<float>();

			if (mGroundQuery->getTerrainHeightAtXZPosition(midPoint2D.x, midPoint2D.y, realHeight))
				return mGroundPlaneProjector.insertValue(midPoint2D, realHeight);

			return cellMidPoint;
		}

		template <typename Grid>
		gridEntry::Result GridBlocker<Grid>::addSphereBlocker(const glm::vec3& midpoint, UnsignedFloat radius)
		{
			// Brute force version testing all the cells in a linear order.
			// Perhaps this could be optimized to numerically determining the next intersected cell following the circle outline by a bresenham algorithm and then blocking the inside by a scan line algorithm.
			const glm::vec3 obstacleMin = midpoint - glm::vec3(radius, radius, radius);
			const glm::vec3 obstacleMax = midpoint + glm::vec3(radius, radius, radius);

			// Apply optional height restrictions
			const float minHeight = mGroundPlaneProjector.getValue(obstacleMin);
			const float maxHeight = mGroundPlaneProjector.getValue(obstacleMax);
			if (!mHeightRestriction.mUseTerrainHeight &&
				(mHeightRestriction.mMinHeight > maxHeight || mHeightRestriction.mMinHeight + mHeightRestriction.mMaxAdditionalHeight < minHeight))
				return gridEntry::OUTSIDE_BORDERS;

			// Like STL iterators the boundingBoxEnd is composed of indices one-past-the-end of the legal range.
			const CellBoundaries boundingCells = calculateIntersectionWithGrid(obstacleMin, obstacleMax);
			const GridCoordinates& intersectionBegin = boundingCells.first;
			const GridCoordinates& intersectionEnd = boundingCells.second;

			// Obstacles that are out of the area lead to a situation where the begin is greater than the end value for at least one dimension which means the loop body below will not be tested fro them
			bool blockedSomething = false;
			for (unsigned int x = intersectionBegin.x; x < intersectionEnd.x; ++x)
			{
				for (unsigned int y = intersectionBegin.y; y < intersectionEnd.y; ++y)
				{
					const glm::vec3 cellMidPoint = getBasePositionAt(x, y);
					const glm::vec3 closestPoint = Math::getClosestPointOnLineSegment(midpoint, qsf::Segment::fromOriginAndDirection(cellMidPoint, mGroundPlaneProjector.makeVector(mHeightRestriction.mMaxAdditionalHeight)));

					if (glm::distance(midpoint, closestPoint) <= radius)
					{
						mGrid.setBlocked(x, y);
						blockedSomething = true;
					}
				}
			}

			return blockedSomething ? gridEntry::SUCCESS : gridEntry::OUTSIDE_BORDERS;
		}

		template <typename Grid>
		gridEntry::Result GridBlocker<Grid>::addAxisAlignedBoundingBoxBlocker(const glm::vec3& corner1, const glm::vec3& corner2)
		{
			const glm::vec3 obstacleMin = getMinimum(corner1, corner2);
			const glm::vec3 obstacleMax = getMaximum(corner1, corner2);

			// Apply optional height restrictions
			const float minHeight = mGroundPlaneProjector.getValue(obstacleMin);
			const float maxHeight = mGroundPlaneProjector.getValue(obstacleMax);
			if (!mHeightRestriction.mUseTerrainHeight &&
				(mHeightRestriction.mMinHeight > maxHeight || mHeightRestriction.mMinHeight + mHeightRestriction.mMaxAdditionalHeight < minHeight))
				return gridEntry::OUTSIDE_BORDERS;

			const CellBoundaries boundingCells = calculateIntersectionWithGrid(obstacleMin, obstacleMax);
			const GridCoordinates& intersectionBegin = boundingCells.first;
			const GridCoordinates& intersectionEnd = boundingCells.second;

			bool blockedSomething = false;
			for (unsigned int x = intersectionBegin.x; x < intersectionEnd.x; ++x)
			{
				for (unsigned int y = intersectionBegin.y; y < intersectionEnd.y; ++y)
				{
					const glm::vec3 cellMidpoint = getBasePositionAt(x, y);
					const float cellMinHeight = mGroundPlaneProjector.getValue(cellMidpoint);
					const float cellMaxHeight = cellMinHeight + mHeightRestriction.mMaxAdditionalHeight;

					if (cellMinHeight > maxHeight || cellMaxHeight < minHeight)
						continue;

					mGrid.setBlocked(x, y);
					blockedSomething = true;
				}
			}

			return blockedSomething ? gridEntry::SUCCESS : gridEntry::OUTSIDE_BORDERS;
		}

		template <typename Grid>
		gridEntry::Result GridBlocker<Grid>::addOrientedBoundingBoxBlocker(const glm::vec3& anchorPoint, const glm::quat& rotation, const glm::vec3& extensions)
		{
			// TODO(vs) Replace by an optimized algorithm.
			// This is planned as the task hansoft://localhost;Company$20Projects;77b90e39/Task/448138

			// Current implementation is more straightforward but not very efficient.
			// First determine a very rough axis aligned box around the obstacle.
			// We start by creating the circumscribed circle around the rectangle.
			const glm::vec3 absoluteHalfExtensions = glm::abs(extensions) * .5f;
			const glm::quat inverseRotation = glm::inverse(rotation); // TODO(vs) I am currently not sure whether inverting the rotation is correct here

			const UnsignedFloat circumscribedRadius = 0.5f * glm::length(extensions);
			const glm::vec2 groundPlaneRadii(circumscribedRadius, circumscribedRadius);
			const glm::vec3 testRangeMin = anchorPoint - mGroundPlaneProjector.insertValue(groundPlaneRadii, 0.f);
			const glm::vec3 testRangeMax = anchorPoint + mGroundPlaneProjector.insertValue(groundPlaneRadii, circumscribedRadius * UncheckedUnsignedFloat(2.f));

			const float testRangeMinHeight = mGroundPlaneProjector.getValue(testRangeMin);
			const float testRangeMaxHeight = mGroundPlaneProjector.getValue(testRangeMax);

			// Apply optional height restrictions
			// Since the test range might be larger than the extreme obstacle boundaries we could actually fail to ignore an obstacle that is outside the defined boundaries here
			if (!mHeightRestriction.mUseTerrainHeight &&
				(mHeightRestriction.mMinHeight > testRangeMaxHeight || mHeightRestriction.mMinHeight + mHeightRestriction.mMaxAdditionalHeight < testRangeMinHeight))
				return gridEntry::OUTSIDE_BORDERS;

			const CellBoundaries boundingCells = calculateIntersectionWithGrid(testRangeMin, testRangeMax);
			const GridCoordinates& intersectionBegin = boundingCells.first;
			const GridCoordinates& intersectionEnd = boundingCells.second;

			bool blockedSomething = false;
			for (unsigned int x = intersectionBegin.x; x < intersectionEnd.x; ++x)
			{
				for (unsigned int y = intersectionBegin.y; y < intersectionEnd.y; ++y)
				{
					const glm::vec3 intersectionMinPoint = getBasePositionAt(x, y);
					const glm::vec3 intersectionMaxPoint = intersectionMinPoint + mGroundPlaneProjector.makeVector(mHeightRestriction.mMaxAdditionalHeight);

					// Rotate both points into the local coordinate space around the midpoint of the obstacle
					const glm::vec3 rotatedMinPointInLocalCoordinates = anchorPoint + (inverseRotation * (intersectionMinPoint - anchorPoint));
					const glm::vec3 rotatedMaxPointInLocalCoordinates = anchorPoint + (inverseRotation * (intersectionMaxPoint - anchorPoint));

					// Check if one of the points is actually inside the obstacle
					const glm::vec3 differenceToMidpoint = glm::abs(anchorPoint - rotatedMinPointInLocalCoordinates);

					// Compare the distance to the midpoint for all axis
					bool intersects = (differenceToMidpoint.x <= absoluteHalfExtensions.x &&
						differenceToMidpoint.y <= absoluteHalfExtensions.y &&
						differenceToMidpoint.z <= absoluteHalfExtensions.z);

					if (!intersects)
					{
						// convert to bullet for calling the intersection test
						btScalar factorAlongRay = getUninitialized<btScalar>();
						btVector3 normal;
						intersects = btRayAabb(
							convertVector3(rotatedMinPointInLocalCoordinates),
							convertVector3(rotatedMaxPointInLocalCoordinates),
							convertVector3(-absoluteHalfExtensions),
							convertVector3(absoluteHalfExtensions),
							factorAlongRay,
							normal);
					}

					if (intersects)
					{
						mGrid.setBlocked(x, y);
						blockedSomething = true;
					}
				}
			}

			return blockedSomething ? gridEntry::SUCCESS : gridEntry::OUTSIDE_BORDERS;
		}

		template <typename Grid>
		gridEntry::Result GridBlocker<Grid>::addBlockerFromCollisionComponent(const CollisionComponent& collision)
		{
			// TODO(vs) Should we use the topmost ancestor ID here or the IDs of the child collisions?
			// I decided to use the latter to be able to move between non interlapping child collisions
			const void* entityAddress = &collision;

			// Consider each collision by its native representation
			switch (collision.getGeometryType())
			{
				case CollisionComponent::SPHERE:
				{
					glm::vec3 center;
					float radius = getUninitialized<float>();
					QSF_CHECK(collision.getAsSphere(center, radius), "Collision claims to be unable to return the geometry in its native sphere representation, ignoring collision",
						break);

					return addSphereBlocker(entityAddress, center, radius);
				}
				case CollisionComponent::AXIS_ALIGNED_BOUNDING_BOX:
				{
					glm::vec3 corner1;
					glm::vec3 corner2;
					QSF_CHECK(collision.getAsAxisAlignedBoundingBox(corner1, corner2), "Collision claims to be unable to return the geometry in its native AABB representation, ignoring collision",
						break);

					return addAxisAlignedBoundingBoxBlocker(entityAddress, corner1, corner2);
				}
				case CollisionComponent::ORIENTED_BOUNDING_BOX:
				{
					glm::vec3 anchorPoint;
					glm::quat rotation;
					glm::vec3 extensions;
					QSF_CHECK(collision.getAsOrientedBoundingBox(anchorPoint, rotation, extensions), "Collision claims to be unable to return the geometry in its native OBB representation, ignoring collision",
						break);

					return addOrientedBoundingBoxBlocker(entityAddress, anchorPoint, rotation, extensions);
				}
				case CollisionComponent::TRIANGLE_MESH:
					QSF_WARN("Triangle mesh collisions are not supported for the current navigation mesh generation logic",
						return gridEntry::OUTSIDE_BORDERS);
				default:
					QSF_ERROR("Unexpected collision type " << collision.getGeometryType() << " ignoring the collision",
						return gridEntry::OUTSIDE_BORDERS);
			}

			return gridEntry::OUTSIDE_BORDERS;
		}

		template <typename Grid>
		void GridBlocker<Grid>::addOuterBorderObstacle()
		{
			// Mark all outer edges which results in the corners being set twice each
			for (unsigned int x = 0; x < mGridConfig.mDimensions.x; ++x)
			{
				mGrid.setBlocked(x, 0);
				mGrid.setBlocked(x, mGridConfig.mDimensions.y - 1);
			}

			for (unsigned int y = 0; y < mGridConfig.mDimensions.y; ++y)
			{
				mGrid.setBlocked(0, y);
				mGrid.setBlocked(mGridConfig.mDimensions.x - 1, y);
			}
		}
	}
}
