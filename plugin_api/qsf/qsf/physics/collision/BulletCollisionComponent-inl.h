// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"

#include <LinearMath/btTransform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BulletCollisionComponent::BulletCollisionComponent(Prototype* prototype, CollisionComponent::Type type, btCollisionShape* collisionShape) :
		CollisionComponent(prototype, type),
		mIsRegistered(false)
	{
		mBulletObject.setCollisionShape(collisionShape); // Even setting a nullptr here by design
		mBulletObject.setUserPointer(this);
	}

	inline int BulletCollisionComponent::getCollisionFlags() const
	{
		return mBulletObject.getCollisionFlags();
	}

	inline void BulletCollisionComponent::setCollisionFlags(int flags)
	{
		mBulletObject.setCollisionFlags(flags);
	}

	inline const btTransform& BulletCollisionComponent::getTransform() const
	{
		return mBulletObject.getWorldTransform();
	}

	inline void BulletCollisionComponent::setTransform(const btTransform& transform)
	{
		mBulletObject.setWorldTransform(transform);
	}

	inline const btCollisionObject& BulletCollisionComponent::getWrappedBulletCollisionObject() const
	{
		return mBulletObject;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
