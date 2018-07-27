// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/FastPodAllocator.h"
#include "qsf/renderer/material/material/MaterialTexture.h"

#include <OGRE/OgreTexture.h>
#include <OGRE/OgrePixelFormat.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MaterialTexture;
}


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
	*    Information about each individual texture which is being streamed by the manager
	*/
	struct StreamedTexture : public FastPodAllocator<StreamedTexture>
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		enum TextureStreamingFlags
		{
			TSF_STREAMED  = 1<<0,	///< Texture is streamed
			TSF_SRGB	  = 1<<1,	///< The texture is linearized on lookup
			TSF_HALF_SIZE = 1<<2	///< Half-size quality allowed? (acceptable impact if this flag is set)
		};

		/**
		*  @brief
		*    Information about mip levels
		*
		*  @remarks
		*    Actual mip level count depends on the source texture. The first mip level is the smallest and every level
		*    is at least twice and at most 4 times the size of the previous level.
		*/
		struct MipLevelInfo : public FastPodAllocator<MipLevelInfo>
		{
			uint32 sourceStreamSeekOffset;
			uint32 dataLength;
			uint32 totalDataLength;	//< Data size of all mip levels up to this level, used to allocated memory for a whole mip chain
			uint32 width;
			uint32 height;
		};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		StreamedTexture(GlobalAssetId globalTextureAssetId) :
			globalTextureAssetId(globalTextureAssetId),
			textureStreamingFlags(0),
			imageFlags(0),
			imagePixelFormat(Ogre::PF_UNKNOWN),
			numberOfImageMipMaps(0),
			mipLevels(nullptr),
			ioPriority(0.0f)
		{
			// Use black as default, this is causing the fewest visually totally ugly side effects (emissive map, transparent particles, transparent flares etc.)
			averageColor[0] = averageColor[1] = averageColor[2] = 0;

			dynamicStreamingData.currentLoadedMipLevel = 0;
			dynamicStreamingData.targetMipLevel = 0;
			dynamicStreamingData.currentMipLevelInVRAM = 0;
			dynamicStreamingData.mipChainDataShadowCopy = nullptr;
		}

		~StreamedTexture()
		{
			for (MaterialTexture* materialTexture : referencingMaterialTextures)
			{
				materialTexture->mStreamedTexture = nullptr;
			}

			delete [] mipLevels;

			// Due to the dynamic nature of the shadow copy size, it
			// is allocated with "realloc" and thus has to be freed
			// with free()
			free(dynamicStreamingData.mipChainDataShadowCopy);
		}

		bool isStreamed() const
		{
			return (textureStreamingFlags & TSF_STREAMED);
		}


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		// Extended texture asset metadata
		GlobalAssetId	  globalTextureAssetId;
		uint32			  textureStreamingFlags;	///< See "TextureStreamingFlags"
		uint32			  imageFlags;				///< See Ogre::ImageFlags
		Ogre::PixelFormat imagePixelFormat;
		uint32			  numberOfImageMipMaps;
		MipLevelInfo*	  mipLevels;
		float			  ioPriority;				///< Priority for I/O operations of this texture (higher value = more important = needs to be processed before other lower priority stuff)
		uint8			  averageColor[3];

		// Data used at run-time by the streaming manager
		Ogre::TexturePtr	ogreTexturePtr;
		struct
		{
			// Which (virtual) mip level is targeted to be loaded
			// Virtual means, the corresponding mip level might not really exist or be accessible.
			uint32 targetMipLevel;

			// Which (real) mip level is currently scheduled for I/O
			uint32 loadMipLevelTarget;

			// Which mip level is REALLY loaded (i.e. data exists in the shadow copy)
			uint32 currentLoadedMipLevel;

			// Which mip level has been synced to VRAM
			uint32 currentMipLevelInVRAM;

			// System memory shadow copy of loaded mips
			uint8* mipChainDataShadowCopy;

			// Last touch time of the texture
			Time lastTouch;
		} dynamicStreamingData;

		typedef std::vector<MaterialTexture*> ReferencingMaterialTextures;
		ReferencingMaterialTextures referencingMaterialTextures;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
