// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qmimedata.h>


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
		*    Material Qt mime data
		*
		*  @note
		*    - Stores information about a single material
		*/
		class QSF_EDITOR_API_EXPORT MaterialMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/material"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating material Qt mime data by using a given global material asset ID
			*
			*  @param[in] globalAssetId
			*    Global asset ID
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			MaterialMimeData(GlobalAssetId globalAssetId, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~MaterialMimeData();

			/**
			*  @brief
			*    Return the global material asset ID of the material inside this Qt mime data instance
			*
			*  @return
			*    Global material asset ID of the material inside this Qt mime data instance, "qsf::getUninitialized<GlobalAssetId>()" if invalid
			*/
			GlobalAssetId getGlobalAssetId() const;


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
#include "qsf_editor/mimedata/MaterialMimeData-inl.h"
