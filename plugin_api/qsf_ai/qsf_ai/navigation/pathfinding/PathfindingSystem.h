// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/MovementMode.h"

#include <qsf/logic/ProcessState.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/base/WeakPtr.h>

#include <boost/container/flat_map.hpp>

#include <vector>

namespace qsf
{
	class FastDebugDraw;
	namespace logic
	{
		class TargetPointManager;
	}

	namespace ai
	{
		class PathSearchConfiguration;
		class NavigationDebugGroup;
		class NavigationTaskThread;
		class DynamicCollisionLocalPlanner;
		class TrafficLaneWorld;

		/** AI system to plan paths for navigation.
		* There are actually two modes to interact with it.
		* You can manually trigger the search, query the state and retrieve the results through this interface.
		* The actual calculations will be done in the background and through the systems update phase.
		* The alternative is that entities who register their navigation component get an automated treatment that can be triggered by scheduling MoveTo actions.
		* It accesses the world model manager instance who provides the world abstractions to plan paths on.
		*
		* Non copyable by inheritance.
		*/
		class QSF_AI_API_EXPORT PathfindingSystem : public StandardSystem<NavigationComponent, PathfindingSystem>
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Exported CAMP properties
			//@{
			static const unsigned int TURNING_RADIUS_VIOLATION_COST_PENALTY_PROPERTY_ID;
			static const unsigned int MANEUVER_COST_PENALTY_PROPERTY_ID;
			static const unsigned int MOVING_BACKWARDS_COST_PENALTY_PROPERTY_ID;
			static const unsigned int MAX_MANEUVER_STEPS_PROPERTY_ID;

			static const UnsignedFloat DEFAULT_TURNING_RADIUS_VIOLATION_COST_PENALTY;
			static const UnsignedFloat DEFAULT_MANEUVER_COST_PENALTY;
			static const UnsignedFloat DEFAULT_MOVING_BACKWARDS_COST_PENALTY;
			static const unsigned int DEFAULT_MAX_MANEUVER_STEPS;

			// Path search cost penalty for moving through illegal territory if allowed by the position correction settings.
			// Used as a modifier to the distance as well as a one time amount if the distance is not available
			static const ExportedProperty<UnsignedFloat> MOVE_THROUGH_ILLEGAL_SPACE_PENALTY;
			// The minimal length of a straight lane segments that is considered for a lane change.
			static const ExportedProperty<UnsignedFloat> MIN_LENGTH_FOR_LANE_CHANGE;
			// Search cost penalty added for each time we transition between the maps
			static const ExportedProperty<UnsignedFloat> TRANSITION_PENALTY;
			// A guesstimate for the maximum diameter that a single crossing may have in your application.
			// Is used in several advanced algorithms to see when we can safely leave a lane and can still be considered on the same crossing.
			static const ExportedProperty<UnsignedFloat> MAX_CROSSING_DIAMETER;
			//@}


			// Plain text identifier
			static const char* NAME;

			PathfindingSystem(Prototype* prototype);

			// Overridden Component interface
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetActive(bool active) override;
			virtual void onSetDebug(bool debug) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Overridden StandardSystem interface
			//@{
			virtual void updateDebug(const JobArguments& jobArguments) override;
			//@}

			// Required template interface of the StandardSystem
			//@{
			effort::Indicator updateComponent(NavigationComponent& navi, const JobArguments& arguments);
			void createDebugOutput(const NavigationComponent& navi) const;
			void updateGlobals(const JobArguments&);
			//@}

			void addMovementMode(const MovementMode& mode);
			const MovementMode& getMovementMode(unsigned int id) const;

			// Cost penalty applied during path finding for violating the turning radius.
			// Applied once for every lane transition or lane where the condition is violated.
			//@{
			void setTurningRadiusViolationPenalty(UnsignedFloat oneTimePenalty);
			UnsignedFloat getTurningRadiusViolationCostPenalty() const;
			//@}

			// Cost penalty applied during path finding for maneuvering.
			// Applied once for every maneuver step.
			//@{
			void setManeuverCostPenalty(UnsignedFloat oneTimePenalty);
			UnsignedFloat getManeuverCostPenalty() const;
			//@}

			// Cost penalty applied during path finding for moving backwards.
			// Applied as a factor for each world unit moved backwards.
			//@{
			void setMovingBackwardsCostPenalty(UnsignedFloat factor);
			UnsignedFloat getMovingBackwardsCostPenalty() const;
			//@}

			// Get and set the minimal segment length for considering a lane change
			//@{
			void setMinLengthForLaneChange(UnsignedFloat length);
			UnsignedFloat getMinLengthForLaneChange() const;
			//@}

			void setMaxCrossingDiameter(UnsignedFloat diameter);
			UnsignedFloat getMaxCrossingDiameter() const;

			// Set the maximum number of steps during a single maneuvering action to reach a desired orientation.
			// If a maneuver would require more steps it is not considered a valid move.
			//@{
			void setMaxManeuverSteps(unsigned int steps);
			unsigned int getMaxManeuverSteps() const;
			//@}

			void setMoveThroughIllegalSpacePenalty(UnsignedFloat penalty);
			UnsignedFloat getMoveThrougIllegalSpacePenalty() const;

			void setTransitionPenalty(UnsignedFloat penalty);
			UnsignedFloat getTransitionPenalty() const;

			// The path finding system needs to know about the target point manager to derive the exact goal positions when moving to interact with a specific target point.
			// This needs to be set during startup before any path finding request is managed.
			// The target manager is stored internally and need to be kept alive for the whole simulation time.
			void setTargetPointManager(const logic::TargetPointManager& targetPointManager);
			// This might return a nullptr if no target point provider was set
			const logic::TargetPointManager* tryGetTargetPointManager() const;

			//[-------------------------------------------------------]
			//[ Debug ("qsf::ai::PathfindingSystem" must be in debug mode to see something) ]
			//[-------------------------------------------------------]
			// Set a trafficLaneWorld / pathfinding map to show its debug lanes
			void showDebugTrafficLaneWorld(const TrafficLaneWorld& trafficLaneWorld);

			// Switch to the next ai map
			void nextTrafficLaneDebug();
			void setTrafficLaneDebug(int trafficLaneId);

			// Show the traffic lane area instead of just a line
			void showDebugTrafficLaneArea(bool showArea);

		private:
			void debugPrintSearchStatistics() const;

			// see whether there is a path adaptation planned by the local planner and integrate it into a running path
			bool tryIntegrateLocalPlannerResult(NavigationComponent& navi);

			// Interrupt running tasks to give other tasks a change
			void updateTaskInterrupt(const JobArguments&);

			// The known movement modes mapped from their unique id
			typedef boost::container::flat_map<unsigned int, MovementMode> MovementModes;
			MovementModes mMovementModes;

			// TaskThread control
			Time	 mInterruptTime;
			JobProxy mUpdateTaskInterruptJobProxy;

			// Optional pointer to the debug settings
			NavigationDebugGroup* mDebugSettings;
			// Optional pointer to the target point manager
			const logic::TargetPointManager* mTargePointManager;

			FastDebugDraw* mFastDebugDraw;

			// TrafficLaneWorld we want to show; may be -1 for none
			int mDebugTrafficLaneWorldId;

			// Do we want to show the path area
			bool mDebugShowArea;

			// Cost penalty settings
			UnsignedFloat mTurningRadiusViolationCostPenalty;
			UnsignedFloat mManeuverCostPenalty;
			UnsignedFloat mMovingBackwardsCostPenalty;
			UnsignedFloat mMoveThroughIllegalSpacePenalty;
			UnsignedFloat mTransitionCostPenalty;
			UnsignedFloat mMaxCrossingDiameter;
			unsigned int mMaxManeuverSteps;

			UnsignedFloat mMinLengthForLaneChange;

			glm::vec3 mLastPartialSuccessAdaptionStart;
			float mLastPartialSuccessDistanceToTarget;

			WeakPtr<NavigationTaskThread> mTaskThread; // pointer to the navigation thread that calculates all the pathfinding requests
			WeakPtr<DynamicCollisionLocalPlanner> mLocalPathPlanner; // pointer to the local planner instance at the core entity

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::PathfindingSystem);

#include "qsf_ai/navigation/pathfinding/PathfindingSystem-inl.h"
