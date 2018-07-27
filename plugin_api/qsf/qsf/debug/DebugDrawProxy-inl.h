// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DebugDrawProxy::DebugDrawProxy() :
		mDebugDrawObject(nullptr)
	{
		// Nothing to do in here
	}

	inline DebugDrawProxy::~DebugDrawProxy()
	{
		// Unregister at manager (if registered at all)
		unregister();
	}

	inline bool DebugDrawProxy::isValid() const
	{
		return (nullptr != mDebugDrawObject);
	}

	inline bool DebugDrawProxy::unregister()
	{
		// It is ok to try to unregister a message that has not been registered;
		// e.g. because the destructor calls this method
		if (nullptr == mDebugDrawObject)
		{
			return false;
		}

		// Unregister at manager
		return unregisterInternal();
	}

	inline bool DebugDrawProxy::setTransform(const Transform& transform) const
	{
		// Is there an associated debug draw object instance?
		if (nullptr == mDebugDrawObject)
			return false;

		// Set the new transformation
		setTransformInternal(transform);
		return true;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void DebugDrawProxy::invalidate()
	{
		// Reset internal state
		mDebugDrawObject = nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
