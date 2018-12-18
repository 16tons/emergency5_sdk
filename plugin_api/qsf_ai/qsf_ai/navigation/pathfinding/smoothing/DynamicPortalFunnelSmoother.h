// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/navigation/Waypoint.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/base/Line2D.h"

#include <qsf/math/PlanarProjector.h>

#include <boost/optional.hpp>

#include <deque>


namespace qsf
{
	namespace ai
	{
		class Path;

		/**
		* Implements a path smoothing algorithm based on portals that are defined by a circular space around a node (dynamic portal).
		* Implemented as a function object with state for easier debugging for example.
		* It modifies the waypoints deque that is passed in as a reference.
		*/
		class QSF_AI_API_EXPORT DynamicPortalFunnelSmoother
		{
		public:
			// C'tor with the data that is usually available during real pathfinding situations
			DynamicPortalFunnelSmoother(const SearchContactPoint& startData, const boost::optional<DynamicPortal>& startSupportPortal, std::deque<Waypoint>& waypoints,
				const boost::optional<DynamicPortal>& endSupportPortal, const PathSearchConfiguration& searchConfig, Path& result);
			// C'tor with raw data that is optimized to testing situations
			DynamicPortalFunnelSmoother(const SpatialConfiguration3D& startConfig, const TurningConstraint& constraints, bool mayReduceTurningCircle,
				const boost::optional<DynamicPortal>& startSupportPortal, std::deque<Waypoint>& waypoints, const boost::optional<DynamicPortal>& endSupportPortal,
				Path& result, bool drawDebug = false);

			// main function that actually executes the algorithm and smooths out the waypoints
			void execute();

			// Function for single step logic for debugging purposes.
			// Returns whether the algorithm is finished (true) or should continue (false).
			bool executeStep();

		private:
			// A corridor is a movement restriction defined by two lines. The entity should stay between these two
			typedef boost::optional<std::pair<Line2D, Line2D>> Corridor;

			// Ensures at the start that the initial direction is not zero
			void ensureCorrectInitialDirection();

			// Data preprocessing step that shrinks all dynamic portals by the lateral free space required by the entity.
			// Portals that have less free space are reduced to a point.
			void shrinkPortalsByLateralFreeSpace();

			// Insert a new point at the defined data and also enter necessary points where relevant waypoints have been crossed from current config towards the new configuration
			void addTurningPoint(const glm::vec3& newPos, boost::optional<glm::vec3> directionAtNewPos, std::size_t finalCrossedWaypoint, const Corridor& corridor);

			// Helper function to create points where portals have been crossed.
			// It writes the intersections when going from the from config to the to config with the candidates from mStartCalculatingIntersectionsAt to lastIndexToCheck.
			// All intersections are calculated in 2D.
			// It marks the indices it has successfully integrated in mWaypointsModified.
			// The turning circle option is optional.
			// If not set the entity is moving straight.
			// The distance moved seems straightforward to calculate from the data but sometimes we already have it available which comes in handy especially for the curve situations.
			// Via the flag forceIntoSegment you can force creating an intersection at the closest point along the segment if this would intersect outside otherwise.
			// The flag forceIntoRange allows creating intersections if these are outside the range defined by the portal free space
			void createCrossedPortalEntries(const SpatialConfiguration3D& from, const SpatialConfiguration3D& to, const boost::optional<TurningConfiguration>& turningConfiguration,
				UnsignedFloat distanceMoved, bool forceIntoSegment, bool forceIntoRange);

			// Helper function to modify a waypoints exact crossing data (position and direction).
			// It detects the closest point on the segment presented to the portal midpoint.
			// Ideally this point is also the closest along the extended segment (be it an infinite line or complete circle) and within the distance bounds defined by the portal free space.
			// If not you may enter the closest point data anyway or refuse to do so.
			// This is controlled via the two error correction flags forceIntoSegment and forceIntoRange.
			// The first ignores closest points that are outside the segment defined by from and to while the latter ignores larger distances.
			// The returned percentage defines where along the segment the closest point is and remains uninitialized if the point was not inserted
			// The function does not add the index to the resulting path collection because this is sorted on the outside with other entries.
			boost::optional<Percentage> modifyWaypointCrossingPosition(const SpatialConfiguration3D& from, const SpatialConfiguration3D& to,
				const boost::optional<TurningConfiguration>& turningConfiguration, std::size_t waypoint, bool forceIntoSegment, bool forceIntoRange);

			// calculate the fitting portal data for a waypoint that is to be introduced between nodeIndex - 1 and nodeIndex.
			DynamicPortal interpolatePortalTowardsNode(std::size_t nodeIndex, const SpatialConfiguration3D& intermediate) const;

			// Pure debugging helper function that checks last performs sanity checks for the last waypoint added to the path.
			// The created turning configuration is compared with a previous waypoint to see whether it describes a correct circle.
			// The direction should not be zero at this point.
			void checkLastCreatedWaypoint() const;

			// Helper function that tries to create a corridor to remain inside for the current movement.
			// This is mainly needed for entities that need to comply to a turning radius
			Corridor tryCreateMovementCorridor(std::size_t approachedWaypointIndex) const;

			// Push the initial position onto the result vector
			void addInitialPoint();

			// After the algorithm is done with the initially reduced free space, the space is recreated again afterwards
			void inflatePortalsByLateralSpace();

			// Adds a new node but avoids adding duplicate positions to the path if possible
			void addResultNode(const Waypoint& newWp);

			// Set the current config and ensure that we don't end up with a zero direction here
			void updateCurrentConfig(const SpatialConfiguration3D& newCurrentConfig);

			// Special case helper for the situation that the exact position to move towards is not identified uniquely by the funnel algorithm
			// This may happen at the end of a path if there is a real portal or in between if we reverse movement direction for example.
			glm::vec3 calculateTargetPositionInsideFunnel(const glm::vec3& idealGoal);


			// Private data
			// search constraints, static data
			const TurningConstraint& mConstraints;
			const PlanarProjector<float>& mGroundPlaneProjector;
			bool mMayReduceTurningCircle;
			bool mDrawDebug;
			// Supporting portals for start and end are portals that define the incident nodes to the first and final path node respectively.
			// These may be important if we need to maneuver at the beginning or end of the path and therefore need to know the free space available.
			// If these are not provided we are only allowed to maneuver inside the corridor defined by the path nodes
			boost::optional<DynamicPortal> mStartSupportPortal;
			boost::optional<DynamicPortal> mEndSupportPortal;

			// search state description
			SpatialConfiguration3D mCurrentConfig;
			glm::vec3 mLeftFunnelBorder;
			glm::vec3 mRightFunnelBorder;
			std::deque<Waypoint>& mWaypoints;
			std::deque<std::size_t> mCrossedRelevantWaypointIndices; // tracks the indices of nodes into the waypoint vector which we have crossed since current that need to be stored in the path
			std::size_t mNextWaypointIndex; // index into waypoints that is the next waypoint we should look at
			std::vector<bool> mWaypointsModified; // Keeps track of which waypoints already got their state updated by the smoothing. Simple sanity check to avoiding adding points several time
			Path& mResult; // The final path is created here
		};
	}
}
