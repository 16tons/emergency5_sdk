// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		inline int ObjectBrowserModel::columnCount(const QModelIndex& parent) const
		{
			// We only have a single column
			return 1;
		}

		inline bool ObjectBrowserModel::hasChildren(const QModelIndex& parent) const
		{
			return (rowCount(parent) > 0);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
