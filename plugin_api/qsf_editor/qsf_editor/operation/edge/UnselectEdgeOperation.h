// Copyright (C) 2012-2017 Promotion Software GmbH


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
		*    Unselect edge component edge operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, the user can only select entities and not entity local stuff by selecting and hence locking e.g. single edges
		*/
		class UnselectEdgeOperation : public EdgeSelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::SelectEdgeOperation" unique operation ID


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
			*  @param[in] edgeId
			*    ID of the edge to unselect
			*/
			UnselectEdgeOperation(uint64 entityId, uint32 componentId, unsigned int edgeId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~UnselectEdgeOperation();

			/**
			*  @brief
			*    Return the ID of the edge selected by this operation
			*
			*  @return
			*    The ID of the edge selected by this operation
			*/
			unsigned int getEdgeId() const;

			/**
			*  @brief
			*    Return the IDs of the previously selected edges
			*
			*  @return
			*    The IDs of the previously selected edges, do not destroy the instance
			*/
			const EdgeSelectionManager::IdSet& getPreviousEdgeIdSet() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual unsigned int getId() const;
			virtual size_t getMemoryConsumption() const;
			virtual void undoImpl();
			virtual void redoImpl();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			unsigned int				mEdgeId;			///< Edge ID to unselect
			EdgeSelectionManager::IdSet mPreviousEdgeIdSet;	///< The IDs of the previously selected edges


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
