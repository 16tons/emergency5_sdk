// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/worker/WorkerQueueBase.h"

#include <mutex>
#include <queue>


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
	*    Worker queue
	*
	*  @note
	*    - Meant for task-parallel use-cases
	*/
	class QSF_API_EXPORT WorkerQueue : public WorkerQueueBase
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend WorkerSystem;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		WorkerQueue();
		virtual ~WorkerQueue();

		bool registerWorker(const StringHash& workerId);
		void unregisterWorker(bool forceRunPendingJobs);

		bool scheduleTask(std::shared_ptr<WorkerTask> task);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::WorkerQueueBase methods           ]
	//[-------------------------------------------------------]
	public:
		virtual bool isTaskPending() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::WorkerQueueBase methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual std::shared_ptr<WorkerTask> getNextTask() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::queue<std::shared_ptr<WorkerTask>> mTasks;
		mutable std::mutex						mTasksMutex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
