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
	*    Simple and easy to use memory consumption watch class
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    ScopedMemoryZone scopedMemoryZone;
	*    ... do some work...
	*    uint64 consumedMemory = scopedMemoryZone.getConsumed();
	*    @endcode
	*/
	class QSF_API_EXPORT ScopedMemoryZone : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] startAtOnce
		*    If this parameter is "true", the memory watch is started automatically at once
		*/
		inline explicit ScopedMemoryZone(bool startAtOnce = true);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~ScopedMemoryZone();

		/**
		*  @brief
		*    Start the memory watch
		*
		*  @note
		*    - If the memory watch is already running it's restarted
		*/
		inline void start();

		/**
		*  @brief
		*    Stop the memory watch
		*
		*  @return
		*    The consumed memory since "qsf::ScopedMemoryZone::start()"
		*
		*  @note
		*    - Often it's adequate to just request the consumed memory using
		*      e.g. "qsf::ScopedMemoryZone::getConsumed()" and not explicitly stopping the memory watch
		*/
		inline int64 stop();

		/**
		*  @brief
		*    Returns the consumed memory since the last start without stopping the memory tracking.
		*
		*  @return
		*    The consumed memory since the last start without stopping the memory tracking.
		*/
		inline int64 getConsumed() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Return the current free virtual memory in bytes
		*
		*  @return
		*    The current free virtual memory in bytes
		*/
		uint64 getFreeVirtualMemory() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   mRunning;	///< Is the memory watch currently running?
		uint64 mStart;		///< Memory watch start time
		uint64 mStop;		///< Memory watch stop time


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/profiling/ScopedMemoryZone-inl.h"
