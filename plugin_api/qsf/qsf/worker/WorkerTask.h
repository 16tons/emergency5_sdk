// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

#include <condition_variable>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class WorkerSystem;
	class WorkerQueue;
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
	class QSF_API_EXPORT WorkerTask : boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend WorkerSystem;
		friend WorkerQueue;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum Status
		{
			STATUS_NEW,
			STATUS_SCHEDULED,
			STATUS_EXECUTING,
			STATUS_DONE,
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		WorkerTask();
		virtual ~WorkerTask();
		Status getStatus() const;
		void wait();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void setStatus(Status status);
		void execute();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::editor::server::WorkerTask methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void executeImpl() = 0;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Status					mStatus;
		mutable std::mutex	    mStatusMutex;
		std::condition_variable mStatusCondition;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
