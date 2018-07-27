// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/worker/WorkerQueue.h>


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
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			class QSF_EDITOR_BASE_API_EXPORT NetworkTransferManager : public Manager
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 WORKER_QUEUE_ID;	///< Worker queue identifier (string hash of "qsf::editor::base::NetworkTransferManager")


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				inline NetworkTransferManager();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~NetworkTransferManager();

				bool onStartup();

				void onShutdown();

				inline bool scheduleTask(std::shared_ptr<WorkerTask> task);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				WorkerQueue mWorkerQueue;


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/network/transfer/NetworkTransferManager-inl.h"
