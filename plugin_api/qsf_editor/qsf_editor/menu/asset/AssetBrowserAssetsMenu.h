// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/menu/asset/AssetMenu.h"


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
		*    Asset browser assets menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class AssetBrowserAssetsMenu : public AssetMenu
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
			*  @param[in] selectedAssetPackages
			*    List of selected asset packages
			*  @param[in] globalAssetIds
			*    List of global asset IDs this menu should operate on
			*  @param[in] assetSelectionMode
			*    "true" if the asset browser is in asset selection mode, else "false"
			*/
			AssetBrowserAssetsMenu(const AssetPackages& selectedAssetPackages, const std::vector<GlobalAssetId>& globalAssetIds, bool assetSelectionMode);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetBrowserAssetsMenu();


		//[-------------------------------------------------------]
		//[ Private static methods                                ]
		//[-------------------------------------------------------]
		private:
			static void onMoveToCategoryAssetLockResponse(GlobalAssetId globalAssetId, bool isSuccess, const std::string& category);
			static void onSetTagsAssetLockResponse(GlobalAssetId globalAssetId, bool isSuccess, const std::string& tags);
			static void onDeleteAssetLockResponse(GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// Multi-selection
			void onTriggeredMoveToCategory();
			void onTriggeredAddTag();
			void onTriggeredRemoveTag();
			void onTriggeredRemoveAllTags();
			void onTriggeredMoveToAssetPackage();
			void onTriggeredCopyToAssetPackage();
			void onTriggeredRemoveFromAssetPackage();
			void onTriggeredExportMap();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
