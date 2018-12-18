// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/DynamicPortal.h"
#include "qsf_ai/navigation/goal/NavigationTargetPoint.h"

#include <qsf/logic/ProcessState.h>

#include <qsf/math/Transform.h>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <vector>
#include <deque>


namespace qsf
{
	class Entity;
	class BinarySerializer;

	namespace ai
	{
		class Waypoint;

		/** Defines a goal for the navigation system.
		* This is the abstract superclass which contains the common core data.
		*/
		class QSF_AI_API_EXPORT NavigationGoal
		{
		public:
			// The known legal subtypes of navigation goals
			enum Type
			{
				ARRIVE_AT_STATIC_POSITION,
				ARRIVE_AT_ONE_OF_SEVERAL_STATIC_POSITIONS,
				ARRIVE_AT_OBJECT_TARGET_POINT,
				ACHIEVE_LINE_OF_SIGHT,
				AVOID_LINE_OF_SIGHT,
				AVOID_THREATS,
			};

			NavigationGoal(Type type, bool dynamicTarget);
			virtual ~NavigationGoal();

			Type getType() const;
			process::State getState() const;
			bool isTargetDynamic() const;

			// This function is called for goals with a dynamic target.
			// It needs to reevaluate the goal situation and return whether it has changed enough so we might need a new path.
			// It is expected that all necessary extracted data is written during this function call.
			virtual bool checkForChangedGoalSituation(const Entity& entity) = 0;

			// Check if the target is moving closer on going away, assuming we are not moving
			virtual bool isTargetMovingCloser() const = 0;

			// Calculate the current execution state for the entity with the given id, changing the current state and returning the result.
			virtual process::State evaluateState(const Entity& entity) = 0;
			// Specific variant of the evaluate state function for dynamic targets.
			// Unlike static targets, these can be fulfilled earlier than planned so we need to check while still trying to reach the goal.
			// The main difference is that we don't fail the goal in case it is not achieved yet but treat it as a running state.
			// It may still fail here if for example the target vanished in between
			process::State evaluateStateWhileRunning(const Entity& entity);

			// Some goal types don't introduce dedicated goal states but create a search where the remaining distance towards the goal defines whether we have reached a final state.
			bool doesRemainingDistanceIndicateGoalState() const;

			// Estimate the distance to the target without further knowledge about the world
			virtual UnsignedFloat estimateDistanceToTarget(const glm::vec3& fromPos) const = 0;

			// retrieve all possible target positions.
			// TODO(vs) This is a stop-gap solution to be able to introduce further types without breaking the current interface
			typedef std::vector<NavigationTargetPoint> GoalConfigurationList;
			const GoalConfigurationList& getGoalConfigurations() const;

			// Create an exact clone as a replacement for a copy constructor
			virtual std::auto_ptr<NavigationGoal> clone() const = 0;

			// Root serialization function.
			// If used with a reading binary serializer it is expected that the type is already read and the specific object was created through the NavigationGoalFactory
			void serialize(BinarySerializer& serializer);

			// Allows to tweak a list of waypoints that were gathered by the path search before and are to be compiled into a path to follow.
			// Since the abstract search mainly moves towards static positions we might actually need to tweak the result for goals that optimize other criteria like a distance to some other object
			// The default implementation is empty because the need to actually tweak the list should be a very special case.
			// The supporting portals for the first and last node respectively are used to provide corridor data for the legal area to move through because first and last node often don't have valid portal data themselves
			virtual void tweakWaypointSolutionList(const DynamicPortal& startSupportingPortal, std::deque<Waypoint>& waypoints, const DynamicPortal& endSupportingPortal) const;

			virtual bool shouldCheckTargetPositionForLocalRouter() const  { return true; }

		protected:
			// This routine needs to be overwritten and provide the subgoal specific parts of the serialized data
			virtual void serializeSpecificData(BinarySerializer& serializer) = 0;

			// Some helper functions that are reused in many goals with a GoalConfigurationList
			// Calculates the closest distance to point from any of the entries in goals
			// Always includes the distance and the flag controls whether the tolerance should also be considered.
			static UnsignedFloat calculateClosestDistanceTo(const glm::vec3& point, const GoalConfigurationList& goals, bool subtractTolerance = false);

			// Just a wrapper around the necessary accesses to resolve a target point
			static void calculateGoalConfigurationsFromTargetPoint(const Entity& caller, const Transform& callerTransform, const Entity& target, const Transform& targetTransform, uint32 targetPointId, GoalConfigurationList& goals);

			// helper function to read the position from an entities transform component
			static const glm::vec3& getPositionFromEntity(const Entity& entity);

			process::State mState;
			bool mDynamicTarget; // Whether the goal is static or dynamic in nature
			GoalConfigurationList mGoalConfigurations;

		private:
			Type mType;
		};


		// inline implementation
		inline NavigationGoal::NavigationGoal(Type type, bool dynamicTarget) :
			mState(process::RUNNING),
			mDynamicTarget(dynamicTarget),
			mType(type)
		{}

		inline NavigationGoal::~NavigationGoal()
		{}

		inline process::State NavigationGoal::getState() const
		{
			return mState;
		}

		inline bool NavigationGoal::isTargetDynamic() const
		{
			return mDynamicTarget;
		}

		inline void NavigationGoal::tweakWaypointSolutionList(const DynamicPortal&, std::deque<Waypoint>&, const DynamicPortal&) const
		{
			// default implementation is empty by design
		}

		inline NavigationGoal::Type NavigationGoal::getType() const
		{
			return mType;
		}

		inline bool NavigationGoal::doesRemainingDistanceIndicateGoalState() const
		{
			return false; // no fitting type implemented yet
		}

		inline const NavigationGoal::GoalConfigurationList& NavigationGoal::getGoalConfigurations() const
		{
			return mGoalConfigurations;
		}

	}
}

// Helper macro to avoid the code repetition in the implementation of the clone method.
// In effect, the clone is just mapped from the automatically created copy constructor
#define QSF_AI_DEFINE_CLONE_GOAL_IMPLEMENTATION(type) \
		std::auto_ptr<NavigationGoal> type::clone() const \
		{ \
			return std::auto_ptr<NavigationGoal>(new type(*this)); \
		}
