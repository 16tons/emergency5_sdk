// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/entity/EntitySelectionOperation.h"

#include <qsf/selection/EntitySelectionManager.h>


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
			*    Clear selection and select entity operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT SelectEntityOperation : public EntitySelectionOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::SelectEntityOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~SelectEntityOperation();

				/**
				*  @brief
				*    Return the IDs of the previously selected entities
				*
				*  @return
				*    The IDs of the previously selected entities, do not destroy the instance
				*/
				const EntitySelectionManager::IdSet& getPreviousEntityIdSet() const;
				void setPreviousEntityIdSet(const EntitySelectionManager::IdSet& previousEntityIdSet);

				// Special treatment for CAMP (yes we have to return a full vector copy since a set has
				// no indices and thus cannot be safely accessed through the normal array interface)
				std::string getPreviousEntityIdList() const;
				void setPreviousEntityIdList(const std::string& entityIdSet);


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
				SelectEntityOperation();

				/**
				*  @brief
				*    Default constructor which clears the selection
				*
				*  @param[in] previousEntityIdSet
				*    The IDs of the previously selected entities
				*/
				explicit SelectEntityOperation(const EntitySelectionManager::IdSet& previousEntityIdSet);

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] previousEntityIdSet
				*    The IDs of the previously selected entities
				*  @param[in] entityId
				*    The unique entity identifier of the new selected entity
				*/
				SelectEntityOperation(const EntitySelectionManager::IdSet& previousEntityIdSet, uint64 entityId);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				EntitySelectionManager::IdSet mPreviousEntityIdSet;	///< The IDs of the previously selected entities


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::SelectEntityOperation)
