// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/clock/Clock.h"


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
	*    Real-time clock class
	*/
	class QSF_API_EXPORT RealTimeClock : public Clock
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline RealTimeClock();

		/**
		*  @brief
		*    Destructor
		*/
		inline ~RealTimeClock();

		/**
		*  @brief
		*    Reset clock state
		*/
		inline virtual void reset() override;

		/**
		*  @brief
		*    Update the clock using real-time. Depending on the configuration, this leads to triggering of clock signals
		*/
		void updateClock();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void triggerClockSignal(const Time& timePassed);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::optional<Time> mLastSystemTime;	///< System time in last update call; invalid if not set at all


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/clock/RealTimeClock-inl.h"
