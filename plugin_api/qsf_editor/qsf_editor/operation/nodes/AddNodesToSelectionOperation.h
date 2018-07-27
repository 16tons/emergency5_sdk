// Copyright (C) 2012-2017 Promotion Software GmbH


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
		*    Add nodes to selection operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, the user can only select entities and not entity local stuff by selecting and hence locking e.g. single nodes
		*/
		class AddNodesToSelectionOperation : public NodeSelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::AddNodesToSelectionOperation" unique operation ID


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
			*  @param[in] nodeIdSet
			*    The unique node identifiers to add to the selection
			*/
			AddNodesToSelectionOperation(uint64 entityId, uint32 componentId, const NodeSelectionManager::IdSet& nodeIdSet);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AddNodesToSelectionOperation();

			/**
			*  @brief
			*    Return the unique node identifiers to add to the selection
			*
			*  @return
			*    The unique node identifiers to add to the selection, do not destroy the instance
			*/
			const NodeSelectionManager::IdSet& getNodeIdSet() const;


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
			NodeSelectionManager::IdSet mNodeIdSet;	///< The unique node identifiers to add to the selection


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
