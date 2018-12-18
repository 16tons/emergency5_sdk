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
		*    Sequence tracks Qt mime data
		*
		*  @note
		*    - Stores information about 1..n tracks
		*    - does only store REFERENCES to tracks, meaning clipboard data can get out of date if source keys/tracks are deleted before data is pasted/dropped somewhere
		*   TODO(oi) Look into the possibilities of doing a full data backup here (i.e. JSON serialization or binary serialization) instead of thinking in references
		*/
		class QSF_EDITOR_API_EXPORT SequenceTracksMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/sequenceTracks"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating tracks Qt mime data by using given track IDs
			*
			*  @param[in] trackSet
			*    Set of tracks to put into this data
			*/
			SequenceTracksMimeData(const SequenceEditorSelectionManager::TrackSelectionSet& trackSet);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~SequenceTracksMimeData();

			/**
			*  @brief
			*    Fill a given uint32 (track id) set using the Qt mime data
			*
			*  @param[out] trackSet
			*    Track set to fill, list is not cleared before adding new tracks
			*/
			void getTrackSelectionSet(SequenceEditorSelectionManager::TrackSelectionSet& trackSet) const;


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
#include "qsf_editor/mimedata/SequenceTracksMimeData-inl.h"
