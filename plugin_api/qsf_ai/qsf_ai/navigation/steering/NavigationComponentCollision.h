// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/EvadedCollisionInfo.h"

#include <qsf/time/Time.h>

#include <glm/vec3.hpp>

namespace qsf
{
	class Entity;

	namespace ai
	{
		// Helper struct encapsulating the information about two vehicles colliding at a certain distance and time.
		// distance and time may legally be negative, indicating an existing penetration (possibly only of safe zone)
		class NavigationComponentCollision
		{
		public:
			NavigationComponentCollision(Entity& colliding, float distance, float relativeSpeed, Time timeToCollide, bool physical, const glm::vec3& otherVelocity);

			// convert to the permanent format stored with a path
			EvadedCollisionInfo toEvadedCollisionInfo() const;

			//compares by criticality, earlier collision is more critical. tie braking (mostly for case t=0) by distance.
			bool operator < (const NavigationComponentCollision& other) const;

			Entity* mCollidingEntity; // always initialized, never a nullptr.

			Time mTimeToCollide;

			// Positive is towards me, negative is away from me.
			// Assuming we move generally in the same direction and the collision is in front of me.
			float mRelativeSpeed;
			float mDistance;
			bool mIsPhysical; // else reservation collision
			glm::vec3 mOtherVelocity;
		};


		// inline implementation
		inline NavigationComponentCollision::NavigationComponentCollision(Entity& colliding, float distance, float relativeSpeed, Time timeToCollide, bool physical, const glm::vec3& otherVelocity) :
			mCollidingEntity(&colliding),
			mTimeToCollide(timeToCollide),
			mRelativeSpeed(relativeSpeed),
			mDistance(distance),
			mIsPhysical(physical),
			mOtherVelocity(otherVelocity)
		{}

		inline EvadedCollisionInfo NavigationComponentCollision::toEvadedCollisionInfo() const
		{
			EvadedCollisionInfo result(*mCollidingEntity);
			if (mIsPhysical)
				result.makePhysicalCollision();
			else
				result.makeReservationCollision();

			return result;
		}

		inline bool NavigationComponentCollision::operator < (const NavigationComponentCollision& other) const
		{
			if (mTimeToCollide == other.mTimeToCollide)
				return mDistance < other.mDistance;

			return mTimeToCollide < other.mTimeToCollide;
		}
	}
}
