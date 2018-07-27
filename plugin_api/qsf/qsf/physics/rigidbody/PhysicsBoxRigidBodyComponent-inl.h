// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsBoxRigidBodyComponent::PhysicsBoxRigidBodyComponent(Prototype* prototype) :
		PhysicsRigidBodyComponent(prototype)
	{
		// Nothing to do in here
	}

	inline PhysicsBoxRigidBodyComponent::~PhysicsBoxRigidBodyComponent()
	{
		// Nothing to do in here
	}

	inline const glm::vec3& PhysicsBoxRigidBodyComponent::getHalfExtents() const
	{
		return mHalfExtents;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
