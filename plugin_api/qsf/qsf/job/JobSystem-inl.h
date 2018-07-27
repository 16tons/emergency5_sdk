// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline JobSystem::JobSystem() :
		mDefaultManager(nullptr)
	{
		// Nothing to do in here
	}

	inline JobSystem::~JobSystem()
	{
		// All job managers should have been destroyed already in the onShutdown() call
		QSF_ASSERT(mJobManagers.empty(), "Job system is destroyed, but there are job managers remaining", QSF_REACT_NONE);
	}

	inline JobManager* JobSystem::getJobManagerById(const StringHash& id) const
	{
		// Find the right job manager
		JobManagerMap::const_iterator iterator = mJobManagers.find(id);
		if (iterator == mJobManagers.cend())
		{
			// None found...
			return nullptr;
		}

		// Return the job manager found
		return iterator->second;
	}

	inline JobManager& JobSystem::getDefaultJobManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mDefaultManager, "The default job manager instance is invalid", QSF_REACT_THROW);
		return *mDefaultManager;
	}

	inline JobManager* JobSystem::getCurrentlyUpdatingJobManager() const
	{
		return (mJobManagerStack.empty()) ? nullptr : mJobManagerStack.top();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* JobSystem::getName() const
	{
		return "Job system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
