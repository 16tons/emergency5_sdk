// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/base/NormalizedVector.h"
#include "qsf_ai/navigation/steering/ApproachedSpecialStateCallback.h"
#include "qsf_ai/navigation/steering/CollisionCallback.h"
#include "qsf_ai/navigation/steering/SteeringControlPoint.h"
#include "qsf_ai/navigation/steering/PathLookaheadResult.h"
#include "qsf_ai/navigation/steering/NavigationComponentCollision.h"
#include "qsf_ai/navigation/pathfinding/localPlanning/DynamicCollisionLocalPlanner.h"

#include <qsf/reflection/CampDefines.h>

#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MovableComponent;
	class FastDebugDraw;

	namespace ai
	{
		class NavigationComponent;
		class NavigationDebugGroup;
		class WorldElementStateCollection;
		class Waypoint;
		class Path;
		class ReservationSystem;
		class SpeedLimitFunctor;
		class WorldModel;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/** AI system to let entities move according to their navigation goals and planned paths.
		* Turning change rate is only handled correctly during curve segments (that means two consecutive path waypoints are on a circle).
		* This should be enough if the path is always correct and there is no internal miscalculation.
		* Non copyable by inheritance because Job as well as System are noncopyable.
		*/
		class QSF_AI_API_EXPORT SteeringSystem : public StandardSystem<NavigationComponent, SteeringSystem>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier of the system
			static const char* NAME;

			SteeringSystem(Prototype* prototype);

			// Public template interface requirement of qsf::ai::StandardSystem
			//@{
			effort::Indicator updateComponent(NavigationComponent& navi, const JobArguments& arguments);
			void createDebugOutput(const NavigationComponent& navi) const;
			void updateGlobals(const JobArguments&);
			//@}

			// public System implementation override
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;
			virtual void onSetDebug(bool debug) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Needs to be managed on the outside, the steering system does not take ownership.
			// Reset by calling with a nullptr parameter.
			void setApproachedSpecialStateHandler(ApproachedSpecialStateCallback* handler);

			// Needs to be managed on the outside, the steering system does not take ownership.
			// Reset by calling with a nullptr parameter.
			void setCollisionHandler(CollisionCallback* handler);

			// Needs to be managed on the outside, steering system does not take ownership.
			// Reset by calling with a nullptr parameter.
			void setSpeedLimitFunctor(const SpeedLimitFunctor* speedLimit);

			// returns the entity orientation as facing vector from the movable orientation quaternion, is never ZERO
			static glm::vec3 getMovementDirectionVector(const NavigationComponent& navi, const MovableComponent& movable);

			// Tell the Steering which entity is selected for better debug
			void onEntitySelected(uint64 entityId);
			void onEntityDeselected(uint64 entityId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Internal helper function, called by updateComponent. Around this method, exceptions are caught and reservations are handled in this case.
			effort::Indicator updateSteeringSystem(NavigationComponent& navi, const JobArguments& arguments);

			// Handles initial removal of reservations and - if no reservations made - adding a reservation for the current area id (if any)
			void removeReservationsIfNeeded(NavigationComponent& navi);
			void reserveCurrentAreaIfNeeded(NavigationComponent& navi, Time now);

			// Stops an entity during this update, so next update it will have velocity 0, updating movable acceleration and velocity
			// Currently also adding a randomized update pause here
			effort::Indicator comeToHalt(NavigationComponent& navi, MovableComponent& movable, Time now);

			// Stops an entity during this update, so next update it will have velocity 0, updating movable acceleration and velocity. called by comeToHalt, does not add random waiting time
			effort::Indicator doHalt(NavigationComponent& navi, MovableComponent& movable);

			// sets a movable position, making sure the position is valid (not containing NaN)
			static void moveToPosition(MovableComponent& movable, const glm::vec3& position);

			// Combination of "moveToPosition()" and "rotateToOrientation()", use this instead invidual calls when possible (performance)
			static void moveToPositionAndRotateToOrientation(MovableComponent& movable, const glm::vec3& position, glm::vec3 direction);

			// updates movable velocity and acceleration, checking both for invalid values.
			void updateVelocity(MovableComponent& movable, const glm::vec3& newVelocity);

			// iterates a steering path, erasing all segments after the first blocked node, which is annotated DEMOTE_HALTING_POINT. does nothing if no such point.
			static void eraseAfterFirstBlocked(std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints);

			// calculates the speed portion pointing in the given direction (performs dot product, but normalizes direction if needed)
			static float calculateDirectionalSpeed(const glm::vec3& velocity, const Normalized<glm::vec3>& direction);

			// Sanity check function to see whether a position is still on the ideal path.
			// Writes the ideal rotation the entity should have at this moment if it can be uniquely derived by the current configuration
			static bool isOnIdealPath(const glm::vec3& pos, const Path& path, glm::vec3& idealRotation);

			// reserves the predefined waiting time while standing still.
			void reserveWhileStandingStill(NavigationComponent& navi, Time now) const;

			// Encapsulates a steering/movement desire when not having a path
			effort::Indicator steerLocally(NavigationComponent& navi, MovableComponent& movable, Time now);

			static worldElement::State getAreaBlockedState(const WorldElementStateCollection* elementsInfo, const Waypoint& waypoint);
			static worldElement::State getNodeBlockedState(const WorldElementStateCollection* elementsInfo, const Waypoint& waypoint);
			static const WorldElementStateCollection* getCorrespondingWorldElementsState(const WorldModel& primaryWorld, const WorldModel* secondaryWorld, const Waypoint& waypoint);

			// Takes into account current speed, size of the navigation component and it's other physical limitations
			static Time calculateNecessaryLookaheadTime(UnsignedFloat currentSpeed, const NavigationComponent& navi);

			// Calculates the desired speed at each waypoint in the path
			// The navigation component is not const because part of this function is reserving the areas and as an optimization step also storing which areas have been reserved with the entity
			void calculateDesiredSpeedForPath(NavigationComponent& navi, const MovableComponent& movable, const SpatialConfiguration3D& currentConfig,
				Time maxPathLookahead, Time now, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, const WorldModel& primaryWorld, const WorldModel* secondaryWorld, const std::unordered_set<Entity*>& persistentCollisions);

			// moves the braking point back, as far as needed so that the entity actually stops *before* the point, not on it.
			static void demoteBrakingPoint(std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, steering::PathLookaheadResult& lookahead, UnsignedFloat keepDistanceAhead);

			// makes a steering control point a halting point and recalculates needed braking force
			static void makeStoppingPoint(steering::SteeringControlPoint& waypointInfo, steering::PathLookaheadResult& lookahead, UnsignedFloat initialSpeed);

			// Forward loop over the Path, assuming maximal acceleration until max speed in any segment. tests for blocked nodes and sets their velocity to 0. expects desiredSpeedsForWaypoints to be empty.
			steering::PathLookaheadResult calculateMaximalSpeedsForPath(const Path& path, NavigationComponent& navi, const MovableComponent& movable, const SpatialConfiguration3D& startConfig,
				std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, Time maxPathLookahead, const WorldModel& primaryWorld, const WorldModel* secondaryWorld) const;

			// Backwards loop over desiredSpeedsForWaypoints, flattening all braking maneuvers such that they are possible with lookahead.mMaxAcceleration
			void calculateOptimalSpeedsForPath(const NavigationComponent& navi, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, const steering::PathLookaheadResult& lookahead) const;

			// Checks the entity for collisions with other entities in its current state. Returns a special reaction enum telling how to react.
			CollisionCallback::Reaction checkForCurrentCollisions(NavigationComponent& navi, std::unordered_set<Entity*>& persistentCollisions) const;

			// uses dynamical collision system fed with physical data to find other physics components around, performing Action on them.
			template <typename Action>
			void forAllRouterCollisions(const MovableComponent& movable, const NavigationComponent& navi, bool isMovingForward, Action& action) const;

			// uses bullet collision system fed with physical data to find other physics components around, performing Action on them.
			template <typename Action>
			void forAllBulletCollisions(const MovableComponent& movable, const NavigationComponent& navi, bool isMovingForward, Action& action) const;

			// tries to reserve all needed reservations set in desiredSpeedsForWaypoints, extending duration by the time needed to enter or leave an area (or timeWhileStandingStill, if v=0). returns how far ahead reservation was possible
			// This method also writes the correct reservation times into the steering path as a side effect
			steering::PathLookaheadResult tryReserveAreas(const NavigationComponent& navi, const WorldModel& primaryWorld, const WorldModel* secondaryWorld, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, Time now, boost::optional<ReservationConflictResolver::ContainerType::const_iterator>& blockingReservation) const;

			// inserts all reservations in desiredSpeedsForWaypoints, ignoring collisions - make sure those are handled first!
			// The waypointIndex reference parameter holds the index into the original path which was the last one to be reserved. It is incremented for everycontrolPoint tagged "selectNexWP" and should be initialized to the current WP index
			void insertReservations(const std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, unsigned int& lastReservedWPIndex) const;

			// loops desiredSpeedsForWaypoints' reservations for concurrent reservations, checks for potential collisions, also queries forAllPhysicalCollisions(), returns the NavigationComponentCollision that was most critical (see operator <) - if any
			// The most critical so far acts as an upper limit. Everything that is less or only equally critical is ignored.
			boost::optional<NavigationComponentCollision> hasCollisionsOnPath(const NavigationComponent& navi, const SpatialConfiguration3D& currentConfig, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, const boost::optional<NavigationComponentCollision>& mostCriticalCollisionSoFar, const std::unordered_set<Entity*>& persistentCollisions) const;

			// Inserts a virtual waypoint at the position where NavigationComponentCollision is happening, reducing the speed to match the other's speed. removes path behind collision if v=0.
			// In case we plan to use the local planner afterwards to move around the collision, we need to make sure that any halting position we determine is collision free given the local planner.
			// This can be requested via the flag forceCollisionFreeHaltingPosition. This flag has no effect in case the entity only needs to slow down, not come to a halt completely
			// Returns whether such a points needs to be inserted at all. In case the collision is behind the planned movement, no change is actually done at all
			bool handleCollisionByBraking(const NavigationComponent& navi, const MovableComponent& movable, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints,
				const NavigationComponentCollision& collision, bool forceCollisionFreeHaltingPosition) const;

			// possible results for handling found collisions
			enum CollisionHandlingResult
			{
				MOVE, // continue moving, perhaps with reduced speed as defined by the remaining planned control points
				STOP, // come to a halt
				INSTANT_REPLAN, // plan was changed to evade a collision
				PLAN_ROUTE_AROUND_TARGET, // try to plan a route around the target in the background but treat the current obstacle like a stop situation until a solution is found
			};

			// repeatedly and alternatively tries to reserve all areas (removing bits of the path at the end if impossible) and checking for collisions. returns empty desiredSpeedsForWaypoints if no way could be found to brake out of the collision.
			// The distance to closest collision is only written in case there was a collision at all
			CollisionHandlingResult handleReservationsAndCollisions(NavigationComponent& navi, const MovableComponent& movable, const SpatialConfiguration3D& currentConfig,
				const WorldModel& primaryWorld, const WorldModel* secondaryWorld,
				std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints, Time now, const std::unordered_set<Entity*>& persistentCollisions, float& distanceToClosestCollision);

			// the method removes virtual points so that they can be re-inserted later on. see also recalculateMaximalSpeedsForPath() why this is needed.
			void eraseAllIntermediateVirtualPoints(std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints);

			// during handleReservationsAndCollisions(), speed limits are changed (velocity matching etc), thus, virtual points have to recalculated. repeats the acceleration phase, inserting a speedup point if needed. no more checking for blocked areas or area speed limits, as this information is still preserved in the steering path.
			steering::PathLookaheadResult recalculateMaximalSpeedsForPath(const NavigationComponent& navi, std::vector<steering::SteeringControlPoint>& desiredSpeedsForWaypoints) const;

			// request the local router to adapt a path around a collision found
			void requestLocalRouterAdaptation(NavigationComponent& navi, const SpatialConfiguration3D& plannedEvasionStart, float distanceToClosestCollision);

			// Debug output sub functions
			static bool shouldDrawDebug(const NavigationDebugGroup* debug, const NavigationComponent& navi);
			void drawPersistentDebugInfo(const NavigationComponent& navi);
			void drawPathDebug(const NavigationComponent& navi) const;
			void drawSteeringSpeed(const MovableComponent& movable) const;
			void drawCollisionFlags(const MovableComponent& movable, const NavigationComponent& navi) const;
			void drawGoalPoints(const MovableComponent& movable, const NavigationComponent& navi) const;
			void drawReservations(const std::vector<steering::SteeringControlPoint>& steeringControlPoints, Time absoluteNow) const;
			void drawPlannedSpeeds(const std::vector<steering::SteeringControlPoint>& steeringControlPoints, const Color4& color) const;

			// Uses the settings for randomized wait times to create a time duration for which to wait until becoming active again
			Time getRandomWaitTime(const NavigationComponent& navi, Time now) const;

			void logMemoryConsumption() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			NavigationDebugGroup* mDebugSettings; // pointer to the debug settings, may be null meaning no debugging output should be generated
			WeakPtr<ReservationSystem> mReservationSystem;	// Direct component references for efficient access
			WeakPtr<DynamicCollisionLocalPlanner> mDynamicCollisionLocalPlaner;	// Direct component references for efficient access

			FastDebugDraw* mFastDebugDraw;
			boost::container::flat_set<uint64> mSelectedEntities;

			// Optional reaction to approaching a node or area with a special state. Maybe a nullptr.
			ApproachedSpecialStateCallback* mApproachedSpecialStateHandler;

			// Functors that allow tweaking the steering core mechanics in an application specific manner
			CollisionCallback* mCollisionCallback;
			const SpeedLimitFunctor* mSpeedLimits;

			//volatile data, only used while running.
			Time mLastUpdateDuration;

			//volatile data, only used while running.
			std::vector<steering::SteeringControlPoint> mDesiredSpeedsForWaypoints;

			//volatile data, only used while running to copy over mDesiredSpeedsForWaypoints.
			std::vector<steering::SteeringControlPoint> mVectorSwapTarget;

			QSF_CAMP_RTTI();
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/steering/SteeringSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SteeringSystem);
