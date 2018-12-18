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

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class TerrainTexturingTool;
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
		class TerrainTexturingEditMode;
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
		*    Terrain texturing tool class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class TerrainTexturingTool : public TerrainTool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainTexturingTool" unique pluginable tool ID

			static const QColor SELECTED_COLOR;			// Colors for QWidget Selection
			static const QColor SELECTED_VISIBLE_COLOR;
			static const QColor SELECTED_ERROR_COLOR;

			static const QColor VISIBLE_COLOR_1;		// Colors for QWidget Rows
			static const QColor VISIBLE_COLOR_2;

			static const QColor ERROR_COLOR_1;
			static const QColor ERROR_COLOR_2;

			static const QColor CHUNK_COLOR_WHITE;		// Color for Chunk Visualitzation
			static const QColor CHUNK_COLOR_WARNING;
			static const QColor CHUNK_COLOR_ERROR;


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
			explicit TerrainTexturingTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainTexturingTool();


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
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const int PRIORITY_COLUMN		= 0;	///< Column of priority
			static const int NAME_COLUMN			= 1;	///< Column of material name
			static const int PIXEL_NUMBER_COLUMN	= 2;	///< Column of number of blended pixel


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);
			void onClickedImport();
			void onClickedExport();
			void onClickedTouchAll();
			void onToggledPaintMode(bool checked);
			void onLayerSelected();
			// qsf::editor::Application
			void onPreMapRebuild();
			// qsf::editor::EditModeManager
			void onChangeEditMode(EditMode* newEditMode, EditMode* previousEditMode);
			// qsf::editor::BrushEditMode
			void onBrushSizeChanged();
			void onBrushIntensityChanged();
			void onChunkChanged(uint32 chunkX, uint32 chunkY);
			void onMultiLayerToggled(bool checked);
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
			void onAssetsChanged(const Assets& assets);

			std::string getLayerName(GlobalAssetId globalAssetId) const;
			void setSelectionColor(int selectionStyle);

			void selectLayer(int rowIndex);
			int getRowIndexFromLayerIndex(int layerIndex) const;

			float getPriority(GlobalAssetId materialId) const;

			void updateLayers();
			void updateBrushData();
			void updateBrushFrame();
			void guiUpdate();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TerrainTexturingEditMode&	mTerrainTexturingEditMode;
			Ui::TerrainTexturingTool*	mUiTerrainTexturingTool;	///< UI terrain texturing tool instance, always valid, we have to destroy the instance in case we no longer need it
			Ui::BrushEditFrame*			mUiBrushEditFrame;			///< UI brush edit frame instance, always valid, we have to destroy the instance in case we no longer need it
			QGridLayout*				mLayoutLayer;
			std::vector<QPushButton*>	mLayerButton;
			std::vector<GlobalAssetId>	mLayerList;
			GlobalAssetId				mSelectedTerrainLayerGlobalAssetId;
			BrushData					mBrushDetail;
			int							mNumberOfVisibleLayer;
			bool						mIsAllVisible;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainTexturingTool)
