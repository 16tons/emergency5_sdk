// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/entity/EntityOperation.h"


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
			*    Entity selection operation base class
			*/
			class QSF_EDITOR_BASE_API_EXPORT EntitySelectionOperation : public EntityOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::EntitySelectionOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public static methods                                 ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Return whether or not the given operation ID is an entity select operation
				*
				*  @return
				*    "true" if the given operation ID is an entity select operation, else "false"
				*/
				static bool isEntitySelectionOperation(uint32 operationId);


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~EntitySelectionOperation();


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual bool isReplicated() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				EntitySelectionOperation();


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::EntitySelectionOperation)
