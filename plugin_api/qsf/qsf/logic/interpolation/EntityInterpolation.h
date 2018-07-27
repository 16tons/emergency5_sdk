// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"
#include "qsf/time/Stopwatch.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
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
	*    Entity interpolation controller class
	*/
	class QSF_API_EXPORT EntityInterpolation : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Simulation tick start call
		*
		*  @note
		*    - Prefer using the non-static method instead
		*/
		static void onTickStart(const Clock& simulationClock);

		/**
		*  @brief
		*    Simulation tick end call
		*
		*  @note
		*    - Prefer using the non-static method instead
		*/
		static void onTickEnd();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EntityInterpolation();

		/**
		*  @brief
		*    Destructor
		*/
		~EntityInterpolation();

		/**
		*  @brief
		*    Set relative animation delay between 0.0f and 1.0f
		*
		*  @remarks
		*    - Higher values (up to 1.0f) will lead to a delay of interpolated animation transformations up to one tick
		*    - Lower values (down to 0.0f) can lead to less smooth animation, because more extrapolation using velocity data is needed
		*    - The default value is 0.5f
		*/
		void setRelativeAnimationDelay(float relativeDelay);

		/**
		*  @brief
		*    Simulation tick start call
		*/
		void tickStarted(const Clock& simulationClock);

		/**
		*  @brief
		*    Simulation tick end call
		*/
		void tickEnded();

		/**
		*  @brief
		*    Update the entity interpolation
		*/
		void applyEntityInterpolation(const Clock& simulationClock);

		/**
		*  @brief
		*    Update entity transformations from without performing interpolation
		*/
		void applyMovementWithoutInterpolation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Time	  mLastInterpolationTimestamp;	///< Stores the timestamp of the last entity interpolation update (in game time)
		Stopwatch mSimulationStopwatch;
		float	  mRelativeAnimationDelay;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
