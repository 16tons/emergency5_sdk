// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/entity/EntitySelectionOperation.h"

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
			*    Remove entities to selection operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT RemoveEntitiesFromSelectionOperation : public EntitySelectionOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::RemoveEntitiesFromSelectionOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~RemoveEntitiesFromSelectionOperation();

				/**
				*  @brief
				*    Return the unique entity identifiers to remove from the selection
				*
				*  @return
				*    The unique entity identifiers to remove from the selection, do not destroy the instance
				*/
				const EntitySelectionManager::IdSet& getEntityIdSet() const;
				void setEntityIdSet(const EntitySelectionManager::IdSet& entityIdSet);

				// Special treatment for CAMP (yes we have to return a full vector copy since a set has
				// no indices and thus cannot be safely accessed through the normal array interface)
				std::string getEntityIdList() const;
				void setEntityIdList(const std::string& entityIdSet);


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual size_t getMemoryConsumption() const override;
				virtual void sealImpl() override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				RemoveEntitiesFromSelectionOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityIdSet
				*    The unique entity identifiers to remove from the selection
				*/
				explicit RemoveEntitiesFromSelectionOperation(const EntitySelectionManager::IdSet& entityIdSet);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				EntitySelectionManager::IdSet mEntityIdSet;	///< The unique entity identifiers to remove from the selection


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::RemoveEntitiesFromSelectionOperation)
