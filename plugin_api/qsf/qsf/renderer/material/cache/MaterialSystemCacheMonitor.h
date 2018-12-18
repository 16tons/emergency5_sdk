// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"
#include "qsf/renderer/material/cache/MaterialSystemCacheManager.h"

#include <boost/noncopyable.hpp>

#include <thread>
#include <atomic>


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
	*    Material system cache monitor
	*/
	class MaterialSystemCacheMonitor : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystemCacheManager;	///< The material system cache manager is the master of this class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Enable/disable the monitor
		*
		*  @param[in] enabled
		*    "true" to enable the monitor, else "false"
		*/
		void setMonitorEnabled(bool enable);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialSystem
		*    Owner material system cache manager instance
		*/
		explicit MaterialSystemCacheMonitor(MaterialSystemCacheManager& materialSystemCacheManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialSystemCacheMonitor();

		/**
		*  @brief
		*    Update method that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateMonitor(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Thread worker method
		*/
		void threadWorker();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialSystemCacheManager&					mMaterialSystemCacheManager;	///< Owner material system cache manager instance
		JobProxy									mUpdateJobProxy;				///< Update job proxy
		MaterialSystemCacheManager::DirtyCacheParts mDirtyCacheParts;
		std::atomic<bool>							mHasDirtyCacheParts;
		std::atomic<bool>							mMonitorEnabled;
		std::thread									mThread;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/cache/MaterialSystemCacheMonitor-inl.h"
