// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/job/JobProxy.h"
#include "qsf/base/NamedIdentifier.h"

#include <boost/container/flat_map.hpp>

#include <vector>
#include <stack>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobManager;
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
	*    Job system class
	*/
	class QSF_API_EXPORT JobSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class JobManager;	///< Job manager needs access to the job manager stack


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier DEFAULT_JOB_MANAGER_ID;	///< Default job manager name "Default"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JobSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JobSystem();

		/**
		*  @brief
		*    Get a job manager instance by its unique ID
		*
		*   @param[in] id
		*    Unique ID of the job manager instance to return
		*
		*   @return
		*    Requested job manager instance, null pointer on error, do not destroy the instance
		*/
		inline JobManager* getJobManagerById(const StringHash& id) const;

		/**
		*  @brief
		*    Create a new job manager; throws an exception if unique ID is already in use
		*
		*  @param[in] name
		*    Unique name of the new job manager
		*
		*  @return
		*    Reference to the new job manager, do not destroy the instance
		*/
		JobManager& createJobManager(const NamedIdentifier& name);

		/**
		*  @brief
		*    Create a new job manager; throws an exception if unique ID is already in use
		*
		*  @param[in] name
		*    Unique name of the new job manager
		*  @param[in] providerJobManagerName
		*    Unique name of the job manager where the job proxy of the new job manager connects to in order to receive updates
		*  @param[in] ordering
		*    Ordering of the job manager in its provider job manager
		*
		*  @return
		*    Reference to the new job manager, do not destroy the instance
		*/
		JobManager& createJobManager(const NamedIdentifier& name, const NamedIdentifier& providerJobManagerName, jobs::Ordering ordering = jobs::ORDERING_DEFAULT);

		/**
		*  @brief
		*    Destroy a job manager; throws an exception if unique ID is unknown
		*
		*  @param[in] id
		*    Unique ID of the job manager
		*
		*  @return
		*    true if the job manager was destroyed, false if none with the given ID was found
		*/
		bool destroyJobManagerById(const StringHash& id);

		/**
		*  @brief
		*    Get listing of job managers that use a given provider job manager
		*
		*  @param[in] providerJobManagerId
		*    Unique ID of the job manager that is a provider
		*  @param[out] outListing
		*    Where to output the listing of job managers pointers; the listing is not initially cleared by this method
		*  @param[in] activeOnly
		*    Set to "true" to get only job managers that are currently active
		*/
		void getJobManagersWithProvider(const StringHash& providerJobManagerId, std::vector<JobManager*>& outListing, bool activeOnly = false);

		//[-------------------------------------------------------]
		//[ Default                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get default job manager instance
		*
		*   @return
		*    The default job manager instance, do not destroy the instance
		*/
		inline JobManager& getDefaultJobManager() const;

		//[-------------------------------------------------------]
		//[ Update                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Update default job manager
		*/
		void updateDefaultJobManager(const Clock& clock) const;

		/**
		*  @brief
		*    Return whether or not a job manager is updated automatically
		*
		*  @param[in] id
		*    Unique ID of the job manager
		*
		*  @return
		*    "true" if the job manager is updated automatically, else "false" (also returned for unknown job manager IDs)
		*
		*  @note
		*    - Ease of use method
		*/
		bool isJobManagerAutoupdate(const StringHash& id) const;

		/**
		*  @brief
		*    Set whether or not a job manager is updated automatically
		*
		*  @param[in] id
		*    Unique ID of the job manager
		*  @param[in] autoupdate
		*    "true" if the job manager is updated automatically, else "false"
		*
		*  @return
		*    "true" if all went fine or there was no state change, "false" on error (e.g. the job manager has no provider job manager updating it, also returned for unknown job manager IDs)
		*
		*  @note
		*    - Ease of use method
		*    - Autoupdate is only possible if the job manager has a provider job manager updating it
		*    - By default, job managers are updated automatically when they were created with a provider job manager
		*/
		bool setJobManagerAutoupdate(const StringHash& id, bool autoupdate) const;

		/**
		*  @brief
		*    Return job manager that is currently updating
		*
		*  @return
		*    Pointer to the job manager that is updating, or null pointer if there is none currently in its update process;
		*    in case there are nested updating job managers, the one most recently started is returned
		*/
		inline JobManager* getCurrentlyUpdatingJobManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, JobManager*> JobManagerMap;
		typedef std::stack<JobManager*> JobManagerStack;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobManager*		mDefaultManager;	///< Default job manager, can be a null pointer in case the job system is not started, yet
		JobManagerMap	mJobManagers;		///< All job managers, mapped by their names' string hashes
		JobManagerStack	mJobManagerStack;	///< Stack of currently running job managers


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/JobSystem-inl.h"
