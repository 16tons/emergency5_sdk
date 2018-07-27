// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

#include <QtCore/qabstractitemmodel.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{

	}
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
		*    Prefab browser asset category model system Qt model
		*/
		class AssetCategoryListModel : public QAbstractListModel
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
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parent
			*    Pointer to parent Qt object, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			explicit AssetCategoryListModel(QObject *parent = 0);

			/**
			*  @brief
			*    Gets called, when a new category should be added
			*/
			void addCategory(const std::string& newCategory);

			/**
			*  @brief
			*    Gets called, when a category should be removed
			*/
			void removeCategory(const std::string& category);

			/**
			*  @brief
			*    Initiates an refresh of the model data
			*/
			void refresh();


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent) const override;
			virtual bool hasChildren(const QModelIndex& parent) const override;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		 private:
			 void setupData();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		 private:
			 QMap<QString, uint32>	mAssetCategories;	///< A map of asset categories and count how often the category is used by an asset (when count reaches zero the category is removed)
		 };


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
