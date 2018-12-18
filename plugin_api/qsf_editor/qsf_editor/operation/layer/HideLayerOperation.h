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
		*    Hide layer operation class
		*
		*  @note
		*    - Local atomic operation: For cooperative work, it would be fatal if one user could hide the layer of another user
		*/
		class HideLayerOperation : public base::LayerOperation
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 OPERATION_ID;	///< "qsf::editor::HideLayerOperation" unique operation ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			HideLayerOperation();

			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] layerId
			*    The unique layer identifier of the layer to hide
			*  @param[in] hidden
			*    "true" if the layer is hidden, else "false"
			*/
			HideLayerOperation(unsigned int layerId, bool hidden);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~HideLayerOperation();

			bool getHidden() const;
			void setHidden(bool hidden);

			bool getPreviousHidden() const;
			void setPreviousHidden(bool previousHidden);


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
			bool mPreviousHidden;	///< The previous hidden state of the layer
			bool mHidden;			///< The new hidden state of the layer


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
