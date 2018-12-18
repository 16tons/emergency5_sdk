// Copyright (C) 2012-2018 Promotion Software GmbH


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
		*    Prefab browser view menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class PrefabBrowserViewMenu : public AssetMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Select all prefab instances of a given set of prefab asset global IDs
			*
			*  @param[in] globalAssetIds
			*    List of global prefab asset IDs
			*/
			static void selectAllPrefabInstances(const std::vector<GlobalAssetId>& globalAssetIds);

			/**
			*  @brief
			*    Selects and updates all prefab instances of a given set of prefab asset global IDs
			*
			*  @param[in] globalAssetIds
			*    List of global prefab asset IDs
			*/
			static void selectAndUpdateAllPrefabInstances(const std::vector<GlobalAssetId>& globalAssetIds);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] globalAssetIds
			*    List of global asset IDs this menu should operate on
			*/
			explicit PrefabBrowserViewMenu(const std::vector<GlobalAssetId>& globalAssetIds);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PrefabBrowserViewMenu();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredSelectAllPrefabInstances();
			void onTriggeredSelectAndUpdateAllPrefabInstances();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
