// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/operation/MapOperation.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
	class LayerManager;
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
			*    Abstract layer operation base class
			*
			*  @note
			*    - Works on the primary QSF map ("qsf::Qsf::getMap()")
			*    - 32 bit are required to unique identify the layer
			*/
			class QSF_EDITOR_BASE_API_EXPORT LayerOperation : public MapOperation
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Destructor
				*/
				virtual ~LayerOperation();

				/**
				*  @brief
				*    Return the used layer manager
				*
				*  @return
				*    The used layer manager, do not destroy the instance
				*/
				LayerManager& getLayerManager() const;

				/**
				*  @brief
				*    Return the ID of the layer this operation is performed on
				*
				*  @return
				*    The ID of the layer this operation is performed on
				*/
				uint32 getLayerId() const;
				void setLayerId(uint32 layerId);

				/**
				*  @brief
				*    Return the layer this operation is performed on
				*
				*  @return
				*    The layer this operation is performed on, can be a null pointer, do not destroy the instance
				*
				*  @note
				*    - Ease-of-use method
				*/
				Layer* getLayer() const;


			//[-------------------------------------------------------]
			//[ Public qsf::editor::base::Operation methods           ]
			//[-------------------------------------------------------]
			public:
				virtual size_t getMemoryConsumption() const override;


			//[-------------------------------------------------------]
			//[ Protected methods                                     ]
			//[-------------------------------------------------------]
			protected:
				LayerOperation();


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				uint32 mLayerId;	///< The unique layer identifier, "qsf::getUninitialized<uint32>()" if invalid


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::LayerOperation)
