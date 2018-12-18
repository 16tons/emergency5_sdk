// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qabstractitemmodel.h>

#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class AssetIssue;
		}
	}
}


//[-------------------------------------------------------]
//[ Qt Metatype                                           ]
//[-------------------------------------------------------]
Q_DECLARE_METATYPE(std::shared_ptr<qsf::editor::base::AssetIssue>)


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
		*    Asset issue Qt filter model
		*/
		class AssetMaintenanceModel : public QAbstractItemModel
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			AssetMaintenanceModel(const std::vector<std::shared_ptr<base::AssetIssue>>& issueList, QObject* parent = nullptr);
			virtual ~AssetMaintenanceModel();

			void useIssueList(const std::vector<std::shared_ptr<base::AssetIssue>>& issueList);

			void removeIssue(std::shared_ptr<base::AssetIssue> assetIssue);


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
			virtual QModelIndex parent(const QModelIndex& index) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role) const override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
			virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
			virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<std::shared_ptr<base::AssetIssue>> mIssueList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
