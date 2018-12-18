// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline PathSearch::PathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> searchConfig) :
			NavigationTask(NavigationTask::PATH_SEARCH, taskId),
			mSearchConfig(searchConfig),
			mTimeUsed(Time::ZERO)
		{
			QSF_CHECK(mSearchConfig.get(), "Passing a nullptr as search configuration is not supported",
				QSF_REACT_THROW);
		}

		inline PathSearchConfiguration& PathSearch::getSearchConfiguration()
		{
			return *mSearchConfig;
		}

		inline void PathSearch::setTimeUsed(Time time)
		{
			mTimeUsed = time;
		}

		inline Time PathSearch::getTimeUsed() const
		{
			return mTimeUsed;
		}
	}
}
