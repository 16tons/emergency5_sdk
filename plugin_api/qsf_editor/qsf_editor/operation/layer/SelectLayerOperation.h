// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/operation/layer/LayerOperation.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Select layer operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, it would be fatal if one user could change the currently selected layer of another user
		*/
		class SelectLayerOperation : public base::LayerOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::SelectLayerOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SelectLayerOperation();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] layerId
			*    The unique layer identifier of the new selected layer
			*/
			explicit SelectLayerOperation(uint32 layerId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SelectLayerOperation();

			/**
			*  @brief
			*    Return the ID of the previous layer this operation is performed on
			*
			*  @return
			*    The ID of the previous layer this operation is performed on
			*/
			uint32 getPreviousLayerId() const;
			void setPreviousLayerId(uint32 previousLayerId);

			/**
			*  @brief
			*    Return the previous layer this operation is performed on
			*
			*  @return
			*    The previous layer this operation is performed on, can be a null pointer, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			Layer* getPreviousLayer() const;


		//[-------------------------------------------------------]
		//[ Public qsf::editor::base::Operation methods           ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getId() const override;
			virtual bool isLocal() const override;
			virtual size_t getMemoryConsumption() const override;
			virtual void sealImpl() override;
			virtual void undoImpl() override;
			virtual void redoImpl() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mPreviousLayerId;	///< The unique layer identifier of the previous selected layer, "qsf::getUninitialized<uint32>()" if invalid


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
