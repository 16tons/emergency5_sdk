// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static EMERGENCY 5 AI movement modes definition class
	*/
	class EM5_API_EXPORT MovementModes
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// (vs) The static AI configuration allows only one speed setting, which is used for the default (walk for civilians).
		// Every time we want a civilian to run we need to use this value as a replacement via movement mode or even in the ad hoc speed tweaking functor.
		// TODO Read this from a specs file
		static const float gCivilianRunSpeed;

		// Available movement modes in EMERGENCY 5
		enum Modes
		{
			MOVEMENT_MODE_DEFAULT = 0,
			MOVEMENT_MODE_CARRY_PERSON,
			MOVEMENT_MODE_WALK_BACKWARDS,		// TODO(mk) This value is not supported
			MOVEMENT_MODE_RUN_PANIC,			// TODO(mk) This value is not supported
			MOVEMENT_MODE_CIVIL_CAR_MOVES_TO_THE_SIDE, // Ignoring traffic lights, lowering distance to keep
			MOVEMENT_MODE_PLAYER_VEHICLE_ON_EMERGENCY_OPERATIONS, // Ignoring traffic lights, although this is done inside the functor
			MOVEMENT_MODE_RUN,					// For gangsters and civilians running away
			MOVEMENT_MODE_SEARCHING,			// Currently used for the rescue dog search (persons currently not supported)
			MOVEMENT_MODE_MAGNET,				// For FollowMagnetLogic
			MOVEMENT_MODE_REDIRECT,				// For vehicles changing lanes when they get redirected
			MOVEMENT_MODE_FREE_WALK,			// Civil persons can use the free area with it
			MOVEMENT_MODE_FREE_RUN				// Civil persons can run on the free area with it and run
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Register the movement modes
		*/
		static void registerMovementModes();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
