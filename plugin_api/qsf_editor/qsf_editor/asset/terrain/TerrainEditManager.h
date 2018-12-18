// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/terrain/TerrainData.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/asset/AssetSystemTypes.h>

#include <vector>
#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
		*    Terrain editing manager class
		*/
		class TerrainEditManager : public Manager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::vector<GlobalAssetId> LayerList;	// List of global asset IDs of the used QSF material assets used as terrain layers
			typedef std::map<std::pair<GlobalAssetId, uint64>, std::weak_ptr<TerrainEditHelper>> TerrainEditHelperMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			TerrainEditManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TerrainEditManager();

			/**
			*  @brief
			*    Returns whether the terrain edit manager is ready (= has no more background tasks running, no locks pending)
			*
			*  @return
			*    "true" if the terrain edit manager is ready, "false" otherwise
			*/
			bool isReady() const;

			std::shared_ptr<TerrainEditHelper> getTerrainEditHelper(uint64 entityId);
			std::shared_ptr<TerrainEditHelper> findTerrainEditHelper(uint64 entityId) const;
			LayerList& getLayerList();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onAssetsChanged(const Assets& assets);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TerrainEditHelperMap mTerrainEditHelperMap;
			LayerList			 mLayerList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} //editor
} // qsf
