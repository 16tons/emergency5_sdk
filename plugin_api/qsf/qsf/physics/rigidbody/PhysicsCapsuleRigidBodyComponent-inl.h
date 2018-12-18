// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsCapsuleRigidBodyComponent::PhysicsCapsuleRigidBodyComponent(Prototype* prototype) :
		PhysicsRigidBodyComponent(prototype),
		mRadius(0.0f),
		mHeight(0.0f)
	{
		// Nothing to do in here
	}

	inline PhysicsCapsuleRigidBodyComponent::~PhysicsCapsuleRigidBodyComponent()
	{
		// Nothing to do in here
	}

	inline float PhysicsCapsuleRigidBodyComponent::getRadius() const
	{
		return mRadius;

	}
	inline float PhysicsCapsuleRigidBodyComponent::getHeight() const
	{
		return mHeight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
