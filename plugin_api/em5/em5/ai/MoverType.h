// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


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
	*    Static EMERGENCY 5 AI navigation mover type names.
	*    Helps to access the AI mover types with EM5 specific symbolic names instead of nondescript numbers
	*/
	class MoverType
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Always keep this synchronized with the actual values defined inside the editor
		enum Type
		{
			PLAYER_VEHICLE = 0,
			PLAYER_PERSON,
			CIVIL_VEHICLE,
			CIVIL_PERSON,
			GANGSTER_VEHICLE,
			GANGSTER_PERSON,
			BIG_BOAT,
			FLYING_UNIT,
			TRAIN,
			SMALL_BOAT,
			CIVIL_SHIP
		};


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
