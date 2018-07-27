// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/layer/LayerOperation.h"


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
			*    Set parent layer operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT SetParentLayerOperation : public LayerOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::SetParentLayerOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				SetParentLayerOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] layerId
				*    The unique layer identifier of the layer to change the parent from
				*  @param[in] newParentLayerId
				*    New parent layer, must be a valid
				*/
				SetParentLayerOperation(uint32 layerId, uint32 newParentLayerId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~SetParentLayerOperation();

				uint32 getNewParentId() const;
				void setNewParentId(uint32 newParentId);

				uint32 getPreviousParentId() const;
				void setPreviousParentId(uint32 previousParentId);


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
				uint32 mPreviousParentId;	///< The previous parent layer ID of the layer
				uint32 mNewParentId;		///< The new parent layer ID of the layer


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::SetParentLayerOperation)
