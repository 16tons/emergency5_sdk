// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsTerrainComponent::PhysicsTerrainComponent(Prototype* prototype) :
		PhysicsComponent(prototype)
	{
		// Set a different default for rolling friction for the terrain
		mRollingFriction = 0.025f;
	}

	inline PhysicsTerrainComponent::~PhysicsTerrainComponent()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
