// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtCore/qmimedata.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
}


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
		*    Layer Qt mime data
		*
		*  @note
		*    - Stores information about a single layer
		*/
		class QSF_EDITOR_API_EXPORT LayerMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/layer"


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
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			LayerMimeData(unsigned int layerId, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~LayerMimeData();

			/**
			*  @brief
			*    Return the ID of the layer instance inside this Qt mime data instance
			*
			*  @return
			*    ID of the layer instance inside this Qt mime data instance, "qsf::getUninitialized<uint32>()" if invalid
			*/
			uint32 getLayerId() const;

			/**
			*  @brief
			*    Return a pointer to the layer instance inside this Qt mime data instance
			*
			*  @return
			*    Pointer to the layer instance inside this Qt mime data instance, null pointer on error, do not destroy the instance
			*/
			qsf::Layer* getLayer() const;


		//[-------------------------------------------------------]
		//[ Public virtual QMimeData methods                      ]
		//[-------------------------------------------------------]
		public:
			inline virtual bool hasFormat(const QString& mimeType) const override;
			virtual QStringList formats() const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const QString& mMimeType;	///< Direct reference to the concrete mime type


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/LayerMimeData-inl.h"
