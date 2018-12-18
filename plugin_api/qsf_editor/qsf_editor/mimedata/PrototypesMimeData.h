// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <QtCore/qmimedata.h>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
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
		*    Prototypes Qt mime data
		*
		*  @note
		*    - Stores information about 1..n prototypes
		*
		*  @todo
		*    - TODO(co): Add more useful methods
		*/
		class QSF_EDITOR_API_EXPORT PrototypesMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/prototypes"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating prototypes Qt mime data by using a given UTF-8 prefab asset name
			*
			*  @param[in] prototype
			*    UTF-8 prefab asset name (e.g. "sample/prefab/vehicle/ambulance")
			*/
			explicit PrototypesMimeData(const std::string& prototype);

			/**
			*  @brief
			*    Constructor for creating prototypes Qt mime data by using a given list of UTF-8 prefab asset names
			*
			*  @param[in] qList
			*    List of UTF-8 prefab asset names (e.g. "sample/prefab/vehicle/ambulance", "sample/prefab/animal/squirrel" and so on)
			*/
			explicit PrototypesMimeData(const QList<std::string>& qList);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~PrototypesMimeData();

			/**
			*  @brief
			*    Return a pointer to the first prototype instance inside this Qt mime data instance
			*
			*  @return
			*    Pointer to the first prototype instance inside this Qt mime data instance, null pointer on error
			*/
			Prototype* getFirstPrototypePointer() const;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/PrototypesMimeData-inl.h"
