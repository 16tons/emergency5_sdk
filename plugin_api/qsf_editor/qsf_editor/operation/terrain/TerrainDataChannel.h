// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
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
		*    Terrain data channel class
		*/
		class TerrainDataChannel : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] compress
			*    Compress data channel?
			*/
			explicit TerrainDataChannel(bool compress = true);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by intent
			*/
			~TerrainDataChannel();

			/**
			*  @brief
			*    Return the internal data pointer
			*
			*  @return
			*    Internal data pointer, can be a null pointer, do not destroy the instance
			*/
			float* getData();

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
			*    Set the size of the terrain data channel
			*
			*  @param[in] width
			*    Size for x-dimension
			*  @param[in] height
			*    Size for y-dimension
			*/
			void setSize(uint16 width, uint16 height);

			/**
			*  @brief
			*    Get value at position
			*
			*  @param[in] x
			*    Position x
			*  @param[in] y
			*    Position y
			*
			*  @return
			*    Value at x, y position
			*/
			float getValueAtPosition(int x, int y);

			/**
			*  @brief
			*    Set value at position
			*
			*  @param[in] x
			*    Position x
			*  @param[in] y
			*    Position y
			*  @param[in] value
			*    Value at x, y position
			*/
			void setValueAtPosition(int x, int y, float value);

			/**
			*  @brief
			*    Get channel tag
			*
			*  @return
			*    Tag
			*
			*  @note
			*    - Terrain blend map channel tag semantic = global asset ID of the used terrain layer QSF material
			*/
			uint64 getTag() const;

			/**
			*  @brief
			*    Set channel tag
			*
			*  @param[in] tag
			*    Tag
			*
			*  @see
			*    - "qsf::editor::base::TerrainDataChannel::getTag()"
			*/
			void setTag(uint64 tag);

			/**
			*  @brief
			*    Get the number of pixels with a value of 0.1 in Chunk(x, y)
			*
			*  @param[in] chunkX
			*    Chunk at position x
			*  @param[in] chunkY
			*    Chunk at position y
			*  @param[in] chunksPerEdge
			*    Number of chunks per edge (Total number = chunksPerEdge²)
			*
			*  @return
			*    The number of pixels
			*/
			int getNumberOfPixels(uint32 chunkX, uint32 chunkY, uint32 chunksPerEdge);

			/**
			*  @brief
			*    Get the information on which layer a chunk of this channel is set
			*
			*  @param[in] chunkX
			*	Chunk at position x
			*  @param[in] chunkY
			*   Chunk at position y
			*
			*  @return
			*    Layer of this chunk or "qsf::getUninitialized<int>()" if nothing was found
			*/
			int getLayer(uint32 chunkX, uint32 chunkY) const;

			/**
			*  @brief
			*    Save the information on which layer a chunk is set
			*
			*  @param[in] chunkX
			*    Chunk at position x
			*  @param[in] chunkY
			*   Chunk at position y
			*  @param[in] layer
			*    If layer is "qsf::getUninitialized<int>()" the layer information is removed
			*/
			void setLayer(uint32 chunkX, uint32 chunkY, int layer);

			/**
			*  @brief
			*    Set all pixel from a chunk to a value
			*
			*  @param[in] chunkX
			*    Chunk at position x
			*  @param[in] chunkY
			*   Chunk at position y
			*  @param[in] chunksPerEdge
			*    Number of chunks per edge (Total number = chunksPerEdge²)
			*  @param[in] value
			*    Blend value
			*/
			void setChunkValues(uint32 chunkX, uint32 chunkY, uint32 chunksPerEdge, float value);

			/**
			*  @brief
			*    Get the priority of this layer
			*/
			float getPriority() const;

			/**
			*  @brief
			*    Check if there is a material connected to it
			*/
			bool hasMaterial() const;

			/**
			*  @brief
			*    Serialize terrain data channel
			*
			*  @param[in] serializer
			*    Binary serializer to use
			*/
			void serialize(BinarySerializer& serializer);

			void compress();
			void decompress();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void compressSub(std::vector<float>& newData, int x1, int y1, int x2, int y2);
			void decompressSub(std::vector<float>& newData, int x1, int y1, int x2, int y2);
			void getSubValueAtPosition(int x, int y, int x1, int y1, int x2, int y2, float& result);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint16	mWidth;
			uint16	mHeight;
			uint64	mTag;		// Useful e.g. GlobalAssetId for TerrainDataChannel
			float*	mData;
			uint32	mNumberOfDataElements;
			bool	mCompress;
			bool	mIsCompressed;

			std::map<uint64,int> mChunkLayer;

			// Need for decompression
			int	mDataDecompressPosition;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
