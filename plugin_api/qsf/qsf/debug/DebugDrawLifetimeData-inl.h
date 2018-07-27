// Copyright (C) 2012-2017 Promotion Software GmbH


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
	inline DebugDrawLifetimeData::DebugDrawLifetimeData() :
		mJobManagerId(getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	inline DebugDrawLifetimeData::DebugDrawLifetimeData(Time duration) :
		mDuration(duration >= Time::ZERO ? duration : boost::optional<Time>()), // TODO(vs) bad idea - silently ignoring some value ... use an Unsigned Time data type instead!
		mJobManagerId(getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	inline DebugDrawLifetimeData::DebugDrawLifetimeData(Time duration, const StringHash& jobManagerId) :
		mDuration(duration >= Time::ZERO ? duration : boost::optional<Time>()), // TODO(vs) bad idea - silently ignoring some value ... use an Unsigned Time data type instead!
		mJobManagerId(jobManagerId)
	{
		// Nothing to do in here
	}

	inline bool DebugDrawLifetimeData::isDurationLimited() const
	{
		return mDuration.is_initialized();
	}

	inline Time DebugDrawLifetimeData::getDuration() const
	{
		return *mDuration;
	}

	inline uint32 DebugDrawLifetimeData::getJobManagerId() const
	{
		return mJobManagerId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
