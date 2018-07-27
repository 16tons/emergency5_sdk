// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsComponent::~PhysicsComponent()
	{
		// Nothing to do in here
	}

	inline short PhysicsComponent::getCollisionFilterGroup() const
	{
		return mCollisionFilterGroup;
	}

	inline short PhysicsComponent::getCollisionMask() const
	{
		return mCollisionMask;
	}

	inline uint32 PhysicsComponent::getCollisionTypeId() const
	{
		return mCollisionTypeId;
	}

	inline float PhysicsComponent::getFriction() const
	{
		return mFriction;
	}

	inline float PhysicsComponent::getRollingFriction() const
	{
		return mRollingFriction;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline PhysicsComponent::PhysicsComponent(Prototype* prototype) :
		Component(prototype),
		mCollisionFilterGroup(mDefaultCollisionFilterGroup),
		mCollisionMask(mDefaultCollisionMask),
		mCollisionTypeId(getUninitialized<uint32>()),
		mFriction(0.5f), // Default value from bullet
		mRollingFriction(0.025f) // A value != 0 so that an moving object with an applied rolling friction (value != 0) has an effect when rolling on this physics geometry)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
