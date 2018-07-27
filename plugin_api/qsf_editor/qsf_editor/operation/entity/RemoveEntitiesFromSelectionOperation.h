// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/entity/RemoveEntitiesFromSelectionOperation.h>


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
		*    Remove entities to selection operation class
		*/
		class RemoveEntitiesFromSelectionOperation : public base::RemoveEntitiesFromSelectionOperation
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
			friend class EntitySelectionManager;	///< Only the entity selection manager is allowed to create instances of this operation in order to guarantee that entity locks are respected by the client


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*
			*  @note
			*    - Only to be used through the CAMP reflection system to generate generic operation instances
			*/
			RemoveEntitiesFromSelectionOperation();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~RemoveEntitiesFromSelectionOperation();


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual void undoImpl() override;
			virtual void redoImpl() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] entityIdSet
			*    The unique entity identifiers to remove from the selection
			*/
			explicit RemoveEntitiesFromSelectionOperation(const EntitySelectionManager::IdSet& entityIdSet);


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::RemoveEntitiesFromSelectionOperation)
