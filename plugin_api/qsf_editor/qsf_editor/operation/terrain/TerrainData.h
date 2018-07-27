// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/operation/terrain/TerrainDataMap.h"

#include <boost/noncopyable.hpp>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Terrain;
	class TerrainGroup;
}
namespace qsf
{
	class TerrainComponent;
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
		*    Terrain data class
		*/
		class TerrainData : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const unsigned int MAX_NUMBER_OF_VISIBLE_LAYER = 6;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit TerrainData(uint64 entityId);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			~TerrainData();

			/**
			*  @brief
			*    Return the used terrain component instance
			*
			*  @return
			*    The used terrain component instance, null pointer on error, do not destroy the instance
			*
			*  @note
			*    - Ease-of-use method
			*/
			TerrainComponent* getTerrainComponent() const;

			TerrainDataMap* getTerrainDataMap(TerrainDataType dataType, bool createIfNotExist = true);
			void updateOgreMap(TerrainDataType dataType);

			void updateOgreTerrainHeightMap();
			void updateOgreTerrainColorMap();
			void updateOgreTerrainBlendMap();

			void setOgreTerrainHeight(uint16 x, uint16 y, float value, uint16 segments, Ogre::TerrainGroup& ogreTerrainGroup, std::unordered_set<Ogre::Terrain*>& modifiedOgreTerrains);

			void initHeightMap();
			void initColorMap();
			void initBlendMap();
			void clearWorkspace();
			uint64 getEntityId() const;

			/**
			*  @brief
			*    Export terrain data map
			*
			*  @param[in] dataType
			*    Which type of terrain data map should be export
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*  @param[in] scaleAndBias
			*    Scale and bias the imported values?
			*  @param[in] minimumValue
			*    Minimum value, only relevant if scale and bias is enabled
			*  @param[in] maximumValue
			*    Maximum value, only relevant if scale and bias is enabled
			*/
			void importMap(TerrainDataType dataType, const std::string& absoluteFilename, bool scaleAndBias, float minimumValue, float maximumValue);

			/**
			*  @brief
			*    Export terrain data map
			*
			*  @param[in] dataType
			*    Which type of terrain data map should be export
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*  @param[in] normalize
			*    Normalize the data for ohm, raw, f32, r32 before exporting (internal data will not be manipulated)
			*/
			void exportMap(TerrainDataType dataType, const std::string& absoluteFilename, bool normalize);

			void importChannel(TerrainDataType dataType, int channelIndex, const std::string& absoluteFilename);
			void exportChannel(TerrainDataType dataType, int channelIndex, const std::string& absoluteFilename);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			//[-------------------------------------------------------]
			//[ Source asset                                          ]
			//[-------------------------------------------------------]
			std::string getSourceDataPath(TerrainDataType dataType) const;
			bool loadSourceData(TerrainDataType dataType);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint64			mEntityId;
			TerrainDataMap* mHeightMap;
			TerrainDataMap* mColorMap;
			TerrainDataMap* mBlendMap;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
