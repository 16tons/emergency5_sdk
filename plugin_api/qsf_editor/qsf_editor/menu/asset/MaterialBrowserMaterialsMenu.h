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
		*    Material browser materials menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class MaterialBrowserMaterialsMenu : public AssetMenu
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
			*    Save given materials
			*
			*  @param[in] globalAssetIds
			*    List of global asset IDs to save
			*
			*  @todo
			*    - TODO(co) Just a first quick'n'dirty solution to have something for the artists, move this stuff elsewhere
			*/
			static void saveMaterials(const std::vector<GlobalAssetId>& globalAssetIds, bool forceCompile);


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
			explicit MaterialBrowserMaterialsMenu(const std::vector<GlobalAssetId>& globalAssetIds);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MaterialBrowserMaterialsMenu();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredSaveMaterial();
			void onTriggeredForceCompileMaterial();
			void onTriggeredCloneMaterial();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
