// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline JobProxy::JobProxy() :
		mJobManager(nullptr),
		mJobId(getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	inline JobProxy::~JobProxy()
	{
		// Unregister at manager (if registered at all)
		unregister();
	}

	inline bool JobProxy::isValid() const
	{
		return isInitialized(mJobId);
	}

	inline JobManager* JobProxy::getJobManager() const
	{
		return mJobManager;
	}

	inline uint32 JobProxy::getId() const
	{
		return mJobId;
	}

	inline bool JobProxy::unregister()
	{
		// It is ok to try to unregister a job that has not been registered;
		// e.g. because the destructor calls this method
		if (nullptr == mJobManager)
			return false;

		// Unregister at the manager
		return unregisterInternal();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void JobProxy::invalidate()
	{
		// Reset internal state
		mJobManager = nullptr;
		setUninitialized(mJobId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
