// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/renderer/MiniMapIcons.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MiniMapIconRegistrationLogic;
}
namespace qsf
{
	class Entity;
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
	*    Static minimap helper class
	*/
	class EM5_API_EXPORT MinimapHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static MiniMapIconRegistrationLogic& getMiniMapIconRegistrationLogic();

		static void registerIcon(uint64 entityId, MiniMapIconType minimapIconType, bool onlyForLocalPlayer = false);
		static void registerIcon(const qsf::Entity& entity, MiniMapIconType minimapIconType, bool onlyForLocalPlayer = false);

		static void unregisterIcon(uint64 entityId, bool onlyForLocalPlayer = false);
		static void unregisterIcon(const qsf::Entity& entity, bool onlyForLocalPlayer = false);

		static void registerEventIcon(uint64 entityId);
		static void registerEventIcon(const qsf::Entity& entity);

		static void registerPingByWorldPosition(const glm::vec3& position, bool cyclesAnimation = true, bool playSound = true);
		static void registerPingByEntity(const qsf::Entity& entity, bool cyclesAnimation = true, bool playSound = true);
		static void registerPingByEntityId(uint64 entityId, bool cyclesAnimation = true, bool playSound = true);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	public:
		// No instance constructable, this class is purely static
		MinimapHelper() {}
		~MinimapHelper() {}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
