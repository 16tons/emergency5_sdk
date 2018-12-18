// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Layer;
}
namespace em5
{
	class FreeplayEvent;
	class VehicleComponent;
}


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
	*    EMERGENCY 5 car accident helper
	*/
	class EM5_API_EXPORT CarAccidentHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static qsf::Entity& prepareAccidentCar(qsf::Entity& vehicleEntity, FreeplayEvent& freeplayEvent, bool startFire, bool replaceVehicle = true);
		static qsf::Entity& prepareAccidentCar(qsf::Entity& vehicleEntity, qsf::Layer& layer, FreeplayEvent& freeplayEvent, bool startFire, bool replaceVehicle = true);

		// Fixing editing. Vehicles are currently without fire and collision components, but we need both.
		// So we replace the _dmg entity by its undamaged counterpart and delete the damaged one.
		static qsf::Entity& replaceVehicleEntity(qsf::Entity& vehicleEntity);

		static std::string getUndamagedEntityName(qsf::Entity& damagedEntity);
		static void getPersonEntitiesInVehicle(VehicleComponent& vehicleComponent, std::vector<qsf::Entity*>& entities);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline CarAccidentHelper() {}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
