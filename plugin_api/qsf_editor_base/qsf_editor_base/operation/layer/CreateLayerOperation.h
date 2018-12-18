// Copyright (C) 2012-2018 Promotion Software GmbH


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
			*    Create layer operation class
			*/
			class QSF_EDITOR_BASE_API_EXPORT CreateLayerOperation : public LayerOperation
			{


			//[-------------------------------------------------------]
			//[ Public definitions                                    ]
			//[-------------------------------------------------------]
			public:
				static const uint32 OPERATION_ID;	///< "qsf::editor::base::CreateLayerOperation" unique operation ID


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				CreateLayerOperation();

				/**
				*  @brief
				*    Constructor
				*
				*  @param[in] parentLayerId
				*    The unique layer identifier of the parent layer the new layer is in
				*  @param[in] layerId
				*    The unique layer identifier of the created layer
				*/
				CreateLayerOperation(uint32 parentLayerId, uint32 layerId);

				/**
				*  @brief
				*    Destructor
				*/
				virtual ~CreateLayerOperation();

				/**
				*  @brief
				*    Return the ID of the parent layer the created layer is in
				*
				*  @return
				*    The ID of the parent layer the created layer is in
				*/
				uint32 getParentLayerId() const;
				void setParentLayerId(uint32 parentLayerId);


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
				uint32 mParentLayerId;	///< The unique layer identifier of the parent layer the created layer is in, "qsf::getUninitialized<uint32>()" if invalid


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::CreateLayerOperation)
