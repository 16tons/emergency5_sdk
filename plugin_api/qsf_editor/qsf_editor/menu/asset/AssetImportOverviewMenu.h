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
		*    Asset import overview menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class AssetImportOverviewMenu : public AssetMenu
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
			*/
			AssetImportOverviewMenu(const AssetPackages& selectedAssetPackages, const std::vector<GlobalAssetId>& globalAssetIds);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetImportOverviewMenu();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredExcludeAsset();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
