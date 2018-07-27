// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/Dialog.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QTreeWidgetItem;
QT_END_NAMESPACE
namespace Ui
{
	class AssetBrowserDialog;
}
namespace qsf
{
	class Project;
	class AssetPackage;
	class ParameterGroup;
	namespace editor
	{
		class SearchWidget;
		class ProjectImport;
		class AssetBrowserModel;
		class AssetBrowserFilterModel;
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
		*    QSF editor asset browser dialog
		*
		*  @remarks
		*    The asset browser has a special asset selection mode. Wherever appropriated, instead of a file dialog the asset browser will be shown
		*    in selection mode allowing the user to choose an asset. We are aware that modes should best be avoided if possible, but in this case
		*    this solution results in that we can reuse the asset browser for multiple tasks.
		*/
		class AssetBrowserDialog : public Dialog
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class AssetBrowserAssetsMenu;	// TODO(co) For now "qsf::editor::AssetBrowserAssetsMenu" needs access to "fillAssetsTableWidget()"


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::AssetBrowserDialog" unique pluginable dialog ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			AssetBrowserDialog(DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetBrowserDialog();

			//[-------------------------------------------------------]
			//[ Asset selection mode relevant                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return whether or not the asset browser is in asset selection mode
			*
			*  @return
			*    "true" if the asset browser is in asset selection mode, else "false"
			*/
			bool isAssetSelectionMode() const;

			/**
			*  @brief
			*    Startup the asset selection mode of the asset browser
			*
			*  @param[in] globalAssetId
			*    ID to be used as current global asset ID, "qsf::getUninitialized<GlobalAssetId>()" is there's no current global asset ID
			*  @param[in] assetTypeId
			*    Asset type ID to be used as fixed asset type filter, "qsf::getUninitialized<AssetTypeId>()" is there's no fixed asset type filter
			*  @param[in] assetSelectionToLoad
			*    "true" if the selected asset is considered to be loaded in a following step, else "false".
			*    In case the asset is selected in order to be loaded in the next step, we have to perform an additional security check
			*    so that the user can not select an asset which cached asset is out-of-date (editing an old asset version is no good idea).
			*    Do not overuse this security option to not have an negative impact on the coorperative editing.
			*
			*  @return
			*    Return code of the modal dialog, "QDialog::Rejected" on error.
			*    On success, use "qsf::editor::AssetBrowserDialog::getCurrentGlobalAssetId()" in order to get the global ID of the selected asset.
			*/
			int startupAssetSelectionMode(GlobalAssetId globalAssetId, AssetTypeId assetTypeId, bool assetSelectionToLoad = false);

			/**
			*  @brief
			*    Startup the asset browser
			*
			*  @param[in] globalAssetId
			*    ID to be used as current global asset ID, "qsf::getUninitialized<GlobalAssetId>()" is there's no current global asset ID
			*/
			void startupAssetMode(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Return the current global asset ID (the focus of attention)
			*
			*  @return
			*    The current global asset ID, "qsf::getUninitialized<GlobalAssetId>()" is there's no current global asset ID
			*/
			GlobalAssetId getCurrentGlobalAssetId() const;

			//[-------------------------------------------------------]
			//[ Project and asset management                          ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Create a new project
			*/
			void newProject();

			/**
			*  @brief
			*    Import an existing project
			*/
			void importProject();

			/**
			*  @brief
			*    Delete a project
			*
			*  @param[in] project
			*    Project to be deleted
			*/
			void deleteProject(Project& project);

			/**
			*  @brief
			*    Create a new asset package inside the given project
			*
			*  @param[in] project
			*    Project to create the new asset package in
			*/
			void createAssetPackage(Project& project);

			/**
			*  @brief
			*    Delete an asset package
			*
			*  @param[in] assetPackage
			*    Asset package to be deleted
			*/
			void deleteAssetPackage(AssetPackage& assetPackage);

			/**
			*  @brief
			*    Start asset import
			*/
			void importAsset();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onClickedImportAsset(bool checked);
			void onCurrentItemChangedProjects(QTreeWidgetItem* current, QTreeWidgetItem* previous);
			void onClickedDelete(bool checked);
			void onClickedNewAssetPackage(bool checked);
			void onClickedNewProject(bool checked);
			void onClickedImportProject(bool checked);
			void onClickedExpand(bool checked);
			void onClickedCollapse(bool checked);
			void onCurrentIndexChangedAssetType(int index);
			void onClickedAsset(const QModelIndex& index);
			void onSelectionChangedAsset(const QItemSelection& selected, const QItemSelection& deselected);
			void onDoubleClickedAsset(const QModelIndex& index);
			void onCustomContextMenuRequestedProjects(const QPoint& qPoint);
			void onCustomContextMenuRequestedAssets(const QPoint& qPoint);
			void onProjectSearchFieldTextChanged(const QString& searchText);
			void onAssetSearchFieldTextChanged(const QString& searchText);
			// qsf::editor::AssetStorageManager
			void onProjectCreated(const Project& project);
			void onProjectDeleted(ProjectId projectId);
			void onProjectEdited(const Project& project);
			void onAssetPackageCreated(const AssetPackage& assetPackage);
			void onAssetPackageDeleted(ProjectId projectId, const std::string& assetPackageName);
			void onAssetPackageEdited(const AssetPackage& assetPackage);
			void onAssetUpdate(GlobalAssetId globalAssetId);
			void onAssetDelete(GlobalAssetId globalAssetId);
			// qsf::editor::ProjectImport::projectImportFinished
			void onProjectImportFinished(ProjectImport& projectImport);
			void onProjectImportFailed(ProjectImport& projectImport);


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

			/**
			*  @brief
			*    Clear search widgets
			*/
			void clearSearchWidgets();

			/**
			*  @brief
			*    Fill the projects tree widget
			*/
			void fillProjectsTreeWidget();

			/**
			*  @brief
			*    Fill the assets table widget
			*/
			void fillAssetsTableWidget();

			/**
			*  @brief
			*    Update project buttons
			*/
			void updateProjectButtons();

			/**
			*  @brief
			*    Update the assets model filter
			*/
			void updateAssetsModelFilter();

			/**
			*  @brief
			*    Update selection information
			*/
			void updateSelectionInformation();

			/**
			*  @brief
			*    Gathers a list of currently selected assets
			*
			*  @param[in] globalAssetIds
			*    Receives the currently selected global asset IDs, list is not cleared before adding new entires
			*/
			void getSelectedAssets(std::vector<GlobalAssetId>& globalAssetIds) const;

			//[-------------------------------------------------------]
			//[ Asset selection mode relevant                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Set whether or not the asset browser is in asset selection mode
			*
			*  @param[in] assetSelectionMode
			*    "true" if the asset browser is in asset selection mode, else "false"
			*  @param[in] assetSelectionToLoad
			*    "true" if the selected asset is considered to be loaded in a following step, else "false"
			*
			*  @note
			*    - Do only call this method when the GUI is already initialized correctly
			*/
			void setAssetSelectionMode(bool assetSelectionMode, bool assetSelectionToLoad = false);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool					 mAssetPreview;
			int						 mDefaultSectionSize;
			Ui::AssetBrowserDialog*  mUiAssetBrowserDialog;		///< UI asset browser dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it
			AssetBrowserFilterModel* mAssetBrowserFilterModel;	///< Asset browser filter model instance, can be a null pointer, Qt will destroy the instance automatically ("mAssetQTreeView" is the parent)
			AssetBrowserModel*		 mAssetBrowserModel;		///< Asset browser model instance, can be a null pointer, Qt will destroy the instance automatically ("mAssetBrowserFilterModel" is the parent)
			int						 mShownTypeIndex;			///< Combo box index of the currently shown type -- TODO(fw): This variable seems to be useless, as it only gets set inside the constructor
			bool					 mAssetSelectionMode;		///< "true" if the asset browser is in asset selection mode, else "false"
			bool					 mAssetSelectionToLoad;		///< "true" if the selected asset is considered to be loaded in a following step, else "false"
			GlobalAssetId			 mCurrentGlobalAssetId;		///< ID to be used as current global asset ID, "qsf::getUninitialized<GlobalAssetId>()" is there's no current global asset ID
			AssetTypeId				 mFixedAssetTypeId;			///< Asset type ID to be used as fixed asset type filter, "qsf::getUninitialized<AssetTypeId>()" is there's no fixed asset type filter, only used for asset selection mode
			SearchWidget*			 mProjectSearchWidget;		///< Search widget for filtering the project list
			SearchWidget*			 mAssetSearchWidget;		///< Search widget for filtering the asset list
			AssetPackages			 mSelectedAssetPackages;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AssetBrowserDialog)
