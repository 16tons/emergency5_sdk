// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationHelper.h"
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/utility/AnnotationComponent.h>
#include <qsf/prototype/helper/SlotHelper.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Prototype;
	class Component;
	class Transform;
	class PrefabContent;
	namespace editor
	{
		namespace base
		{
			class Operation;
			class CompoundOperation;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static entity operation helper class
		*/
		class QSF_EDITOR_API_EXPORT EntityOperationHelper : public OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum Axis
			{
				AXIS_X = 0,
				AXIS_Y = 1,
				AXIS_Z = 2
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			//[-------------------------------------------------------]
			//[ Create                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that creates a new entity
			*
			*  @param[in]  prototype
			*    The prototype to clone the entity from
			*  @param[in]  position
			*    The world space position where the entity should be placed
			*  @param[in]  layerId
			*    ID of the layer the new entity should reside in, if uninitialized the currently selected layer will be used
			*  @param[in]  select
			*    Automatically select the new entity?
			*  @param[out] createdEntityId
			*    If not a null pointer, this will receive the ID of the created entity, not touched on error
			*  @param[out] copyOverrideFlags
			*    Copy override flags?
			*  @param[out] createdEntityIds
			*    If not a null pointer, this will receive the IDs of all the created entities, list is not cleared before new entries are added, not touched on error
			*  @param[out] createdEntityIds
			*    If not a null pointer, this will receive the IDs of all the created entities, list is not cleared before new entries are added, not touched on error
			*  @param[out] prefabContent
			*    If not a null pointer, this prefab content will be used instead of a local one, useful if you need for example a <name> <-> <entity ID> mapping during operation generation
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCreateEntityOperation(const Prototype& prototype, const glm::vec3& position, bool select = false, uint32 layerId = getUninitialized<uint32>(), uint64* createdEntityId = nullptr, bool copyOverrideFlags = false, std::vector<uint64>* createdEntityIds = nullptr, PrefabContent* prefabContent = nullptr);

			/**
			*  @brief
			*    Build an operation that creates multiple new entities at once
			*
			*  @param[in] idSet
			*    IDs of the entities to clone
			*  @param[in] numberOfInstances
			*    Number of entities to create
			*  @param[in] transformOffset
			*    Transform offset added to each new entity following the first instance
			*  @param[in] localMultiselectionRotation
			*    If true each object will rotate around its own center instead rotating around the current entity
			*  @param[in] referencePosition
			*    World space reference position for rotation of multiselection around a certain point in space (in case "localMultiselectionRotation" is true)
			*  @param[in] individualFollowGround
			*    If true follow ground mode is enabled for each individual object
			*  @param[in] followGroundIgnoreEntityIdSet
			*    The entities to ignore while performing the follow ground height calculation, can be a null pointer
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCreateMultipleEntitiesOperation(const EntitySelectionManager::IdSet& idSet, uint32 numberOfInstances, const Transform& transformOffset, bool localMultiselectionRotation = false, const glm::vec3& referencePosition = glm::vec3(0.0f, 0.0f, 0.0f), bool individualFollowGround = false, const EntitySelectionManager::IdSet* followGroundIgnoreEntityIdSet = nullptr);

			/**
			*  @brief
			*    Build an operation that pastes an entity
			*
			*  @param[in]  entity
			*    The entity to clone the pasted entity from
			*  @param[in]  position
			*    The world space position where the pasted entity should be placed
			*  @param[in]  destroySource
			*    Destroy the copied source entity?
			*  @param[out] createdEntityId
			*    If not a null pointer, this will receive the ID of the pasted entity, not touched on error
			*  @param[in]  select
			*    Automatic selection handling?
			*  @param[in]  layerId
			*    ID of the layer the new entity should reside in, if uninitialized the currently selected layer will be used
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildPasteEntityOperation(Entity& entity, const glm::vec3& position, bool destroySource, uint64* createdEntityId = nullptr, bool select = true, uint32 layerId = getUninitialized<uint32>());

			/**
			*  @brief
			*    Build an operation that pastes entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to clone the pasted entities from
			*  @param[in] position
			*    The world space position where the first pasted entity should be placed
			*  @param[in] destroySource
			*    Destroy the copied source entities?
			*  @param[in] clearSelection
			*    Clear complete selection before adding new entities to the selection?
			*  @param[in]  layerId
			*    ID of the layer the new entity should reside in, if uninitialized the currently selected layer will be used
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildPasteEntitiesOperation(const EntitySelectionManager::IdSet& idSet, const glm::vec3& position, bool destroySource, bool clearSelection = true, uint32 layerId = getUninitialized<uint32>());

			/**
			*  @brief
			*    Build an operation that creates an annotation
			*
			*  @param[in] position
			*    The world space position where to put the annotation
			*  @param[in] annotationType
			*    Annotation type
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* builCreateAnnotationOperation(const glm::vec3& position, AnnotationComponent::AnnotationType annotationType);

			//[-------------------------------------------------------]
			//[ Destroy                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that destroys an entity
			*
			*  @param[in] entity
			*    The entity to be destroyed
			*  @param[in] unselect
			*    Do automatically unselect the entity before destroying it?
			*  @param[in] mercilessDestruction
			*    If true, also soft-linked entities will be destroyed, else only hard-linked entities will be destroyed
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDestroyEntityOperation(Entity& entity, bool unselect = true, bool mercilessDestruction = false);

			/**
			*  @brief
			*    Build an operation that hide / unhide entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to be destroyed
			*  @param[in] show
			*    The flag if the entities set visible or hidden
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildHideEntitiesOperation(const EntitySelectionManager::IdSet& idSet, bool show);

			/**
			*  @brief
			*    Build an operation that destroys entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to be destroyed
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDestroyEntitiesOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that destroys a component of an entity
			*
			*  @param[in] component
			*    The component to be destroy
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDestroyComponentOperation(Component& component);

			//[-------------------------------------------------------]
			//[ Replace                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that replaces entities by a prototype
			*
			*  @param[in] idSetToReplace
			*    The IDs of the entities to be replaced
			*  @param[in] prototype
			*    The prototype to replace the entity with
			*  @param[in] select
			*    Perform automatic selection handling?
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildReplaceEntitiesWithCopyOperation(const EntitySelectionManager::IdSet& idSetToReplace, Prototype& prototype, bool select = true);

			/**
			*  @brief
			*    Build an operation that replaces entities by other entities
			*
			*  @param[in] idSetToReplace
			*    The IDs of the entities to be replaced
			*  @param[in] idSetToCopy
			*    The IDs of the entities to replace the first entities with
			*  @param[in] destroySource
			*    Destroy the copied source entities?
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildReplaceEntitiesWithCopyOperation(const EntitySelectionManager::IdSet& idSetToReplace, const EntitySelectionManager::IdSet& idSetToCopy, bool destroySource);

			/**
			*  @brief
			*    Build an operation that resets entities to their prototype
			*
			*  @param[in] idSet
			*    The IDs of the entities to reset
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildResetEntitiesToPrefabOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that updates entities after their prototype changed
			*
			*  @param[in] idSet
			*    The IDs of the entities to update
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUpdateEntitiesFromPrefabOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that removes all inheritance information from entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to change
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildDisbandEntitiesInstantiationFromPrefabOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that changes entities' base prefab
			*
			*  @param[in] idSet
			*    The IDs of the entities to modify
			*  @param[in] prefabGlobalAssetId
			*    Global asset ID of the prefab to set
			*  @param[in] select
			*    Perform automatic selection handling?
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildChangeEntitiesBasePrefabOperation(const EntitySelectionManager::IdSet& idSet, GlobalAssetId prefabGlobalAssetId, bool select = true);

			//[-------------------------------------------------------]
			//[ Select                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that clears the entity selection
			*
			*  @return
			*    The created operation instance, can be a null pointer, destroy the operation instance if you no longer need it
			*
			*  @remarks
			*    This method only generates an operation in case there's a real selection change. In case there's no change a null pointer will be returned.
			*/
			static base::Operation* buildClearEntitySelectionOperation();

			/**
			*  @brief
			*    Build an operation that selects an object or group
			*
			*  @param[in] entity
			*    The entity to be selected
			*
			*  @return
			*    The created operation instance, can be a null pointer, destroy the operation instance if you no longer need it
			*
			*  @remarks
			*    While the atomic "qsf::editor::SelectEntityOperation"-operation directly selects the given entity, this operation helper method
			*    select the "object" or "group" the entity is in. In case an entity was selected which is linked to another entity, the parent
			*    entity will be selected by this operation. This method only generates an operation in case there's a real selection change. In
			*    case there's no change a null pointer will be returned.
			*/
			static base::Operation* buildSelectOperation(Entity& entity);

			/**
			*  @brief
			*    Build an operation that selects multiple entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to be selected
			*  @param[in] allEntitiesRequired
			*    By default, entities which are currently locked by another editor user are not going to be selected; if this parameter is
			*    "true" it means all or nothing, resulting in a null pointer being returned in case one of the requested entities is currently
			*    locked by another editor user
			*
			*  @return
			*    The created operation instance, can be a null pointer, destroy the operation instance if you no longer need it
			*
			*  @remarks
			*    This operation uses the atomic operation "qsf::editor::AddEntitiesToSelectionOperation" that adds multiple entities to the
			*    entity selection, but clears the selection first.
			*/
			static base::Operation* buildSelectEntitiesOperation(const EntitySelectionManager::IdSet& idSet, bool allEntitiesRequired = false);

			/**
			*  @brief
			*    Build an operation that selects multiple entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to be selected
			*  @param[in] idSetRemoved
			*    The IDs of the entities to be removed from the selection
			*
			*  @return
			*    The created operation instance, can be a null pointer, destroy the operation instance if you no longer need it
			*
			*  @remarks
			*    This operation uses the atomic operation "qsf::editor::AddEntitiesToSelectionOperation" that adds multiple entities to the
			*    entity selection and "qsf::editor::RemoveEntitiesFromSelectionOperation" to remove entities form the selection
			*/
			static base::Operation* buildSelectEntitiesOperation(const EntitySelectionManager::IdSet& idSet, const EntitySelectionManager::IdSet& idSetRemoved);

			//[-------------------------------------------------------]
			//[ Link                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that groups/ungroups entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to group/ungroup
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*
			*  @remarks
			*    Entity groups is all about multi-selection and linking. Selecting an entity group automatically selects all
			*    group members.
			*/
			static base::Operation* buildGroupUnGroupOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that links entities
			*
			*  @param[in] parentEntity
			*    The entity to link other entity to
			*  @param[in] idSet
			*    The IDs of the entities to link to the given entity
			*  @param[in] linkType
			*    The link type to use
			*  @param[in] slotIdentifier
			*    Information identifying a slot to link to
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildLinkEntitiesOperation(Entity& parentEntity, const EntitySelectionManager::IdSet& idSet, LinkComponent::LinkType linkType, const SlotHelper::SlotIdentifier* slotIdentifier = nullptr);

			/**
			*  @brief
			*    Build an operation that unlinks entities from a certain slot
			*
			*  @param[in] parentEntity
			*    The entity to unlink other entities from
			*  @param[in] slotIdentifier
			*    Information identifying a slot to unlink from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUnlinkEntitiesFromSlotOperation(Entity& parentEntity, const SlotHelper::SlotIdentifier& slotIdentifier);

			/**
			*  @brief
			*    Build an operation that unlinks entities that are not part of any slot
			*
			*  @param[in] parentEntity
			*    The entity to unlink other entities from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUnlinkEntitiesWithoutSlotOperation(Entity& parentEntity);

			/**
			*  @brief
			*    Build an operation that removes a entity from a slot
			*
			*  @param[in] parentEntity
			*    The entity to unlink the specified entity
			*  @param[in] entityIdToBeRemoved
			*    The entity ID to remove from the slot property
			*  @param[in] slotIdentifier
			*    Information identifying a slot to remove given entity from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildRemoveEntityFromParentSlot(Entity& parentEntity, uint64 entityIdToBeRemoved, const SlotHelper::SlotIdentifier& slotIdentifier);

			/**
			*  @brief
			*    Build an operation that adds a entity to an slot
			*
			*  @param[in] parentEntity
			*    The entity to add the specified entity
			*  @param[in] entityIdToBeAdded
			*    The entity ID to added to the slot property
			*  @param[in] slotIdentifier
			*    Information identifying a slot to add given entity from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* builAddEntityToParentSlot(Entity& parentEntity, uint64 entityIdToBeAdded, const SlotHelper::SlotIdentifier& slotIdentifier);

			/**
			*  @brief
			*    Build an operation that unlinks an entity from its parent
			*
			*  @param[in] entity
			*    The entity to unlink from its parent
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUnlinkFromParentEntityOperation(Entity& entity);

			/**
			*  @brief
			*    Build an operation that unlinks direct entities linked to the given parent entity
			*
			*  @param[in] parentEntity
			*    The parent entity to unlink direct entities from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUnlinkDirectLinkedEntitiesOperation(Entity& parentEntity);

			/**
			*  @brief
			*    Build an operation that unlinks all entities linked to the given parent entity
			*
			*  @param[in] parentEntity
			*    The parent entity to unlink all entities from
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildUnlinkAllLinkedEntitiesOperation(Entity& parentEntity);

			/**
			*  @brief
			*    Build an operation that unlinks all entities linked to the given parent entity
			*
			*  @param[in] idSet
			*    The IDs of the entities to group/ungroup
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*
			*  @note
			*    - Multi-selection support was explicitly requested by our artists
			*/
			static base::Operation* buildUnlinkAllLinkedEntitiesOperation(const EntitySelectionManager::IdSet& idSet);

			//[-------------------------------------------------------]
			//[ Utility                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Build an operation that snaps entities to the ground
			*
			*  @param[in] idSet
			*    The IDs of the entities to snap to the ground
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSnapEntitiesToGroundOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that snaps entities to other entities which are not part of the given ID set
			*
			*  @param[in] idSet
			*    The IDs of the entities to snap to other entities which are not part of the given ID set
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSnapEntitiesToEntitiesOperation(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that flips entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to flip
			*  @param[in] axis
			*    Axis to flip the given entities around
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildFlipEntitiesOperation(const EntitySelectionManager::IdSet& idSet, Axis axis);

			/**
			*  @brief
			*    Build an operation that set the rotation of entities
			*
			*  @param[in] idSet
			*    The IDs of the entities to flip
			*  @param[in] rotation
			*    The rotation value the entities get
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildRotateEntitiesOperation(const EntitySelectionManager::IdSet& idSet, const glm::quat& rotation);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::unordered_map<uint64, uint64> UniqueIdMap;	///< Key = Prototype ID, value = entity ID


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			static bool buildCreateEntityOperationInternal(base::CompoundOperation& compoundOperation, const Prototype& prototype, const glm::vec3& position, bool select, uint32 layerId, uint64* createdEntityId, bool copyOverrideFlags, std::vector<uint64>* createdEntityIds, const UniqueIdMap& uniqueIdMap, PrefabContent* prefabContent);
			static bool buildReplaceEntityWithCopyOperationInternal(base::CompoundOperation& compoundOperation, Entity& entityToReplace, Prototype& prototypeToCopy, bool destroySource, bool select, bool copyOverrideFlags);
			static bool buildUpdateEntityFromPrefabOperation(base::CompoundOperation& compoundOperation, Entity& entityToUpdate, Prototype& prefabMainPrototype);
			static void buildInstantiateTemporaryPrototypesOperation(base::CompoundOperation& compoundOperation, const std::vector<Prototype*>& temporaryPrototypes, uint32 layerId, bool select);
			static void buildUnlinkAllLinkedEntitiesOperationRecursive(base::CompoundOperation& compoundOperation, EntitySelectionManager::IdSet& idSet, LinkComponent& parentLinkComponent);
			static void buildRemovePrototypesFromAllSlotsOperation(base::CompoundOperation& compoundOperation, Prototype& parentPrototype, const EntitySelectionManager::IdSet& childIdSet);
			static void buildUnlinkEntityOperation(base::CompoundOperation& compoundOperation, uint64 entityId);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
