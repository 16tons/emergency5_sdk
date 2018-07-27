// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/HighResolutionStopwatch.h"


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
	*    High resolution stopwatch measuring time elapsed in its lifetime
	*
	*  @remarks
	*    High resolution stopwatch that automatically adds the measured time during its lifetime
	*    to a defined "qsf::Time" instance
	*/
	class ScopedHighResolutionStopwatch : protected HighResolutionStopwatch
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] outputTime
		*    Reference to a "qsf::Time" instance to add time to
		*/
		inline explicit ScopedHighResolutionStopwatch(Time& outputTime);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~ScopedHighResolutionStopwatch();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Time& mOutputTime;		///< Where to add the result to


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/ScopedHighResolutionStopwatch-inl.h"
