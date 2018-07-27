// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/time/Time.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <OGRE/OgreSharedPtr.h>
#include <OGRE/OgrePrerequisites.h>
#include <OGRE/OgrePixelFormat.h>
#include <OGRE/OgreResource.h>
// OGRE includes an OS header resulting in the usual fancy definition-hell, undefine these evil MS Windows definitions
#undef ERROR
#undef TRANSPARENT

#include <atomic>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Image;
	class Texture;
	class TextureManager;
}
namespace qsf
{
	class CachedAsset;
	class PlatformSystem;
	struct StreamedTexture;
	class RendererSettingsGroup;
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
	*    Texture streamer implementation
	*
	*  @remarks
	*    The texture streamer requires the following asset properties to be able to do its work:
	*    - "TextureStreamingFlags" (uint32)
	*      - Description: Texture streaming flags (example "qsf::StreamedTexture::TSF_STREAMED")
	*    - "ImageFlags" (uint32)
	*      - Description: General image flags (example: "Ogre::IF_3D_TEXTURE", "Ogre::IF_CUBEMAP")
	*    - "ImagePixelFormat" (uint32)
	*      - Description: Pixel format (example: "Ogre::PF_A8R8G8B8")
	*    - "ImageWidth" (uint32)
	*      - Description: With of the image (in case of textures with mipmaps, this is the size of the base map)
	*    - "ImageHeight" (uint32)
	*      - Description: Height of the image (in case of textures with mipmaps, this is the size of the base map)
	*    - "NumberOfImageMipMaps" (uint32)
	*      - Description: Number of texture mip-maps
	*/
	class QSF_API_EXPORT TextureStreamer : public Ogre::ManualResourceLoader
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/// Forward declaration of the IO manager interface
		class TextureStreamerIOManager;

		typedef boost::container::flat_map<GlobalAssetId, StreamedTexture*> StreamedTextureMap;
		typedef boost::container::flat_map<Ogre::TexturePtr, StreamedTexture*> StreamedOgreTextureMap;
		typedef std::vector<StreamedTexture*> StreamedTextureList;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TextureStreamer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TextureStreamer();

		/**
		*  @brief
		*    Add a texture which should be streamed
		*
		*  @remarks
		*    Textures which should be streamed must not be defined in the OGRE resource manager! If they are,
		*    the texture cannot be marked as streamed anymore due to the injection of a manual resource to
		*    the texture manager which only works when the texture does not exist yet from that point of view.
		*/
		void addTexture(GlobalAssetId globalTextureAssetId);

		/**
		*  @brief
		*    Unload a texture from being handled by the streaming manager
		*
		*  @note
		*    - Has a bad impact on stability if the texture is still used somewhere, so we can't remove it but just unload it
		*/
		void unloadTexture(GlobalAssetId globalTextureAssetId);

		/**
		*  @brief
		*    Return the streamed texture wrapping a given OGRE texture
		*
		*  @param[in] ogreTexturePtr
		*    OGRE texture to return the streamed texture for
		*
		*  @return
		*    The streamed texture wrapping the given OGRE texture, can be a null pointer, do not destroy the instance
		*/
		StreamedTexture* getStreamedTextureByOgreTexturePtr(const Ogre::TexturePtr& ogreTexturePtr) const;

		/**
		*  @brief
		*    Change the target mip level of a streamed texture
		*
		*  @note
		*    - The manager will try to load/unload the necessary texture data
		*    - Currently no per-texture prioritizing is implemented. Loading order is random.
		*/
		void setTextureTargetMipLevel(StreamedTexture& streamedTexture, uint32 targetMipLevel);

		/**
		*  @brief
		*    Change the target mip level of a streamed texture referenced by using a given global texture asset ID
		*
		*  @note
		*    - The manager will try to load/unload the necessary texture data
		*    - Currently no per-texture prioritizing is implemented. Loading order is random.
		*    - Don't use this method if you can avoid it
		*/
		void setTextureTargetMipLevelByGlobalAssetId(GlobalAssetId globalTextureAssetId, uint32 targetMipLevel);

		/**
		*  @brief
		*    Update the streaming manager from the main thread, i.e. where the render loop is also running
		*
		*  @note
		*    - Necessary for uploading texture data in a thread-safe manner
		*/
		void onUpdate();

		/**
		*  @brief
		*    Will set all streamed textures to the lowest possible mip level and then restore the target mip levels so they will be streamed in again
		*
		*  @remarks
		*    This will create a small hiccup in frame times as the "reducing" of the textures
		*    does currently also involve the loading of the low mip levels from disk (which takes time).
		*/
		void reloadAllTextures();

		/**
		*  @brief
		*    Flush streamed textures so that the queue of textures to load is empty, again
		*
		*  @param[in] lowestPossibleMipLevel
		*    Lowest possible mip level?
		*
		*  @note
		*    - Blocking, do not use this method if you don't have to
		*/
		void flushEnqueuedStreamedTextures(bool lowestPossibleMipLevel = false);

		/**
		*  @brief
		*    Enable progressive mip loading
		*
		*  @remarks
		*    Mip levels will then be loaded one at a time, leaving more room for the loading of diverse textures due to
		*    interlacing of individual mip level loads. But it takes longer for a single texture to load into the target
		*    mip level and due to more distributed I/O it is taking longer in general.
		*/
		inline bool isProgressiveMipLoading() const;
		inline void enableProgressiveMipLoading(bool enabled);

		/**
		*  @brief
		*    Enable the stream-in of the whole texture when it is first loaded
		*
		*  @note
		*    - Without a functional streaming operator, this essentially serves as a delayed background-loading method without memory management
		*/
		inline bool isFullStreamInByDefaultEnabled() const;
		void enableFullStreamInByDefault(bool enabled);

		/**
		*  @brief
		*    By default, only "loaded" textures are considered for streaming
		*
		*  @note
		*    - You can override this behavior by telling the streamer it should stream all textures that are known to him
		*/
		inline bool isAllTextureConsideredForStreamingEnabled() const;
		inline void enableAllTexturesConsideredForStreaming(bool enabled);

		/**
		*  @brief
		*    By default, up to the maximum mip levels of a given source texture are streamed
		*
		*  @param[in] instantlyEnforceUpdate
		*    If "true", this will instantly enforce the new mip level limits on all registered textures, else "false" if only relevant for newly touched textures
		*
		*  @note
		*    - Use this to artificially limit the number of top mip levels to stream in (i.e. reduction of maximum texture quality and also of maximum VRAM usage)
		*/
		inline uint32 getTopMipLevelReductionCount() const;
		void setTopMipLevelReductionCount(uint32 topMipLevelsToShaveOff, bool instantlyEnforceUpdate = true);

		/**
		*  @brief
		*    By default, textures which are not touched are removed from memory after a certain amount of time
		*
		*  @note
		*    - This method allows to enable/disable this behavior
		*/
		inline bool isChronologicCacheClearingEnabled() const;
		void enableChronologicCacheClearing(bool enabled);

		inline const Time& getLastTouch() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Will inspect the dynamic cached asset properties of the given cached texture asset to construct most of the fields in the streamed texture structure
		*/
		StreamedTexture* createStreamedTextureByReadingTextureMetadataFromSourceAssetProperties(const CachedAsset& cachedTextureAsset);

		/**
		*  @brief
		*    Does calculate which mip levels have to be loaded, does VRAM-Redaback (if enabled) and updates the texture resource
		*/
		void processStreamedTexture(StreamedTexture& streamedTexture);

		/**
		*  @brief
		*    Calculate up to which mip level the texture should be loaded
		*
		*  @note
		*    - In the case of progressive mip loading it will only load the next-to-current mip level
		*/
		uint32 calculateStreamedTextureNextMipLevelLoadTarget(StreamedTexture& streamedTexture) const;

		/**
		*  @brief
		*    Check if this texture should be currently considered as "active" in streaming
		*/
		bool considerTextureForStreaming(const StreamedTexture& streamedTexture) const;

		/**
		*  @brief
		*    Will compute the "real" (also: physical) target mip level of the texture
		*
		*  @remarks
		*    The target level in the texture is "virtual" and might exceed the actual count of mipmaps in the texture,
		*    this method will calculate a physical mip level which actually exists in the texture.
		*/
		uint32 getTargetMipLevel(StreamedTexture& streamedTexture) const;

		/**
		*  @brief
		*    Synchronizes contents of the shadow copy to VRAM
		*/
		void synchronizeShadowCopyToVRAM(StreamedTexture& streamedTexture);

		/**
		*  @brief
		*    Touches the texture and records the current timestamp
		*/
		void touchTexture(StreamedTexture& streamedTexture);


	//[-------------------------------------------------------]
	//[ Private virtual Ogre::ManualResourceLoader methods    ]
	//[-------------------------------------------------------]
	private:
		virtual void prepareResource(Ogre::Resource* resource) override;
		virtual void loadResource(Ogre::Resource* resource) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PlatformSystem&				mPlatformSystem;		///< Direct reference to the platform system, always valid, do not destroy the instance
		RendererSettingsGroup&		mRendererSettingsGroup;	///< Direct reference to the renderer settings group instance, always valid, do not destroy the instance
		Ogre::TextureManager*		mOgreTextureManager;	///< Direct pointer to the OGRE texture manager, always valid, do not destroy the instance
		StreamedTextureMap			mStreamedTextures;
		StreamedOgreTextureMap		mStreamedOgreTextures;
		bool						mIsProgressiveMipLoadingEnabled;
		bool						mIsFullStreamInByDefaultEnabled;
		bool						mConsiderAllTexturesForStreaming;
		bool						mEnableChronologicCacheClearing;
		uint32						mTopLevelMipReductionCount;
		Time						mLastTouch;
		std::atomic<uint64>			mCurrentMemoryUsage;
		TextureStreamerIOManager*	mIOManager;			///< Abstraction for the I/O part which could be implemented in different fashions


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/texture/TextureStreamer-inl.h"
