// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/units/UnitPool.h"


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
	*    EMERGENCY 5 static unit pool helper class
	*/
	class EM5_API_EXPORT UnitPoolHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		UnitPoolHelper();
		~UnitPoolHelper();


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setUnitPoolForLocalPlayer(const std::string& unitPoolName);

		static void setUnitPoolToStartup();
		static void setUnitPoolToFixedAmount(int amount = 20);

		/**
		*  @brief
		*    Make all blocked units available
		*/
		static void unblockAllBlockedUnits();

		static bool isUnitAvailable(const std::string& unitName);

		static bool isFireFighterDECONPAvailable();

		/**
		*  @brief
		*    Extract the units that are currently in use
		*/
		static void getUnitsInUseFromCurrentUnitPool(std::vector<uint64>& playerUnitsInUse);

		/**
		*  @brief
		*    Add a list of player unit ids on the map and add them as "inUse" chapter to the current unit pool
		*/
		static void addUintsInUseToCurrentUnitPool(const std::vector<uint64>& playerUnitsInUse);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
