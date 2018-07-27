// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/reflection/ExportedProperty.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/debug/group/DebugGroup.h>

namespace qsf
{
	namespace ai
	{
		/**
		* Encapsulates the configurations for all available debugging options inside AI navigation systems.
		* Noncopyable by inheritance.
		*/
		class QSF_AI_API_EXPORT NavigationDebugGroup : public DebugGroup, public UniqueInstance<NavigationDebugGroup>
		{
		public:
			// Unique CAMP Ids and initial values
			//@{
			QSF_DECLARE_CAMP_PLUGINABLE_ID;
			static const ExportedProperty<bool> SHOW_ENTITY_PATH; // Flag whether entities current paths should be shown
			static const ExportedProperty<bool> SHOW_ENTITY_PATH_STEPS; // Flag whether entities current path steps should be shown

			static const unsigned int NAVIGATION_MAP_ID; // The ID of the navigation map to debug
			static const unsigned int SHOW_NAVIGATION_AREAS_SETTING; // Boolean value whether the area outlines should be shown
			static const unsigned int SHOW_MAP_PATH_SETTING; // Boolean value whether the ideal paths should be shown
			static const unsigned int CONNECTED_NAVIGATION_MAP; // The optional id of a navigation map for which to show the connections.
			static const unsigned int CONNECTION_TO_SHOW; // The id of a connection to highlight. This value is only interpreted if current and connected navigation map hold legal values
			static const unsigned int SHOW_NODE_STATE; // Boolean value whether the dynamic states of the nodes should be shown
			static const unsigned int SHOW_SEARCH_STATISTICS; // Boolean value whether the statistics about finished path searches are shown
			static const unsigned int SHOW_STEERING_SPEED; // Boolean value whether the current speed should be shown as text output
			static const unsigned int SHOW_COLLISION_FLAGS; //Boolean value whether the collision flags of each navigation entity should be shown, if any
			static const unsigned int SHOW_STEERING_VELOCITIES; // Boolean whether planned velocities are plotted as a height profile ever the next waypoints. also gives braking reasons.
			static const unsigned int SHOW_STEERING_ARRIVAL_TIMES; // Boolean value whether to show projected steering arrival times for each waypoint.
			static const unsigned int SHOW_FOR_ALL_ENTITIES; // Flag to force showing the debug output for all entities. Otherwise only the component debug state that is triggered on selection activates it

			static const unsigned int DEFAULT_NAVIGATION_MAP; // Set to zero as it is assumed that this world is the most important
			static const unsigned int DEFAULT_CONNECTION_TO_SHOW; // Uninitialized to avoid that this specific option is triggered accidentally together with other options
			static const bool DEFAULT_SHOW_MAP_PATH_SETTING;
			static const bool DEFAULT_SHOW_MAP_AREAS_SETTING;
			static const bool DEFAULT_SHOW_NODE_STATE_SETTING;
			static const bool DEFAULT_SHOW_SEARCH_STATISTICS;
			static const bool DEFAULT_SHOW_STEERING_SPEED;
			static const bool DEFAULT_SHOW_COLLISION_FLAGS;
			static const bool DEFAULT_SHOW_STEERING_VELOCITIES;
			static const bool DEFAULT_SHOW_STEERING_RESERVATIONS;
			static const bool DEFAULT_SHOW_STEERING_RESERVATION_COLLISIONS;
			static const bool DEFAULT_SHOW_STEERING_PHYSICAL_COLLISIONS;
			static const bool DEFAULT_SHOWN_FOR_ALL_ENTITIES;

			static const ExportedProperty<bool> SIMULATE_DEBUGGED_ONLY; // Flag to limit the simulation of entities to only the ones that are currently debugged - by default only the selected entity
			static const ExportedProperty<bool> SHOW_CROSSED_PORTALS; // Flag to show the portals used during pathfinding
			static const ExportedProperty<unsigned int> SHOW_LANE_CHANGE_OPTION; // Id of a lane change option to show
			static const ExportedProperty<bool> SHOW_PATHFINDING_RESULTS; // Flag to give a short feedback about any pathfinding attempt
			static const ExportedProperty<bool> SHOW_TIGHT_LANES; // Flag to debug draw lanes that are to tight for an entity to pass on their first pathfinding attempt.
			static const ExportedProperty<bool> SHOW_DYNAMIC_OBSTACLES_STATE; // Flag whether the state of a dynamic obstacle is shown (actually static or dynamic)
			static const ExportedProperty<bool> SHOW_STUCK_RESOLVING; // Flag to show state changes of the stuck resolving system
			static const ExportedProperty<bool> SHOW_DYNAMIC_EVASION; // Flag to show the state during dynamic evasion
			static const ExportedProperty<bool> SHOW_GOAL_POINTS; // Draw circles around and lines to the goal points
			//@}

			// Constructor required by DebugGroup interface
			NavigationDebugGroup(DebugGroupManager* dgm);

			// Access to show entity path and steering times settings
			//@{
			void showEntityPaths(bool enable);
			bool areEntityPathsShown() const;

			void showEntityPathSteps(bool enable);
			bool areEntityPathStepsShown() const;

			void showSteeringSpeed(bool enable);
			bool isSteeringSpeedShown() const;

			void showReservationCollisions(bool enable);
			bool areReservationCollisionsShown() const;

			void showPhysicalCollisions(bool enable);
			bool arePhysicalCollisionsShown() const;

			void showSteeringVelocities(bool enable);
			bool areSteeringVelocitiesShown() const;

			void showReservations(bool enable);
			bool areReservationsShown() const;

			void showPathfindingResults(bool enable);
			bool arePathfindingResultsShown() const;
			//@}

			// Access to show collision flags for an attached collision component, if any
			//@{
			bool areCollisionFlagsShown() const;
			void showCollisionFlags(bool enable);
			//@}

			// Access to show goal points
			//@{
			void showGoalPoints(bool enable);
			bool areGoalPointsShown() const;
			//@}

			void setDrawDebugForAllEntities(bool enable);
			bool isDebugForAllEntitiesDrawn() const;

			// Access to the setting which navigation map is being debugged
			//@{
			void setDebuggedNavigationMap(unsigned int mapId); // logs a warning if the id is not a legal world id except if it is an uninitialized value
			void disableNavigationMapDebugging(); // shortcut for calling setDebuggedNavigationMap with Uninitialized<...> parameter
			bool isNavigationMapDebugged() const;
			unsigned int getIdOfDebuggedNavigationMap() const;
			//@}

			// Access to the specific navigation map debugging options
			//@{
			void showMapAreaOutlines(bool enable);
			bool areMapAreaOutlinesShown() const;

			void showMapPaths(bool enable);
			bool areMapPathsShown() const;

			void showNodeStates(bool enable);
			bool areNodeStatesShown() const;

			void setLaneChangeOptionToShow(unsigned int id);
			unsigned int getLaneChangeOptionToShow() const;

			void showTightLanes(bool enable);
			bool areTightLanesShown() const;

			void showDynamicObstacleState(bool enable);
			bool areDynamicObstacleStatesShown() const;
			//@}

			void setSimulateDebuggedEntitiesOnly(bool enable);
			bool areOnlyDebuggedEntitiesSimulated() const;

			void showCrossedPortals(bool enable);
			bool areCrossedPortalsShown() const;

			// Access to the setting for showing connections to a different map.
			// If the value is the same as the primary map or illegal nothing is highlighted.
			//@{
			void setConnectedNavigationMap(unsigned int mapId);
			void setConnectionId(unsigned int connection);
			unsigned int getIdOfConnectedNavigationMap() const;
			unsigned int getConnectionId() const;
			//@}

			void showSearchStatistics(bool enable);
			bool areSearchStatisticsShown() const;

			void showStuckResolvingState(bool enable);
			bool areStuckResolvingStatesShown() const;

			void showDynamicEvasionState(bool enable);
			bool isDynamicEvasionStateShown() const;

		private:
			unsigned int mDebuggedNavigationMap; // Contains the id of the world state to visualize, Uninitialized<...> if not set
			unsigned int mConnectedNavigationMap; // May contain a legal map id to indicate that the connection between these two is debugged
			unsigned int mConnectionId; // May contain a legal connection id to indicate that this connection should be highlighted
			unsigned int mLaneChangeOptionId; // Uninitialized by default means no connection to show
			bool mShowEntityPaths;
			bool mShowEntityPathSteps;
			bool mShowMapAreaOutlines;
			bool mShowMapPaths;
			bool mShowNodeStates;
			bool mShowSearchStatistics;
			bool mShowSteeringSpeed;
			bool mShowCollisionFlags;
			bool mShowSteeringVelocities;
			bool mShowSteeringReservations;
			bool mShowReservationCollisions;
			bool mShowPhysicalCollisions;
			bool mShowForAllEntities;
			bool mSimulateDebuggedEntitiesOnly;
			bool mShowCrossedPortals;
			bool mShowPathfindingResults;
			bool mShowTightLanes;
			bool mShowDynamicObstacleState;
			bool mShowStuckResolvingState;
			bool mShowDynamicEvasionState;
			bool mShowGoalPoints;

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::NavigationDebugGroup);

#include "qsf_ai/navigation/NavigationDebugGroup-inl.h"
