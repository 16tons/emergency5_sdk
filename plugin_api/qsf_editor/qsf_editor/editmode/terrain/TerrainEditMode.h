// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/brush/BrushEditMode.h"
#include "qsf_editor/operation/terrain/TerrainDataMap.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TerrainComponent;
	namespace editor
	{
		class TerrainEditHelper;
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
		*    Terrain edit mode base class
		*/
		class TerrainEditMode : public BrushEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainEditMode();


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit TerrainEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Convert the world space position to local space
			*
			*  @param[in] terrainComponent
			*    Terrain component
			*  @param[in] position
			*    World space position
			*
			*  @return
			*    Local space position
			*/
			glm::vec3 worldToLocalSpace(const TerrainComponent& terrainComponent, const glm::vec3& position) const;

			/**
			*  @brief
			*    Convert the local space position to point space
			*
			*  @param[in] terrainComponent
			*    Terrain component
			*  @param[in] position
			*    Local space position
			*
			*  @return
			*    Point space position
			*/
			glm::vec3 localToPointSpace(const TerrainComponent& terrainComponent, const glm::vec3& position) const;

			/**
			*  @brief
			*    Convert the world space position to point space (aka texture/texel space)
			*
			*  @param[in] terrainComponent
			*    Terrain component
			*  @param[in] position
			*    World space position
			*
			*  @return
			*    Point space position
			*/
			glm::vec3 worldToPointSpace(const TerrainComponent& terrainComponent, const glm::vec3& position) const;

			/**
			*  @brief
			*    Convert the local space position to world space
			*
			*  @param[in] terrain
			*    Terrain component
			*  @param[in] position
			*    Local space position
			*
			*  @return
			*    World space position
			*/
			glm::vec3 localToWorldSpace(const TerrainComponent& terrainComponent, const glm::vec3& position) const;

			/**
			*  @brief
			*    Add a new terrain value to a map of terrain values
			*
			*  @param[in, out] values
			*    Terrain value map
			*  @param[in] addValue
			*    New terrain value
			*/
			void addTerrainValue(std::map<uint64, TerrainDataMapValue>& values, const TerrainDataMapValue& addValue) const;

			/**
			*  @brief
			*    Calculate the minimum and maximum points for a brush by position and size
			*
			*  @param[in] terrainComponent
			*    Terrain component
			*  @param[in] position
			*    Position of brush in local terrain space
			*  @param[in] width
			*    Width of data layer
			*  @param[out] pointXMin
			*    Minimal point of terrain in X direction
			*  @param[out] pointXMax
			*    Maximal point of terrain in X direction
			*  @param[out] pointZMin
			*    Minimal point of terrain in Z direction
			*  @param[out] pointZMax
			*    Maximal point of terrain in Z direction
			*/
			void getEditArea(const TerrainComponent& terrainComponent, const glm::vec3& position, int width, int& pointXMin, int& pointXMax, int& pointZMin, int& pointZMax) const;

			/**
			*  @brief
			*    Get the intensity value by a specified brush type
			*
			*  @param[in] terrainComponent
			*    Terrain component
			*  @param[in] position
			*    Position of brush in local terrain space
			*  @param[in] u
			*    u parameter of terrain (0..1)
			*  @param[in] v
			*    v parameter of terrain (0..1)
			*
			*  @return
			*    Intensity value ranges from 0 to 1
			*/
			float intensityByBrush(const TerrainComponent& terrainComponent, const glm::vec3& position, float u, float v) const;

			/**
			*  @brief
			*    Get a list of terrain components influenced by the brush
			*
			*  @param[out] terrainComponentsInfluencedByBrush
			*    List of terrain components influenced by the brush, list is not cleared before adding new entries
			*/
			void getTerrainComponentsInfluencedByBrush(std::vector<TerrainComponent*>& terrainComponentsInfluencedByBrush) const;

			/**
			*  @brief
			*    Gather brush modifications on the given terrain
			*
			*  @param[in] terrainComponent
			*    Terrain component the gatherd values are meant for
			*  @param[in] width
			*    Data map width
			*  @param[out] valueList
			*    Gatherd values, list is not cleared before adding new entries
			*/
			void getValueList(const TerrainComponent& terrainComponent, int width, std::vector<TerrainDataMapValue>& valueList) const;

			/**
			*  @brief
			*    Start editing by using the first found terrain component
			*
			*  @return
			*    The first found terrain component, can be a null pointer, do not destroy the instance
			*/
			TerrainComponent* editFirstFoundTerrainComponent();

			/**
			*  @brief
			*    Return the terrain component this edit mode is currently manipulating
			*
			*  @return
			*    The terrain component this edit mode is currently manipulating, can be a null pointer, do not destroy the instance
			*/
			TerrainComponent* getTerrainComponent() const;

			/**
			*  @brief
			*    Return the terrain edit helper instance
			*
			*  @return
			*    Terrain edit helper instance, do not destroy the instance
			*
			*  @note
			*    - ATTENTION: Do only call this method if getTerrainComponent() != nullptr
			*/
			TerrainEditHelper& getTerrainEditHelper() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::BrushEditMode methods  ]
		//[-------------------------------------------------------]
		protected:
			virtual bool evaluateBrushPosition(const QPoint& mousePosition, glm::vec3& position) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void onShutdown(EditMode* nextEditMode) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::shared_ptr<TerrainEditHelper> mTerrainEditHelper;
			TerrainComponent*				   mTerrainComponent;		///< The terrain component this edit mode is currently manipulating, can be a null pointer, do not destroy the instance


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainEditMode)
