// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	namespace ai
	{
		inline NavigationDebugGroup::NavigationDebugGroup(DebugGroupManager* dgm) :
			DebugGroup(dgm),
			mDebuggedNavigationMap(DEFAULT_NAVIGATION_MAP),
			mConnectedNavigationMap(DEFAULT_NAVIGATION_MAP),
			mConnectionId(DEFAULT_CONNECTION_TO_SHOW),
			mLaneChangeOptionId(SHOW_LANE_CHANGE_OPTION.mDefault),
			mShowEntityPaths(SHOW_ENTITY_PATH.mDefault),
			mShowEntityPathSteps(SHOW_ENTITY_PATH_STEPS.mDefault),
			mShowMapAreaOutlines(DEFAULT_SHOW_MAP_AREAS_SETTING),
			mShowMapPaths(DEFAULT_SHOW_MAP_PATH_SETTING),
			mShowNodeStates(DEFAULT_SHOW_NODE_STATE_SETTING),
			mShowSearchStatistics(DEFAULT_SHOW_SEARCH_STATISTICS),
			mShowSteeringSpeed(DEFAULT_SHOW_STEERING_SPEED),
			mShowCollisionFlags(false),
			mShowSteeringVelocities(DEFAULT_SHOW_STEERING_VELOCITIES),
			mShowSteeringReservations(DEFAULT_SHOW_STEERING_RESERVATIONS),
			mShowReservationCollisions(DEFAULT_SHOW_STEERING_RESERVATION_COLLISIONS),
			mShowPhysicalCollisions(DEFAULT_SHOW_STEERING_PHYSICAL_COLLISIONS),
			mShowForAllEntities(DEFAULT_SHOWN_FOR_ALL_ENTITIES),
			mSimulateDebuggedEntitiesOnly(SIMULATE_DEBUGGED_ONLY.mDefault),
			mShowCrossedPortals(SHOW_CROSSED_PORTALS.mDefault),
			mShowPathfindingResults(SHOW_PATHFINDING_RESULTS.mDefault),
			mShowTightLanes(SHOW_TIGHT_LANES.mDefault),
			mShowDynamicObstacleState(SHOW_DYNAMIC_OBSTACLES_STATE.mDefault),
			mShowStuckResolvingState(SHOW_STUCK_RESOLVING.mDefault),
			mShowDynamicEvasionState(SHOW_DYNAMIC_EVASION.mDefault),
			mShowGoalPoints(SHOW_GOAL_POINTS.mDefault)
		{}

		inline void NavigationDebugGroup::showEntityPaths(bool enable)
		{
			assignAndPromoteChange(mShowEntityPaths, enable, SHOW_ENTITY_PATH.mId);
		}

		inline bool NavigationDebugGroup::areEntityPathsShown() const
		{
			return mShowEntityPaths;
		}

		inline void NavigationDebugGroup::showEntityPathSteps(bool enable)
		{
			assignAndPromoteChange(mShowEntityPathSteps, enable, SHOW_ENTITY_PATH_STEPS.mId);
		}

		inline bool NavigationDebugGroup::areEntityPathStepsShown() const
		{
			return mShowEntityPathSteps;
		}

		inline void NavigationDebugGroup::setDebuggedNavigationMap(unsigned int mapId)
		{
			assignAndPromoteChange(mDebuggedNavigationMap, mapId, NAVIGATION_MAP_ID);
		}

		inline void NavigationDebugGroup::disableNavigationMapDebugging()
		{
			setUninitialized(mDebuggedNavigationMap);
		}

		inline bool NavigationDebugGroup::isNavigationMapDebugged() const
		{
			return isUninitialized(mDebuggedNavigationMap);
		}

		inline unsigned int NavigationDebugGroup::getIdOfDebuggedNavigationMap() const
		{
			return mDebuggedNavigationMap;
		}

		inline void NavigationDebugGroup::showMapAreaOutlines(bool enable)
		{
			assignAndPromoteChange(mShowMapAreaOutlines, enable, SHOW_NAVIGATION_AREAS_SETTING);
		}

		inline bool NavigationDebugGroup::areMapAreaOutlinesShown() const
		{
			return mShowMapAreaOutlines;
		}

		inline void NavigationDebugGroup::showMapPaths(bool enable)
		{
			assignAndPromoteChange(mShowMapPaths, enable, SHOW_MAP_PATH_SETTING);
		}

		inline bool NavigationDebugGroup::areMapPathsShown() const
		{
			return mShowMapPaths;
		}

		inline void NavigationDebugGroup::setConnectedNavigationMap(unsigned int mapId)
		{
			assignAndPromoteChange(mConnectedNavigationMap, mapId, CONNECTED_NAVIGATION_MAP);
		}

		inline unsigned int NavigationDebugGroup::getIdOfConnectedNavigationMap() const
		{
			return mConnectedNavigationMap;
		}

		inline void NavigationDebugGroup::setConnectionId(unsigned int connection)
		{
			assignAndPromoteChange(mConnectionId, connection, CONNECTION_TO_SHOW);
		}

		inline unsigned int NavigationDebugGroup::getConnectionId() const
		{
			return mConnectionId;
		}

		inline bool NavigationDebugGroup::areNodeStatesShown() const
		{
			return mShowNodeStates;
		}

		inline void NavigationDebugGroup::showNodeStates(bool enable)
		{
			assignAndPromoteChange(mShowNodeStates, enable, SHOW_NODE_STATE);
		}

		inline void NavigationDebugGroup::showSearchStatistics(bool enable)
		{
			assignAndPromoteChange(mShowSearchStatistics, enable, SHOW_SEARCH_STATISTICS);
		}

		inline bool NavigationDebugGroup::areSearchStatisticsShown() const
		{
			return mShowSearchStatistics;
		}

		inline void NavigationDebugGroup::showSteeringSpeed(bool enable)
		{
			assignAndPromoteChange(mShowSteeringSpeed, enable, SHOW_STEERING_SPEED);
		}

		inline bool NavigationDebugGroup::isSteeringSpeedShown() const
		{
			return mShowSteeringSpeed;
		}

		inline void NavigationDebugGroup::showCollisionFlags(bool enable)
		{
			assignAndPromoteChange(mShowCollisionFlags, enable, SHOW_COLLISION_FLAGS);
		}

		inline bool NavigationDebugGroup::areCollisionFlagsShown() const
		{
			return mShowCollisionFlags;
		}

		inline void NavigationDebugGroup::showGoalPoints(bool enable)
		{
			assignAndPromoteChange(mShowGoalPoints, enable, SHOW_GOAL_POINTS.mId);
		}

		inline bool NavigationDebugGroup::areGoalPointsShown() const
		{
			return mShowGoalPoints;
		}

		inline void NavigationDebugGroup::showSteeringVelocities(bool enable)
		{
			assignAndPromoteChange(mShowSteeringVelocities, enable, SHOW_STEERING_VELOCITIES);
		}

		inline bool NavigationDebugGroup::areSteeringVelocitiesShown() const
		{
			return mShowSteeringVelocities;
		}

		inline void NavigationDebugGroup::showReservations(bool enable)
		{
			mShowSteeringReservations = enable;
		}

		inline bool NavigationDebugGroup::areReservationsShown() const
		{
			return mShowSteeringReservations;
		}

		inline void NavigationDebugGroup::showReservationCollisions(bool enable)
		{
			mShowReservationCollisions = enable;
		}

		inline bool NavigationDebugGroup::areReservationCollisionsShown() const
		{
			return mShowReservationCollisions;
		}

		inline void NavigationDebugGroup::showPhysicalCollisions(bool enable)
		{
			mShowPhysicalCollisions = enable;
		}

		inline bool NavigationDebugGroup::arePhysicalCollisionsShown() const
		{
			return mShowPhysicalCollisions;
		}

		inline void NavigationDebugGroup::setDrawDebugForAllEntities(bool enable)
		{
			mShowForAllEntities = enable;
		}

		inline bool NavigationDebugGroup::isDebugForAllEntitiesDrawn() const
		{
			return mShowForAllEntities;
		}

		inline void NavigationDebugGroup::setSimulateDebuggedEntitiesOnly(bool enable)
		{
			mSimulateDebuggedEntitiesOnly = enable;
		}

		inline bool NavigationDebugGroup::areOnlyDebuggedEntitiesSimulated() const
		{
			return mSimulateDebuggedEntitiesOnly;
		}

		inline void NavigationDebugGroup::showCrossedPortals(bool enable)
		{
			assignAndPromoteChange(mShowCrossedPortals, enable, SHOW_CROSSED_PORTALS.mId);
		}

		inline bool NavigationDebugGroup::areCrossedPortalsShown() const
		{
			return mShowCrossedPortals;
		}

		inline void NavigationDebugGroup::setLaneChangeOptionToShow(unsigned int id)
		{
			assignAndPromoteChange(mLaneChangeOptionId, id, SHOW_LANE_CHANGE_OPTION.mId);
		}

		inline unsigned int NavigationDebugGroup::getLaneChangeOptionToShow() const
		{
			return mLaneChangeOptionId;
		}

		inline void NavigationDebugGroup::showPathfindingResults(bool enable)
		{
			assignAndPromoteChange(mShowPathfindingResults, enable, SHOW_PATHFINDING_RESULTS.mId);
		}

		inline bool NavigationDebugGroup::arePathfindingResultsShown() const
		{
			return mShowPathfindingResults;
		}

		inline void NavigationDebugGroup::showTightLanes(bool enable)
		{
			assignAndPromoteChange(mShowTightLanes, enable, SHOW_TIGHT_LANES.mId);
		}

		inline bool NavigationDebugGroup::areTightLanesShown() const
		{
			return mShowTightLanes;
		}

		inline void NavigationDebugGroup::showDynamicObstacleState(bool enable)
		{
			assignAndPromoteChange(mShowDynamicObstacleState, enable, SHOW_DYNAMIC_OBSTACLES_STATE.mId);
		}

		inline bool NavigationDebugGroup::areDynamicObstacleStatesShown() const
		{
			return mShowDynamicObstacleState;
		}

		inline void NavigationDebugGroup::showStuckResolvingState(bool enable)
		{
			assignAndPromoteChange(mShowStuckResolvingState, enable, SHOW_STUCK_RESOLVING.mId);
		}

		inline bool NavigationDebugGroup::areStuckResolvingStatesShown() const
		{
			return mShowStuckResolvingState;
		}

		inline void NavigationDebugGroup::showDynamicEvasionState(bool enable)
		{
			assignAndPromoteChange(mShowDynamicEvasionState, enable, SHOW_DYNAMIC_EVASION.mId);
		}

		inline bool NavigationDebugGroup::isDynamicEvasionStateShown() const
		{
			return mShowDynamicEvasionState;
		}
	}
}
