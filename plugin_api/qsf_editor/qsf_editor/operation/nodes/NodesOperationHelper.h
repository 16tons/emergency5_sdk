// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/OperationHelper.h"
#include "qsf_editor/selection/node/NodeSelectionManager.h"

#include <qsf/component/nodes/Node.h>

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class NodesComponent;
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
		*    Static nodes component operation helper class
		*/
		class QSF_EDITOR_API_EXPORT NodesOperationHelper : public OperationHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Build an operation that creates an entity with a nodes component instance
			*
			*  @param[in] componentId
			*    ID of the concrete nodes component type
			*  @param[in] name
			*    The general name of the entity instance, the operation might change the name in detail
			*  @param[in] position
			*    The world space position where the entity should be placed
			*  @param[in] nodes
			*    Nodes
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildCreateNodesOperation(uint32 componentId, const std::string& name, const glm::vec3& position, const std::vector<Node>& nodes);

			/**
			*  @brief
			*    Build an operation that aligns nodes component to a given new center
			*
			*  @param[in] nodesComponent
			*    Nodes component to align to its center
			*  @param[in] newCenter
			*    New center
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSetCenterNodesOperation(const NodesComponent& nodesComponent, const glm::vec3& newCenter);

			/**
			*  @brief
			*    Build an operation that aligns nodes component to its center
			*
			*  @param[in] nodesComponent
			*    Nodes component to align to its center
			*  @param[in] newCenter
			*    New center
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildAlignToCenterOperation(const NodesComponent& nodesComponent);

			/**
			*  @brief
			*    Build an operation that selects a set of nodes
			*
			*  @param[in] idSet
			*    The IDs of the nodes to be selected
			*
			*  @return
			*    The created operation instance, can be a null pointer, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSelectNodesOperation(uint64 entityId, uint32 componentId, const NodeSelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that snaps nodes to the ground
			*
			*  @param[in] nodesComponent
			*    Nodes component to work with
			*  @param[in] idSet
			*    The IDs of the nodes to snap to the ground
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSnapNodesToGroundOperation(const NodesComponent& nodesComponent, const NodeSelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Build an operation that snaps nodes to entities
			*
			*  @param[in] nodesComponent
			*    Nodes component to work with
			*  @param[in] idSet
			*    The IDs of the nodes to snap to entities
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			static base::Operation* buildSnapNodesToEntitiesOperation(const NodesComponent& nodesComponent, const NodeSelectionManager::IdSet& idSet);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
