// Copyright (C) 2012-2017 Promotion Software GmbH
#ifdef QSF_PROFILING


	//[-------------------------------------------------------]
	//[ Header guard                                          ]
	//[-------------------------------------------------------]
	#pragma once


	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "qsf/time/profiling/ProfilingElement.h"
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
		*    Scoped profiling zone class
		*/
		class QSF_API_EXPORT ScopedProfilingZone
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline ScopedProfilingZone(ProfilingElement& element);

			/**
			*  @brief
			*    Destructor
			*/
			inline ~ScopedProfilingZone();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ProfilingElement&		mElement;			///< Profiling element to register update at
			HighResolutionStopwatch	mStopwatch;			///< Stopwatch object for time measurement
			ScopedMemoryZone		mScopedMemoryZone;	///< Memory watch object for memory consumption measurement


		};


	//[-------------------------------------------------------]
	//[ Namespace                                             ]
	//[-------------------------------------------------------]
	} // qsf


	//[-------------------------------------------------------]
	//[ Implementation                                        ]
	//[-------------------------------------------------------]
	#include "qsf/time/profiling/ScopedProfilingZone-inl.h"
#endif
