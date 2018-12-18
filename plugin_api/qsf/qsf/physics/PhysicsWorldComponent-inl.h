// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const glm::vec3& PhysicsWorldComponent::getGravity() const
	{
		return mGravity;
	}

	inline void PhysicsWorldComponent::setTargetFramesPerSecond(float targetFramesPerSecond)
	{
		mTargetFramesPerSecond = targetFramesPerSecond;
	}

	inline void PhysicsWorldComponent::registerCollisionComponent(BulletCollisionComponent& collisionComponent)
	{
		CollisionComponentAdded(reinterpret_cast<CollisionComponent&>(collisionComponent));
	}

	inline void PhysicsWorldComponent::unregisterCollisionComponent(BulletCollisionComponent& collisionComponent)
	{
		CollisionComponentRemoved(reinterpret_cast<CollisionComponent&>(collisionComponent));
	}

	inline const PhysicsWorldComponent::CollidingComponentsMultimap& PhysicsWorldComponent::getCurrentCollisions() const
	{
		return mCurrentlyCollidingEntities;
	}

	inline bool PhysicsWorldComponent::hasEnqueuedUpdates() const
	{
		return !mEnqueuedUpdates.empty();
	}

	inline void PhysicsWorldComponent::enqueueBulletWorldTransformChange(PhysicsMotionState& physicsMotionState)
	{
		mEnqueuedBulletWorldTransformChanges.push_back(&physicsMotionState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline void PhysicsWorldComponent::onSetSimulating(bool)
	{
		updateJobProxyRegistration();
	}

	inline void PhysicsWorldComponent::onSetActive(bool)
	{
		updateJobProxyRegistration();
	}

	inline void PhysicsWorldComponent::onSetDebug(bool debug)
	{
		setShowCollision(debug);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void PhysicsWorldComponent::enqueuePhysicsUpdate(const JobArguments& jobArguments)
	{
		// TODO(co) Horrible emergency EMERGENCY 5 hack to reduce the impact of the physics update (see "qsf::RendererSystem")
		//  -> Instead of directly updating physics here, we just enqueue an update to do later -- in parallel to rendering
		mEnqueuedUpdates.push_back(jobArguments);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
