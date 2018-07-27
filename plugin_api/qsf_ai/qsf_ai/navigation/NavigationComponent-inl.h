// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/MovementMode.h"
#include "qsf_ai/base/BitMaskHelper.h"

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/math/GlmHelper.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/log/LogSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline NavigationComponent::~NavigationComponent()
		{
			// Nothing to do
		}

		inline bool NavigationComponent::hasGoal() const
		{
			return mGoal.get() != nullptr;
		}

		inline NavigationGoal& NavigationComponent::getGoal()
		{
			QSF_CHECK(hasGoal(), "Trying to access nullptr as navigation goal",
				QSF_REACT_THROW);
			return *mGoal;
		}

		inline const NavigationGoal& NavigationComponent::getGoal() const
		{
			QSF_CHECK(hasGoal(), "Trying to access nullptr as navigation goal",
				QSF_REACT_THROW);
			return *mGoal;
		}

		inline void NavigationComponent::clearGoal()
		{
			setGoal(nullptr);
		}

		inline UnsignedFloat NavigationComponent::getNativeMaxForwardSpeed() const
		{
			return mMaxSpeed;
		}

		inline UnsignedFloat NavigationComponent::getMaxForwardSpeed() const
		{
			if (mMovementMode)
			{
				if (mMovementMode->mSpeedOverwrite)
					return *mMovementMode->mSpeedOverwrite;
				if (mMovementMode->mSpeedFactor)
					return *mMovementMode->mSpeedFactor * mMaxSpeed;
			}

			return mMaxSpeed;
		}

		inline void NavigationComponent::setNativeMaxForwardSpeed(UnsignedFloat maxSpeed)
		{
			assignAndPromoteChange(mMaxSpeed, maxSpeed, MAX_SPEED.mId);
		}

		inline void NavigationComponent::setBackwardSpeedFactor(UnsignedFloat factor)
		{
			assignAndPromoteChange(mBackwardSpeedFactor, factor, BACKWARD_SPEED_FACTOR_PROPERTY_ID);
		}

		inline UnsignedFloat NavigationComponent::getBackwardSpeedFactor() const
		{
			return mBackwardSpeedFactor;
		}

		inline UnsignedFloat NavigationComponent::getMaxBackwardSpeed() const
		{
			return getMaxForwardSpeed() * mBackwardSpeedFactor; // This applies the possible movement mode override inside getMaxForwardSpeed
		}

		inline void NavigationComponent::setMaxAcceleration(UnsignedFloat maxAcceleration)
		{
			assignAndPromoteChange(mMaxAcceleration, maxAcceleration, MAX_ACCELERATION_PROPERTY_ID);
		}

		inline UnsignedFloat NavigationComponent::getMaxAcceleration() const
		{
			return mMaxAcceleration;
		}

		inline unsigned int NavigationComponent::getNativePrimaryNavigationMapId() const
		{
			return mPrimaryNavigationMapId;
		}

		inline unsigned int NavigationComponent::getPrimaryNavigationMapId() const
		{
			return (mMovementMode && mMovementMode->mPrimaryNavMap) ? *mMovementMode->mPrimaryNavMap : mPrimaryNavigationMapId;
		}

		inline unsigned int NavigationComponent::getNativeSecondaryNavigationMapId() const
		{
			return mSecondaryNavigationMapId;
		}

		inline unsigned int NavigationComponent::getSecondaryNavigationMapId() const
		{
			return (mMovementMode && mMovementMode->mSecondaryNavMap) ? *mMovementMode->mSecondaryNavMap : mSecondaryNavigationMapId;
		}

		inline bool NavigationComponent::isCurrentlyUsingPrimaryNavigationMap() const
		{
			return mCurrentlyUsingPrimaryNavigationMap;
		}

		inline unsigned int NavigationComponent::getCurrentNavigationMapId() const
		{
			return mCurrentlyUsingPrimaryNavigationMap ? getPrimaryNavigationMapId() : getSecondaryNavigationMapId();
		}

		inline bool NavigationComponent::usesSecondaryNavigationMap() const
		{
			return isInitialized(getSecondaryNavigationMapId());
		}

		inline bool NavigationComponent::hasPath() const
		{
			return !mPath.isInClearedState();
		}

		inline const Path& NavigationComponent::getPath() const
		{
			return mPath;
		}

		inline Path& NavigationComponent::getPath()
		{
			return mPath;
		}

		inline bool NavigationComponent::selectNextNode()
		{
			return mPath.selectNextNode();
		}

		inline void NavigationComponent::requestPathAdaptation()
		{
			clearPathReservationsIfNecessary();
			mPath.requestAdaptation();
		}

		inline void NavigationComponent::requestPathRunAndAdapt()
		{
			mPath.requestRunAndAdapt();
		}

		inline unsigned int NavigationComponent::getCurrentWorldElementId() const
		{
			return mCurrentWorldElementId;
		}

		inline void NavigationComponent::setCurrentWorldElementId(unsigned int elementId, bool onPrimaryMap)
		{
			if (mCurrentWorldElementId == elementId && mCurrentlyUsingPrimaryNavigationMap == onPrimaryMap)
				return;

			QSF_CHECK(onPrimaryMap || usesSecondaryNavigationMap(), "No secondary navigation map configured, unable to use it",
				return);

			mCurrentWorldElementId = elementId;
			mCurrentlyUsingPrimaryNavigationMap = onPrimaryMap;

			promotePropertyChange(CURRENT_WORLD_ELEMENT_PROPERTY_ID);
		}

		inline bool NavigationComponent::hasSearchId() const
		{
			return mSearchId.is_initialized();
		}

		inline void NavigationComponent::setSearchId(unsigned int id)
		{
			mSearchId = id;
		}

		inline void NavigationComponent::clearSearchId()
		{
			mSearchId.reset();
		}

		inline unsigned int NavigationComponent::getSearchId() const
		{
			// The boost::optional throws an exception if it is not set
			return *mSearchId;
		}

		inline UnsignedFloat NavigationComponent::getTurningRadius() const
		{
			return mTurningConstraint.mTurningRadiusRequired;
		}

		inline void NavigationComponent::setTurningRadius(UnsignedFloat radius)
		{
			assignAndPromoteChange(mTurningConstraint.mTurningRadiusRequired, radius, TURNING_RADIUS_PROPERTY_ID);
		}

		inline Time NavigationComponent::getNextUpdateTime() const
		{
			return mNextUpdateTime;
		}

		inline void NavigationComponent::setNextUpdateTime(Time time)
		{
			mNextUpdateTime = time;
		}

		inline Time NavigationComponent::getGearChangeDuration() const
		{
			return mGearChangeDuration;
		}

		inline void NavigationComponent::setGearChangeDuration(Time duration)
		{
			mGearChangeDuration = duration;
		}

		inline bool NavigationComponent::hasDriver() const
		{
			return isInitialized(mDriverId);
		}

		inline bool NavigationComponent::hasVehicle() const
		{
			return isInitialized(mVehicleId);
		}

		inline uint64 NavigationComponent::getDriverId() const
		{
			return mDriverId;
		}

		inline uint64 NavigationComponent::getVehicleId() const
		{
			return mVehicleId;
		}

		inline void NavigationComponent::setDriverId(uint64 id)
		{
			assignAndPromoteChange(mDriverId, id, DRIVER_PROPERTY_ID);
		}

		inline void NavigationComponent::setVehicleId(uint64 id)
		{
			assignAndPromoteChange(mVehicleId, id, VEHICLE_PROPERTY_ID);
		}

		inline void NavigationComponent::clearDriverId()
		{
			setDriverId(getUninitialized(mDriverId));
		}

		inline void NavigationComponent::clearVehicleId()
		{
			setVehicleId(getUninitialized(mVehicleId));
		}

		inline void NavigationComponent::setNativeCollisionTypesToAvoid(short flags)
		{
			mAvoidedCollisionTypes = flags;
		}

		inline short NavigationComponent::getNativeCollisionTypesToAvoid() const
		{
			return mAvoidedCollisionTypes;
		}

		inline short NavigationComponent::getCollisionTypesToAvoid() const
		{
			return (mMovementMode && mMovementMode->mCollisionAvoidanceFlags) ? *mMovementMode->mCollisionAvoidanceFlags : mAvoidedCollisionTypes;
		}

		inline void NavigationComponent::setNativeCollisionAvoidanceMeasures(short collisionReactions)
		{
			mCollisionAvoidanceMeasures = collisionReactions;
		}

		inline short NavigationComponent::getNativeCollisionAvoidanceMeasures() const
		{
			return mCollisionAvoidanceMeasures;
		}

		inline short NavigationComponent::getCollisionAvoidanceMeasures() const
		{
			return (mMovementMode && mMovementMode->mCollisionAvoidanceMeasures) ? *mMovementMode->mCollisionAvoidanceMeasures : mCollisionAvoidanceMeasures;
		}

		inline void NavigationComponent::clearMovementMode()
		{
			mMovementMode = nullptr;
		}

		inline void NavigationComponent::setMaxTurningChange(UnsignedFloat maxTurningChange)
		{
			assignAndPromoteChange(mMaxTurningChange, maxTurningChange, MAX_TURNING_CHANGE_PROPERTY_ID);
		}

		inline UnsignedFloat NavigationComponent::getMaxTurningChange() const
		{
			return mMaxTurningChange;
		}

		inline Time NavigationComponent::getMinPathLookahead() const
		{
			return mMinPathLookahead;
		}

		inline void NavigationComponent::setMinPathLookahead(Time duration)
		{
			mMinPathLookahead = duration;
		}

		inline UnsignedFloat NavigationComponent::getDesiredAcceleration() const
		{
			return mDesiredAcceleration;
		}

		inline void NavigationComponent::setDesiredAcceleration(UnsignedFloat desiredAcceleration)
		{
			mDesiredAcceleration = desiredAcceleration;
		}

		inline void NavigationComponent::setMoverType(unsigned int type)
		{
			assignAndPromoteChange(mMoverType, type, MOVER_TYPE.mId);
		}

		inline unsigned int NavigationComponent::getMoverType() const
		{
			return mMoverType;
		}

		inline bool NavigationComponent::isMovementOn2DPlane() const
		{
			// TODO(tl): Flying units have moverType 7 in em5
			return mMoverType == 7;
		}

		inline bool NavigationComponent::isMovementOn2DPlane(unsigned int moverType)
		{
			// TODO(tl): Flying units have moverType 7 in em5
			return moverType == 7;
		}

		inline const TurningConstraint& NavigationComponent::getTurningConstraint() const
		{
			return mTurningConstraint;
		}

		inline void NavigationComponent::setNativeMayMoveForward(bool enable)
		{
			if (getNativeMayMoveForward() == enable)
				return;

			bitmasking::setIn(mMovementFlags, MOVE_FORWARD, enable);
			promotePropertyChange(MAY_MOVE_FORWARD.mId);
		}

		inline bool NavigationComponent::getNativeMayMoveForward() const
		{
			return bitmasking::holdsIn(mMovementFlags, MOVE_FORWARD);
		}

		inline void NavigationComponent::setNativeMayMoveBackwards(bool enable)
		{
			if (getNativeMayMoveBackwards() == enable)
				return;

			bitmasking::setIn(mMovementFlags, MOVE_BACKWARDS, enable);
			promotePropertyChange(MAY_MOVE_BACKWARD.mId);
		}

		inline bool NavigationComponent::getNativeMayMoveBackwards() const
		{
			return bitmasking::holdsIn(mMovementFlags, MOVE_BACKWARDS);
		}

		inline void NavigationComponent::setNativeMayManeuver(bool enable)
		{
			if (getNativeMayManeuver() == enable)
				return;

			bitmasking::setIn(mMovementFlags, MANEUVER, enable);
			promotePropertyChange(MAY_MANEUVER.mId);
		}

		inline bool NavigationComponent::getNativeMayManeuver() const
		{
			return bitmasking::holdsIn(mMovementFlags, MANEUVER);
		}

		inline void NavigationComponent::setNativeMayReduceTurningCircle(bool enable)
		{
			if (getNativeMayReduceTurningCircle() == enable)
				return;

			bitmasking::setIn(mMovementFlags, REDUCE_TURNING_CIRCLE, enable);
			promotePropertyChange(MAY_REDUCE_TURNING_RADIUS.mId);
		}

		inline bool NavigationComponent::getNativeMayReduceTurningCircle() const
		{
			return bitmasking::holdsIn(mMovementFlags, REDUCE_TURNING_CIRCLE);
		}

		inline void NavigationComponent::setNativeSyncVelocityToDirection(bool enable)
		{
			if (getNativeSyncVelocityToDirection() == enable)
				return;

			bitmasking::setIn(mMovementFlags, SYNC_VELOCITY_TO_DIRECTION, enable);
			promotePropertyChange(SHOULD_SYNC_VELOCITY_TO_DIRECTION.mId);
		}

		inline bool NavigationComponent::getNativeSyncVelocityToDirection() const
		{
			return bitmasking::holdsIn(mMovementFlags, SYNC_VELOCITY_TO_DIRECTION);
		}

		inline void NavigationComponent::setLimitDirectionChange(bool enable)
		{
			if (getLimitDirectionChange() == enable)
				return;

			bitmasking::setIn(mMovementFlags, LIMIT_DIRECTION_CHANGE, enable);
			promotePropertyChange(SHOULD_LIMIT_DIRECTION_CHANGE.mId);
		}

		inline bool NavigationComponent::getLimitDirectionChange() const
		{
			return bitmasking::holdsIn(mMovementFlags, LIMIT_DIRECTION_CHANGE);
		}

		inline void NavigationComponent::setUsesPrioritySearch(bool enable)
		{
			if (usesPrioritySearch() == enable)
				return;

			bitmasking::setIn(mMovementFlags, PRIORITY_SEARCH, enable);
			promotePropertyChange(USE_PRIORITY_SEARCH.mId);
		}

		inline bool NavigationComponent::usesPrioritySearch() const
		{
			return bitmasking::holdsIn(mMovementFlags, PRIORITY_SEARCH);
		}

		inline bool NavigationComponent::getNativeMoveToClosestInCaseOfFailedSearch() const
		{
			return bitmasking::holdsIn(mMovementFlags, MOVE_TO_CLOSEST_ON_FAILURE);
		}

		inline void NavigationComponent::setNativeMoveToClosestInCaseOfFailedSearch(bool enable)
		{
			if (getNativeMoveToClosestInCaseOfFailedSearch() == enable)
				return;

			bitmasking::setIn(mMovementFlags, MOVE_TO_CLOSEST_ON_FAILURE, enable);
			promotePropertyChange(MOVE_TO_CLOSEST_ON_FAILED_SEARCH.mId);
		}

		inline bool NavigationComponent::getSyncVelocityToDirection() const
		{
			return mMovementMode && mMovementMode->mSyncVelocityToDirection ? *mMovementMode->mSyncVelocityToDirection : bitmasking::holdsIn(mMovementFlags, SYNC_VELOCITY_TO_DIRECTION);
		}

		inline Percentage NavigationComponent::getCurveSmoothingFactor() const
		{
			return mTurningConstraint.mCurveSmoothingFactor;
		}

		inline void NavigationComponent::setCurveSmoothingFactor(Percentage factor)
		{
			assignAndPromoteChange(mTurningConstraint.mCurveSmoothingFactor, factor, CURVE_SMOOTHING_FACTOR.mId);
		}

		inline void NavigationComponent::setCollisionMask(short flags)
		{
			assignAndPromoteChange(mOwnCollisionMask, flags, OWN_COLLISION_MASK.mId);
		}

		inline short NavigationComponent::getCollisionMask() const
		{
			return mOwnCollisionMask;
		}

		inline void NavigationComponent::setNativeDistanceToKeepAhead(UnsignedFloat distance)
		{
			assignAndPromoteChange(mDistanceToKeepAhead, distance, DISTANCE_TO_KEEP_AHEAD.mId);
		}

		inline UnsignedFloat NavigationComponent::getNativeDistanceToKeepAhead() const
		{
			return mDistanceToKeepAhead;
		}

		inline UnsignedFloat NavigationComponent::getDistanceToKeepAhead() const
		{
			return mMovementMode && mMovementMode->mDistanceToKeepAhead ? *mMovementMode->mDistanceToKeepAhead : mDistanceToKeepAhead;
		}

		inline bool NavigationComponent::shouldCorrectIllegalStartPositions() const
		{
			return mPositionCorrectionSettings.mStartSettings.mCorrectIllegalState;
		}

		inline bool NavigationComponent::shouldCorrectIllegalGoalPositions() const
		{
			return mPositionCorrectionSettings.mGoalSettings.mCorrectIllegalState;
		}

		inline bool NavigationComponent::shouldCorrectBlockedStartPositions() const
		{
			return mPositionCorrectionSettings.mStartSettings.mCorrectPermanentBlockedState;
		}

		inline bool NavigationComponent::shouldCorrectBlockedGoalPositions() const
		{
			return mPositionCorrectionSettings.mGoalSettings.mCorrectPermanentBlockedState;
		}

		inline bool NavigationComponent::shouldCorrectStartToIdealPositions() const
		{
			return mPositionCorrectionSettings.mStartSettings.mCorrectToIdealPosition;
		}

		inline bool NavigationComponent::shouldCorrectGoalToIdealPositions() const
		{
			return (mMovementMode && mMovementMode->mCorrectGoalToIdealPosition) ? *mMovementMode->mCorrectGoalToIdealPosition : mPositionCorrectionSettings.mGoalSettings.mCorrectToIdealPosition;
		}

		inline void NavigationComponent::setCorrectIllegalStartPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mStartSettings.mCorrectIllegalState, enable, CORRECT_ILLEGAL_START_POSITION.mId);
		}

		inline void NavigationComponent::setCorrectIllegalGoalPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mGoalSettings.mCorrectIllegalState, enable, CORRECT_ILLEGAL_GOAL_POSITION.mId);
		}

		inline void NavigationComponent::setCorrectBlockedStartPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mStartSettings.mCorrectPermanentBlockedState, enable, CORRECT_BLOCKED_START_POSITION.mId);
		}

		inline void NavigationComponent::setCorrectBlockedGoalPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mGoalSettings.mCorrectPermanentBlockedState, enable, CORRECT_BLOCKED_GOAL_POSITION.mId);
		}

		inline void NavigationComponent::setCorrectStartToIdealPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mStartSettings.mCorrectToIdealPosition, enable, CORRECT_START_TO_IDEAL_POSITION.mId);
		}

		inline void NavigationComponent::setCorrectGoalToIdealPositions(bool enable)
		{
			assignAndPromoteChange(mPositionCorrectionSettings.mGoalSettings.mCorrectToIdealPosition, enable, CORRECT_GOAL_TO_IDEAL_POSITION.mId);
		}

		inline const position::CorrectionSettings& NavigationComponent::getPositionCorrectionSettings() const
		{
			return mPositionCorrectionSettings;
		}

		inline bool NavigationComponent::canIgnoreWorldElementState(const worldElement::State& state) const
		{
			if (!mMovementMode || !mMovementMode->mIgnoreWorldStatesUpTo)
				return false;

			return !mMovementMode->mIgnoreWorldStatesUpTo->isLessUrgent(state);
		}

		inline boost::optional<bool> NavigationComponent::shouldUseFunnelPathSmoothing() const
		{
			return mMovementMode ? mMovementMode->mForceUseFunnelSmoothing : boost::optional<bool>();
		}

		inline void NavigationComponent::setPathReservationWindow(const Path::IdRange& reservedIds)
		{
			mPath.setReservationWindow(reservedIds);
		}

		inline const Path::IdRange& NavigationComponent::getPathReservationWindow() const
		{
			return mPath.getReservationWindow();
		}

		inline void NavigationComponent::setMinWaitTimeAfterHalt(Time waitMin)
		{
			assignAndPromoteChange(mMinWaitTimeAfterHalt, waitMin, MIN_WAIT_AFTER_HALT.mId);
		}

		inline Time NavigationComponent::getMinWaitTimeAfterHalt() const
		{
			return mMinWaitTimeAfterHalt;
		}

		inline void NavigationComponent::setMaxWaitTimeAfterHalt(Time waitMax)
		{
			assignAndPromoteChange(mMaxWaitTimeAfterHalt, waitMax, MAX_WAIT_AFTER_HALT.mId);
		}

		inline Time NavigationComponent::getMaxWaitTimeAfterHalt() const
		{
			return mMaxWaitTimeAfterHalt;
		}

		inline bool NavigationComponent::hasActiveObstacle() const
		{
			return mActiveObstacle.is_initialized();
		}

		inline const NavigationComponent::ObstacleInfo& NavigationComponent::getActiveObstacle() const
		{
			return mActiveObstacle;
		}

		inline void NavigationComponent::setActiveObstacle(const ObstacleInfo& obstacle)
		{
			mActiveObstacle = obstacle;
		}

		inline void NavigationComponent::clearActiveObstacle()
		{
			mActiveObstacle.reset();
		}

		inline void NavigationComponent::setBrakingReason(steering::BrakingReason reason)
		{
			mLastBrakingReason = reason;
		}

		inline steering::BrakingReason NavigationComponent::getBrakingReason() const
		{
			return mLastBrakingReason;
		}

		inline void NavigationComponent::clearSearchStepsToIgnore()
		{
			mSearchStepsToIgnore.clear();
		}

		inline void NavigationComponent::addSearchStepToIgnore(const SearchStepToIgnore& searchStep)
		{
			mSearchStepsToIgnore.push_back(searchStep);
		}

		inline const std::vector<SearchStepToIgnore>& NavigationComponent::getSearchStepsToIgnore() const
		{
			return mSearchStepsToIgnore;
		}

		inline bool NavigationComponent::isTurningRateValid(UnsignedFloat turningRate)
		{
			return turningRate != 0.f && isInitialized(turningRate) && turningRate != std::numeric_limits<float>::infinity();
		}

		inline void NavigationComponent::setAdditionalCollisionLookaheadDistance(UnsignedFloat distance)
		{
			assignAndPromoteChange(mAdditionalCollisionLookaheadDistance, distance, ADDITIONAL_COLLISION_LOOKAHEAD_DISTANCE.mId);
		}

		inline UnsignedFloat NavigationComponent::getAdditionalCollisionLookaheadDistance() const
		{
			return mAdditionalCollisionLookaheadDistance;
		}

		inline void NavigationComponent::setRouterPathAdaptationStart(unsigned int wpIndex)
		{
			mRouterPathAdaptationStartIndex = wpIndex;
		}

		inline unsigned int NavigationComponent::getRouterPathAdaptationStart() const
		{
			return mRouterPathAdaptationStartIndex;
		}

		inline void NavigationComponent::setRouterPathAdaptationGoalIndices(const std::vector<unsigned int>& adaptationGoalIndices)
		{
			mRouterPathAdaptationGoals = adaptationGoalIndices;
		}

		inline void NavigationComponent::clearRouterPathAdaptationGoalIndices()
		{
			mRouterPathAdaptationGoals.clear();
		}

		inline const std::vector<unsigned int>& NavigationComponent::getRouterPathAdaptationGoalIndices() const
		{
			return mRouterPathAdaptationGoals;
		}

		inline long NavigationComponent::getLocalRouterPathRequestTime() const
		{
			return mRouterRequestTimeForCurrentPath;
		}

		inline void NavigationComponent::adaptLocalRouterPathRequestTime()
		{
			mRouterRequestTimeForCurrentPath = mRouterRequestTime;
		}

		inline void NavigationComponent::setDynamicTargetChanged(bool changed)
		{
			mDynamicTargetChanged = changed;
		}

		inline bool NavigationComponent::getDynamicTargetChanged()
		{
			return mDynamicTargetChanged;
		}

		inline void NavigationComponent::setDynamicTargetMovingCloser(bool TargetMovingCloser)
		{
			mDynamicTargetMovingCloser = TargetMovingCloser;
		}

		inline bool NavigationComponent::getDynamicTargetMovingCloser()
		{
			return mDynamicTargetMovingCloser;
		}

		inline bool NavigationComponent::isUnit() const
		{
			return (mMoverType == 0 || mMoverType == 1 || mMoverType == 6 || mMoverType == 9);
		}

		inline bool NavigationComponent::isWaterUnit() const
		{
			return (mMoverType == 6 || mMoverType == 9 || mMoverType == 10);
		}

		inline Time NavigationComponent::getUpdateRate() const
		{
			return mUpdateRate;
		}

		inline void NavigationComponent::setUpdateRate(Time updateRate)
		{
			if (mUpdateRate != updateRate)
			{
				mUpdateRate = updateRate;

				onUpdateRateChanged();
			}
		}

		inline Time NavigationComponent::addToAccumulatedUpdateTime(Time timeElapsed)
		{
			mAccumulatedPastTimeSinceUpdate += timeElapsed;
			if (mAccumulatedPastTimeSinceUpdate + mUpdateTimeOffset >= mUpdateRate)
			{
				const Time time = mAccumulatedPastTimeSinceUpdate;
				mUpdateTimeOffset = Time::ZERO;
				mAccumulatedPastTimeSinceUpdate = Time::ZERO;
				return time;
			}
			else
			{
				return Time::ZERO;
			}
		}

		inline const glm::vec3& NavigationComponent::getLastVelocity() const
		{
			return mLastVelocity;
		}

		inline void NavigationComponent::setVelocity(const glm::vec3& velocity)
		{
			mLastVelocity = velocity;
		}

		inline bool NavigationComponent::getDisableSteering()
		{
			return mDisableSteering;
		}

		inline void NavigationComponent::setDisableSteering(bool disable)
		{
			mDisableSteering = disable;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
