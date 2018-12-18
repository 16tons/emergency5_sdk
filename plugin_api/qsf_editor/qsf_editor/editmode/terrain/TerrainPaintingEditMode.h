// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/terrain/TerrainEditMode.h"

#include <qsf/math/Color3.h>


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
		*    Terrain painting edit mode class
		*/
		class TerrainPaintingEditMode : public TerrainEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainPaintingEditMode" unique pluginable edit mode ID

			/**
			*  @brief
			*    Terrain paint mode
			*/
			enum PaintMode
			{
				COLOR_PAINT = 0,	///< First function "paint"
				CAVE_PAINT  = 1		///< First function "cut cave" and second function "close cave"
			};


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
			explicit TerrainPaintingEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainPaintingEditMode();

			/**
			*  @brief
			*    Return the terrain paint mode
			*
			*  @return
			*   The terrain paint mode
			*/
			PaintMode getPaintMode() const;

			/**
			*  @brief
			*    Set the terrain paint mode
			*
			*  @param[in] paintMode
			*   Terrain paint mode to set
			*/
			void setPaintMode(PaintMode paintMode);

			/**
			*  @brief
			*    Return the brush color
			*
			*  @return
			*    The brush color, do not destroy the instance
			*/
			const Color3& getBrushColor() const;

			/**
			*  @brief
			*    Set the brush color
			*
			*  @param[in] brushColor
			*    The brush color, do not destroy the instance
			*/
			void setBrushColor(const Color3& brushColor);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::BrushEditMode methods  ]
		//[-------------------------------------------------------]
		protected:
			virtual void onBrushingStartup() override;
			virtual void onBrushing() override;
			virtual void onBrushingShutdown() override;


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
			*    Paint on the terrain with specified brush
			*/
			void editColorPainting();

			/**
			*  @brief
			*    Paint caves into the terrain with specified brush
			*/
			void editCavePainting();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PaintMode	mPaintMode;		///< The current terrain paint mode
			Color3		mBrushColor;	///< Brush color


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainPaintingEditMode)
