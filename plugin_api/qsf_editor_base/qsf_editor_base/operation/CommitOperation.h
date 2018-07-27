// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/Operation.h"


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
			*    Commit operation class
			*
			*  @remarks
			*    Whenever this operation is "redone" all entities, except those currently selected, are unlocked.
			*    The local operation history must be completely cleared of all non-local operations in that moment.
			*/
			class QSF_EDITOR_BASE_API_EXPORT CommitOperation : public Operation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::CommitOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Constructor
				*/
				CommitOperation();

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~CommitOperation();


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual bool isReplicated() const override;
				virtual bool isReversible() const override;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CommitOperation)
