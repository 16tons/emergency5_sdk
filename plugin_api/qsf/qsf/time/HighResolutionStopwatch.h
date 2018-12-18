// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

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
	*    Simple and easy to use high resolution stopwatch class
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    HighResolutionStopwatch stopwatch;
	*    ... do some work...
	*    float pastTime = stopwatch.getElapsed().getSeconds();
	*    @endcode
	*
	*    Using Boost time features results in somewhat cryptic code. Example:
	*    @code
	*    boost::posix_time::ptime ptimeStart = boost::posix_time::microsec_clock::local_time();
	*    // ... do some work...
	*    uint64 pastTime = const boost::posix_time::time_duration(boost::posix_time::microsec_clock::local_time() - ptimeStart).total_milliseconds();
	*    @endcode
	*
	*  @note
	*    - Depending on the used operation system, using this stopwatch might have a major performance impact, so don't overuse it and avoid using it in end-user-builds
	*/
	class QSF_API_EXPORT HighResolutionStopwatch : public boost::noncopyable
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
		*    If this parameter is "true", the stopwatch is started automatically at once
		*/
		inline explicit HighResolutionStopwatch(bool startAtOnce = true);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~HighResolutionStopwatch();

		/**
		*  @brief
		*    Reset the stopwatch
		*/
		inline void reset();

		/**
		*  @brief
		*    Start the stopwatch
		*
		*  @note
		*    - If the stopwatch is already running it's restarted
		*/
		inline void start();

		/**
		*  @brief
		*    Stop the stopwatch
		*
		*  @return
		*    The elapsed time since "qsf::HighResolutionStopwatch::start()"
		*
		*  @note
		*    - Often it's adequate to just request the past time using
		*      e.g. "qsf::HighResolutionStopwatch::getElapsed()" and not explicitly stopping the stopwatch
		*/
		inline Time stop();

		/**
		*  @brief
		*    Returns the elapsed time since the last start without stopping the time tracking.
		*
		*  @return
		*    The elapsed time since the last start without stopping the time tracking.
		*/
		inline Time getElapsed() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Retrieve the time since the system was started.
		*
		*  @return
		*    The time since the system was started.
		*/
		Time getSystemTime() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mRunning;	///< Is the stopwatch currently running?
		Time mStart;	///< Stopwatch start time
		Time mStop;		///< Stopwatch stop time


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/HighResolutionStopwatch-inl.h"
