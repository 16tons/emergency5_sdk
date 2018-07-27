// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/component/ComponentOperation.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StreetComponent;
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
		*    Edge selection operation base class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, the user can only select entities and not entity local stuff by selecting and hence locking e.g. single edges
		*/
		class EdgeSelectionOperation : public base::ComponentOperation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EdgeSelectionOperation();

			/**
			*  @brief
			*    Return the entity edges component this operation is performed on
			*
			*  @return
			*    The entity edges component this operation is performed on, can be a null pointer, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			StreetComponent* getEdgeComponent() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual bool isLocal() const override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityId
			*    The unique entity identifier
			*  @param[in] componentId
			*    The unique (inside the entity) component identifier
			*/
			EdgeSelectionOperation(uint64 entityId, uint32 componentId);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
