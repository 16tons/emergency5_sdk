// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qsortfilterproxymodel.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		class MaterialBrowserPropertyFilterModel : public QSortFilterProxyModel
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MaterialBrowserPropertyFilterModel(QObject* parent);


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
			virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
