// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsRagdollComponent::PhysicsRagdollComponent(Prototype* prototype) :
		PhysicsComponent(prototype)
	{
		// Nothing to do in here
	}

	inline PhysicsRagdollComponent::~PhysicsRagdollComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& PhysicsRagdollComponent::getRagdollPrefab() const
	{
		return mRagdollPrefab;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
