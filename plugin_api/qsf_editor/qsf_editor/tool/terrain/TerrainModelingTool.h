// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/terrain/TerrainTool.h"
#include "qsf_editor/editmode/brush/BrushEditMode.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class TerrainModelingTool;
	class BrushEditFrame;
}
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
		class TerrainModelingEditMode;
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
		*    Terrain modeling tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*
		*  @todo
		*    - TODO(co) Listen and react and edit mode changes and current entity changes as e.g. seen inside "qsf::editor::PolygonTool" (keyword: Toggle GUI button pressed state)
		*    - TODO(co) Enable/disable "Set to Selection"-button according to weather or not something is currently selected
		*/
		class TerrainModelingTool : public TerrainTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainModelingTool" unique pluginable tool ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] toolManager
			*    The tool manager this tool instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit TerrainModelingTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainModelingTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual void readSettings(QSettings& qSettings) override;
			virtual void writeSettings(QSettings& qSettings) override;
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;
			virtual void retranslateUi(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::TerrainTool methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual void onTerrainReady(bool isSuccess) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			void onToggledRaiseLower(bool checked);
			void onToggledSmooth(bool checked);
			void onToggledSetPick(bool checked);
			void onEditingFinishedHeight();
			void onClickedSetToSelection();
			void onClickedImport();
			void onClickedExport();
			void onClickedTouchAll();
			// qsf::editor::TerrainModelingEditMode
			void onHeightPicked();
			// qsf::editor::Application
			void onPreMapRebuild();
			// qsf::editor::EditModeManager
			void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode);
			// qsf::editor::BrushEditMode
			void onBrushSizeChanged();
			void onBrushIntensityChanged();
			// Brush frame
			void onChangeBrushType(int type);
			void onChangeBrushSize(int size);
			void onChangeBrushIntensity(int intensity);
			void onChangeBrushRandomness(int randomness);
			// Terrain tool hotkeys
			void onTerrainToolHotkey();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateBrushData();
			void updateBrushFrame();
			void guiUpdate();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TerrainModelingEditMode& mTerrainModelingEditMode;
			Ui::TerrainModelingTool* mUiTerrainModelingTool;	///< UI terrain modeling tool instance, always valid, we have to destroy the instance in case we no longer need it
			Ui::BrushEditFrame*		 mUiBrushEditFrame;			///< UI brush edit frame instance, always valid, we have to destroy the instance in case we no longer need it
			BrushData				 mBrushRaise;
			BrushData				 mBrushSmooth;
			BrushData				 mBrushSet;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainModelingTool)
