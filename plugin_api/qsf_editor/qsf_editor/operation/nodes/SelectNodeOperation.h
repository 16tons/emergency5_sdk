// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/node/NodeSelectionManager.h"
#include "qsf_editor/operation/nodes/NodeSelectionOperation.h"


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
		*    Select node component nodes operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, the user can only select entities and not entity local stuff by selecting and hence locking e.g. single nodes
		*/
		class SelectNodeOperation : public NodeSelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::SelectNodeOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier
			*  @param[in] nodeId
			*    ID of the node to select
			*/
			SelectNodeOperation(uint64 entityId, uint32 componentId, unsigned int nodeId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SelectNodeOperation();

			/**
			*  @brief
			*    Return the ID of the node selected by this operation
			*
			*  @return
			*    The ID of the node selected by this operation
			*/
			unsigned int getNodeId() const;

			/**
			*  @brief
			*    Return the IDs of the previously selected nodes
			*
			*  @return
			*    The IDs of the previously selected nodes, do not destroy the instance
			*/
			const NodeSelectionManager::IdSet& getPreviousNodeIdSet() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual size_t getMemoryConsumption() const override;
			virtual void undoImpl() override;
			virtual void redoImpl() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			unsigned int				mNodeId;			///< Node ID to select
			NodeSelectionManager::IdSet mPreviousNodeIdSet;	///< The IDs of the previously selected nodes


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
