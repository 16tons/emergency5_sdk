// Copyright (C) 2012-2018 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/time/HighResolutionStopwatch.h"
	#include "qsf/time/profiling/ScopedMemoryZone.h"


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
		*    Profiling element class
		*/
		class QSF_API_EXPORT ProfilingElement
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline ProfilingElement();

			/**
			*  @brief
			*    Destructor
			*/
			inline ~ProfilingElement();

			/**
			*  @brief
			*    Return the number of updates to this element
			*/
			inline uint32 getNumberOfUpdates() const;

			/**
			*  @brief
			*    Reset this element's data back to default
			*/
			inline void reset();

			/**
			*  @brief
			*    Register an update
			*
			*  @param[in] stopwatch
			*    High resolution stopwatch used for measuring the time interval length associated with this update
			*  @param[in] scopedMemoryZone
			*    scoped memory zone associated with this update
			*/
			inline void registerUpdate(const HighResolutionStopwatch& stopwatch, const ScopedMemoryZone& scopedMemoryZone);

			//[-------------------------------------------------------]
			//[ Time                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the accumulated time of registered updates
			*/
			inline Time getTotalTime() const;

			/**
			*  @brief
			*    Return the average update time of all registered updates
			*/
			inline Time getAverageTime() const;

			/**
			*  @brief
			*    Return the maximum update time of all registered updates
			*/
			inline Time getPeakTime() const;

			//[-------------------------------------------------------]
			//[ Memory                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the accumulated memory of registered updates
			*/
			inline int64 getTotalMemory() const;

			/**
			*  @brief
			*    Return the average update memory of all registered updates
			*/
			inline int64 getAverageMemory() const;

			/**
			*  @brief
			*    Return the maximum update memory of all registered updates
			*/
			inline int64 getPeakMemory() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::ProfilingElement methods          ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Register an update
			*
			*  @param[in] timeElapsed
			*    The time interval length associated with this update
			*  @param[in] memoryConsumed
			*    The memory consumed with this update
			*/
			inline virtual void registerUpdate(const Time& timeElapsed, int64 memoryConsumed);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32	mUpdates;		///< Number of updates to this element
			// Time
			Time	mTotalTime;		///< Accumulated time of registered updates
			Time	mPeakTime;		///< Maximum update time of all registered updates
			// Memory
			int64	mTotalMemory;	///< Accumulated memory of registered updates
			int64	mPeakMemory;	///< Maximum update memory of all registered updates


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf


	//[-------------------------------------------------------]
	//[ Implementation                                        ]
	//[-------------------------------------------------------]
	#include "qsf/time/profiling/ProfilingElement-inl.h"
#endif
