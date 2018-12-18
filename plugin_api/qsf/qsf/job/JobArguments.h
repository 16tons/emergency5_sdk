// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobProxy;
	class Clock;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Job arguments class: holds data to be passed to the called job method by the job manager
	*/
	class QSF_API_EXPORT JobArguments
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class JobManager;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Constructor */
		inline JobArguments();

		/** Constructor */
		inline JobArguments(const JobArguments& jobArguments, const Time& timePassed);

		/** Get the clock */
		inline const Clock& getClock() const;

		/** Get the time passed since last clock update */
		inline const Time& getTimePassed() const;

		/** Get the time passed since last clock update in seconds */
		inline float getSecondsPassed() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Clock* mClock;		///< Clock the job update is using, always valid, do not destroy the instance
		Time		 mTimePassed;	///< Time passed since the last update; this is not necessarily identical to the clock's passed time
		JobProxy*	 mJobProxy;		///< Job proxy the job arguments are meant for, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/JobArguments-inl.h"
