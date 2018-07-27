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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class BackupPrototypeOperationData;
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
			*    Destroy entity operation class
			*
			*  @note
			*    - It's highly recommended to use the compound operation "qsf::editor::buildDestroyEntityOperation()" instead of this atomic entity operation
			*/
			class QSF_EDITOR_BASE_API_EXPORT DestroyEntityOperation : public EntityOperation
			{


			//[-------------------------------------------------------]
			//[ Friend classes                                        ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to member variables


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::DestroyEntityOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				DestroyEntityOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier of the entity to destroy
				*  @param[in] backupPrototypeOperationData
				*    Operation data holding the prototype backup needed for undo, can be a null pointer, do not destroy the instance
				*/
				DestroyEntityOperation(uint64 entityId, BackupPrototypeOperationData* backupPrototypeOperationData = nullptr);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~DestroyEntityOperation();

				BackupPrototypeOperationData* getBackupPrototypeOperationData() const;
				void setBackupPrototypeOperationData(BackupPrototypeOperationData* backupPrototypeOperationData);

				uint32 getBackupPrototypeOperationDataId() const;
				void setBackupPrototypeOperationDataId(uint32 backupPrototypeOperationDataId);

				uint32 getLayerId() const;


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
				BackupPrototypeOperationData* mBackupPrototypeOperationData;		///< Operation data holding the prototype backup needed for undo, can be a null pointer, do not destroy the instance
				uint32						  mBackupPrototypeOperationDataId;		///< Operation data ID used for recovering after deserialization, "qsf::getUninitialized<uint32>()" if invalid
				bool						  mBackupPrototypeOperationDataOwned;	///< Operation data is owned by this Operation and needs to be destructed in its destructor


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::DestroyEntityOperation)
