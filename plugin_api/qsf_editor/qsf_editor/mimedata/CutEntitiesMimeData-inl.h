// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline CutEntitiesMimeData::CutEntitiesMimeData(uint64 entityId) :
			EntitiesMimeData(entityId, MIME_TYPE)
		{
			// Nothing to do in here
		}

		inline CutEntitiesMimeData::CutEntitiesMimeData(const EntitySelectionManager::IdSet& idSet) :
			EntitiesMimeData(idSet, MIME_TYPE)
		{
			// Nothing to do in here
		}

		inline CutEntitiesMimeData::CutEntitiesMimeData(const QList<uint64>& qList) :
			EntitiesMimeData(qList, MIME_TYPE)
		{
			// Nothing to do in here
		}

		inline CutEntitiesMimeData::~CutEntitiesMimeData()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual QMimeData methods                      ]
		//[-------------------------------------------------------]
		inline bool CutEntitiesMimeData::hasFormat(const QString& mimeType) const
		{
			return (MIME_TYPE == mimeType) || EntitiesMimeData::hasFormat(mimeType);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
