// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/entity/EntityOperation.h"

#include <qsf/selection/EntitySelectionManager.h>
#include <qsf/reflection/type/CampUint64.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Report an entity as selected to the server
			*
			*  @note
			*    - This operation is intended to be used by dialogs that not really want to change the editor selection but need the locking logic of the server to believe that an entity is about to be edited
			*    - Hidden selections are managed on a separate selection stack then regular selections on the server side, so it is safe to hidden de-/select an entity that is also selected using the EntitySelectionManager
			*/
			class QSF_EDITOR_BASE_API_EXPORT SelectEntityHiddenOperation : public EntityOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::SelectEntityHiddenOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				SelectEntityHiddenOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier to be reported as selected to the server
				*/
				explicit SelectEntityHiddenOperation(uint64 entityId);

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityIdSet
				*    The unique entity identifiers to be reported as selected to the server
				*/
				explicit SelectEntityHiddenOperation(const EntitySelectionManager::IdSet& entityIdSet);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~SelectEntityHiddenOperation();

				const EntitySelectionManager::IdSet& getEntityIdSet() const;
				void setEntityIdSet(const EntitySelectionManager::IdSet&);

				// Special treatment for CAMP
				std::string getEntityIdList() const;
				void setEntityIdList(const std::string& entityIdSet);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual bool isReversible() const override;
				virtual uint32 getId() const override;
				virtual void sealImpl() override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				EntitySelectionManager::IdSet mEntityIdSet;	///< The unique entity identifiers to add to the selection


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::SelectEntityHiddenOperation)
