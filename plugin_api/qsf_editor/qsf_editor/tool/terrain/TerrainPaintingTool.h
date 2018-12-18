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

#include <qsf/math/Color3.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QGridLayout;
QT_END_NAMESPACE
namespace Ui
{
	class TerrainPaintingTool;
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
		class TerrainPaintingEditMode;
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
		*    Terrain painting tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class TerrainPaintingTool : public TerrainTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainPaintingTool" unique pluginable tool ID


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
			explicit TerrainPaintingTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainPaintingTool();


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
			void onToggledPaintMode(bool checked);
			void onToggledCaveMode(bool checked);
			void onClickedBrushColor(bool checked);
			void onClickedAddColor();
			void onClickedRemoveColor();
			void onClickedImport();
			void onClickedExport();
			void onClickedTouchAll();
			void onClickedPaletteImport();
			void onClickedPaletteExport();
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
			// Palette
			void onPaletteButtonClicked();
			void onPaletteHotkey();
			void onClickedHotkey();
			// Terrain tool hotkeys
			void onTerrainToolHotkey();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateBrushData();
			void updateBrushFrame();
			void updatePalette();
			void updateColor();
			void selectColor(int index);
			void guiUpdate();


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const unsigned int NUMBER_OF_HOTKEYS = 5;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TerrainPaintingEditMode&	mTerrainPaintingEditMode;
			Ui::TerrainPaintingTool*	mUiTerrainPaintingTool;	///< UI terrain painting tool instance, always valid, we have to destroy the instance in case we no longer need it
			Ui::BrushEditFrame*			mUiBrushEditFrame;		///< UI brush edit frame instance, always valid, we have to destroy the instance in case we no longer need it
			QGridLayout*				mLayoutPalette;
			std::vector<QPushButton*>	mColorButton;
			std::vector<Color3>			mColorPalette;
			BrushData					mBrushColor;
			BrushData					mBrushCave;
			int							mPaletteSelectedIndex;
			int							mHotkeyIndex[NUMBER_OF_HOTKEYS];


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainPaintingTool)
