// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/map/PrefabInstancingEditMode.h"


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
		*    Asset import edit mode class
		*
		*  @remarks
		*    Special prefab instancing variant which is used to import assets via drag'n'drop.
		*
		*    1. As soon as the user drags a file into the editor which is known to the asset importer, a prototype is automatically created
		*       and the editor switches into the asset import edit mode which is a variant of the prefab instancing edit mode
		*    2. During drag the instances preview prefab instance is placed accordingly inside the map
		*    3.1. In case the user instanced the preview prefab inside the prefab instancing edit mode, the import process continues
		*    3.2. In case the user did not instance the preview prefab inside the prefab instancing edit mode, the import process is canceled
		*         and all intermediate import data is unloaded and deleted from the hard drive
		*    4. The import process continues. Before instancing the prefab, show an import asset dialog.
		*    4.1. The user has to choose a category and an existing asset package the new imported assets should be merged into. In this dialog,
		*         the user can still cancel the import process.
		*    4.2. On confirmation, the asset merge process starts. Assets which are already in the "database" are not imported, again. All other
		*         new assets are merged into the specified asset package.
		*    4.3. The prefab is instanced using the updated imported prefab and data
		*/
		class AssetImportEditMode : public PrefabInstancingEditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::AssetImportEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit AssetImportEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AssetImportEditMode();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::PrefabInstancingEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void createEntityAt(Prototype& prototype, const QPoint& mousePosition) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mAssetImportAccepted;	///< Asset import accepted or rejected?


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::AssetImportEditMode)
