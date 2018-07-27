// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>	//< For "GlobalAssetId"

#include <QtCore/qabstractitemmodel.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetSystem;
	class ParameterGroup;
	namespace editor
	{
		class AssetStorageManager;
		class AssetPreviewCache;
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
		*    Prefab browser model system Qt model
		*/
		class PrefabBrowserModel : public QAbstractItemModel
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
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*  @param[in] assetSystem
			*    Asset system instance this Qt model wraps up
			*/
			PrefabBrowserModel(QObject* parent, const AssetSystem& assetSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PrefabBrowserModel();

			/**
			*  @brief
			*    Gets called when an asset gets mounted
			*/
			void onAssetMounted(const Asset& addedAsset);

			/**
			*  @brief
			*    Gets called when an asset gets unmounted
			*/
			void onAssetUnmounted(const Asset& removedAsset);

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

			void setupData(const AssetSystem& assetSystem);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					   mAssetPreview;
			const AssetStorageManager& mAssetStorageManager;
				  AssetPreviewCache&   mAssetPreviewCache;
			std::vector<Asset*>		   mAssets;
			QIcon					   mDefaultPreviewIcon;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
