// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsCylinderRigidBodyComponent::PhysicsCylinderRigidBodyComponent(Prototype* prototype) :
		PhysicsRigidBodyComponent(prototype),
		mRadius(0.0f),
		mHeight(0.0f)
	{
		// Nothing to do in here
	}

	inline PhysicsCylinderRigidBodyComponent::~PhysicsCylinderRigidBodyComponent()
	{
		// Nothing to do in here
	}

	inline float PhysicsCylinderRigidBodyComponent::getRadius() const
	{
		return mRadius;

	}
	inline float PhysicsCylinderRigidBodyComponent::getHeight() const
	{
		return mHeight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
