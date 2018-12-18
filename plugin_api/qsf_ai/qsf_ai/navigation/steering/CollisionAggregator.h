// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/navigation/steering/NavigationComponentCollision.h"
#include "qsf_ai/navigation/steering/CollisionCallback.h"
#include "qsf_ai/base/NormalizedVector.h"

namespace qsf
{
	class MovableComponent;
	class TransformComponent;

	namespace ai
	{
		class SpeedLimitFunctor;

		/**
		* @brief Class that is used to aggregate collisions and keep track of the most critical one.
		* It is used as a functor with several collision detection algorithms.
		* The mode needs to be set before starting to test / integrate a number of collisions.
		* Currently physical as well as reservation collisions are known modes.
		*/
		class CollisionAggregator
		{
		public:
			// Initializes moving forward to true and drawCollisions to false by default.
			// Accumulates the most critical collision on the reference parameter passed
			// Starts in reservation collision mode by default.
			CollisionAggregator(const NavigationComponent& navi, CollisionCallback* collisionCallback, const glm::vec3& position, const glm::vec3& movementDirection, UnsignedFloat ownSpeed, bool isMovingForward,
				boost::optional<NavigationComponentCollision>& mostCriticalCollision, const std::unordered_set<Entity*>& persistentCollisions, bool drawCollisions, const SpeedLimitFunctor* speedLimits);

			// Switch mode for the upcoming collisions
			//@{
			void startIntegratingPhysicalCollisions();
			void startIntegratingReservationCollisions();
			//@}

			// Allow avoiding some tests against collisions based on entity ID
			bool shouldConsiderCollision(uint64 entityId) const;

			// Reset considered Collisions
			void resetConsideredCollision();

			// Enable or disable debug drawing mode
			void setDrawCollisions(bool enable);
			bool shouldDrawCollisions() const;

			// Core function to be called with a new collision with relative or absolute data
			void considerCollisionWithAbsoluteData(const TransformComponent& otherTransform, UnsignedFloat distanceToKeepAhead, const glm::vec3& collisionPos);
			void considerCollisionWithAbsoluteData(const MovableComponent& otherMovable, UnsignedFloat distanceToKeepAhead, const glm::vec3& collisionPos);
			void considerCollisionWithRelativeData(const MovableComponent& otherMovable, UnsignedFloat distanceToKeepAhead, const glm::vec3& collisionPos);

			// Helper function, returns true if other is behind own, regarding own movement direction
			static bool isInMovementDirectionBehind(const Normalized<glm::vec3>& ownDirection, const glm::vec3& otherRelativePos, bool isMovingForward);

			// Helper function, returns the distance the vehicle can brake in with the given acceleration
			static UnsignedFloat getSafetyDistance(UnsignedFloat speed, UnsignedFloat possibleAcceleration);

			// Returns true if we have already found a most critical collision with time to collide <= now
			bool hasFoundImmediateCollision() const;

		private:
			// Extracted core collision handling function
			// bullet returns the collision position absolute, but we'd like to have it relative
			// in this case, we only need our own speed to reach the collision (closing in on fixed point)
			// else (for reservation collision) we have a relative position (the collision is a moving vehicle), so we also use relative speed.
			void considerCollision(Entity& colliding, const glm::vec3& collidingCurrentPos, const glm::vec3& collidingCurrentVelocity,
				UnsignedFloat distanceToKeepAhead, const glm::vec3& relativeCollisionPosition, float relativeSpeedTowardsCollision);

			// debug draw helper function
			static void drawReservationCollision(Entity& colliding, const glm::vec3& ownPosition, const glm::vec3& relativeCollisionPosition, const glm::vec3& colliderCurrentPos,
				float distanceToKeepAhead, float safetyDistance);

			const NavigationComponent& mColliderNavigator;
			CollisionCallback* mCustomCollisionListener; // optional, may be a nullptr
			glm::vec3 mOwnPosition;
			glm::vec3 mOwnMovementDirection;
			UnsignedFloat mOwnSpeed;
			bool mDrawCollisions;
			bool mIsMovingForward;
			bool mPhysicalCollisionMode; // The collisions may either be reservation or physical collisions. We can easily set the mode before presenting a number of collisions from each source.
			boost::optional<NavigationComponentCollision>& mMostCriticalCollision;
			const SpeedLimitFunctor* mSpeedLimits;

			// (fw) Made this static as a performance / memory usage optimization; of course, this is a problem when having multiple instances at once
			static std::unordered_set<uint64> mConsideredCollisions; // Registering handled entity IDs to avoid duplicate checks
		};


		// inline implementation
		inline void CollisionAggregator::setDrawCollisions(bool enable)
		{
			mDrawCollisions = enable;
		}

		inline bool CollisionAggregator::hasFoundImmediateCollision() const
		{
			return mMostCriticalCollision && mMostCriticalCollision->mTimeToCollide <= Time::ZERO;
		}

		inline void CollisionAggregator::startIntegratingPhysicalCollisions()
		{
			mPhysicalCollisionMode = true;
		}

		inline void CollisionAggregator::startIntegratingReservationCollisions()
		{
			mPhysicalCollisionMode = false;
		}

		inline bool CollisionAggregator::shouldDrawCollisions() const
		{
			return mDrawCollisions;
		}
	}
}
