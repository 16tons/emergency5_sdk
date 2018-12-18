// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MovableObjectRenderingListener::MovableObjectRenderingListener() :
		mAllowRegistration(true),
		mNumberOfConnections(0),
		mState(SLEEPING),
		mLastConnectionComponentId(0xffffffff)	// = getUninitialized<uint32>(), but this would require another include
	{
		// Nothing to do in here
	}

	inline bool MovableObjectRenderingListener::isMovableObjectRenderingListenerRegistered() const
	{
		return (mNumberOfConnections > 0);
	}

	inline MovableObjectRenderingListener::State MovableObjectRenderingListener::getState() const
	{
		return mState;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	inline Component* MovableObjectRenderingListener::getMovableObjectRenderingListenerComponent()
	{
		return nullptr;
	}

	inline void MovableObjectRenderingListener::onObjectDestroyed()
	{
		if (AWAKE == mState)
		{
			onObjectRenderingShutdown();
		}
	}

	inline void MovableObjectRenderingListener::onObjectRenderingStartup()
	{
		// Nothing to do in here
	}

	inline void MovableObjectRenderingListener::onObjectRendering(const Component&, const Ogre::Camera&)
	{
		// Nothing to do in here
	}

	inline void MovableObjectRenderingListener::onObjectRenderingShutdown()
	{
		mState = SLEEPING;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
