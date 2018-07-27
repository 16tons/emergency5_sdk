// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/EntitiesMimeData.h"


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
		*    Cut entities Qt mime data
		*/
		class QSF_EDITOR_API_EXPORT CutEntitiesMimeData : public EntitiesMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/entities/cut"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating entities Qt mime data by using a given entity ID
			*
			*  @param[in] entityId
			*    Entity ID
			*/
			inline explicit CutEntitiesMimeData(uint64 entityId);

			/**
			*  @brief
			*    Constructor for creating entities Qt mime data by using given entity IDs
			*
			*  @param[in] idSet
			*    Entity IDs
			*/
			inline explicit CutEntitiesMimeData(const EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Constructor for creating entities Qt mime data by using a given list of entity IDs
			*
			*  @param[in] qList
			*    List of entity IDs
			*/
			inline explicit CutEntitiesMimeData(const QList<uint64>& qList);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CutEntitiesMimeData();


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
#include "qsf_editor/mimedata/CutEntitiesMimeData-inl.h"
