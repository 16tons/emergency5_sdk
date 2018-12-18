// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/reflection/CampClass.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	namespace editor
	{
		class TerrainDataChannel;
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
		//[ Enum                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Terrain data type
		*
		*  @note
		*    - Enum registered in CAMP - please update Plugin.cpp accordingly when changing this enum!
		*/
		enum TerrainDataType
		{
			DATATYPE_HEIGHTMAP,		///< Height map
			DATATYPE_COLORMAP,		///< Color map
			DATATYPE_BLENDMAP,		///< Blend map
			DATATYPE_DECORATIONMAP	///< Decoration map	// TODO(co) To be implemented, I assume
		};


		//[-------------------------------------------------------]
		//[ Structures                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Helper structure that holds the manipulation data for terrain data map operation
		*/
		struct TerrainDataMapValue
		{
			uint16	x;			///< X coordinate inside the terrain data map
			uint16	y;			///< Y coordinate inside the terrain data map
			uint8	channel;	///< Channel of the terrain data map
			float	value;		///< Values at the x/y coordinate
			float	valueOld;	///< Old values at the x/y coordinate

			virtual ~TerrainDataMapValue() {}

			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI() // Only adds the virtual method "campClassId()", nothing more
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Terrain data map class
		*
		*  @note
		*    - Holds a 2D map of float data
		*    - It can have multiple channels
		*/
		class TerrainDataMap : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static std::string getTerrainDataTypeAsString(TerrainDataType terrainDataType);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] terrainDataType
			*    Terrain data type
			*  @param[in] compress
			*    Compress data channel?
			*/
			explicit TerrainDataMap(TerrainDataType terrainDataType, bool compress = true);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			~TerrainDataMap();

			/**
			*  @brief
			*    Get the terrain data type
			*
			*  @return
			*    The terrain data type
			*/
			TerrainDataType getTerrainDataType() const;

			/**
			*  @brief
			*    Get size for x-dimension
			*
			*  @return
			*    Width of terrain data map
			*/
			int getWidth() const;

			/**
			*  @brief
			*    Get size for y-dimension
			*
			*  @return
			*    Height of terrain data map
			*/
			int getHeight() const;

			/**
			*  @brief
			*    Return the number of channels
			*
			*  @return
			*    The number of channels
			*/
			int getNumberOfChannels() const;

			/**
			*  @brief
			*    Set size and channels of terrain data map
			*
			*  @param[in] width
			*    Size for x-dimension
			*  @param[in] height
			*    Size for y-dimension
			*  @param[in] numberOfChannels
			*    Number of channels
			*/
			void setSize(uint16 width, uint16 height, uint16 numberOfChannels);

			/**
			*  @brief
			*    Return a specified channel
			*
			*  @param[in] channelIndex
			*    Channel
			*
			*  @return
			*    Internal pointer of a channel, do not destroy the instance
			*/
			TerrainDataChannel* getChannel(uint16 channelIndex) const;

			/**
			*  @brief
			*    Add a new channel
			*
			*  @return
			*    Index of new channel
			*/
			int addChannel();

			/**
			*  @brief
			*    Remove all channels
			*/
			void removeAllChannels();

			/**
			*  @brief
			*    Return the internal data pointer of a channel
			*
			*  @param[in] channelIndex
			*    Channel
			*
			*  @return
			*    Internal data pointer, can be a null pointer, do not destroy the instance
			*/
			float* getChannelData(uint16 channelIndex) const;

			/**
			*  @brief
			*    Check whether or not the given position is valid
			*
			*  @param[in] x
			*    Position x
			*  @param[in] y
			*    Position y
			*
			*  @return
			*    "true" if position is valid, else "false"
			*/
			bool isValid(int x, int y) const;

			/**
			*  @brief
			*    Get value at position
			*
			*  @param[in] x
			*    Position x
			*  @param[in] y
			*    Position y
			*  @param[in] channelIndex
			*    Channel
			*
			*  @return
			*    Value at x, y position
			*/
			float getValueAtPosition(int x, int y, uint16 channelIndex) const;

			/**
			*  @brief
			*    Set value at position
			*
			*  @param[in] x
			*    Position x
			*  @param[in] y
			*    Position y
			*  @param[in] channelIndex
			*    Channel index
			*  @param[in] value
			*    Value at x, y position
			*/
			void setValueAtPosition(int x, int y, uint16 channelIndex, float value);

			/**
			*  @brief
			*    Apply scale and bias
			*
			*  @param[in] minimumValue
			*    Minimum value
			*  @param[in] maximumValue
			*    Maximum value
			*/
			void scaleAllValuesByMinimumMaximum(float minimumValue, float maximumValue);

			/**
			*  @brief
			*    Compress all channels, if internal compression is enabled
			*/
			void compressMap();

			/**
			*  @brief
			*    Decompress channel
			*
			*  @param[in] channelIndex
			*    Channel index
			*/
			void decompressChannel(uint16 channelIndex);

			//[-------------------------------------------------------]
			//[ Workspace                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Clear workspace
			*
			*  @note
			*    - The workspace can be used by operations as central and managed intermediate buffer
			*/
			void clearWorkspace();

			/**
			*  @brief
			*    Return a value map workspace
			*
			*  @return
			*    Value map workspace, do not destroy the instance
			*
			*  @see
			*    - "qsf::editor::TerrainDataMap::clearWorkspace()"
			*/
			std::map<uint64, TerrainDataMapValue>& getWorkspaceValueMap();

			/**
			*  @brief
			*    Return a value vector workspace
			*
			*  @return
			*    Value vector workspace, do not destroy the instance
			*
			*  @see
			*    - "qsf::editor::TerrainDataMap::clearWorkspace()"
			*/
			std::vector<TerrainDataMapValue>& getWorkspaceValueVector();

			//[-------------------------------------------------------]
			//[ Fill                                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Fill the terrain data map with random numbers
			*/
			void fillWithRandom();

			/**
			*  @brief
			*    Fill the terrain data map with numbers
			*
			*  @param[in] value
			*    Value to be set
			*/
			void fillWithValue(float value);

			/**
			*  @brief
			*    Fill a terrain channel data map with numbers
			*
			*  @param[in] channelIndex
			*    Channel index
			*  @param[in] value
			*    Value to be set
			*/
			void fillChannelWithValue(uint16 channelIndex, float value);

			//[-------------------------------------------------------]
			//[ Dirty                                                 ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Mark complete terrain data map as dirty
			*/
			void setDirtyAll();

			/**
			*  @brief
			*    Mark none of terrain data map as dirty
			*/
			void setDirtyClear();

			/**
			*  @brief
			*    Return the dirty minimum X
			*
			*  @return
			*    Dirty minimum X
			*/
			int getDirtyMinX() const;

			/**
			*  @brief
			*    Return the dirty maximum X
			*
			*  @return
			*    Dirty maximum X
			*/
			int getDirtyMaxX() const;

			/**
			*  @brief
			*    Return the dirty minimum Y
			*
			*  @return
			*    Dirty minimum Y
			*/
			int getDirtyMinY() const;

			/**
			*  @brief
			*    Return the dirty maximum Y
			*
			*  @return
			*    Dirty maximum Y
			*/
			int getDirtyMaxY() const;

			/**
			*  @brief
			*    Return channel by channel tag
			*
			*  @param[in] tag
			*    Tag
			*
			*  @return
			*    Channel index, <0 when no channel with this ID exist
			*
			*  @note
			*    - Terrain blend map channel tag semantic = global asset ID of the used terrain layer QSF material
			*/
			int getChannelIndexByTag(uint64 tag) const;

			//[-------------------------------------------------------]
			//[ Serialization                                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Load terrain data map
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*/
			void loadMap(const std::string& absoluteFilename);

			/**
			*  @brief
			*    Save terrain data map
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*/
			void saveMap(const std::string& absoluteFilename);

			/**
			*  @brief
			*    Serialize terrain map
			*
			*  @param[in] serializer
			*    Binary serializer to use
			*/
			void serialize(BinarySerializer& serializer);

			/**
			*  @brief
			*    Import terrain data map
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation to import from
			*  @param[in] scaleAndBias
			*    Scale and bias the imported values?
			*  @param[in] minimumValue
			*    Minimum value, only relevant if scale and bias is enabled
			*  @param[in] maximumValue
			*    Maximum value, only relevant if scale and bias is enabled
			*/
			void importMap(const std::string& absoluteFilename, bool scaleAndBias, float minimumValue, float maximumValue);

			/**
			*  @brief
			*    Export terrain data map
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*  @param[in] normalize
			*    Normalize the data for ohm, raw, f32, r32 before exporting (internal data will not be manipulated)
			*/
			void exportMap(const std::string& absoluteFilename, bool normalize) const;

			/**
			*  @brief
			*    Import terrain data map channel
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation to import from
			*  @param[in] channelIndex
			*    Index of the terrain data map channel to import
			*/
			void importChannel(const std::string& absoluteFilename, uint16 channelIndex);

			/**
			*  @brief
			*    Export terrain data map channel
			*
			*  @param[in] absoluteFilename
			*    The absolute UTF-8 filename in platform-independent notation where to export
			*  @param[in] channelIndex
			*    Index of the terrain data map channel to export
			*/
			void exportChannel(const std::string& absoluteFilename, uint16 channelIndex) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			TerrainDataType					 mTerrainDataType;
			uint16							 mWidth;
			uint16							 mHeight;
			uint16							 mNumberOfChannels;
			std::vector<TerrainDataChannel*> mTerrainDataChannels;
			bool							 mCompress;
			// Workspace
			std::map<uint64, TerrainDataMapValue>* mWorkspaceValueMap;		// Workspace value map operations can use
			std::vector<TerrainDataMapValue>*	   mWorkspaceValueVector;	// Workspace value vector operations can use
			// Dirty
			int	mDirtyMinX;
			int	mDirtyMinY;
			int	mDirtyMaxX;
			int	mDirtyMaxY;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TerrainDataMapValue)
