// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/component/ComponentOperation.h"


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
			*    Create entity component operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT CreateComponentOperation : public ComponentOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::CreateComponentOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				CreateComponentOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier
				*  @param[in] componentId
				*    The unique (inside the entity) component identifier
				*/
				CreateComponentOperation(uint64 entityId, uint32 componentId);

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier
				*  @param[in] componentId
				*    The unique (inside the entity) component identifier
				*  @param[in] entityName
				*    The UTF-8 name of the entity, used only for the operation text
				*  @param[in] componentName
				*    The unhashed ASCII string of the unique (inside the entity) component identifier, used only for the operation text
				*/
				CreateComponentOperation(uint64 entityId, uint32 componentId, const std::string& entityName, const std::string& componentName);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~CreateComponentOperation();


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual size_t getMemoryConsumption() const override;
				virtual void undoImpl() override;
				virtual void redoImpl() override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CreateComponentOperation)
