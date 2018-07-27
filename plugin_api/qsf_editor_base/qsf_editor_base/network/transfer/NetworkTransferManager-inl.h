// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline NetworkTransferManager::NetworkTransferManager()
			{
				// Nothing to do in here
			}

			inline NetworkTransferManager::~NetworkTransferManager()
			{
				// Nothing to do in here
			}

			inline bool NetworkTransferManager::scheduleTask(std::shared_ptr<WorkerTask> task)
			{
				return mWorkerQueue.scheduleTask(task);
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
