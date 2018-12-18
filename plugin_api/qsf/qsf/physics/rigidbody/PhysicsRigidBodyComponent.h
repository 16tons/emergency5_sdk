// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsComponent.h"
#include "qsf/reflection/type/CampGlmVec3.h"
#include "qsf/reflection/type/CampGlmQuat.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btVector3;
class btTransform;
class btRigidBody;
class btMotionState;
class btCompoundShape;
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
	*    Physics rigid body component class
	*
	*  @note
	*    - "qsf::BulletCollisionComponent" is for static or via game logic moved objects while "qsf::PhysicsRigidBodyComponent" is for dynamic physics moved objects
	*/
	class QSF_API_EXPORT PhysicsRigidBodyComponent : public PhysicsComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PhysicsWorldComponent;	// Needs to access "qsf::PhysicsRigidBodyComponent::mListen"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsRigidBodyComponent" unique component ID
		static const uint32 MASS;			///< "Mass" unique class property ID inside the class
		static const uint32 LOCAL_POSITION;	///< "LocalPosition" unique class property ID inside the class
		static const uint32 LOCAL_ROTATION;	///< "LocalRotation" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit PhysicsRigidBodyComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsRigidBodyComponent();

		/**
		*  @brief
		*    Return the mass
		*
		*  @return
		*    The mass
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMass() const;

		/**
		*  @brief
		*    Set the mass
		*
		*  @param[in] mass
		*    The mass to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMass(float mass);

		/**
		*  @brief
		*    Return the restitution
		*
		*  @return
		*    The restitution
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRestitution() const;

		/**
		*  @brief
		*    Set the restitution
		*
		*  @param[in] restitution
		*    The restitution to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRestitution(float restitution);

		/**
		*  @brief
		*    Return the linear damping
		*
		*  @return
		*    The linear damping
		*/
		inline float getLinearDamping() const;

		/**
		*  @brief
		*    Set linear damping
		*
		*  @param[in] linearDamping
		*    The linear damping to set
		*/
		void setLinearDamping(float linearDamping);

		/**
		*  @brief
		*    Return the angular damping
		*
		*  @return
		*    The angular damping
		*/
		inline float getAngularDamping() const;

		/**
		*  @brief
		*    Set angular damping
		*
		*  @param[in] angularDamping
		*    The angular damping to set
		*/
		void setAngularDamping(float angularDamping);

		//[-------------------------------------------------------]
		//[ Local transform                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the local position
		*
		*  @return
		*    The position, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getLocalPosition() const;

		/**
		*  @brief
		*    Set the local position
		*
		*  @param[in] localPosition
		*    The local position
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLocalPosition(const glm::vec3& localPosition);

		/**
		*  @brief
		*    Return the local rotation
		*
		*  @return
		*    The rotation, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::quat& getLocalRotation() const;

		/**
		*  @brief
		*    Set the local rotation
		*
		*  @param[in] localRotation
		*    The local rotation quaternion
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLocalRotation(const glm::quat& localRotation);

		/**
		*  @brief
		*    Set the damping values
		*
		*  @param[in] linearDamping
		*    The linear damping value
		*  @param[in] angularDamping
		*    The angular damping value
		*
		*  @note
		*    - This changes only for the currently created rigid body the damping values
		*      These values get lost when the rigid body get recreated after this method was called or wasn't created when this method is called
		*/
		void setDamping(float linearDamping, float angularDamping);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the Bullet collision shape
		*
		*  @return
		*    The Bullet collision shape, can be a null pointer, do not destroy the instance
		*
		*  @note
		*    - Don't use Bullet directly if you don't have to
		*/
		inline btCollisionShape* getBulletCollisionShape() const;

		/**
		*  @brief
		*    Return the Bullet rigid body
		*
		*  @return
		*    The Bullet rigid body, can be a null pointer, do not destroy the instance
		*
		*  @note
		*    - Don't use Bullet directly if you don't have to
		*/
		inline btRigidBody* getBulletRigidBody() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsRigidBodyComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual btCollisionShape* createBulletCollisionShape() const = 0;
		virtual btMotionState* createMotionState(PhysicsWorldComponent& physicsWorldComponent, const btTransform& transform);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void onSetDebug(bool debug) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onCollisionFilterGroupOrCollisionMaskChanged() override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		btCollisionShape*	mBulletCollisionShape;	///< Bullet collision shape, can be a null pointer, destroy the instance if you no longer need it
		btCompoundShape*	mBulletCompoundShape;	///< Bullet compound shape, only used if there's a local transform, can be a null pointer, destroy the instance if you no longer need it
		btMotionState*		mBulletMotionState;		///< Bullet motion state can be a null pointer, destroy the instance if you no longer need it
		btRigidBody*		mBulletRigidBody;		///< Bullet rigid body, can be a null pointer, destroy the instance if you no longer need it
		bool				mListen;

		float				mRestitution;
		float				mLinearDamping;
		float				mAngularDamping;

		// Connected to the CAMP reflection system
		float	  mMass;			///< Mass
		glm::vec3 mLocalPosition;	///< Local position
		glm::quat mLocalRotation;	///< Local rotation


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		PhysicsWorldComponent* getPhysicsWorldComponent() const;
		void calculateLocalInertia(btVector3& bulletVector3) const;
		void createBulletRigidBody(PhysicsWorldComponent& physicsWorldComponent);
		void destroyBulletRigidBody(PhysicsWorldComponent& physicsWorldComponent);
		void recreateBulletRigidBody();


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/rigidbody/PhysicsRigidBodyComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsRigidBodyComponent)
