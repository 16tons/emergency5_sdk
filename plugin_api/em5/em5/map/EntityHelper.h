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

#include <qsf/base/NamedIdentifier.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/map/EntityHelper.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
	class GangsterBaseLogic;
	class OrderInfo;
	class RoadVehicleComponent;
}
namespace qsf
{
	class CameraControlComponent;
	class Component;
	class Entity;
	class GameLogic;
	class Map;
	class TransformComponent;
	namespace game
	{
		class EquipmentComponent;
		class PrototypeContainer;
	}
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
	*    EMERGENCY 5 entity helper class
	*/
	class EM5_API_EXPORT EntityHelper : public qsf::EntityHelper
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*   Types of load a unit can carry.
		*   This is a bitset (not because units can carry more than one but to check them together)
		*/
		enum CarryLoad
		{
			CARRY_PERSON = 1,	// currently also used for guide person (arresting)
			PULL_PERSON = 2,
			PULL_OBJECT = 4,

			ANY = 7	// adapt to contain all other types
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setupEntityAsGhost(qsf::Entity& entity, uint32 playerIndex);
		static void setupEntityAsGhost(qsf::Entity& entity, const qsf::Entity& parentEntity);
		static void teardownEntityAsGhost(qsf::Entity& entity);
		static bool isPositionVisibleForAnyPlayer(const glm::vec3& entityPosition, float additionalRadius = 0.0f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline explicit EntityHelper(qsf::Entity& entity);
		explicit EntityHelper(const qsf::WeakPtr<qsf::Entity>& entity);	// Entity must be valid
		EntityHelper(uint64 entityId, const qsf::Map& map);	// Entity must be valid

		/**
		*  @brief
		*    Destructor
		*/
		inline ~EntityHelper();

		/**
		*  @brief
		*    Is this entity inside the given box?
		*/
		bool isInsideBox(const qsf::TransformComponent& boxTransform, bool yAxisCentered) const;
		bool isInsideBox(const qsf::Entity& boxEntity) const;

		/**
		*  @brief
		*    Is this entity inside the gangster event area?
		*/
		bool isInGangsterEventArea() const;

		/**
		*  @brief
		*    Is entity damaged?
		*/
		bool isEntityDamaged() const;

		void makeImmovable() const;

		/**
		*  @brief
		*    Turn on/off receiver for damage. Currently not work for buildings, complex fire, bushes/trees.
		*/
		void setInvincible(bool invincible) const;

		/**
		*  @brief
		*    Update the placement for the entity with it's current placement component
		*
		*  @param[in] warpMovable
		*    When "true" the movableComponent will be warped to the new position (avoiding interpolation)
		*/
		void applyPlacement(bool warpMovable = false) const;

		/**
		*  @brief
		*    Applies entity basing placement to the given position
		*
		*  @param[in,out] position
		*    Position to apply placement to
		*/
		void applyPlacementToPosition(glm::vec3& position) const;

		/**
		*  @brief
		*    Warp the entity to the given position in a multiplay-compatible way
		*
		*  @param[in] position
		*    Position to warp the entity to
		*/
		void setPositionForMultiplay(glm::vec3& position) const;

		//[-------------------------------------------------------]
		//[ Game logic                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return a game logic instance of a certain type
		*
		*  @return requested
		*    The requested game logic instance or a null pointer if none exists in this entity; do no destroy the returned instance
		*/
		template<typename T>
		T* getGameLogic() const;

		/**
		*  @brief
		*    Return a game logic instance of a certain type
		*
		*  @param[in] gameLogicTypeId
		*    Type ID of the game logic
		*
		*  @return
		*    The requested game logic instance or a null pointer if none exists in this entity; do no destroy the returned instance
		*/
		qsf::GameLogic* getGameLogicByTypeId(uint32 gameLogicTypeId) const;

		/**
		*  @brief
		*    Destroy a game logic instance of a certain type
		*
		*  @param[in] gameLogicTypeId
		*    Type ID of the game logic
		*
		*  @return
		*    "true" if a game logic was actually destroyed, "false" if none was found
		*/
		bool destroyGameLogicByTypeId(uint32 gameLogicTypeId) const;

		//[-------------------------------------------------------]
		//[ Events                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is the entity a valid event target?
		*
		*  @remarks
		*    This includes the following checks:
		*     - Is the entity already affected by any event, or when a freeplay event is given: not affected by the given one?
		*     - Is the entity outside of the map boundaries?
		*     - Is the entity hidden?
		*/
		bool isEntityValidEventTarget(FreeplayEvent* freeplayEvent = nullptr) const;

		//[-------------------------------------------------------]
		//[ Camera & Visibility                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is the entity hidden?
		*/
		bool isEntityHidden() const;

		/**
		*  @brief
		*    Is the entity visible on the local player's screen
		*/
		bool isEntityVisibleOnScreen(bool includeShadow = true) const;

		/**
		*  @brief
		*    Is the entity inside a specific distance to the given camera
		*/
		bool isEntityVisibleToCamera(qsf::CameraControlComponent& cameraControlComponent) const;

		/**
		*  @brief
		*    Checks if the entity is inside a specific distance to the view position of any active player
		*/
		bool isEntityVisibleForAnyPlayer() const;

		bool isInMapBoundaries() const;
		bool isInEventBoundaries() const;

		bool isCloseToMapBoundaries(float tolerance = 1.0f) const;
		bool isCloseToEventBoundaries(float tolerance = 1.0f) const;

		//[-------------------------------------------------------]
		//[ Persons                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "true" if this entity is a person of any kind
		*/
		bool isEntityPerson() const;

		/**
		*  @brief
		*    "true" if this entity is a animal
		*/
		bool isEntityAnimal() const;

		/**
		*  @brief
		*    "true" if this entity is a civil person
		*/
		bool isCivilPerson() const;

		/**
		*  @brief
		*    "true" if this entity is a gangster person
		*/
		bool isGangsterPerson() const;

		/**
		*  @brief
		*    "true" if this entity can become a gangster
		*/
		bool isGangsterCandidate(GangsterPool gangsterPoolType, bool mustBeInGangsterEventArea = true) const;

		/**
		*  @brief
		*    "true" if this entity can be attacked by the specified gangster.
		*	 needsToBeCivilTarget can be set to exclude commandable units or gangsters as valid targets
		*/
		bool isValidGangsterEventTarget(qsf::Entity& gangsterEntity, bool needsToBeCivilTarget) const;

		/**
		*  @brief
		*    "true" if this entity is stunned. Checks the BeStunnedAction
		*/
		bool isStunned() const;

		/**
		*  @brief
		*    "true" if this entity is a commandable squad object
		*/
		bool isCommandable() const;

		/**
		*  @brief
		*    "true" if this entity is a commandable squad person
		*/
		bool isSquadPerson() const;

		/**
		*  @brief
		*    Is this person injured?
		*/
		bool isPersonInjured() const;

		/**
		*  @brief
		*    Is this person contaminated?
		*/
		bool isPersonContaminated() const;

		/**
		*  @brief
		*    Is the person waving out of an window
		*/
		bool isPersonWaving() const;

		/**
		*  @brief
		*    Is the person buried
		*/
		bool isPersonBuried() const;

		/**
		*  @brief
		*    Check if a unit is carrying any of the given types
		*/
		bool isCarrying(CarryLoad type = ANY) const;

		/**
		*  @brief
		*    Get the entity ID of the carried object. You can check for types together as it's not possible to carry more than 1 object at a time.
		*/
		uint64 getCarryTarget(CarryLoad type) const;

		/**
		*  @brief
		*    Reset the carry flag for the given type (or types)
		*/
		void clearCarryTarget(CarryLoad type) const;

		/**
		*  @brief
		*    "true" if the entity has logic for carrying an person
		*/
		bool isCarryingAnotherPerson() const;

		/**
		*  @brief
		*    "true" if the entity has an action to guide another person (for example an arrested person)
		*/
		bool isGuidingPerson() const;

		bool isPullingPerson() const;

		bool isPullingObject() const;

		bool isCarriedByOtherPerson() const;

		bool isSquadPersonInValidState() const;
		bool isSquadVehicleInValidState() const;
		bool isSquadUnitInValidState() const;

		bool isCivilPersonInValidState() const;

		bool isUnitInValidState() const;
		bool isUnitInValidAndHealthyState() const;

		bool isBuildingInValidState() const;

		bool isArrested() const;

		bool canUseUmbrella() const;

		/**
		*  @brief
		*   "true" if the person has the right editing and state for using umbrella at rain time. This function ignores if the umbrella is currently equipped or not.
		*/
		bool isRedirectingTraffic() const;

		/**
		*  @brief
		*    Use to disconnect firemen from hydrants. This functions removes all reservations and free the hydrant. No animations are playing.
		*/
		void disconnectFromHydrant() const;

		/**
		*  @brief
		*    The entity can play acoustic responds when it gets selected
		*/
		bool canPlaySelectionFeedback() const;

		/**
		*  @brief
		*    The entity can play acoustic responds when it gets a command
		*/
		bool canPlayCommandFeedback() const;

		//[-------------------------------------------------------]
		//[ Gangster                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Turn entity into a gangster of specific type
		*
		*  @return
		*    The new gangster logic instance, or a null pointer in case of an error
		*/
		GangsterBaseLogic& turnIntoGangsterByType(const qsf::StringHash& gangsterTypeId) const;

		/**
		*  @brief
		*    Turn entity into a gangster using a given gangster logic
		*
		*  @return
		*    The new gangster logic instance, or a null pointer in case of an error
		*
		*  @note
		*    - Deprecated: Use "turnIntoGangsterByType" instead
		*/
		GangsterBaseLogic& turnIntoGangster(const qsf::StringHash& gangsterLogicId, bool fleeInCar) const;

		/**
		*  @brief
		*    Is the gangster fleeing from police
		*/
		bool isGangsterFleeing() const;

		//[-------------------------------------------------------]
		//[ Building                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is this entity a building or not?
		*/
		bool isEntityBuilding() const;

		/**
		*  @brief
		*    Is entity a industrial plant or not?
		*/
		bool isEntityIndustrialPlant() const;

		/**
		*  @brief
		*    Is building damaged?
		*/
		bool isBuildingDamaged() const;

		/**
		*  @brief
		*    Is building destroyed?
		*/
		bool isBuildingDestroyed() const;

		void placeEntityAtBuildingDoor(qsf::Entity& buildingEntity, bool lookToDoor = true) const;

		//[-------------------------------------------------------]
		//[ Vehicle                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "true if the entity is any kind of vehicle"
		*/
		bool isVehicle() const;

		/**
		*  @brief
		*    "true if the entity is a helicopter"
		*/
		bool isHelicopter() const;

		/**
		*  @brief
		*    "true if the entity is a motorboat boat"
		*/
		bool isBoat() const;

		/**
		*  @brief
		*    "true if the entity is a FLB ship"
		*/
		bool isShip() const;

		/**
		*  @brief
		*    "true if the entity is flying (Starting the rotor is already considered flying)
		*/
		bool isFlying() const;

		/**
		*  @brief
		*    "true if the entity is a unit with
		*/
		bool isUnit(qsf::NamedIdentifier unitName) const;

		bool isVehicleDestroyed() const;

		//[-------------------------------------------------------]
		//[ Road vehicle                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    "true if the entity is a road vehicle of any kind"
		*/
		bool isRoadVehicle() const;

		/**
		*  @brief
		*    "true" if entity is a civil road vehicle
		*/
		bool isCivilRoadVehicle() const;

		/**
		*  @brief
		*    "true" if entity is a road vehicle squad
		*/
		bool isSquadRoadVehicle() const;

		/**
		*  @brief
		*    "true" if entity is a vehicle squad
		*/
		bool isSquadVehicle() const;

		/**
		*  @brief
		*    "true" if road vehicle is damaged
		*/
		bool isRoadVehicleDamaged() const;

		/**
		*  @brief
		*    "true if the entity is a road vehicle with a gangster contained inside
		*/
		bool isGangsterRoadVehicle() const;

		/**
		*  @brief
		*    "true" if target road vehicle has enough space for this entity
		*
		*  @param[in] vehicleEntity
		*    The entity we want to enter
		*
		*  @note
		*    Use this function for checking for seats
		*/
		bool checkVehicleHasFreeSeats(qsf::Entity& vehicleEntity) const;
		bool checkVehicleHasFreeSeatsForSquadPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo, const std::string& callerUnitName) const;
		bool checkVehicleHasFreeSeatsForPassengerPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo) const;

		int countVehicleFreeSeatsForSquadPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo, const std::string& callerUnitName) const;
		int countVehicleFreeSeatsForPassengerPerson(qsf::Entity& vehicleEntity, OrderInfo& targetVehicleOrderInfo) const;

		/**
		*  @brief
		*	Return a road vehicle that supports the given equipment type. Uses the nearest one found.
		*/
		RoadVehicleComponent* getReachableRoadVehicleWithEquipment(const std::string& equipmentName) const;

		/**
		*  @brief
		*	Check if the entity has the given equipment in it's InventoryComponent.
		*   (typically used for checking if unit vehicles support a certain equipment)
		*/
		bool hasEquipment(const std::string& equipmentName) const;

		//[-------------------------------------------------------]
		//[ Vegetation                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is entity part of the vegetation?
		*/
		bool isEntityVegetation() const;

		/**
		*  @brief
		*    Is entity a tree
		*/
		bool isEntityTree() const;

		//[-------------------------------------------------------]
		//[ Fire                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is entity burning?
		*/
		bool isBurning() const;

		/**
		*  @brief
		*    Is this entity burned?
		*/
		bool isBurned() const;

		/**
		*  @brief
		*    Is this entity generally burnable (even if it was already fully burned or is burning right now)?
		*/
		bool isBurnable() const;

		/**
		*  @brief
		*    Can this entity be set on fire right now, i.e. is it burnable, not fully burned yet and does not burn already?
		*/
		bool canBeSetOnFireNow() const;

		/**
		*  @brief
		*    Let this entity start to burn
		*
		*  @param[in] freeplayEvent
		*    Optional freeplay event that the burning objects will get counted for
		*  @param[in] fullFire
		*    If "true", fire energy gets set to 100% and for complex fires, all fire components get incinerated
		*
		*  @return
		*    "true" if the entity is set on fire, "false" otherwise
		*/
		bool startFire(FreeplayEvent* freeplayEvent = nullptr, bool fullFire = false) const;

		/**
		*  @brief
		*    Let this entity stop burning
		*
		*  @return
		*    "true" if the entity is set on fire, "false" otherwise
		*/
		bool stopFire() const;

		/**
		*  @brief
		*    Enable/Disable that this entity can burn
		*
		*  @param[in] canBurn
		*    "true" when the entity can burn otherwise "false"
		*
		*  @note
		*    This method activates/deactivates the fire component associated with the entity
		*    When deactivated the fire component is excluded from any updates
		*/
		void setCanBurn(bool canBurn) const;

		//[-------------------------------------------------------]
		//[ Container                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return "true" if the container of the entity is empty
		*/
		bool isContainerEmpty() const;

		/**
		*  @brief
		*    Return the entity in which the owner is inside, or null pointer if there is none
		*/
		qsf::Entity* getContainerEntity() const;

		/**
		*  @brief
		*    Return the container entity the entity is nested in
		*/
		qsf::Entity& getContainerEntitySafe() const;

		/**
		*  @brief
		*    Return "true" if entity is inside building
		*/
		bool isEntityInBuilding() const;

		/**
		*  @brief
		*    Return "true" if entity is inside vehicle
		*/
		bool isEntityInVehicle() const;
		bool isEntityInRoadVehicle() const;

		qsf::game::PrototypeContainer* getPrototypeContainer() const;

		bool isEntityHardLinked() const;
		void unlinkFromParent() const;

		/**
		*  @brief
		*    The entity handled by the EntityHelper instance enters a container
		*
		*  @param[in] containerEntity
		*     The container entity which should be entered
		*  @param[in] containerCategory
		*     The container category inside the container
		*  @param[in] hidden
		*     Should the entity after entering the container be hidden?
		*/
		void enterContainer(qsf::Entity& containerEntity, uint32 containerCategory = 0, bool hidden = true) const;
		void leaveContainer(bool unhide = true) const;
		bool isInContainer(qsf::Entity& containerEntity, uint32 containerCategory) const;
		bool isInContainer(qsf::Entity& containerEntity) const;

		void setCollisionEnabled(bool collision) const;

		bool isEntityBuried() const;

		//[-------------------------------------------------------]
		//[ Cleanup                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Are the fire components ready to cleanup
		*/
		bool isFireReadyToCleanup() const;

		/**
		*  @brief
		*    Cleanup fire component
		*/
		void cleanupFire() const;

		//[-------------------------------------------------------]
		//[ Event tag                                             ]
		//[-------------------------------------------------------]
		bool checkEntityHasEventTag(const std::string& tag) const;

		//[-------------------------------------------------------]
		//[ Event ID component                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if there is an initialized event ID component
		*/
		bool checkForInitializedEventIdComponent(FreeplayEvent* freeplayEvent = nullptr) const;

		/**
		*  @brief
		*    Check if there is an event ID component at all
		*/
		bool checkForEventIdComponent() const;

		/**
		*  @brief
		*    Get the event of the event ID component of the entity. Can return null pointer.
		*/
		FreeplayEvent* getFreeplayEvent() const;

		/**
		*  @brief
		*    Get the event ID of the event that effects the entity
		*
		*  @return
		*    The event ID of the event ID component or uninitialized if there is none
		*/
		uint32 getFreeplayEventId() const;

		//[-------------------------------------------------------]
		//[ Equipment / Inventory                                 ]
		//[-------------------------------------------------------]
		qsf::Entity* getActiveEquipmentEntity() const;
		qsf::game::EquipmentComponent* getActiveEquipmentComponent() const;

		/**
		*  @brief
		*    Return "true" if the active equipment has the given name (e.g.. "chainsaw")
		*/
		bool checkIsActiveEquipment(const std::string& equipmentName) const;

		void storeActiveEquipmentInInventory() const;
		void storeInventoryEquipmentAsActive() const;

		/**
		*  @brief
		*    Call to destroy the currently active equipment (if any), also take takes care for correct handling in inventory and selection
		*/
		void deleteActiveEquipment(qsf::Time timeToDelete = qsf::Time::ZERO) const;
		void linkEquipmentToEntity(qsf::Entity& parent) const;

		/**
		*  @brief
		*		Call to create new equipment from prefab and make it visible (equipped)
		*		Does nothing if this is already the active equipment
		*		Does call deleteActiveEquipment() when another equipment is active
		*  @param[in] equipmentPrefab
		*		The path to the prefab, e.g. "em5/prefab/equipment/equipment_police_pistol".
		*		As shortcut the constants defined in the "EquipmentAssets.h" can used. In this case the prefab must be inside the folder "em5/prefab/equipment/" e.g. showEquipment(equipment::HANDCUFFS)
		*  @param[in] forceTintColorIndex
		*		-1 means random tint color, everything else is used as tint color for the equipment (if possible)
		*/
		void showEquipment(const std::string& equipmentPrefab, int forceTintColorIndex = -1) const;

		/**
		*  @brief
		*    Destroy all linked child entities. Also delete MeshAnimationTestComponent. Use-case are e.g. pre-edited gangster or gaper how changed back to normal behavior (e.g. ambient gangster with beer, know turning into real gangster with weapon)
		*/
		void deleteAllLinkedChild();

		void moveEntityInOwnerLayer(qsf::Entity& entityToMove) const;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		uint32 getUnitPlayerIndex() const;

		/**
		*  @brief
		*    Returns true if the entity is a unit and assigned to the local player
		*/
		bool isUnitFromLocalPlayer() const;

		/**
		*  @brief
		*    Returns true if the entity is a unit and assigned to a another player (i.e. not the local player)
		*/
		bool isUnitOfDifferentPlayer() const;

		/**
		*  @brief
		*    Get the order info of the entity. Can return null pointer.
		*/
		OrderInfo* getOrderInfo() const;

		void unregisterFromUnitPool() const;
		void blockFromUnitPool() const;

		/**
		*  @brief
		*    Help to combine the redundant code between unregisterFromUnitPool() and blockFromUnitPool(), the boolean parameter indicates what branch is going
		*
		*  @param[in] unregister
		*    "true" if unit must unregister from unit pool, false if unit must blocking
		*/
		void unregisterOrBlockFromUnitPool(bool unregister) const;

		/**
		*  @brief
		*    Get the parent entity of the link component, returns nullptr if not linked
		*/
		qsf::Entity* getParentEntity() const;

		/**
		*  @brief
		*    Enable/Disable shine-through effect for the wrapped entity
		*/
		void enableShineThroughEffect(bool enable = true) const;

		/**
		*  @brief
		*    Find a linked flare and change its activity
		*/
		void setFlareActive(bool active) const;

		//[-------------------------------------------------------]
		//[ Lights                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Activates/Deactivates lights in the entity
		*
		*  @param[in] active
		*    "true" if the light should be active, otherwise false
		*/
		void setLightActive(bool active) const;

		/**
		*  @brief
		*    Finds the entity closest to this one
		*	 Use case: You already picked those entity candidates, that fulfill certain requirements,
		*	 now you need the one that is at nearest distance
		*
		*	 This function can deal with null pointers in "entities".
		*
		*  @return
		*    The closest entity from the given vector
		*/
		qsf::Entity* getClosestEntity(const std::vector<qsf::Entity*>& entities) const;

		//[-------------------------------------------------------]
		//[ Fade                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check if entity is fading in or out using the "em5::FadeEffectComponent"
		*/
		bool isFading() const;

		/**
		*  @brief
		*    Fade in an entity using the "em5::FadeEffectComponent"
		*
		*  @param[in] fadeTime
		*    Time taken for fading (1 second as default)
		*/
		void fadeIn(qsf::Time fadeTime = qsf::Time::fromSeconds(1.0f)) const;

		/**
		*  @brief
		*    Fade out an entity using the "em5::FadeEffectComponent"; the entity is not deleted, just hidden
		*
		*  @param[in] fadeTime
		*    Time taken for fading (1 second as default)
		*  @param[in] deleteOwner
		*    "true" if owner is deleted from map after fading (false as default)
		*/
		void fadeOut(qsf::Time fadeTime = qsf::Time::fromSeconds(1.0f), bool deleteOwner = false) const;

		/**
		*  @brief
		*    Set entity visible or not and fade to that state
		*
		*  @param[in] fadeTime
		*    Time taken for fading (1 second as default)
		*/
		void setVisibleAndFade(bool visible, qsf::Time fadeTime = qsf::Time::fromSeconds(1.0f)) const;

		//[-------------------------------------------------------]
		//[ Misc                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return a vector of component instances of a certain type
		*
		*  @param[out] output
		*    An vector of the requested component instances, zero size vector if no components were found, do not destroy the component instances in the vector
		*/
		void getComponentsFromChildrenById(uint32 componentId, std::vector<qsf::Component*>& output) const;

		/**
		*  @brief
		*    Return a vector of component instances of a certain type
		*
		*  @param[out] output
		*    An vector of the requested component instances, zero size vector if no components were found, do not destroy the component instances in the vector
		*/
		template<typename T> void getComponentsFromChildren(std::vector<T*>& output) const;

		/**
		*  @brief
		*    Check whether or not the entity has an enabled mesh animation test component
		*/
		bool hasEnabledMeshAnimationTestComponent() const;

		/**
		*  @brief
		*    Disable (but not destroy) mesh animation test component
		*/
		void disableMeshAnimationTestComponent();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/map/EntityHelper-inl.h"
