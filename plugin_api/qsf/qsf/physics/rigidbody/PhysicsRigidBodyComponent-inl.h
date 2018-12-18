// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsRigidBodyComponent::PhysicsRigidBodyComponent(Prototype* prototype) :
		PhysicsComponent(prototype),
		mBulletCollisionShape(nullptr),
		mBulletCompoundShape(nullptr),
		mBulletMotionState(nullptr),
		mBulletRigidBody(nullptr),
		mListen(true),
		mRestitution(0.0f),
		mLinearDamping(0.2f), // Change some Bullet default values to have a usable default behaviour
		mAngularDamping(0.85f), // Change some Bullet default values to have a usable default behaviour
		mMass(0.0f)
	{
		// Nothing to do in here
	}

	inline PhysicsRigidBodyComponent::~PhysicsRigidBodyComponent()
	{
		// Nothing to do in here
	}

	inline float PhysicsRigidBodyComponent::getMass() const
	{
		return mMass;
	}

	inline float PhysicsRigidBodyComponent::getRestitution() const
	{
		return mRestitution;
	}

	inline float PhysicsRigidBodyComponent::getLinearDamping() const
	{
		return mLinearDamping;
	}

	inline float PhysicsRigidBodyComponent::getAngularDamping() const
	{
		return mAngularDamping;
	}

	inline const glm::vec3& PhysicsRigidBodyComponent::getLocalPosition() const
	{
		return mLocalPosition;
	}

	inline const glm::quat& PhysicsRigidBodyComponent::getLocalRotation() const
	{
		return mLocalRotation;
	}

	inline btCollisionShape* PhysicsRigidBodyComponent::getBulletCollisionShape() const
	{
		return mBulletCollisionShape;
	}

	inline btRigidBody* PhysicsRigidBodyComponent::getBulletRigidBody() const
	{
		return mBulletRigidBody;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
