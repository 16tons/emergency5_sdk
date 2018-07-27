// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BulletCylinderCollisionComponent::BulletCylinderCollisionComponent(Prototype* prototype) :
		BulletCollisionComponent(prototype, CollisionComponent::SPHERE, &mCylinderShape),
		mRadius(BulletCylinderCollisionComponent::DEFAULT_RADIUS),
		mHeight(BulletCylinderCollisionComponent::DEFAULT_HEIGHT),
		mCylinderShape(btVector3(BulletCylinderCollisionComponent::DEFAULT_RADIUS, BulletCylinderCollisionComponent::DEFAULT_HEIGHT, BulletCylinderCollisionComponent::DEFAULT_RADIUS))
	{
		// Nothing here
	}

	inline float BulletCylinderCollisionComponent::getRadius() const
	{
		return mRadius;
	}

	inline float BulletCylinderCollisionComponent::getHeight() const
	{
		return mHeight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
