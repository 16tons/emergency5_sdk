// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/navigation/steering/CollisionAggregator.h"
#include "qsf_ai/navigation/pathfinding/localPlanning/DynamicCollisionLocalPlanner.h"
#include "qsf_ai/navigation/em4Router/wrapper/actor/EGenericObject.h"

#include <qsf/component/move/MovableComponent.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>

#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <map>

class btConvexShape;
class ECollisionObject;


namespace qsf
{
	namespace ai
	{
		namespace steering
		{
			/**
			* Provides functions to iterate over all relevant router collisions and integrate them into a CollisionAggregator.
			* It linearly extrapolates movement for a pair of objects in their respective movement directions and calculates whether these collide.
			*/
			class RouterCollisionAggregator
			{
			public:
				// Time to look ahead while standing still
				static const Time sFixedLookaheadWhileStandingStill;

				RouterCollisionAggregator(DynamicCollisionLocalPlanner& routerCollisionWorld, CollisionAggregator& aggregator, const MovableComponent& queryMovable, const NavigationComponent& queryNavi,
					const BulletCollisionComponent& originalCollision, UnsignedFloat queryRange, UnsignedFloat distanceToKeepBetween, bool isMovingForward);

				// Main function doing the real work
				void processCollisions() const;

			private:
				// Check an eventual active collision stored during the last update first before going into the costly search
				void testActiveCollision(const btConvexShape& queryShape) const;

				// Internal variant if the entity has CloseEntityTracking. This will search dynamically
				void findClosestWithDynamicProxy(const btConvexShape& queryShape) const;

				// Present a single collision option to the aggregator and project along movement direction
				void considerCollision(const btConvexShape& convexQuery, const btCollisionObject* collision, ECollisionObject* routerCollision) const;

				DynamicCollisionLocalPlanner& mRouterCollisionWorld;
				CollisionAggregator& mAggregator;
				const UnsignedFloat mQueryRange;
				const UnsignedFloat mDistanceToKeepBetween;
				const BulletCollisionComponent& mOriginalRealCollision;
				const MovableComponent& mQueryMovableComponent;
				const NavigationComponent& mQueryNavigationComponent;
				const float mDirectionFactor;

				EGenericObject* mGenericObject;
				int mOwnerPlayer;
				EObjectTerrainClass mTerrainClass;
			};
		}
	}
}
