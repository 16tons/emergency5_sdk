// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qabstractitemmodel.h>

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class FileSystem;
	class AssetPackage;
	class ParameterGroup;
	namespace editor
	{
		class AssetPreviewCache;
		class AssetStorageManager;
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
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		typedef std::vector<AssetPackage*> AssetPackages;


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Asset browser model system Qt model
		*/
		class AssetBrowserModel : public QAbstractItemModel
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const int PREVIEW_COLUMN		  = 0;	///< Column of the preview
			static const int ID_COLUMN			  = 1;	///< Column of the global asset ID
			static const int PACKAGE_COLUMN		  = 2;	///< Column of the project and asset package name
			static const int NAME_COLUMN		  = 3;	///< Column of the asset name
			static const int TYPE_COLUMN		  = 4;	///< Column of the asset type
			static const int CATEGORY_COLUMN	  = 5;	///< Column of the asset category
			static const int MOUNTPOINT_COLUMN	  = 6;	///< Column of the asset package mount point
			static const int TAGS_COLUMN		  = 7;	///< Column of the asset tags
			static const int AUTHOR_COLUMN		  = 8;	///< Column of the asset author
			static const int DATE_CREATED_COLUMN  = 9;	///< Column of the date the asset was created
			static const int DATE_MODIFIED_COLUMN = 10;	///< Column of the date the asset was modified the last time
			static const int DESCRIPTION_COLUMN	  = 11;	///< Column of the asset description
			static const int STATE_COLUMN		  = 12;	///< Column of the asset state


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*  @param[in] selectedAssetPackages
			*    Lists used for filtering by project / asset package
			*/
			AssetBrowserModel(QObject* parent, const AssetPackages& selectedAssetPackages);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetBrowserModel();

			/**
			*  @brief
			*    Performs an reset on the model
			*
			*  @param[in] selectedAssetPackages
			*    Lists used for filtering by project / asset package
			*/
			void reset(const AssetPackages& selectedAssetPackages);

			/**
			*  @brief
			*    Update the internal model state when an asset gets updated from outside
			*
			*  @note
			*    - In this case the model simply emits a data change signal for the associated row if the asset is part of the model
			*/
			void onAssetUpdated(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Return an asset package by using a given asset
			*
			*  @param[in] asset
			*    Asset to return the asset package from
			*
			*  @return
			*    The asset package the given asset resides in
			*/
			const AssetPackage* getAssetPackageByAsset(const Asset& asset) const;


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
			virtual QStringList mimeTypes() const override;
			virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onAssetPreviewChanged(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_map<const Asset*, const AssetPackage*> AssetToAssetPackageMap;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					   mAssetPreview;
			const AssetStorageManager& mAssetStorageManager;
				  AssetPreviewCache&   mAssetPreviewCache;
			const FileSystem&		   mFileSystem;
			std::vector<Asset*>		   mAssets;
			AssetToAssetPackageMap	   mAssetToAssetPackageMap;	///< Each entry must be valid, so no additional checks when using are required


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
