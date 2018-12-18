// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/component/ComponentOperation.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class BackupComponentOperationData;
		}
	}
}


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
			*    Destroy entity component operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT DestroyComponentOperation : public ComponentOperation
			{


			//[-------------------------------------------------------]
			//[ Friend classes                                        ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to member variables


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::DestroyComponentOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				DestroyComponentOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier
				*  @param[in] componentId
				*    The unique (inside the entity) component identifier
				*  @param[in] backupComponentOperationData
				*    Operation data holding the component backup needed for undo, can be a null pointer, do not destroy the instance
				*/
				DestroyComponentOperation(uint64 entityId, uint32 componentId, BackupComponentOperationData* backupComponentOperationData = nullptr);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~DestroyComponentOperation();

				BackupComponentOperationData* getBackupComponentOperationData() const;
				void setBackupComponentOperationData(BackupComponentOperationData* backupComponentOperationData);

				uint32 getBackupComponentOperationDataId() const;
				void setBackupComponentOperationDataId(uint32 backupComponentOperationDataId);


			//[-------------------------------------------------------]
			//[ Public virtual qsf::Object methods                    ]
			//[-------------------------------------------------------]
			public:
				virtual void onPreDeserialize() override;
				virtual void onPostDeserialize() override;


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual uint32 getId() const override;
				virtual size_t getMemoryConsumption() const override;
				virtual void sealImpl() override;
				virtual void undoImpl() override;
				virtual void redoImpl() override;


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				BackupComponentOperationData* mBackupComponentOperationData;		///< Operation data holding the component backup needed for undo, can be a null pointer, do not destroy the instance
				uint32				  mBackupComponentOperationDataId;		///< Operation data ID used for recovering after deserialization, "qsf::getUninitialized<uint32>()" if invalid
				bool						  mBackupComponentOperationDataOwned;	///< Operation data is owned by this Operation and needs to be destructed in its destructor


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::DestroyComponentOperation)
