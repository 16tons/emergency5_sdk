// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/gangster/GangsterPool.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/StringHash.h>
#include <qsf/map/MapHelper.h>

#include <glm/gtx/quaternion.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Layer;
	class Map;
	class ParticlesComponent;
	class Transform;
	class TransformComponent;
}
namespace em5
{
	class Player;
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
	*    EMERGENCY 5 map helper class
	*/
	class EM5_API_EXPORT MapHelper : public qsf::MapHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destroy the given entity
		*/
		static bool destroyEntity(qsf::Entity& entity);

		/**
		*  @brief
		*    Fly the main camera to the position
		*/
		static void flyCameraToPosition(const glm::vec3& position, const Player& player, bool keepCurrentZoom);

		/**
		*  @brief
		*    Fly the main camera to the entity
		*/
		static void flyCameraToEntity(const qsf::Entity& focusEntity, const Player& player, bool keepCurrentZoom);

		/**
		*  @brief
		*    "true" if the unit automatism flag is active
		*/
		static bool isUnitAutomatismActive();

		/**
		*  @brief
		*    "true" if the damage system flag is active
		*/
		static bool isDamageSimulationActive();

		/**
		*  @brief
		*    Resets the stored information about the gangster event area
		*/
		static void resetStoredGangsterEventAreaInformation();

		static void resetNextEntityIdForMultiplay();

		static void activateLayer(qsf::Layer& layer, bool activate);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit inline MapHelper(qsf::Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MapHelper();

		/**
		*  @brief
		*    Destroy the entity by its given ID
		*/
		bool destroyEntityById(uint64 entityId);

		/**
		*  @brief
		*    Create an entity from the given prefab
		*/
		qsf::Entity* createObjectByLocalPrefabAssetId(const qsf::StringHash& localAssetId, bool sendMessage = true) const;

		/**
		*  @brief
		*    Create an entity from the given Prototype. WARNING: copies only the entity, not the children
		*  @todo
		*    TODO(db): Make usable for multiplayer
		*/
		qsf::Entity* createEntityByPrototypeReference(const qsf::Prototype& prototype, bool sendMessage = true) const;

		/**
		*  @brief
		*    Entity particles are shown
		*
		*  @param[in] particleEntityIds
		*    List of entityIds for example from an entity slot
		*  @param[in] show
		*    If particle are shown or not / Is irrelevant if showByHeight is true
		*  @param[in] repeatAnimation
		*    If animations are repeated or not
		*  @param[in] showByHeight
		*    Height check with worldYOffset is used (for example for clipping)
		*  @param[in] worldYOffset
		*    Used for height check
		*  @param[in] restart
		*    Used for restart if the particle effect is already finished
		*/
		void showEntityParticleIds(const std::vector<uint64>& particleEntityIds, bool show, bool repeatAnimation, bool showByHeight = false, float worldYOffset = 0.0f, bool restart = false) const;

		/**
		*  @brief
		*    Entity particles are disabled
		*
		*  @param[in] particleEntityIds
		*    List of entityIds for example from an entity slot
		*/
		void disableEntityParticleIds(const std::vector<uint64>& particleEntityIds) const;

		/**
		*  @brief
		*    Hides all particle links
		*
		*  @note
		*    - Use this only in the special case of map saving
		*/
		void hideParticleChildrenAtMapSave(std::vector<uint64>& particleIds) const;

		/**
		*  @brief
		*    Create a fire and forget particle which destructs itself again after the given time
		*/
		qsf::ParticlesComponent* spawnParticle(qsf::LocalAssetId prefabAssetId, float timeout, const glm::vec3& position, const glm::vec3& scale=glm::vec3(1.f, 1.f, 1.f), const glm::quat& rotation=glm::quat(), bool isMultiplayerRelevant = true);

		void showEntityIds(std::vector<uint64>& entityIds, bool show, bool showByHeight = false, float worldYOffset = 0.0f) const;

		template <typename T>
		void showComponentofEntityIds(std::vector<uint64>& entityIds, bool show) const;

		/**
		*  @brief
		*    Return "true" is time of day between 8pm (20.00Uhr) and 7am (7:00Uhr)
		*/
		bool isNightTime() const;

		/**
		*  @brief
		*    Return the entity that represents the headquarter
		*
		*  @note
		*    - The headquarter is currently the entity with the em5::SpawnPointComponent with type RTW -> Entry
		*/
		qsf::Entity* getHeadQuarter() const;

		/**
		*  @brief
		*    Return "true" if the position is inside an active contamination area.
		*
		*  @note
		*    - Careful, might be slow, so don't use constantly.
		*/
		bool isContaminationArea(const glm::vec3& position) const;

		/**
		*  @brief
		*    Returns the gangster event area, i.e. the area where gangster events shall happen.
		*	 If it has not been specified with the map (i.e. no tag "GANGSTER_EVENT_AREA" exists)
		*	 std::pair<nullptr, ...> is returned.
		*	 The second argument of the returned pair depicts whether the box is centered at pivot.
		*/
		std::pair<qsf::TransformComponent*, bool> getGangsterEventArea() const;

		/**
		*  @brief
		*    Creates the given prefab somewhere in the "gangster event area".
		*	 'Somewhere' means the entity is positioned at a random spawnpoint within that area.
		*	 Returns nullptr if area does not exist or if there is no spawnpoint in that area.
		*/
		qsf::Entity* createObjectWithinGangsterEventAreaByLocalPrefabAssetId(const qsf::StringHash& localAssetId) const;

		/**
		*  @brief
		*    Creates a gangster from the given pool somewhere in the "gangster event area".
		*	 'Somewhere' means the entity is positioned at a random spawnpoint within that area.
		*	 Returns nullptr if area does not exist or if there is no spawnpoint in that area.
		*/
		qsf::Entity* createGangsterFromPoolAndSpawnItInGangsterEventArea(GangsterPool gangsterPoolType) const;

		/**
		*  @brief
		*	 Maps the gangster pool enum to the spawn pool assets
		*/
		std::string getSpawnpoolNameFromGangsterPool(GangsterPool gangsterPoolType) const;

		/**
		*  @brief
		*	 Returns a gangster candidate from the given pool that is located in the gangster area.
		*	 If such a gangster cannot be found, a new entity is created.
		*	 Returns nullptr if area does not exist or if there is no spawnpoint in that area.
		*/
		qsf::Entity* getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool gangsterPoolType, const std::string& gangsterTag = "") const;

		/**
		*  @brief
		*	 Replaces a entity, which is a combined item (prefab name has _combined as postfix in name) with its "real" counterpart and returns the new instance
		*    If a replacing item could not be found the combined one is returned
		*    Note: The navigation and move to despawn components are deactivated on the real one. So anyone who needs the ai navigation active must set it up properly
		*/
		qsf::Entity* replaceCombinedEntityWithRealOne(qsf::Entity& combinedEntity) const;

		bool isInMapBoundaries(const glm::vec3& position) const;
		bool isInEventBoundaries(const glm::vec3& position) const;

		/**
		*  @brief
		*	 Returns whether the given entity is close to the map boundaries
		*/
		bool isEntityCloseToMapBoundaries(qsf::Entity& entity, float tolerance = 1.0f);

		/**
		*  @brief
		*	 Returns whether the given entity is close to the event boundaries
		*/
		bool isEntityCloseToEventBoundaries(qsf::Entity& entity, float tolerance = 1.0f);

		/**
		*  @brief
		*	 The entities outside the map boundaries don't need special game play stuff (in other words, its important to turn them off)
		*/
		void prepareMapBorderLayer(const std::string& layerName);

		/**
		*  @brief
		*	 Search the map for entities with the upgrade name and unhide them
		*/
		void unlockUpgradeComponent(const std::string& upgradeName);

		/**
		*  @brief
		*    Enable/disable energy sections identified by meta-component tags
		*
		*  @remarks
		*    Usage example: Place "Global asset ID 619601 - Local asset name "em5/prefab/debug/debug_box" - Local asset ID 3641566145" and
		*    use scale to define the area. Add an event tag component ("qsf::game::EventTagComponent") and define tags
		*    (e.g. "EnergySection1" or "EnergySection2") to make it possible for game-logic to address this defined area.
		*
		*  @note
		*    - Activates/deactivates "em5::LightTimeControlComponent" instances inside the energy section
		*/
		void setEnergySectionEnabled(const qsf::StringHash& eventTag, bool enabled) const;
		void setEnergySectionEnabled(const std::vector<qsf::Entity*>& areaBoxes, bool enabled) const;

		/**
		*  @brief
		*    Evacuate all units (have "em5::CommandableComponent") from a given area to another given area
		*
		*  @remarks
		*    Usage example: Place "Global asset ID 619601 - Local asset name "em5/prefab/debug/debug_box" - Local asset ID 3641566145" and
		*    use scale to define the area. Add an event tag component ("qsf::game::EventTagComponent") and define tags
		*    (e.g. "FromArea" or "ToArea") to make it possible for game-logic to address this defined area.
		*/
		void evacuateAllUnits(const qsf::StringHash& fromEventTag, const qsf::StringHash& toEventTag, const std::vector<uint64>& ignoreEntityIds = std::vector<uint64>()) const;

		void removeAllCiviliansInArea(const qsf::StringHash& areaEventTag);
		void removeAllCivilPersonsInArea(const qsf::StringHash& areaEventTag);
		void removeAllCivilVehiclesInArea(const qsf::StringHash& areaEventTag);

		void removeCiviliansInArea(const qsf::StringHash& areaEventTag, bool ignorePersons, bool ignoreVehicles);

		bool getPointOnClosestLane(const glm::vec3& position, uint32 aiWorldIndex, uint32 laneTypeId, qsf::Transform& outTransform, boost::optional<glm::vec3> wantedDirection = boost::optional<glm::vec3>());


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static std::pair<qsf::TransformComponent*, bool> mGangsterEventArea;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/map/MapHelper-inl.h"
