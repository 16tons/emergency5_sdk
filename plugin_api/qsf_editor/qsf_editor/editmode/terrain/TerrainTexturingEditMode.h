// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/terrain/TerrainEditMode.h"
#include "qsf/math/Color4.h"


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
		*    Terrain texturing edit mode class
		*/
		class TerrainTexturingEditMode : public TerrainEditMode
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted when the chunk selection is changed
			*/
			void chunkChanged(uint32 chunkX, uint32 chunkY);


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainTexturingEditMode" unique pluginable edit mode ID


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
			explicit TerrainTexturingEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainTexturingEditMode();

			/**
			*  @brief
			*    Return the brush texture index
			*
			*  @return
			*    The brush texture index
			*/
			uint32 getBrushTextureIndex() const;

			/**
			*  @brief
			*    Set the brush texture index
			*
			*  @param[in] brushTextureIndex
			*    The brush texture index
			*/
			void setBrushTextureIndex(uint32 brushTextureIndex);

			/**
			*  @brief
			*    Set the color we use to draw the chunk
			*/
			void setChunkColor(QColor color);

			/**
			*  @brief
			*    Set if we want to paint into multiple layers
			*/
			void setMultiLayerUsed(bool multiLayerUsed);

			/**
			*  @brief
			*    Set if we want to paint into multiple layers
			*/
			void setMultiLayerIndizes(const std::vector<uint32>& multiLayerIndizes);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::BrushEditMode methods  ]
		//[-------------------------------------------------------]
		protected:
			virtual void onBrushingStartup() override;
			virtual void onBrushing() override;
			virtual void onBrushingShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Private virtual qsf::editor::BrushEditMode methods    ]
		//[-------------------------------------------------------]
		private:
			virtual void buildBrushShape() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Start the brush visualization
			*/
			void startChunkVisualization();

			/**
			*  @brief
			*    Update the brush visualization transform
			*/
			void updateChunkVisualizationTransform();

			/**
			*  @brief
			*    Stop the brush visualization
			*/
			void stopChunkVisualization();

			/**
			*  @brief
			*    Paint on the terrain with specified brush
			*/
			void editTexturePainting();

			/**
			*  @brief
			*    Paint into channel
			*/
			void channelPainting(const TerrainComponent* terrainComponent, TerrainDataMap* terrainDataMap, int channelIndex, float paint);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32				mBrushTextureIndex;
			unsigned int		mChunkDrawRequestId;	///< Debug draw request IDs for chunk visualisation
			qsf::Color4			mChunkColor;
			bool				mMultiLayerUsed;
			std::vector<uint32>	mMultiLayerIndizes;		///< All texture indizes we want to draw


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainTexturingEditMode)
