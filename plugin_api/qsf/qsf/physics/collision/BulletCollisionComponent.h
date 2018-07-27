// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/CollisionComponent.h"

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btTransform;
class btCollisionShape;
namespace qsf
{
	class PhysicsWorldComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    A component that uses a bullet collision object as collision geometry
	*
	*  @remarks
	*    This is the super class for all bullet collision components.
	*    It can not be directly instantiated because of the additional constructor parameters.
	*
	*  @note
	*    - We are using the custom user pointer inside btCollisionObject for a pointer back to the containing BulletCollisionComponent.
	*    - "qsf::BulletCollisionComponent" is for static or via game logic moved objects while "qsf::PhysicsRigidBodyComponent" is for dynamic physics moved objects
	*/
	class QSF_API_EXPORT BulletCollisionComponent : public CollisionComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		friend class CollisionHelper; // The collision helper needs to access some data that should stay private to regular clients

		static const uint32 COMPONENT_ID;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// passing a nullptr for the collision shape is allowed and indicates that the shape will be created at a later point.
		inline BulletCollisionComponent(Prototype* prototype, CollisionComponent::Type type, btCollisionShape* collisionShape);

		// The collision flags are export options that only need to be set in very special cases.
		// Refer to the bullet object documentation for details
		inline int getCollisionFlags() const;
		inline void setCollisionFlags(int flags);

		// Directly setting the bullet transform does only make sense for static objects as it is overwritten by position changes otherwise
		inline const btTransform& getTransform() const;
		inline void setTransform(const btTransform& transform);

		// Return the managed bullet collision object
		inline const btCollisionObject& getWrappedBulletCollisionObject() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CollisionComponent methods        ]
	//[-------------------------------------------------------]
	public:
		virtual bool getAsSphere(glm::vec3& center, float& radius) const override;
		virtual bool getAsAxisAlignedBoundingBox(glm::vec3& corner1, glm::vec3& corner2) const override;
		virtual bool getAsOrientedBoundingBox(glm::vec3& anchorPoint, glm::quat& rotation, glm::vec3& extensions) const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BulletCollisionComponent methods ]
	//[-------------------------------------------------------]
	protected:
		// Convert between glm and bullet position information.
		// This is more than simply copying between the vector3 formats because of the different position semantics.
		// See qsf/math/GlmBulletConversion for details.
		//@{
		virtual glm::vec3 convertPosition(const btVector3& bulletPosition) const = 0;
		virtual btVector3 convertPosition(const glm::vec3& qsfPosition) const = 0;
		//@}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onCollisionFilterGroupOrCollisionMaskChanged() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void onSetDebug(bool debug) override;
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		// Register the collision object with the bullet world
		// Returns whether everything succeeded and the object is now known as a physics obstacle.
		// Is usually called during onStartup
		bool registerCollisionObject();

		// Remove the collision object from the bullet world.
		// Silently does nothing if the object was not registered at all
		void unregisterCollisionObject();

		// optional late initialization mode if it was not set via the constructor.
		// The shape has to be set during onStartup at the latest
		void setCollisionShape(btCollisionShape* collisionShape);

		// Minor helper that removes the collision object and inserts it again after a property change that can't be set dynamically.
		void reAddObject();

		PhysicsWorldComponent* getPhysicsWorldComponent() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// helper function to determine whether an object is static from the point of view of the physics simulation
		bool isStatic() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		btCollisionObject mBulletObject;	///< The wrapped bullet object instance
		bool			  mIsRegistered;	///< Track whether the bullet object was already registered to the bullet world.


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BulletCollisionComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCollisionComponent-inl.h"
