// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <boost/function.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
	class JobProxy;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace jobs
	{


		/**
		*	Ordering parameter is used to influence the order of calls, which is otherwise the order of insertion.
		*	To be exact, the job calls are executed in ascending order of the job IDs.
		*	Ordering manipulates the generation of job IDs so that higher (later) orders lead to much higher IDs.
		*	Jobs are guaranteed to be in ordered by their ordering parameters (except for the theoretical case that
		*	there are very many, namely over 800 million jobs of a ordering category inserted into a single job manager).
		*/
		enum Ordering
		{
			ORDERING_PRE = 0,	///< Ordering before all other jobs; use this ordering as little as possible
			ORDERING_EARLY,		///< Early job ordering priority, i.e. before all default priorized jobs
			ORDERING_DEFAULT,	///< Default job ordering priority
			ORDERING_LATE,		///< Late job ordering priority, i.e. after all default priorized jobs
			ORDERING_POST,		///< Ordering after all other jobs; use this ordering as little as possible
			_NUM_ORDERINGS
		};

		/** Internal job identifier */
		typedef uint32 JobId;

		/** Function binding signature for job callbacks */
		typedef void JobBindingSignature(const JobArguments&);

		/** Boost function binding definition for job callbacks */
		typedef boost::function<JobBindingSignature> JobFunctionBinding;

		/** Job configuration settings structure */
		struct JobConfiguration
		{
			Ordering			mOrdering;				///< Ordering priority of the job
			uint32				mNumberOfCalls;			///< Number of calls, or infinite if this is uninitialized
			Time				mTimeBetweenCalls;		///< Minimum time to wait between two calls

			inline JobConfiguration(Ordering ordering = ORDERING_DEFAULT);		// Not explicit by intent
		};

		/** Internal representation of a job */
		struct JobDescriptor
		{
			JobProxy*			mJobProxy;				///< Job proxy, always valid, do not destroy the instance
			JobFunctionBinding	mFunctionBinding;		///< Function callback bound to this job
			JobConfiguration	mJobConfiguration;		///< Job configuration
			bool				mValid;					///< If "false", the job should be removed as soon as safely possible
			bool				mForceUpdate;			///< If "true", next update will be triggered ignoring configuration's "mTimeBetweenCalls"
			uint32				mCallsRemaining;		///< Number of calls remaining, or infinite if this is uninitialized
			Time				mTimeSinceLastCall;		///< Time since the last call; zero by default

			inline JobDescriptor(JobProxy& jobProxy, const JobFunctionBinding& functionBinding, const JobConfiguration& jobConfiguration);
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // jobs
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/JobDefinitions-inl.h"
