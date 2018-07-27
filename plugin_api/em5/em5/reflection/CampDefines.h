// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/reflection/CampDefines.h>


//[-------------------------------------------------------]
//[ Defines                                               ]
//[-------------------------------------------------------]

// Macros to streamline CAMP export

/*
 *  @brief
 *    Mark the class as a freeplay event factory
 */
#define EM5_CAMP_IS_FREEPLAY_EVENT_FACTORY \
		.base<em5::FreeplayEventFactory>() \
		.constructor0()

/*
 *  @brief
 *    Mark the class as a freeplay event factory
 */
#define EM5_CAMP_IS_MEDICAL_EMERGENCY_EVENT_FACTORY \
		.base<em5::MedicalEmergencyEventFactory>() \
		.constructor0()

/*
 *  @brief
 *    Mark the class as a freeplay event
 */
#define EM5_CAMP_IS_FREEPLAY_EVENT \
		.base<em5::FreeplayEvent>() \
		.constructor0()

/*
 *  @brief
 *    Mark the class as a medical emergency event
 */
#define EM5_CAMP_IS_MEDICAL_EMERGENCY_EVENT \
		.base<em5::MedicalEmergencyEvent>() \
		.constructor0()

/*
 *  @brief
 *
 */
#define EM5_CAMP_IS_MAIN_EVENT \
		.base<em5::MainEvent>() \
		.constructor0()
