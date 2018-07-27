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
			*    Create entity operation class
			*
			*  @note
			*    - New entities are automatically inside the currently selected layer, so, after this operation
			*      it's guaranteed that there's a metadata component
			*/
			class QSF_EDITOR_BASE_API_EXPORT CreateEntityOperation : public EntityOperation
			{


			//[-------------------------------------------------------]
			//[ Friend classes                                        ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to member variables


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::CreateEntityOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				CreateEntityOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] entityId
				*    The unique entity identifier of the entity to create
				*  @param[in] layerId
				*    Layer to put the new entity in
				*  @param[in] backupPrototypeOperationData
				*    Operation data holding the prototype to clone, must stay valid as long as this create entity operation exists, can be a null pointer
				*
				*  @remarks
				*    New entities are automatically inside the layer which was the currently the selected layer when committing the operation.
				*    By handling this requirement only in here we can realize it in a simple and consistent way.
				*/
				CreateEntityOperation(uint64 entityId, uint32 layerId, BackupPrototypeOperationData* backupPrototypeOperationData = nullptr);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~CreateEntityOperation();

				uint32 getLayerId() const;
				void setLayerId(uint32 layerId);

				BackupPrototypeOperationData* getBackupPrototypeOperationData() const;
				void setBackupPrototypeOperationData(BackupPrototypeOperationData* backupPrototypeOperationData);

				uint32 getBackupPrototypeOperationDataId() const;
				void setBackupPrototypeOperationDataId(uint32 backupPrototypeOperationDataId);


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
				uint32						  mLayerId;								///< The ID of the layer which was the currently the selected layer when committing the operation, 0 is the base layer, "qsf::getUninitialized<uint32>()" if invalid (should not happen)
				BackupPrototypeOperationData* mBackupPrototypeOperationData;		///< Backup prototype operation data, can be a null pointer, do not destroy the instance
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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CreateEntityOperation)
