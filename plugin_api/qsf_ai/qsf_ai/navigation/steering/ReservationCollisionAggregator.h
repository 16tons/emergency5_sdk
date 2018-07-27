// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/steering/SteeringControlPoint.h"
#include "qsf_ai/navigation/reservation/ReservationSystem.h"
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/navigation/steering/CollisionAggregator.h"
#include "qsf_ai/navigation/pathfinding/localPlanning/DynamicCollisionLocalPlanner.h"

#include <vector>


namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			/**
			* Provides functions to iterate over all relevant reservation system collisions and integrate them into a CollisionAggregator.
			* It linearly extrapolates movement for a pair of objects in their respective movement directions and calculates whether these collide assuming they drive behind each other.
			*/
			class ReservationCollisionAggregator
			{
			public:
				ReservationCollisionAggregator(DynamicCollisionLocalPlanner* routerCollisionWorld, CollisionAggregator& aggregator, ReservationSystem& reservationSystem, const NavigationComponent& queryNavi, const glm::vec3& currentQueryPos, const std::vector<SteeringControlPoint>& mControlPoints);

				// Main function doing the real work
				void processCollisions();

				// This is a delegate to the consider Collision function so instances of this class may be used as a functor for the reservation system
				void operator ()(const Reservation& conflictingReservation);

			private:
				// Check an eventual active collision stored during the last update first before going into the costly search
				void testActiveCollision();

				// Extracted function to consider a single Reservation entry as a collision
				void considerCollision(const NavigationComponent& conflictingReservation);

				// static configuration data
				CollisionAggregator& mCollisionAggregator;
				DynamicCollisionLocalPlanner* mRouterCollisionWorld;
				ReservationSystem& mReservationSystem;
				const NavigationComponent& mQueryNavigationComponent;
				const std::vector<SteeringControlPoint>& mControlPoints;
				const glm::vec3 mOwnPosition;

				// dynamic data
				AreaConfiguration mCurrentArea; // which area are we currently investigating
			};
		}
	}
}
