// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/reflection/ExportedProperty.h"
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>

#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


namespace qsf
{
	namespace ai
	{
		/**
		* A component that manages a proxy collision object to provide efficient and persistent close entities by tracking physical collisions.
		* Technically realized as a persistent ghost (proxy) object inside the bullet world.
		* Since we have several range based queries inside the AI we have a generic proxy object that uses the maximum of potentially many registered ranges for different use cases.
		* The individual components therefore still need to apply range and custom type based filtering for their respective requirements.
		* Each use case should register it with a radial range here and the tracking is done with the maximum of all use cases.
		*/
		class CloseEntitiesTrackingComponent : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			static const ExportedProperty<UnsignedFloat> RADIUS;
			// Access the test object filter flags statically before starting the simulation here.
			// These are used to find collision to evade dynamically.
			//@{
			static void setBulletTestObjectCollisionFilterGroup(short filter);
			static short getBulletTestObjectCollisionFilterGroup();
			static void setBulletTestObjectCollisionMask(short mask);
			static short getBulletTestObjectCollisionMask();
			//@}

			CloseEntitiesTrackingComponent(Prototype* prototype);

			// Different clients may register a desire to track close entities with a range here.
			// It is not possible to undo a registration this way currently.
			void registerTrackingDesire(UnsignedFloat radius);

			// Public Component interface implementation
			//@{
			virtual void onSetSimulating(bool simulating) override;
			virtual void onShutdown() override;
			virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
			//@}

			// Direct access to the bullet proxy object for using the results
			const btGhostObject& getBulletProxyObject() const;
		private:
			// Helper to add the collision proxy to the physics simulation.
			void addCollisionProxy();

			// Helper to remove the collision proxy object from the physics simulation.
			// Silently does nothing if the object was not added or there is no physics simulation running at all.
			void tryRemoveCollisionProxy();

			// Add the proxy object again if some important parameter changed
			void reAddCollisionProxy();

			UnsignedFloat mTrackingRange; // The maximum of all range tracking use cases requested.
			btSphereShape mCollisionShape;
			btGhostObject mQueryObject;
			bool mHasBeenAddedToPhysics;

			static short sBulletTestObjectCollisionFilterGroup;
			static short sBulletTestObjectCollisionMask;

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::CloseEntitiesTrackingComponent);
