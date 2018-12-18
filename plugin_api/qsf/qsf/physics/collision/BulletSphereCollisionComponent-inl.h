// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BulletSphereCollisionComponent::BulletSphereCollisionComponent(Prototype* prototype) :
		BulletCollisionComponent(prototype, CollisionComponent::SPHERE, &mSphereShape),
		mSphereShape(DEFAULT_RADIUS)
	{
		// Nothing here
	}

	inline float BulletSphereCollisionComponent::getRadius() const
	{
		return mSphereShape.getRadius();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
