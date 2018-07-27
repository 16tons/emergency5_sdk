// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


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
	*    Class representing a damped spring
	*/
	class QSF_API_EXPORT DampedSpring
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		DampedSpring();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] springConstant
		*    Spring constant in 1/(sec*sec)
		*  @param[in] minimalSpeed
		*    Minimal speed to prevent endless approximation [value unit]/sec
		*
		*  @remarks
		*    A good minimal speed will make it possible to reach our target quickly without ruining the illusion of a endless deceleration
		*    we can save performance in a few implementations when we now we have reached our target and finished our movement (speed == 0.0f).
		*/
		DampedSpring(float springConstant, float minimalSpeed);

		/**
		*  @brief
		*    Give back the next value, forced in direction of target
		*
		*  @param[in] target
		*    Target value we want to move our value but with a realistic acceleration and deceleration
		*  @param[in] timeStep
		*    Time between pushToTarget calls in sec
		*/
		float pushToTarget(float target, float timeStep);

		/**
		*  @brief
		*    Return our value we want to move
		*/
		inline float getValue() const;

		/**
		*  @brief
		*    Set our value we want to move
		*/
		inline void setValue(float value);

		/**
		*  @brief
		*    Return the spring constant
		*/
		inline float getSpringConstant() const;

		/**
		*  @brief
		*    Set the spring constant
		*
		*  @note
		*    - This constant should be in 1/(sec*sec)
		*/
		inline void setSpringConstant(float springConstant);

		/**
		*  @brief
		*    Return the damping constant
		*/
		inline float getDampingConstant() const;

		/**
		*  @brief
		*    Set the damping constant
		*
		*  @note
		*    - This constant should be 2.0 if we want a critically damped spring
		*    - Lower values as 2.0 will give as an oscillating underdamped spring
		*    - Higher values as 2.0 will give as an overdamped spring
		*/
		inline void setDampingConstant(float dampingConstant);

		/**
		*  @brief
		*    Return if we stop our value at a target or if we can "overshoot" a value and oscillate
		*/
		inline bool isStoppingAtTarget() const;

		/**
		*  @brief
		*    Set if we stop our value at a target or if we can "overshoot" a value and oscillate
		*/
		inline void setStoppingAtTarget(bool stoppingAtTarget);

		/**
		*  @brief
		*    Check if we stopped (speed == 0.0f), use in conjunction with "StoppingAtTarget"
		*/
		inline bool hasStopped();

		/**
		*  @brief
		*    Get our minimal speed to prevent endless approximation
		*/
		inline float getMinimalSpeed() const;

		/**
		*  @brief
		*    Set our minimal speed to prevent endless approximation
		*
		*  @note
		*    - Speed is in [value unit]/sec
		*    - Only works with if "isStoppingAtTarget()" is true
		*/
		inline void setMinimalSpeed(float minimalSpeed);

		/**
		*  @brief
		*    Get the internal speed of the spring
		*
		*  @note
		*    - If speed is 0.0f the calculation is finished, we reached target value
		*/
		inline float getSpeed() const;

		/**
		*  @brief
		*    Set the internal speed to setup for example a non idle start state
		*
		*  @note
		*    - Speed is in [value unit]/sec
		*/
		inline void setSpeed(float speed);


	//[-------------------------------------------------------]
	//[ Private members                                       ]
	//[-------------------------------------------------------]
	private:
		float				mValue;

		// Spring Parameter
		float				mSpringConstant;
		float				mDampingConstant;

		// Parameter to cut our calculation and stop at target value
		bool				mStoppingAtTarget;
		float				mMinimalSpeed;

		// Internal
		float				mSpeed;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/DampedSpring-inl.h"
