// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/WeakPtr.h"
#include "qsf/component/base/TransformComponent.h"

#include <LinearMath/btMotionState.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PhysicsWorldComponent;
	class PhysicsRigidBodyComponent;
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
	*    Bullet physics motion state
	*/
	class PhysicsMotionState : public btMotionState
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PhysicsWorldComponent;	// Needs access for dispatch


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		PhysicsMotionState(PhysicsWorldComponent& physicsWorldComponent, const btTransform& bulletWorldTransform, PhysicsRigidBodyComponent& physicsRigidBodyComponent);
		inline virtual ~PhysicsMotionState();


	//[-------------------------------------------------------]
	//[ Public virtual btMotionState methods                  ]
	//[-------------------------------------------------------]
	public:
		inline virtual void getWorldTransform(btTransform& worldTrans) const override;
		virtual void setWorldTransform(const btTransform& worldTrans) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PhysicsWorldComponent&		mPhysicsWorldComponent;
		PhysicsRigidBodyComponent&	mPhysicsRigidBodyComponent;
		WeakPtr<TransformComponent>	mTransformComponent;
		btTransform					mBulletWorldTransform;
		bool						mBulletWorldTransformChangeEnqueued;	///< Due to simulation sub-steps, it's possible that "btMotionState::setWorldTransform" is called multiple times, compensate this so it's enqueued only ones inside the list of Bullet world transform changes


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/rigidbody/PhysicsMotionState-inl.h"
