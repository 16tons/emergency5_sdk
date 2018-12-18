// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/terrain/TerrainEditMode.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
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
		*    Terrain modeling edit mode class
		*/
		class TerrainModelingEditMode : public TerrainEditMode
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
			*    Qt signal emitted after an height pick action has occurred
			*/
			void heightPicked();


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainModelingEditMode" unique pluginable edit mode ID

			/**
			*  @brief
			*    Terrain transform mode
			*/
			enum TransformMode
			{
				RAISE_LOWER_TRANSFORM = 0,	///< First function "raise" and second function "lower"
				SMOOTH_TRANSFORM	  = 1,	///< First function "smooth"
				SET_PICK_TRANSFORM    = 2	///< First function "set" and second function "pick"
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
			explicit TerrainModelingEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainModelingEditMode();

			/**
			*  @brief
			*    Return the terrain modeling transform mode
			*
			*  @return
			*   The terrain modeling transform mode
			*/
			TransformMode getTransformMode() const;

			/**
			*  @brief
			*    Set the terrain modeling transform mode
			*
			*  @param[in] transformMode
			*   Terrain transform edit mode to set
			*/
			void setTransformMode(TransformMode transformMode);

			/**
			*  @brief
			*    Return the height
			*
			*  @return
			*    Height
			*/
			float getHeight() const;

			/**
			*  @brief
			*    Set the height
			*
			*  @param[in] height
			*    Height to set
			*/
			void setHeight(float height);

			/**
			*  @brief
			*    Set height map values to selected entities
			*/
			void setHeightValuesToSelection();

			/**
			*  @brief
			*    Set height map values to selected entities
			*
			*  @todo
			*    - TODO(co) This feature was not asked for and is not required by our artists, we might want to remove it later on
			*/
			void applyGlobalHeightSmooth();


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
			*    Upate the OGRE terrain directly
			*
			*  @param[in] terrain
			*    OGRE terrain to manipulate
			*  @param[in] newValues
			*    Vector of terrain values
			*/
			void updateOgreTerrainHeightMapDirectly(const TerrainComponent& terrainComponent, const std::vector<TerrainDataMapValue>& newValues) const;

			/**
			*  @brief
			*    Raise or lower terrain values
			*/
			void editHeightRaise();

			/**
			*  @brief
			*    Smooth an area of terrain
			*/
			void editHeightSmooth();

			/**
			*  @brief
			*    Get the height at brush position
			*
			*  @return
			*    The normalized height, so it is [0..1]
			*/
			float getHeightAt() const;

			/**
			*  @brief
			*    Set an area of the terrain to a specified height
			*/
			void editHeightSet();

			/**
			*  @brief
			*    Set the area of the terrain to the height of a entity
			*
			*  @param[in] entity
			*    The entity
			*/
			void setHeightValuesToEntity(const Entity& entity);

			/**
			*  @brief
			*    Smooth the whole terrain
			*
			*  @param[in] newValues
			*    Vector that holds the terrain values
			*/
			void applyGlobalHeightSmoothInternal();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TransformMode	mTransformMode;	///< The current terrain modeling transform mode
			float			mHeight;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainModelingEditMode)
