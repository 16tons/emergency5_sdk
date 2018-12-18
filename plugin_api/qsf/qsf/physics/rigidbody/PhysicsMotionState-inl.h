// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PhysicsMotionState::~PhysicsMotionState()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual btMotionState methods                  ]
	//[-------------------------------------------------------]
	inline void PhysicsMotionState::getWorldTransform(btTransform& worldTrans) const
	{
		worldTrans = mBulletWorldTransform;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
