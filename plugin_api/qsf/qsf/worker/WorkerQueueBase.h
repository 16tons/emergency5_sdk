// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"

#include <boost/noncopyable.hpp>

#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class WorkerSystem;
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
	class QSF_API_EXPORT WorkerQueueBase : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend WorkerSystem;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief Default constructor
		*/
		WorkerQueueBase();

		/**
		*  @brief Destructor
		*/
		virtual ~WorkerQueueBase();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::WorkerQueueBase methods           ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief Return "true" if a call to getNextTask() will return a task
		*/
		virtual bool isTaskPending() const = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief Register this worker queue to the worker system using the given unique worker identifier
		*
		*  @return "true" if this worker queue was successfully registered to the worker system, "false" otherwise
		*/
		bool registerWorker(const StringHash& workerId);

		/**
		*  @brief Unregister this worker queue from the worker system
		*
		*  @note
		*    - This method logs an error if this worker queue wasn't previously registered
		*/
		void unregisterWorker();

		/**
		*  @brief Returns "true" if this worker queue is currently registered to the worker system, "false" otherwise
		*
		*  @return "true" if this worker queue is currently registered to the worker system, "false" otherwise
		*/
		bool isWorkerRegistered() const;

		/**
		*  @brief Notifies the worker system that it should call getNextTask() until isTaskPending() returns "false"
		*
		*  @note
		*    - It is safe to call this method even if we aren't certain if we really have a task to be executed, but since a lock is involved it isn't free
		*    - getNextTask() will be called at least once after this method was called regardless of the return value of isTaskPending()
		*/
		void notifyWorkerSystemAboutNewTask() const;

		/**
		*  @brief Returns the worker system instance
		*
		*  @return the worker system instance, always valid, do not destroy the instance
		*/
		WorkerSystem& getWorkerSystem() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::WorkerQueueBase methods        ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief Returns the next task to be executed
		*
		*  @return the next task to be executed or "nullptr", if there is no task available for execution
		*/
		virtual std::shared_ptr<WorkerTask> getNextTask() = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WorkerSystem& mWorkerSystem;
		uint32		  mWorkerId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
