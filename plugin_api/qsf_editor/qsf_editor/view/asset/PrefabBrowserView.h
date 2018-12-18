// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/view/View.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTreeView;
	class QListView;
QT_END_NAMESPACE
namespace Ui
{
	class PrefabBrowserView;
}
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class PrefabBrowserModel;
		class PrefabBrowserFilterModel;
		class FilterConfiguration;
		class SearchWidget;
		class AssetCategoryListModel;
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
		*    Prefab browser view class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*    - Introspection of prefab assets currently mounted inside the asset system
		*    - The name "Prefab Browser" is the decision of the editor vision keeper, that's the reason why this class is named "qsf::editor::PrefabBrowserView" and not "qsf::editor::PrefabsView"
		*/
		class PrefabBrowserView : public View
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::PrefabBrowserView" unique pluginable view ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] viewManager
			*    Optional pointer to the view manager this view should be registered to, can be a null pointer
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this view instance)
			*/
			PrefabBrowserView(ViewManager* viewManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PrefabBrowserView();

			/**
			*  @brief
			*    Select the given prefab asset
			*
			*  @param[in] globalAssetId
			*    Global asset ID of the prefab to select
			*/
			void selectPrefab(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onClicked(const QModelIndex& qModelIndex);
			// Search widget
			void onSearchFieldTextChanged(const QString& searchText);
			void onSearchFilterChanged(const FilterConfiguration& newFilter);
			// ListView
			void onCurrentItemChanged(const QModelIndex& current, const QModelIndex& previous);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Perform a GUI rebuild
			*/
			void rebuildGui();

			// "qsf::AssetSystem"
			void onAssetsMounted(const Assets& assets);
			void onAssetsUnmounted(const Assets& assets);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QTreeView*					mPrefabQTreeView;			///< Qt tree view instance, can be a null pointer, Qt will destroy the instance automatically
			QListView*					mCategoriesQListView;		///< Qt list widget instance, can be a null pointer, Qt will destroy the instance automatically
			PrefabBrowserFilterModel*	mPrefabBrowserFilterModel;	///< Prefab browser filter model instance, can be a null pointer, Qt will destroy the instance automatically ("mPrefabQTreeView" is the parent)
			PrefabBrowserModel*			mPrefabBrowserModel;		///< Prefab browser model instance, can be a null pointer, Qt will destroy the instance automatically ("mPrefabBrowserFilterModel" is the parent)
			Ui::PrefabBrowserView*		mUiPrefabBrowserView;		///< UI prefab browser instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			SearchWidget*				mSearchWidget;				///< The search widget instance, Qt will destroy the instance automatically
			AssetCategoryListModel*		mCategoriesListModel;		///< List model for asset categories, can be a null pointer, Qt will destroy the instance automatically ("mCategoriesQListWidget" is the parent)


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::PrefabBrowserView)
