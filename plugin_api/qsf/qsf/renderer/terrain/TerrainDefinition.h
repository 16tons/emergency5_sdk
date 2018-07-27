// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/asset/AssetSystemTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Terrain definition container/handler
	*/
	class QSF_API_EXPORT TerrainDefinition
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char	FORMAT_TYPE[];
		static const uint32 FORMAT_VERSION;
		static const char	FILE_EXTENSION[];


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		TerrainDefinition(GlobalAssetId globalMapAssetId, uint64 entityId);
		TerrainDefinition(const TerrainDefinition& terrainDefinition);

		GlobalAssetId getGlobalMapAssetId() const;
		uint64 getEntityId() const;

		std::string getName() const;

		bool isValid() const;

		GlobalAssetId getColorMap() const;
		GlobalAssetId getNormalMap() const;

		GlobalAssetId getSourceBlendMap() const;
		GlobalAssetId getSourceColorMap() const;
		GlobalAssetId getSourceHeightMap() const;

		uint32 getTerrainChunksPerEdge() const;
		GlobalAssetId getOgreTerrainChunk(uint32 x, uint32 y) const;

		void getAllGlobalAssetIds(std::vector<GlobalAssetId>& globalAssetIds) const;
		void getSourceGlobalAssetIds(std::vector<GlobalAssetId>& globalAssetIds) const;
		void getDerivedGlobalAssetIds(std::vector<GlobalAssetId>& globalAssetIds) const;

		void clear();

		void initialize(GlobalAssetId globalColorMapAssetId, GlobalAssetId globalNormalMapAssetId, GlobalAssetId globalSourceBlendMapAssetId, GlobalAssetId globalSourceColorMapAssetId, GlobalAssetId globalSourceHeightMapAssetId, const std::vector<std::vector<GlobalAssetId>>& globalTerrainChunkAssetIds);
		bool loadFromAsset(GlobalAssetId globalTerrainAssetId);

		void saveByFilename(const std::string& filename);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GlobalAssetId mGlobalMapAssetId;
		uint64 mEntityId;

		GlobalAssetId mColorMap;
		GlobalAssetId mNormalMap;

		GlobalAssetId mSourceBlendMap;
		GlobalAssetId mSourceColorMap;
		GlobalAssetId mSourceHeightMap;

		uint32 mTerrainChunksPerEdge;
		std::vector<std::vector<GlobalAssetId>> mOgreTerrainChunks;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
