// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationHelper.h"
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	namespace editor
	{
		namespace base
		{
			class Operation;
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
		*    Static layer operation helper class
		*/
		class QSF_EDITOR_API_EXPORT LayerOperationHelper : public OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Build an operation that creates a layer
			*
			*  @param[in]  parentLayerId
			*    The unique layer identifier of the parent layer the new layer is in
			*  @param[in]  selectNewLayer
			*    "true" in case the operation should automatically select the created layer
			*  @param[out] createdLayerId
			*    If not a null pointer, this will receive the ID of the created entity, not touched on error
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCreateLayerOperation(uint32 parentLayerId, bool selectNewLayer = true, uint32* createdLayerId = nullptr);

			/**
			*  @brief
			*    Build an operation that destroys a layer
			*
			*  @param[in] layerId
			*    The unique layer identifier of the layer to destroy, please note that it's not allowed to destroy the base layer which has the ID 0
			*  @param[in] moveToLayerId
			*    The unique layer identifier of the layer to move the entities of the destroyed layer to, if "qsf::getUninitialized<uint32>()"
			*    the entities are destroyed; if it's a child layer of the destroyed later the entities will be destroyed
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*
			*  @note
			*    - Unlike the "qsf::editor::base::DestroyLayerOperation()" operation, the created compound operation also destroys the layers which are inside the destroyed layer
			*    - Unlike the "qsf::editor::base::DestroyLayerOperation()" operation, the created compound operation also destroys the entities which are inside the destroyed layer
			*/
			static base::Operation* buildDestroyLayerOperation(uint32 layerId, uint32 moveToLayerId = getUninitialized<uint32>());

			/**
			*  @brief
			*    Build an operation that selects a layer, but only if the layer isn't selected yet
			*
			*  @param[in] layerId
			*    The unique layer identifier of the layer to select
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSelectLayerOperation(uint32 layerId);

			/**
			*  @brief
			*    Build an operation that moves an entity to a layer
			*
			*  @param[in] entity
			*    Entity to move to a layer
			*  @param[in] layerId
			*    The unique layer identifier of the layer to move the given entity to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildMoveEntityToLayerOperation(Entity& entity, uint32 layerId);

			/**
			*  @brief
			*    Build an operation that moves entities to a layer
			*
			*  @param[in] idSet
			*    IDs of the entities to move to a layer
			*  @param[in] layerId
			*    The unique layer identifier of the layer to move the given entities to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildMoveEntitiesToLayerOperation(const EntitySelectionManager::IdSet& idSet, uint32 layerId);

			/**
			*  @brief
			*    Build an operation that copies an entity to a layer
			*
			*  @param[in] entity
			*    Entity to copy to a layer
			*  @param[in] layerId
			*    The unique layer identifier of the layer to copy the given entity to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCopyEntityToLayerOperation(Entity& entity, uint32 layerId);

			/**
			*  @brief
			*    Build an operation that copies entities to a layer
			*
			*  @param[in] entity
			*    IDs of the entities to copy to a layer
			*  @param[in] layerId
			*    The unique layer identifier of the layer to copy the given entities to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCopyEntitiesToLayerOperation(const EntitySelectionManager::IdSet& idSet, uint32 layerId);

			/**
			*  @brief
			*    Build an operation that moves an layer to a layer
			*
			*  @param[in] layerFromId
			*    Layer to move to a layer
			*  @param[in] layerToId
			*    The unique layer identifier of the layer to move the given layer to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*
			*  @note
			*    - This deletes/merges the layer and moves all entities to the other layer
			*/
			static base::Operation* buildMoveLayerToLayerOperation(uint32 layerFromId, uint32 layerToId);

			/**
			*  @brief
			*    Build an operation that copy an layer with all entities and sublayer to a layer
			*
			*  @param[in] layerFromId
			*    Layer to move to a layer
			*  @param[in] layerToId
			*    The unique layer identifier of the layer to move the given layer to, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCopyLayerToLayerOperation(uint32 layerFromId, uint32 layerToId);

			/**
			*  @brief
			*    Build an operation that reparent an layer
			*
			*  @param[in] layerId
			*    Layer to be reparent
			*  @param[in] newParentlayerToId
			*    The unique layer identifier of the layer which gets the new parent, must be valid
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildReparentLayerOperation(uint32 layerId, uint32 newParentlayerToId);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
