// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/Tool.h"
#include "qsf_editor/operation/terrain/TerrainDataMap.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QLabel;
QT_END_NAMESPACE
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
		*    Terrain tool base class
		*/
		class TerrainTool : public QObject, public Tool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TerrainTool" unique pluginable tool ID


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
			explicit TerrainTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainTool();

			/**
			*  @brief
			*    Return a list of terrain component instances which are currently influenced by this tool
			*
			*  @param[out] terrainComponents
			*    List will receive the terrain component instances which are currently influenced by this tool, list is not cleared before adding new entries
			*/
			void getInfluencedTerrainComponents(std::vector<TerrainComponent*>& terrainComponents);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::TerrainTool methods    ]
		//[-------------------------------------------------------]
		protected:
			virtual void onTerrainReady(bool isSuccess) = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Update the GUI showing information about the currently edited terrain map
			*
			*  @param[in] qLabel
			*    Qt label to update
			*  @param[in] terrainDataType
			*    Terrain data type
			*/
			void updateTerrainMapInformationGui(QLabel& qLabel, TerrainDataType terrainDataType);

			/**
			*  @brief
			*    Return the terrain edit helper instance
			*
			*  @return
			*    Terrain edit helper instance, do not destroy the instance
			*/
			TerrainEditHelper& getTerrainEditHelper(uint64 entityId) const;

			/**
			*  @brief
			*    Return the terrain edit helper instance map
			*
			*  @return
			*    Terrain edit helper instance map, do not modify the map
			*/
			const std::map<uint64, std::shared_ptr<TerrainEditHelper>>& getTerrainEditHelperMap() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onTerrainEditHelperReady();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::map<uint64, std::shared_ptr<TerrainEditHelper>> mTerrainEditHelperMap;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainTool)
