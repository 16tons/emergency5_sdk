// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Proxy.h"
#include "qsf/base/StringHash.h"	// No forward declaration by intent
#include "qsf/job/JobDefinitions.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobManager;
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
	*    Job proxy class
	*
	*  @note
	*    - Job proxies have two major tasks:
	*       * They are a container for data connected to a job, namely the job manager where it is registered and the job's ID there.
	*       * They offer a more comfortable and secure access to a job manager's functions because some information needed to call
	*         the manager is already stored in the proxy (e.g. the job manager to call for unregistration).
	*    - Best practise: Declare a job proxy directly as a part of the class you want to be regularly called as a job (-> composition),
	*       avoid dynamic allocation (new JobProxy) and never define a function-local job proxy. The reason is the job's lifetime
	*       being connected to the proxy's lifetime: When the proxy is destroyed, the job is automatically unregistered.
	*       This is done mainly for security reasons, to avoid calling of a method of an object that was already destroyed.
	*/
	class QSF_API_EXPORT JobProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class JobManager;	///< Job manager may change private data


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JobProxy();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~JobProxy();

		/**
		*  @brief
		*    Return job manager or null pointer if proxy is invalid, do not destroy the instance
		*/
		inline JobManager* getJobManager() const;

		/**
		*  @brief
		*    Returns false if proxy is not registered or the job has become invalid
		*/
		inline bool isValid() const;

		/**
		*  @brief
		*    Return job's ID or "qsf::getUninitialized<uint32>()" if proxy is invalid
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Register at a job manager; throws an exception on fatal error
		*
		*  @param[in] jobManagerId
		*    Unique ID of the job manager
		*  @param[in] function
		*    A function binding to the method to be called as a job
		*  @param[in] configuration
		*    Optional job configuration structure with additional settings for the job
		*
		*  @return
		*    "true" if registration was successful, "false" if the job manager does not exist
		*/
		bool registerAt(const StringHash& jobManagerId, const jobs::JobFunctionBinding& function, const jobs::JobConfiguration& configuration = jobs::JobConfiguration());

		/**
		*  @brief
		*    Unregister at current job manager
		*
		*  @return
		*    "true" if unregistration was successful, "false" if job proxy was not registered at all
		*/
		inline bool unregister();

		//[-------------------------------------------------------]
		//[ Configuration                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the current job configuration
		*
		*  @return
		*    The job configuration; has uninitialized values if the job proxy is not registered
		*/
		const jobs::JobConfiguration& getJobConfiguration() const;

		/**
		*  @brief
		*    Set the time between update calls inside the job configuration
		*
		*  @param[in] newTimeBetweenCalls
		*    Time between calls to set
		*/
		void changeTimeBetweenCalls(const Time& newTimeBetweenCalls);

		/**
		*  @brief
		*    Force an update of the given job proxy next time, ignoring time between updates once
		*/
		void forceUpdateNextTime();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void invalidate();
		bool unregisterInternal();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		JobManager* mJobManager;	///< The job manager instance this job proxy is assigned to, can be a null pointer, do not destroy the instance
		jobs::JobId	mJobId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy-inl.h"
