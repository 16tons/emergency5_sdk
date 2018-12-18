// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CutLayerMimeData::CutLayerMimeData(unsigned int layerId) :
			LayerMimeData(layerId, MIME_TYPE)
		{
			// Nothing to do in here
		}

		inline CutLayerMimeData::~CutLayerMimeData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual QMimeData methods                      ]
		//[-------------------------------------------------------]
		inline bool CutLayerMimeData::hasFormat(const QString& mimeType) const
		{
			return (MIME_TYPE == mimeType) || LayerMimeData::hasFormat(mimeType);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
