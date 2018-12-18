// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/goal/NavigationGoal.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Percentage.h"
#include "qsf_ai/navigation/EvadedCollisionInfo.h"
#include "qsf_ai/navigation/SearchStepToIgnore.h"
#include "qsf_ai/navigation/pathfinding/PositionCorrection.h"
#include "qsf_ai/navigation/steering/BrakingReason.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterTarget.h"
#include "qsf_ai/reflection/ExportedProperty.h"
#include "qsf_ai/worldModel/WorldElementState.h"

#include <qsf/component/Component.h>
#include <qsf/map/Entity.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/time/Time.h>

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class MovableComponent;
	class TransformComponent;
	class CollisionComponent;
	class BulletCollisionComponent;

	namespace ai
	{
		class MovementMode;
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
		/** Component for entities using the navigation system to move.
		* This adds the ability to have a path or a goal for dynamic navigation assigned as well as all the movement parameters.
		* The component is registered at the navigation systems (pathfinding and steering) and these are then triggered as required.
		* There is actually always a path assigned but it is empty if the entity is not currently moving along a path.
		* Initially the path is also empty.
		* The movement mode is an optional override for certain properties during the time an entity moves.
		* The overridden values can be still be read or written by the Get/SetNative<X> functions while the Get<X> always retrieves the currently effective value
		* be it supplied by a movement mode override or the native data field.
		*/
		class QSF_AI_API_EXPORT NavigationComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Unique CAMP Ids for the component, the exported properties and default values for the properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int PATH_PROPERTY_ID;
			static const unsigned int BACKWARD_SPEED_FACTOR_PROPERTY_ID;
			static const unsigned int MAX_ACCELERATION_PROPERTY_ID; // Max Acceleration is the magnitude of the velocity change. There is no distinction between speeding up and down or moving sideways here.
																	// This is included by the max turning angle change instead
			static const unsigned int MAX_TURNING_CHANGE_PROPERTY_ID;
			static const unsigned int PRIMARY_NAVIGATION_MAP_PROPERTY_ID;
			static const unsigned int SECONDARY_NAVIGATION_MAP_PROPERTY_ID; // This one is by default uninitialized and allows to configure an additional fallback to use for navigation if the goal can't be reached on the primary map
			static const unsigned int CURRENT_WORLD_ELEMENT_PROPERTY_ID; // This property is not exported as it makes no sense to configure it in the editor
			static const unsigned int TURNING_RADIUS_PROPERTY_ID;
			static const unsigned int DRIVER_PROPERTY_ID;
			static const unsigned int VEHICLE_PROPERTY_ID;
			static const unsigned int AVOID_COLLISION_TYPES_PROPERTY_ID;
			static const unsigned int COLLISION_AVOIDANCE_MEASURES_PROPERTY_ID;

			static const ExportedProperty<unsigned int> MOVER_TYPE; // type of entity regarding to which lanes it may use.
			static const ExportedProperty<UnsignedFloat> MAX_SPEED;
			static const ExportedProperty<bool> MAY_MOVE_FORWARD;
			static const ExportedProperty<bool> MAY_MOVE_BACKWARD;
			static const ExportedProperty<bool> MAY_MANEUVER;
			static const ExportedProperty<bool> MAY_REDUCE_TURNING_RADIUS;
			static const ExportedProperty<bool> SHOULD_SYNC_VELOCITY_TO_DIRECTION;
			static const ExportedProperty<bool> SHOULD_LIMIT_DIRECTION_CHANGE;
			static const ExportedProperty<Percentage> CURVE_SMOOTHING_FACTOR;
			static const ExportedProperty<short> OWN_COLLISION_MASK;
			static const ExportedProperty<UnsignedFloat> DISTANCE_TO_KEEP_AHEAD;
			static const ExportedProperty<UnsignedFloat> ADDITIONAL_COLLISION_LOOKAHEAD_DISTANCE;
			static const ExportedProperty<bool> CORRECT_ILLEGAL_START_POSITION;
			static const ExportedProperty<bool> CORRECT_ILLEGAL_GOAL_POSITION;
			static const ExportedProperty<bool> CORRECT_BLOCKED_START_POSITION;
			static const ExportedProperty<bool> CORRECT_BLOCKED_GOAL_POSITION;
			static const ExportedProperty<bool> CORRECT_START_TO_IDEAL_POSITION;
			static const ExportedProperty<bool> CORRECT_GOAL_TO_IDEAL_POSITION;
			static const ExportedProperty<bool> USE_PRIORITY_SEARCH;
			static const ExportedProperty<bool> MOVE_TO_CLOSEST_ON_FAILED_SEARCH;

			// Every entity gets a random delay before moving again after coming to a halt.
			// If the entity still doesn't move want to move again after the delay the delay is calculated and applied again.
			//@{
			static const ExportedProperty<Time> MIN_WAIT_AFTER_HALT;
			static const ExportedProperty<Time> MAX_WAIT_AFTER_HALT;
			//@}

			static const UnsignedFloat DEFAULT_BACKWARD_SPEED_FACTOR;
			static const Time DEFAULT_GEAR_CHANGE_DURATION;
			static const Time DEFAULT_MIN_PATH_LOOKAHEAD;
			static const UnsignedFloat DEFAULT_DESIRED_ACCELERATION;
			static const unsigned int DEFAULT_PRIMARY_NAVIGATION_MAP;
			static const unsigned int DEFAULT_SECONDARY_NAVIGATION_MAP;
			static const UnsignedFloat DEFAULT_TURNING_RADIUS;
			static const UnsignedFloat DEFAULT_MAX_ACCELERATION;
			static const UnsignedFloat DEFAULT_MAX_TURNING_CHANGE;
			static const short DEFAULT_AVOID_COLLISION_TYPES;
			static const short DEFAULT_COLLISION_AVOIDANCE_MEASURES;
			//@}

			/** Global helper functions to select the navigation component that currently governs an entities movement.
			* In case a driver entered a vehicle, the vehicle component is returned instead because this is moving instead of the driver.
			* Always access the navigation component through this helper if you want to be sure to access the relevant movement properties.
			* For technical reasons (serialization, editing) you should of course always access the real entity component no matter if its values are currently used or not.
			*/
			//@{
			static NavigationComponent* tryGetRelevantNavData(Entity& entity);
			static NavigationComponent& getRelevantNavData(Entity& entity);
			static NavigationComponent& getRelevantNavData(NavigationComponent& nativeData);
			//@}

			static const Percentage IDEAL_LANE_DEFAULT_MOVE_OFFSET; // Offset where to move on an ideal lane. 0.f is completely on the left side while 1.f is completely on the right side. 0.5 is the middle of the road

			// Defines the possible measures to avoid projected collisions.
			// These options can be combined into a bitmask to define how the entity may react.
			enum CollisionAvoidanceMeasures
			{
				SLOW_DOWN = 1 << 0,
				EVADE = 1 << 1,
			};

			// A collection of boolean options which may be activated for movement.
			// These settings can be combined into a bitmask to define the range of options.
			// Most of these are not relevant when no turning constraint is used and the entity can turn on the spot.
			enum MovementFlag
			{
				MOVE_FORWARD = 1 << 0, // The very basic option of moving forward. Will almost always be set to true
				MOVE_BACKWARDS = 1 << 1, // This is used in combination with the backwards speed factor. An entity will only move backwards to reach a position if this flag is set and the speed factor is positive.
									// An entity might still move backwards locally during maneuvering if that flag is set but not the move backwards flag
				MANEUVER = 1 << 2, // Means every combination of iteratively reversing and going forward in a small area with the goal of changing the orientation. Even if it may not move backwards
				REDUCE_TURNING_CIRCLE = 1 << 3, // means moving according to the circle becomes a weak criteria that may be violated if it helps reach the goal
				SYNC_VELOCITY_TO_DIRECTION = 1 << 4, // means the entity automatically looks where it is going. Otherwise the orientation has to be handled externally
				PRIORITY_SEARCH = 1 << 5, // priority searches are executed as fast as possible before scheduled regular searches
				MOVE_TO_CLOSEST_ON_FAILURE = 1 << 6, // move to the closest point reached in case the whole search is failed instead of just standing still as a result
				LIMIT_DIRECTION_CHANGE = 1 << 7 // limit the direction change between steering updates to the rotational speed limit
			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit NavigationComponent(Prototype* prototype);
			~NavigationComponent();

			// Overridden Component interface
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;
			virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Navigation goal settings
			//@{
			bool hasGoal() const;
			void setGoal(const NavigationGoal* goal); // creates and stores a copy of the goal if it is not a nullptr
			void transferGoal(std::auto_ptr<NavigationGoal> goal); // transfers ownership of the goal, the pointer is reset during the call
			void clearGoal();
			void evaluateGoal(); // force an evaluation of the goal at this point
			NavigationGoal& getGoal();
			const NavigationGoal& getGoal() const;
			//@}

			// Effective (considering possible movement mode override) and native speed setting access
			//@{
			UnsignedFloat getNativeMaxForwardSpeed() const;
			UnsignedFloat getMaxForwardSpeed() const;
			void setNativeMaxForwardSpeed(UnsignedFloat maxSpeed);
			//@}

			// Backward speed is considered to be at a constant factor of the forward speed.
			// You can set or get this factor or get the resulting backward max speed but not set the latter directly.
			// A backward speed of zero indicates that this entity can't move backwards.
			//@{
			UnsignedFloat getBackwardSpeedFactor() const;
			void setBackwardSpeedFactor(UnsignedFloat factor);
			UnsignedFloat getMaxBackwardSpeed() const;
			//@}

			// Max possible Acceleration in world units / second^2 - default is infinity
			//@{
			void setMaxAcceleration(const UnsignedFloat maxAcceleration);
			UnsignedFloat getMaxAcceleration() const;
			//@}

			// Max Turning change in radians / second
			//@{
			void setMaxTurningChange(const UnsignedFloat maxTurningChange);
			UnsignedFloat getMaxTurningChange() const;
			//@}

			// Access to the ids of the navigation maps to use for moving around.
			//@{
			unsigned int getNativePrimaryNavigationMapId() const;
			unsigned int getPrimaryNavigationMapId() const;
			void setNativePrimaryNavigationMapId(unsigned int mapId);

			unsigned int getNativeSecondaryNavigationMapId() const; // uninitialized means no secondary map is used
			unsigned int getSecondaryNavigationMapId() const;
			void setNativeSecondaryNavigationMapId(unsigned int mapId);
			bool usesSecondaryNavigationMap() const; // shortcut for isInitialized(getSecondary...())

			unsigned int getCurrentNavigationMapId() const; // shortcut for if (isCurrentlyUsingPrimaryNavigationMap) return getPrimaryNavigationMapId() : return getSecondaryNavigationMapId();
			bool isCurrentlyUsingPrimaryNavigationMap() const; // false means currently using the secondary nav map

			unsigned int getCurrentWorldElementId() const;
			void setCurrentWorldElementId(unsigned int elementId, bool onPrimaryMap);
			//@}

			// Path access
			//@{
			bool hasPath() const; // Returns true if a dedicated path is set
			const Path& getPath() const; // If there is no dedicated path set, an empty path is returned.
			Path& getPath();
			bool selectNextNode(); // returns whether there was a next node and the entity is still following that path
			void clearPath();
			void setPath(const Path& path);
			void setPathBySwap(Path& path);
			void requestPathAdaptation();	// signal to the pathfinding that the path should be reevaluated and potentially changed. Communication between the steering and pathfinding system
			void requestPathRunAndAdapt();	// signal the path will be changed but we should still move on it
			//@}

			// Access to the path reservation window
			//@{
			void setPathReservationWindow(const Path::IdRange& reservedIds); // mark the path as being reserved inside the window of node whose ids are passed here
			const Path::IdRange& getPathReservationWindow() const;
			void clearPathReservationsIfNecessary();
			//@}

			// The pathfinding stores a search id during the time the path search is being conducted with the navigation component
			//@{
			bool hasSearchId() const;
			void setSearchId(unsigned int id);
			unsigned int getSearchId() const;
			void clearSearchId();
			//@}

			// An optional radius in world units of a minimal circle that the entity needs to follow when turning. Zero means it is not used and turning is not restricted this way.
			//@{
			UnsignedFloat getTurningRadius() const;
			void setTurningRadius(UnsignedFloat radius);
			//@}

			// An absolute time that marks when the steering system should fully update this component.
			//@{
			Time getNextUpdateTime() const;
			void setNextUpdateTime(Time time);
			//@}

			// A duration to switch gears from forward to backward and vice versa. No time needed by default.
			//@{
			Time getGearChangeDuration() const;
			void setGearChangeDuration(Time duration);
			//@}

			// A minimum duration of how far the component will evaluate, and, if necessary, reserve, the path ahead. The steering system may choose a further lookahead, but no smaller.
			//@{
			Time getMinPathLookahead() const;
			void setMinPathLookahead(Time duration);
			//@}

			// Acceleration value which should be predominantly used to brake and accelerate. If not sufficient to brake with this acceleration, any value up to maxAcceleration is used.
			//@{
			UnsignedFloat getDesiredAcceleration() const;
			void setDesiredAcceleration(UnsignedFloat desiredAcceleration);
			//@}

			// Optional driver vehicle relations are signaled by setting the respective entity ids.
			// Uninitialized means not used in all cases.
			//@[
			bool hasDriver() const;
			bool hasVehicle() const;
			uint64 getDriverId() const;
			uint64 getVehicleId() const;
			//@}

			// Makes the navigation component a driver of the other component passed as parameter.
			// The driver component controls the vehicle and moves using the vehicles restrictions but the drivers decisions.
			// Fails if either one of the two components already is a driver or vehicle in some other relation
			// No component can ever by vehicle and driver at the same time.
			// Resets both components path and transfers a goal from driver to the vehicle.
			bool makeDriverOf(NavigationComponent& vehicle);

			// Resolves any driver / vehicle relation the component is involved in.
			// This can be called on the driver or the vehicle and leads to the same result for both components.
			void dissolveDriverVehicleRelation();

			// Tweak the collisions to avoid or ignore during steering.
			//@
			void setNativeCollisionTypesToAvoid(short types);
			short getNativeCollisionTypesToAvoid() const;
			short getCollisionTypesToAvoid() const;
			//@}

			// Access the possible reactions to projected collisions as a combination of values from the enum CollisionReactions into a bitmask.
			//@{
			void setNativeCollisionAvoidanceMeasures(short reactionsBitmask);
			short getNativeCollisionAvoidanceMeasures() const;
			short getCollisionAvoidanceMeasures() const;
			//@}

			// bitmask how this entity is considered as a collision
			void setCollisionMask(short bitmask);
			short getCollisionMask() const;

			void setMoverType(unsigned int type);
			unsigned int getMoverType() const;

			void setMovementMode(unsigned int id);
			unsigned int getMovementModeId();
			const MovementMode* tryGetMovementMode() const;
			void clearMovementMode();

			inline bool isMovementOn2DPlane() const;
			static inline bool isMovementOn2DPlane(unsigned int moverType);

			// Comfort accessors for the single bits of the movement flags
			//@{
			void setNativeMayMoveForward(bool enable);
			bool getNativeMayMoveForward() const;

			void setNativeMayMoveBackwards(bool enable);
			bool getNativeMayMoveBackwards() const;

			void setNativeMayManeuver(bool enable);
			bool getNativeMayManeuver() const;

			void setNativeMayReduceTurningCircle(bool enable);
			bool getNativeMayReduceTurningCircle() const;

			void setNativeSyncVelocityToDirection(bool enable);
			bool getNativeSyncVelocityToDirection() const;
			bool getSyncVelocityToDirection() const;

			void setLimitDirectionChange(bool enable);
			bool getLimitDirectionChange() const;

			bool getNativeMoveToClosestInCaseOfFailedSearch() const;
			void setNativeMoveToClosestInCaseOfFailedSearch(bool enable);
			//@}

			// Comfort access for the single flags of the position correction settings.
			// See the navigation PositionCorrection structures for detailed comments
			//@{
			bool shouldCorrectIllegalStartPositions() const;
			bool shouldCorrectIllegalGoalPositions() const;
			bool shouldCorrectBlockedStartPositions() const;
			bool shouldCorrectBlockedGoalPositions() const;
			bool shouldCorrectStartToIdealPositions() const;
			bool shouldCorrectGoalToIdealPositions() const;
			bool usesPrioritySearch() const;

			void setCorrectIllegalStartPositions(bool enable);
			void setCorrectIllegalGoalPositions(bool enable);
			void setCorrectBlockedStartPositions(bool enable);
			void setCorrectBlockedGoalPositions(bool enable);
			void setCorrectStartToIdealPositions(bool enable);
			void setCorrectGoalToIdealPositions(bool enable);
			void setUsesPrioritySearch(bool enable);
			//@}

			// Returns the whole set of options that are applied when correcting start and goal positions during path finding
			const position::CorrectionSettings& getPositionCorrectionSettings() const;

			// Return a combination of the selected movement flags as a bitmask
			short getMovementFlagsAsBitmask() const;

			Percentage getCurveSmoothingFactor() const;
			void setCurveSmoothingFactor(Percentage factor);

			// Allows accessing the contained TurningConstraint
			//@{
			void recalculateTurningConstraint();
			const TurningConstraint& getTurningConstraint() const;
			//@}

			// Get and set distance to keep clear ahead
			//@{
			void setNativeDistanceToKeepAhead(UnsignedFloat distance);
			UnsignedFloat getNativeDistanceToKeepAhead() const;
			UnsignedFloat getDistanceToKeepAhead() const;
			//@}

			/**
			* Queries whether the entity can ignore the state of some world element that it approaches or plan to cross during its movement.
			* Currently it queries the movement mode used. If no dedicated mode is set, an entity will always react to the states.
			* There are further methods of ignoring the states on an individual basis that also allow more specific reactions.
			*/
			bool canIgnoreWorldElementState(const worldElement::State& state) const;

			/**
			* Returns an optional override for the path smoothing method to apply.
			* Temporary expert option that basically just returns the setting of an eventual movement mode.
			* See the similar option inside the MovementMode for details.
			*/
			boost::optional<bool> shouldUseFunnelPathSmoothing() const;

			// Access the wait time after coming to a halt.
			//@{
			void setMinWaitTimeAfterHalt(Time waitMin);
			Time getMinWaitTimeAfterHalt() const;
			void setMaxWaitTimeAfterHalt(Time waitMax);
			Time getMaxWaitTimeAfterHalt() const;
			//@}

			typedef boost::optional<EvadedCollisionInfo> ObstacleInfo;

			// Access to the active obstacle setting used by the steering system.
			//@{
			bool hasActiveObstacle() const;
			const ObstacleInfo& getActiveObstacle() const;
			void setActiveObstacle(const ObstacleInfo& obstacle);
			void clearActiveObstacle();
			//@}

			// Access to the braking reason flag
			// Not exported to CAMP because it is a runtime only property
			//@{
			void setBrakingReason(steering::BrakingReason reason);
			steering::BrakingReason getBrakingReason() const;
			//@}

			// Get and set additional collision lookahead distance
			//@{
			void setAdditionalCollisionLookaheadDistance(UnsignedFloat distance);
			UnsignedFloat getAdditionalCollisionLookaheadDistance() const;
			//@}

			void clearSearchStepsToIgnore();
			void addSearchStepToIgnore(const SearchStepToIgnore& searchStep);
			bool containsSearchStepToIgnore(const SearchStepToIgnore& searchStep) const;
			const std::vector<SearchStepToIgnore>& getSearchStepsToIgnore() const;

			// checks if a turning rate is nonzero, initialized not infinite
			static bool isTurningRateValid(UnsignedFloat turningRate);

			MovableComponent& getMovableComponent() const;
			MovableComponent* tryGetMovableComponent() const;
			TransformComponent& getTransformComponent() const;
			TransformComponent* tryGetTransformComponent() const;
			CollisionComponent& getCollisionComponent() const;
			CollisionComponent* tryGetCollisionComponent() const;
			BulletCollisionComponent& getBulletCollisionComponent() const;
			BulletCollisionComponent* tryGetBulletCollisionComponent() const;

			// Access to the goal settings for the local router
			//@{
			bool hasLocalRouterTarget() const;
			void resetLocalRouterTarget(ERouterTarget* newTarget); // also works with nullptr like a clear
			ERouterTarget* tryGetLocalRouterTarget();
			void setRouterPathAdaptationStart(unsigned int wpIndex);
			unsigned int getRouterPathAdaptationStart() const;
			void setRouterPathAdaptationGoalIndices(const std::vector<unsigned int>& adaptationGoalIndices);
			void clearRouterPathAdaptationGoalIndices();
			const std::vector<unsigned int>& getRouterPathAdaptationGoalIndices() const;
			long getLocalRouterPathRequestTime() const; // this returns the highest time stamp for a path part that was planned by the local router
			void adaptLocalRouterPathRequestTime(); // this sets the request time for the current path after finished the local routing
			//@}

			// Dynamic target changed
			inline void setDynamicTargetChanged(bool changed);
			inline bool getDynamicTargetChanged();

			inline void setDynamicTargetMovingCloser(bool movingCloser);
			inline bool getDynamicTargetMovingCloser();

			inline bool isUnit() const;
			inline bool isWaterUnit() const;

			inline Time getUpdateRate() const;
			inline void setUpdateRate(Time updateRate);

			inline Time addToAccumulatedUpdateTime(Time timeElapsed);

			inline const glm::vec3& getLastVelocity() const;
			inline void setVelocity(const glm::vec3& velocity);

			// disables the Steering until we have a new path
			inline bool getDisableSteering();
			inline void setDisableSteering(bool disable);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Helper function that tries to unregister the entity from the reservation system.
			void unregisterFromReservationSystem();

			// Write access to the driver and vehicle ids should be done through the functions makeDriverOf / dissolveDriverVehicleRelation
			void setDriverId(uint64 id);
			void setVehicleId(uint64 id);
			void clearVehicleId();
			void clearDriverId();

			void onUpdateRateChanged();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// May actually be a nullptr if there is no active goal
			std::auto_ptr<NavigationGoal> mGoal;

			// The path finding stores the unique search id here during the time the search is being calculated.
			boost::optional<unsigned int> mSearchId;
			Path mPath;
			unsigned int mMoverType; // a generic, application specific type identifier that is used in combination with the lanes to determine where the entity can move

			UnsignedFloat mMaxSpeed; // in world units / second for moving forward
			UnsignedFloat mMaxAcceleration; // in world units / second^2
			UnsignedFloat mMaxTurningChange; // in radians / second

			Time mNextUpdateTime; //absolute time
			Time mGearChangeDuration;
			Time mMinPathLookahead;
			Time mMinWaitTimeAfterHalt;
			Time mMaxWaitTimeAfterHalt;
			UnsignedFloat mDesiredAcceleration;

			UnsignedFloat mBackwardSpeedFactor;
			unsigned int mPrimaryNavigationMapId;
			unsigned int mSecondaryNavigationMapId;
			bool mCurrentlyUsingPrimaryNavigationMap; // false means using secondary
			// A bitmask to be used as filter when selecting which types of collisions are actually avoided.
			// Refer to the collision flags of a bullet collision object for further details.
			// This is used like these flag but in the case of projecting collisions to avoid them prematurely.
			// This can actually be different from what actually collides (for example triggers).
			// TODO(vs) This could probably be made a global property if there are no special rules for individual unit types.
			short mAvoidedCollisionTypes;
			// Bitmask containing the allowed measures to avoid projected collisions. Contains a binary combination of the values in enum CollisionAvoidanceMeasures.
			short mCollisionAvoidanceMeasures;
			// A bitmask to be used as filter whether other entities should avoid this entity during AI navigation.
			// This is by default synchronized to the same setting of an attached collision component.
			// If no such component is attached it is zero by default and therefore won't be avoided during AI collision detection.
			// It still can be manually overwritten so AI collision avoidance can be done without physics information.
			short mOwnCollisionMask;

			// Bitmask containing the selected movement flags. Contains a binary combination of the values in enum MovementFlag.
			short mMovementFlags;

			// Remember the id of the last world element the entity was one.
			// This is used as best place to start looking for the current position of an entity to avoid a search in most cases.
			// This is automatically updated if the entity moves regularly along a path but not every position change is evaluated to keep the costs low.
			// As a result if the entity is teleported for example this information may be plain wrong.
			// It has no guarantee for correctness but is only an indicator for avoiding search costs in good cases.
			// The reserved uninitialized has the meaning that the entity was not placed anywhere on the world.
			// Every other value at least defines the world element that the entity is currently registered no matter whether this is still correct given its position.
			unsigned int mCurrentWorldElementId;

			// Each navigation component could have another entity acting as driver or vehicle for this component.
			// The logic is that the driver delegates all calls to the vehicle component.
			uint64 mDriverId;
			uint64 mVehicleId;

			// Current movement mode, may be nullptr if entity is not moving or none is used for the current movement.
			const MovementMode* mMovementMode;

			TurningConstraint mTurningConstraint;

			// A distance to keep before other entities, e.g. cars should not stop bumper to bumper
			UnsignedFloat mDistanceToKeepAhead;

			// This is an estimation for the longest vehicle in the world that an entity needs to watch out for when deciding how far to look ahead to avoid collisions
			//@{
			UnsignedFloat mAdditionalCollisionLookaheadDistance;
			//@}

			// Individual settings for how to treat start and goal positions for a path search request and correct these.
			position::CorrectionSettings mPositionCorrectionSettings;

			// The entity may store one obstacle here that is the most urgent potential collision that influences its movement.
			// If it is not set, the entity is not braking or halting because of an obstacle.
			// Otherwise the ID of the entity is stored together with a flag whether it is a reservation or physical collision.
			// These are two different modes that the steering system uses to determine collisions.
			// Pure internal runtime property that is not exported to CAMP and doesn't need to be edited.
			ObstacleInfo mActiveObstacle;

			// Information why the entity needs to reduce its speed currently
			// NO_STOP means it doesn't need to reduce its speed and goes with full force.
			// An entity without a path usually has PATH_ENDS since the end of its last path
			steering::BrakingReason mLastBrakingReason;

			// Specific no-go steps during the next path search to ignore
			std::vector<SearchStepToIgnore> mSearchStepsToIgnore;

			// We changed the dynamic target
			bool mDynamicTargetChanged;
			bool mDynamicTargetMovingCloser;

			// Optional target for the local router.
			// Is set in case we need to adapt the path to move around a dynamic collision
			std::unique_ptr<ERouterTarget> mLocalRouterTarget;
			unsigned int mRouterPathAdaptationStartIndex; // index where to insert a local replanning result
			std::vector<unsigned int> mRouterPathAdaptationGoals; // indices of nodes that are goals for the planned router path adaptation
			long mRouterRequestTime; // this is the router time when the last request was started. it is tracked so we can compare against obstacles to decide whether to ignore them
			long mRouterRequestTimeForCurrentPath; // this is the router time when the request was started for the current local path

			// Optimized access to frequently used components:
			WeakPtr<MovableComponent> mMovableComponentPtr;
			WeakPtr<TransformComponent> mTransformComponentPtr;
			WeakPtr<CollisionComponent> mCollisionComponentPtr;
			WeakPtr<BulletCollisionComponent> mBulletCollisionComponentPtr;

			// AI level of detail related
			Time mUpdateRate;						///< Minimum time between updates, "qsf::Time::ZERO" by default for updates in every tick
			Time mUpdateTimeOffset;					///< Additional time offset when update rate was set to have some randomization of updates over multiple entities
			Time mAccumulatedPastTimeSinceUpdate;	///< Simulation time since last update
			glm::vec3 mLastVelocity;				///< Movement velocity vector in last update

			bool mDisableSteering;

		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::NavigationComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent-inl.h"
