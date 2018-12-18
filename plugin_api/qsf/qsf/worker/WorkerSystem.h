// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/worker/ThreadPool.h"

#include <boost/container/flat_map.hpp>

#include <mutex>
#include <list>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class WorkerQueueBase;
	class WorkerTask;
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
	*    TODO(co) Comment me
	*/
	class QSF_API_EXPORT WorkerSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend WorkerQueueBase;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		WorkerSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WorkerSystem();

		/**
		*  @brief
		*    Get the globally usable data parallel thread pool
		*
		*  @remarks
		*    Use this globally usable thread pool whenever you need to perform data parallel.
		*
		*  @return
		*    The globally usable data parallel thread pool
		*
		*  @note
		*    - Also works when the worker system is currently not running
		*/
		ThreadPool<void>& getDataParallelThreadPool();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the name of the system
		*
		*  @return
		*    Null terminated ASCII name of the system, never a null pointer, do not manipulate or destroy the returned name
		*/
		virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the system
		*
		*  @param[in] serverMode
		*    "true" if the system should start in server mode, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Server mode means that e.g. there's no need for audio & visual stuff because there's no display or sound hardware in the first place
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the system
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		bool registerWorkerQueue(uint32 workerId, WorkerQueueBase* worker);
		void unregisterWorkerQueue(uint32 workerId);
		void notifyNewTask(uint32 workerId);
		std::shared_ptr<WorkerTask> getNextTask();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		class WorkerThreadPool;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::container::flat_map<uint32, WorkerQueueBase*> mWorkerQueueMap;
		std::list<uint32>									 mWorkerQueuesWithTasks;
		std::mutex											 mWorkerQueueMutex;
		WorkerThreadPool&									 mWorkerThreadPool;
		ThreadPool<void>									 mDataParallelThreadPool;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
