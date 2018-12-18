// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/renderer/MiniMapIcons.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/math/Color4.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CommandableComponent;
	class MiniMapRenderer;
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
	*    Minimap registration logic
	*/
	class EM5_API_EXPORT MiniMapIconRegistrationLogic
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static uint64 getHeadquarterEntityId();
		static MiniMapIconType translateCommandableMiniMapIconType(const qsf::StringHash& commandableMiniMapIconType);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		MiniMapIconRegistrationLogic();
		~MiniMapIconRegistrationLogic();

		/**
		*  @brief
		*    Startup the minimap icon registration logic
		*/
		void onStartup();

		/**
		*  @brief
		*    Shutdown the minimap icon registration logic
		*/
		void onShutdown();

		void setPathVisualizationEntityId(uint64 entityId);

		void registerPolygon(uint64 entityId, const qsf::Color4& color);
		void registerPolygon(uint64 entityId, const qsf::Color4& color, const std::vector<glm::vec3>& vertices);
		void registerCirclePolygon(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition = false) const;
		void unregisterPolygon(uint64 entityId);

		void registerIcon(uint64 entityId, MiniMapIconType minimapIconType, bool onlyForLocalPlayer = false) const;
		void registerEventIcon(uint64 entityId) const;
		void registerHighlightIcon(const glm::vec3& worldPosition) const;
		void registerHighlightIcon(uint64 entityId) const;
		void registerBombIcon(uint64 entityId) const;
		void registerGangsterIcon(uint64 entityId) const;
		void registerSearchIcon(uint64 entityId) const;
		void unregisterIcon(uint64 entityId, bool onlyForLocalPlayer = false) const;
		void unregisterIcon(const qsf::Entity& entity, bool onlyForLocalPlayer = false) const;
		void registerUnitIcon(qsf::Entity& entity) const;
		void showAllHarborIcons(bool show);

		void registerPingByWorldPosition(const glm::vec3& worldPosition, bool cyclesAnimation = true, bool playSound = true);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Calls to this method are replicated to the multiplyer clients through qsf::Message
		void addIcon(uint64 entityId, MiniMapIconType iconType, float scaleOrMaxSize) const;
		// Calls to this method are replicated to the multiplyer clients through qsf::Message
		void addCirclePolygon(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition) const;
		// Calls to this method are replicated to the multiplyer clients through qsf::Message
		void removeIcon(uint64 entityId) const;

		// Calls to this method are only applied locally
		bool addIconLocally(uint64 entityId, MiniMapIconType type, float scaleOrMaxSize) const;
		// Calls to this method are only applied locally
		void addCirclePolygonLocally(uint64 entityId, float radius, uint32 points, const qsf::Color4& color, bool updatePosition) const;
		// Calls to this method are only applied locally
		void removeIconLocally(uint64 entityId) const;

		CommandableComponent* checkUnit(qsf::Entity& unitEntity) const;
		bool checkFire(qsf::Entity* fireEntity) const;
		bool checkIsHiddenPerson(qsf::Entity* entity) const;
		bool checkInjuredPerson(qsf::Entity* entity) const;

		void onSpawnUnit(const qsf::MessageParameters& parameters);
		void onStartFire(const qsf::MessageParameters& parameters);
		void onStopFire(const qsf::MessageParameters& parameters);
		void onPersonGetsRescuedFromCar(const qsf::MessageParameters& parameters);
		void onPlacePersonOnPolice(const qsf::MessageParameters& parameters);
		void onEntityHidden(const qsf::MessageParameters& parameters);
		void onFindHiddenPerson(const qsf::MessageParameters& parameters);
		void onFindHiddenPersonWithSEKDrone(const qsf::MessageParameters& parameters);
		void onSelectionChange(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Adds icon of given entityId -> first filter is the entityId, parameter "IconType" contains the MiniMapIconType
		*/
		void onAddIcon(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Adds circle polygon of given entityId -> first filter is the entityId
		*/
		void onAddCirclePolygon(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Removes circle polygon of given entityId -> first filter is the entityId
		*/
		void onRemoveCirclePolygon(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Removes icon of given entityId -> first filter is the entityId
		*/
		void onRemoveIcon(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			  mShowAllHarborIcons;
		MiniMapRenderer*  mMiniMapRenderer;
		qsf::MessageProxy mMessageProxySpawnUnit;
		qsf::MessageProxy mMessageProxyDespawnUnit;
		qsf::MessageProxy mMessageProxyEnterVehicle;
		qsf::MessageProxy mMessageProxyEnterBuilding;
		qsf::MessageProxy mMessageProxyExitBuilding;
		qsf::MessageProxy mMessageProxyStartFire;
		qsf::MessageProxy mMessageProxyStopFire;
		qsf::MessageProxy mMessageProxyDestroyedVehicle;
		qsf::MessageProxy mMessageProxyPersonGetsRescuedFromCar;
		qsf::MessageProxy mMessageProxyGangsterEscaped;
		qsf::MessageProxy mMessageProxyPlacedInPolice;
		qsf::MessageProxy mMessageProxyFindHiddenPerson;
		qsf::MessageProxy mMessageProxyFindHiddenPersonWithSEKDrone;
		qsf::MessageProxy mMessageProxyPickedUpByTowCar;
		qsf::MessageProxy mMessageProxyLoadVehicleToTransporter;
		qsf::MessageProxy mMessageProxyUnLoadVehicleFromTransporter;
		qsf::MessageProxy mMessageProxyPickupPersonByParamedic;
		qsf::MessageProxy mMessageProxyArrestGangster;
		qsf::MessageProxy mMessageProxyOnSelectionChange;
		qsf::MessageProxy mMessageProxyMinimapAddIcon;
		qsf::MessageProxy mMessageProxyMinimapAddCirclePolygon;
		qsf::MessageProxy mMessageProxyMinimapRemoveCirclePolygon;
		qsf::MessageProxy mMessageProxyMinimapRemoveIcon;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
