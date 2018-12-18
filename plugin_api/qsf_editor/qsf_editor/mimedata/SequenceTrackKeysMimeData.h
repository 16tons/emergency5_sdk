// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"
#include "qsf_editor/dialog/sequence/SequenceEditorSelectionManager.h" //< pulls in the key identity and KeySelectionSet

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
		*    Sequence track keys Qt mime data
		*
		*  @note
		*    - Stores information about 1..n track keys
		*    - does only store REFERENCES to track keys, meaning clipboard data can get out of date if source keys/tracks are deleted before data is pasted/dropped somewhere
		*   TODO(oi) Look into the possibilities of doing a full data backup here (i.e. JSON serialization or binary serialization) instead of thinking in references
		*/
		class QSF_EDITOR_API_EXPORT SequenceTrackKeysMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/sequenceTrackKeys"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating track keys Qt mime data by using given key selection
			*
			*  @param[in] keySet
			*    Set of selected keys to put into this data
			*/
			SequenceTrackKeysMimeData(const SequenceEditorSelectionManager::KeySelectionSet& keySet);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~SequenceTrackKeysMimeData();

			/**
			*  @brief
			*    Fill a given KeySelection set using the Qt mime data
			*
			*  @param[out] keySet
			*    KeySelection set to fill, list is not cleared before adding new key selections
			*/
			void getKeySelectionSet(SequenceEditorSelectionManager::KeySelectionSet& keySet) const;


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
#include "qsf_editor/mimedata/SequenceTrackKeysMimeData-inl.h"
