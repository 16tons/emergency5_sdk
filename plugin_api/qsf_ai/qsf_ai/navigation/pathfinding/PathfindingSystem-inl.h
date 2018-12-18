// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline PathfindingSystem::PathfindingSystem(Prototype* prototype) :
			StandardSystem<NavigationComponent, PathfindingSystem>(prototype, NAME),
			mDebugSettings(nullptr),
			mTargePointManager(nullptr),
			mFastDebugDraw(nullptr),
			mDebugTrafficLaneWorldId(-1),
			mDebugShowArea(false),
			mTurningRadiusViolationCostPenalty(DEFAULT_TURNING_RADIUS_VIOLATION_COST_PENALTY),
			mManeuverCostPenalty(DEFAULT_MANEUVER_COST_PENALTY),
			mMovingBackwardsCostPenalty(DEFAULT_MOVING_BACKWARDS_COST_PENALTY),
			mMoveThroughIllegalSpacePenalty(MOVE_THROUGH_ILLEGAL_SPACE_PENALTY.mDefault),
			mTransitionCostPenalty(TRANSITION_PENALTY.mDefault),
			mMaxCrossingDiameter(MAX_CROSSING_DIAMETER.mDefault),
			mMaxManeuverSteps(DEFAULT_MAX_MANEUVER_STEPS),
			mMinLengthForLaneChange(MIN_LENGTH_FOR_LANE_CHANGE.mDefault),
			mLastPartialSuccessDistanceToTarget(0.f)
		{}

		inline void PathfindingSystem::createDebugOutput(const NavigationComponent&) const
		{}

		inline void PathfindingSystem::addMovementMode(const MovementMode& mode)
		{
			auto currentEntry = mMovementModes.find(mode.mId);
			QSF_CHECK(currentEntry == mMovementModes.end(), "Trying to overwrite movement mode with id " << mode.mId,
				QSF_REACT_THROW);

			QSF_WARN_IF(mode.mSpeedFactor && mode.mSpeedOverwrite, "movement mode " << mode.mId << " has an absolute overwrite as well as factor assigned. Ignoring the factor.",
				QSF_REACT_NONE);

			mMovementModes.insert(currentEntry, std::make_pair(mode.mId, mode));
		}

		inline const MovementMode& PathfindingSystem::getMovementMode(unsigned int id) const
		{
			auto entry = mMovementModes.find(id);
			QSF_CHECK(entry != mMovementModes.end(), "trying to access unknown movement mode with id " << id,
				QSF_REACT_THROW);

			return entry->second;
		}

		inline void PathfindingSystem::setTurningRadiusViolationPenalty(UnsignedFloat oneTimePenalty)
		{
			assignAndPromoteChange(mTurningRadiusViolationCostPenalty, oneTimePenalty, TURNING_RADIUS_VIOLATION_COST_PENALTY_PROPERTY_ID);
		}

		inline UnsignedFloat PathfindingSystem::getTurningRadiusViolationCostPenalty() const
		{
			return mTurningRadiusViolationCostPenalty;
		}

		inline void PathfindingSystem::setManeuverCostPenalty(UnsignedFloat oneTimePenalty)
		{
			assignAndPromoteChange(mManeuverCostPenalty, oneTimePenalty, MANEUVER_COST_PENALTY_PROPERTY_ID);
		}

		inline UnsignedFloat PathfindingSystem::getManeuverCostPenalty() const
		{
			return mManeuverCostPenalty;
		}

		inline void PathfindingSystem::setMovingBackwardsCostPenalty(UnsignedFloat penaltyFactor)
		{
			assignAndPromoteChange(mMovingBackwardsCostPenalty, penaltyFactor, MOVING_BACKWARDS_COST_PENALTY_PROPERTY_ID);
		}

		inline UnsignedFloat PathfindingSystem::getMovingBackwardsCostPenalty() const
		{
			return mMovingBackwardsCostPenalty;
		}

		inline void PathfindingSystem::setMaxManeuverSteps(unsigned int steps)
		{
			assignAndPromoteChange(mMaxManeuverSteps, steps, MAX_MANEUVER_STEPS_PROPERTY_ID);
		}

		inline unsigned int PathfindingSystem::getMaxManeuverSteps() const
		{
			return mMaxManeuverSteps;
		}

		inline void PathfindingSystem::setTargetPointManager(const logic::TargetPointManager& targetPointManager)
		{
			mTargePointManager = &targetPointManager;
		}

		inline const logic::TargetPointManager* PathfindingSystem::tryGetTargetPointManager() const
		{
			return mTargePointManager;
		}

		inline void PathfindingSystem::setMoveThroughIllegalSpacePenalty(UnsignedFloat penalty)
		{
			assignAndPromoteChange(mMoveThroughIllegalSpacePenalty, penalty, MOVE_THROUGH_ILLEGAL_SPACE_PENALTY.mId);
		}

		inline UnsignedFloat PathfindingSystem::getMoveThrougIllegalSpacePenalty() const
		{
			return mMoveThroughIllegalSpacePenalty;
		}

		inline void PathfindingSystem::setMinLengthForLaneChange(UnsignedFloat length)
		{
			assignAndPromoteChange(mMinLengthForLaneChange, length, MIN_LENGTH_FOR_LANE_CHANGE.mId);
		}

		inline UnsignedFloat PathfindingSystem::getMinLengthForLaneChange() const
		{
			return mMinLengthForLaneChange;
		}

		inline void PathfindingSystem::setTransitionPenalty(UnsignedFloat penalty)
		{
			assignAndPromoteChange(mTransitionCostPenalty, penalty, TRANSITION_PENALTY.mId);
		}

		inline UnsignedFloat PathfindingSystem::getTransitionPenalty() const
		{
			return mTransitionCostPenalty;
		}

		inline void PathfindingSystem::setMaxCrossingDiameter(UnsignedFloat maxDiameter)
		{
			assignAndPromoteChange(mMaxCrossingDiameter, maxDiameter, MAX_CROSSING_DIAMETER.mId);
		}

		inline UnsignedFloat PathfindingSystem::getMaxCrossingDiameter() const
		{
			return mMaxCrossingDiameter;
		}
	}
}
