// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/LayerMimeData.h"


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
		*    Cut layer Qt mime data
		*/
		class QSF_EDITOR_API_EXPORT CutLayerMimeData : public LayerMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/layer/cut"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating layer Qt mime data by using a given layer ID
			*
			*  @param[in] layerId
			*    Layer ID
			*/
			inline explicit CutLayerMimeData(unsigned int layerId);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CutLayerMimeData();


		//[-------------------------------------------------------]
		//[ Public virtual QMimeData methods                      ]
		//[-------------------------------------------------------]
		public:
			inline virtual bool hasFormat(const QString& mimeType) const override;
			virtual QStringList formats() const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/CutLayerMimeData-inl.h"
