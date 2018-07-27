// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/layer/LayerOperation.h"

#include <qsf/reflection/type/CampBoostPTree.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Plugin;
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
			*    Destroy layer operation class
			*
			*  @note
			*    - It's highly recommended to use the compound operation "qsf::editor::LayerOperationHelper::buildDestroyLayerOperation()" instead of this atomic layer operation
			*    - This operation does not deal with layers nor does it deal with entities
			*/
			class QSF_EDITOR_BASE_API_EXPORT DestroyLayerOperation : public LayerOperation
			{


			//[-------------------------------------------------------]
			//[ Friends                                               ]
			//[-------------------------------------------------------]
				friend class OperationMetaclasses;	// For the declaration of the CAMP metaclass, the metaclasses instance needs direct access to "qsf::editor::base::DestroyLayerOperation::mBackupProperties" in order to connect it as CAMP array


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::DestroyLayerOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				DestroyLayerOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] layerId
				*    The unique layer identifier of the layer to destroy, child layers and registered entities are ignored by this operation
				*/
				explicit DestroyLayerOperation(uint32 layerId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~DestroyLayerOperation();

				/**
				*  @brief
				*    Return the ID of the parent layer the destoryed layer is in (backup property for undo)
				*
				*  @return
				*    The ID of the parent layer the destroyed layer is in
				*/
				uint32 getParentLayerId() const;
				void setParentLayerId(uint32 parentLayerId);

				const boost::property_tree::ptree& getBackupProperties() const;
				void setBackupProperties(const boost::property_tree::ptree& backupProperties);


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
				uint32						mParentLayerId;		///< The unique layer identifier of the parent layer the destroyed layer is in, "qsf::getUninitialized<uint32>()" if invalid
				boost::property_tree::ptree mBackupProperties;	///< Backuped layer properties


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::DestroyLayerOperation)
