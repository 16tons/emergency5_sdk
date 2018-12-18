// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/BufferedManager.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"
#include "qsf/base/NamedIdentifier.h"
#include "qsf/job/JobProxy.h"
#include "qsf/job/JobDefinitions.h"

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	#ifdef QSF_PROFILING
		class ManagedProfilingElement;
	#endif
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
	*    Job manager class
	*
	*  @note
	*    - The job manager is a scheduler
	*    - We use the term "job" instead of "task" to avoid a naming clash with gameplay mission/quest/task terminology
	*    - The job manager does not support multi-threading in any way
	*/
	class QSF_API_EXPORT JobManager : private BufferedManager<jobs::JobId, jobs::JobDescriptor, std::map<jobs::JobId, jobs::JobDescriptor>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class JobSystem;	///< Job system may create instances, constructor is private
	friend class JobProxy;	///< Job proxies need to register/unregister themselves


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return job manager's unique ID (result of hashed name)
		*
		*  @return
		*    Unique ID of the job manager
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return job manager's unique name
		*
		*  @return
		*    Name of the job manager
		*/
		inline const std::string& getName() const;

		//[-------------------------------------------------------]
		//[ Update                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Explicitly update the job manager
		*
		*  @param[in] clock
		*    A clock instance that delivers timing information like the time passed
		*/
		void update(const Clock& clock);

		/**
		*  @brief
		*    Return whether or not the job manager is updated automatically
		*
		*  @return
		*    "true" if the job manager is updated automatically, else "false"
		*/
		inline bool isAutoupdate() const;

		/**
		*  @brief
		*    Set whether or not the job manager is updated automatically
		*
		*  @param[in] autoupdate
		*    "true" if the job manager is updated automatically, else "false"
		*
		*  @return
		*    "true" if all went fine or there was no state change, "false" on error (e.g. the job manager has no provider job manager updating it)
		*
		*  @note
		*    - Autoupdate is only possible if the job manager has a provider job manager updating it
		*    - By default, job managers are updated automatically when they were created with a provider job manager
		*/
		bool setAutoupdate(bool autoupdate);

		/**
		*  @brief
		*    Return provider job manager ID
		*
		*  @return
		*    The provider job manager ID, which can be "qsf::getUninitialized<uint32>()"
		*/
		inline uint32 getProviderJobManagerId() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BufferedManager methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onElementRemoval(const ElementKey& key, Element& element) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    Unique name of the job manager
		*  @param[in] providerJobManagerName
		*    Unique name of the job manager were the job proxy of the new job manager connects to in order to receive updates, can be "qsf::getUninitialized<qsf::NamedIdentifier>()"
		*  @param[in] ordering
		*    Ordering of the job manager in its provider job manager
		*
		*  @note
		*    - Private method because only job system may create job manager instances
		*/
		JobManager(const NamedIdentifier& name, const NamedIdentifier& providerJobManagerName, jobs::Ordering ordering = jobs::ORDERING_DEFAULT);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Private method because only job system may destroy job manager instances
		*/
		virtual ~JobManager();

		/**
		*  @brief
		*    Register a job
		*
		*  @param[in] jobProxy
		*    Reference to a job proxy instance to be connected with the job
		*  @param[in] function
		*    Function binding to the method to be called as a job
		*  @param[in] configuration
		*    Optional job configuration structure with additional settings for the job
		*
		*  @return
		*    "true" if all went fine, else "false" (job already registered?)
		*/
		bool registerJob(JobProxy& jobProxy, const jobs::JobFunctionBinding& function, const jobs::JobConfiguration& configuration = jobs::JobConfiguration());

		/**
		*  @brief
		*    Unregister a job; throws an exception on fatal error
		*
		*  @param[in] jobProxy
		*    Job proxy instance to unregister
		*
		*  @return
		*    "true" if all went fine, else "false" (job not registered?)
		*/
		bool unregisterJob(JobProxy& jobProxy);

		/**
		*  @brief
		*    Unregister all jobs
		*/
		void unregisterAllJobs();

		//[-------------------------------------------------------]
		//[ Configuration for job proxies                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the current job configuration
		*
		*  @param[in] jobProxy
		*    Job proxy instance whose configuration to change
		*
		*  @return
		*    The job configuration instance
		*/
		const jobs::JobConfiguration& getJobConfiguration(const JobProxy& jobProxy) const;

		/**
		*  @brief
		*    Set the time between update calls inside the job configuration
		*
		*  @param[in] jobProxy
		*    Job proxy instance whose configuration to change
		*  @param[in] newTimeBetweenCalls
		*    Time between calls to set
		*/
		void changeTimeBetweenCalls(const JobProxy& jobProxy, const Time& newTimeBetweenCalls);

		/**
		*  @brief
		*    Force an update of the given job proxy next time, ignoring time between updates once
		*
		*  @param[in] jobProxy
		*    Job proxy instance to force an update for
		*/
		void forceUpdateNextTime(const JobProxy& jobProxy);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		jobs::JobDescriptor& getJobByProxy(const JobProxy& jobProxy);
		const jobs::JobDescriptor& getJobByProxy(const JobProxy& jobProxy) const;

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Internal update of a single job
		*/
		void updateSingleJob(jobs::JobDescriptor& job, JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		NamedIdentifier	mName;								///< Name and ID of this job manager, must be unique
		StringHash		mProviderJobManagerId;				///< Unique ID of the job manager were the job proxy of the new job manager connects to in order to receive updates, can be "qsf::getUninitialized<uint32>()"
		jobs::Ordering	mProviderJobManagerOrdering;		///< Ordering value inside the provider job manager
		JobProxy		mJobProxy;							///< Job proxy

		ConsecutiveIdGenerator<jobs::JobId> mIdGenerator;	///< Job ID generator

		#ifdef QSF_PROFILING
			// Profiling
			ManagedProfilingElement* mProfilingElement;		///< Profiling element collecting time measurements; can be a null pointer, we're responsible for destroying the instance
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/JobManager-inl.h"
