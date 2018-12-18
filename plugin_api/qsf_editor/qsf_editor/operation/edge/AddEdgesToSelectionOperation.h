// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/edge/EdgeSelectionManager.h"
#include "qsf_editor/operation/edge/EdgeSelectionOperation.h"


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
		*    Add edges to selection operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, the user can only select entities and not entity local stuff by selecting and hence locking e.g. single edges
		*/
		class AddEdgesToSelectionOperation : public EdgeSelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::AddEdgesToSelectionOperation" unique operation ID


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
			*  @param[in] edgeIdSet
			*    The unique edge identifiers to add to the selection
			*/
			AddEdgesToSelectionOperation(uint64 entityId, uint32 componentId, const EdgeSelectionManager::IdSet& edgeIdSet);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AddEdgesToSelectionOperation();

			/**
			*  @brief
			*    Return the unique edge identifiers to add to the selection
			*
			*  @return
			*    The unique edge identifiers to add to the selection, do not destroy the instance
			*/
			const EdgeSelectionManager::IdSet& getEdgeIdSet() const;


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
			EdgeSelectionManager::IdSet mEdgeIdSet;	///< The unique edge identifiers to add to the selection


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
