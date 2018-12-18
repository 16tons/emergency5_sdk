// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline JobProxyMovableObjectRenderingListener::JobProxyMovableObjectRenderingListener() :
		mNonvisibleUpdateTimeout(Time::fromSeconds(2.0f)),
		mUpdatesSinceLastVisible(0)
	{
		// Nothing to do in here
	}

	inline JobProxyMovableObjectRenderingListener::~JobProxyMovableObjectRenderingListener()
	{
		// Nothing to do in here
	}

	inline const Time& JobProxyMovableObjectRenderingListener::getNonvisibleUpdateTimeout() const
	{
		return mNonvisibleUpdateTimeout;
	}

	inline void JobProxyMovableObjectRenderingListener::setNonvisibleUpdateTimeout(const Time& nonvisibleUpdateTimeout)
	{
		mNonvisibleUpdateTimeout = nonvisibleUpdateTimeout;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
