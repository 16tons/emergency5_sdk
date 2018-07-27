// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/asset/AssetFilterModel.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class AssetBrowserModel;
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
		*    Asset browser Qt filter model
		*/
		class AssetBrowserFilterModel : public AssetFilterModel
		{


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
			*/
			explicit AssetBrowserFilterModel(QObject* parent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetBrowserFilterModel();

			/**
			*  @brief
			*    Set asset type filter
			*
			*  @param[in] assetTypeId
			*    ID of the asset type to show, set to "qsf::getUninitialized<qsf::AssetTypeId>()" in order to show assets of all types
			*  @param[in] assetSelectionMode
			*    "true" if currently asset selection mode is enabled, else "false"
			*  @param[in] assetBrowserModel
			*    Asset browser model instance, can be a null pointer
			*/
			void setAssetTypeFilter(AssetTypeId assetTypeId, bool assetSelectionMode, const AssetBrowserModel* assetBrowserModel);


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
			virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			AssetTypeId				 mAssetTypeId;			///< ID of the asset type to show, set to "qsf::getUninitialized<qsf::AssetTypeId>()" in order to show assets of all types
			bool					 mAssetSelectionMode;	///< "true" if currently asset selection mode is enabled, else "false"
			const AssetBrowserModel* mAssetBrowserModel;	///< The asset browser model to filter, can be a null pointer, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
