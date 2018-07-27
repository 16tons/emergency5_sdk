// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsSphereRigidBodyComponent::PhysicsSphereRigidBodyComponent(Prototype* prototype) :
		PhysicsRigidBodyComponent(prototype),
		mRadius(0.0f)
	{
		// Nothing to do in here
	}

	inline PhysicsSphereRigidBodyComponent::~PhysicsSphereRigidBodyComponent()
	{
		// Nothing to do in here
	}

	inline float PhysicsSphereRigidBodyComponent::getRadius() const
	{
		return mRadius;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
