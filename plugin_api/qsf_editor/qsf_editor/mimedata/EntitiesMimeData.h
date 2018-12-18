// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <QtCore/qmimedata.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
		*    Entities Qt mime data
		*
		*  @note
		*    - Stores information about 1..n entities
		*/
		class QSF_EDITOR_API_EXPORT EntitiesMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/entities"


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
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			EntitiesMimeData(uint64 entityId, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Constructor for creating entities Qt mime data by using given entity IDs
			*
			*  @param[in] idSet
			*    Entity IDs
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			EntitiesMimeData(const EntitySelectionManager::IdSet& idSet, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Constructor for creating entities Qt mime data by using a given list of entity IDs
			*
			*  @param[in] qList
			*    List of entity IDs
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			EntitiesMimeData(const QList<uint64>& qList, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~EntitiesMimeData();

			/**
			*  @brief
			*    Return a pointer to the first entity instance inside this Qt mime data instance
			*
			*  @return
			*    Pointer to the first entity instance inside this Qt mime data instance, null pointer on error, do not destroy the instance
			*/
			qsf::Entity* getFirstEntity() const;

			/**
			*  @brief
			*    Fill a given entity ID set by using the Qt mime data
			*
			*  @param[out] idSet
			*    Entity ID set to fill, list is not cleared before adding new entities
			*/
			void getEntityIdSet(EntitySelectionManager::IdSet& idSet) const;


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
#include "qsf_editor/mimedata/EntitiesMimeData-inl.h"
