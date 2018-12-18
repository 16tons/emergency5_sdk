// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/MaterialBlueprint.h"

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class VaoManager;
	class RenderSystem;
	class CommandBuffer;
	struct QueuedRenderable;
}
namespace qsf
{
	class MaterialBlueprintManager;
	class PassConstantBufferManager;
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
	*    Instances buffer manager for managing per-instance constant buffer and texture buffer
	*
	*  @remarks
	*    The used OGRE unlit HLMS used extensive bit shifting. Since this doesn't safe us any memory and there are still two integers unused, I removed the bit shifting (simpler code, viewer instructions).
	*    If at some point later we need to transport more informations as fit into four integers:
	*    - C++ side
	*      // uint4 MaterialIndexArray[]
	*      *currentMappedConstBuffer = static_cast<uint32>((distToWorldMatStart << 9 ) | (hlmsBlueprintDatablock->getAssignedSlot() & 0x1FF));
	*    - Shader side
	*      - x = The lower 9 bits contain the material's start index. The higher 23 bits contain the world matrix start index.
	*      - To get material index = x & 0x1FFu
	*      - To get world matrix index = x >> 9u
	*
	*  @note
	*    - Basing on "Ogre::HlmsBufferManager"
	*    - Class name is plural because the properties of many instances are stored in the buffers (constant and texture)
	*/
	class InstancesBufferManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		InstancesBufferManager(Ogre::RenderSystem& ogreRenderSystem, Ogre::VaoManager& ogreVaoManager, uint16 constantBufferSlotIndex, uint16 textureBufferSlotIndex);
		~InstancesBufferManager();

		void onPreparePassHash();
		void onPreCommandBufferExecution(Ogre::CommandBuffer& ogreCommandBuffer);
		void onPostCommandBufferExecution();
		void onFrameEnded();

		/**
		*  @brief
		*    Fill the instances buffers
		*
		*  @param[in] passConstantBufferManager
		*    Pass constant buffer manager
		*  @param[in] instancesConstantBuffer
		*    Instances constant buffer
		*  @param[in] instancesTextureBuffer
		*    Instances texture buffer
		*  @param[in] ogreQueuedRenderable
		*    OGRE queued renderable
		*  @param[in] renderableCache
		*    Renderable cache
		*  @param[in] ogreCommandBuffer
		*    OGRE command buffer to feed
		*/
		uint32 fillBuffers(const PassConstantBufferManager& passConstantBufferManager, const MaterialBlueprint::ConstantBuffer* instancesConstantBuffer, const MaterialBlueprint::TextureBuffer* instancesTextureBuffer, const Ogre::QueuedRenderable& ogreQueuedRenderable, const MaterialBlueprint::RenderableCache& renderableCache, Ogre::CommandBuffer& ogreCommandBuffer);

		/**
		*  @brief
		*    Bind the instances buffers to the renderer
		*
		*  @param[in] ogreCommandBuffer
		*    OGRE command buffer to fill with instructions
		*  @param[in] useTextureBuffer
		*    Use texture buffer?
		*/
		void bindToRenderer(Ogre::CommandBuffer& ogreCommandBuffer, bool useTextureBuffer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/// Warning: Calling this function affects BOTH mCurrentConstBuffer and mCurrentTexBuffer
		DECL_MALLOC uint32* mapNextConstBuffer(Ogre::CommandBuffer& ogreCommandBuffer);

		/// For compatibility reasons with D3D11 and GLES3, Const buffers are mapped.
		/// Once we're done with it (even if we didn't fully use it) we discard it
		/// and get a new one. We will at least have to get a new one on every pass.
		/// This is affordable since common Const buffer limits are of 64kb.
		/// At the next frame we restart mCurrentConstBuffer to 0.
		void unmapConstBuffer();

		/// Texture buffers are treated differently than Const buffers. We first map it.
		/// Once we're done with it, we save our progress (in mTexLastOffset) and in the
		/// next pass start where we left off (i.e. if we wrote to the first 2MB chunk,
		/// start mapping from 2MB onwards). Only when the buffer is full, we get a new
		/// Tex Buffer.
		/// At the next frame we restart mCurrentTexBuffer to 0.
		///
		/// Tex Buffers can be as big as 128MB, thus "restarting" with another 128MB
		/// buffer on every pass is too expensive. This strategy benefits low level RS
		/// like GL3+ and D3D11.1* (Windows 8) and D3D12; whereas on D3D11 and GLES3
		/// drivers dynamic mapping may discover we're writing to a region not in use
		/// or may internally use a new buffer (wasting memory space).
		///
		/// (*) D3D11.1 allows using MAP_NO_OVERWRITE for texture buffers.
		DECL_MALLOC float* mapNextTexBuffer(Ogre::CommandBuffer& ogreCommandBuffer, size_t minimumSizeBytes);
		void unmapTexBuffer(Ogre::CommandBuffer& ogreCommandBuffer);

		/** Rebinds the texture buffer. Finishes the last bind command to the tbuffer.
		@param resetOffset
			When true, the tbuffer will be offsetted so that the shader samples
			from 0 at the current offset in mCurrentMappedTexBuffer
			WARNING: mCurrentMappedTexBuffer may be modified due to alignment.
			mStartMappedTexBuffer & mCurrentTexBufferSize will always be modified
		@param minimumTexBufferSize
			If resetOffset is true and the remaining space in the currently mapped
			tbuffer is less than minimumSizeBytes, we will call mapNextTexBuffer
		*/
		void rebindTexBuffer(Ogre::CommandBuffer& ogreCommandBuffer, bool resetOffset = false, size_t minimumSizeBytes = 1);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Ogre::ConstBufferPacked*> ConstBufferPackedVec;
		typedef std::vector<Ogre::TexBufferPacked*>   TexBufferPackedVec;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const MaterialBlueprintManager& mMaterialBlueprintManager;
		Ogre::VaoManager&				mOgreVaoManager;
		uint16							mConstantBufferSlotIndex;
		uint16							mTextureBufferSlotIndex;
		bool							mUseTextureBuffer;

		/// The tex. buffer's size. Try raising this number if your API traces/profilers
		/// show we're constantly binding new textures. Should only be relevant if you
		/// have many skeletally animated meshes with lots of bones.
		size_t mTextureBufferDefaultSize;

		uint32				 mCurrentConstBuffer;	/// Resets every to zero every new frame
		uint32				 mCurrentTexBuffer;		/// Resets every to zero every new frame
		ConstBufferPackedVec mConstBuffers;
		TexBufferPackedVec	 mTexBuffers;

		uint32* mStartMappedConstBuffer;
		uint32* mCurrentMappedConstBuffer;
		size_t  mCurrentConstBufferSize;

		/// Holds ptr to the start of the mapped region
		float* mRealStartMappedTexBuffer;

		/// Holds ptr to the start of the **bound** region to the shader slot.
		/// It is always mStartMappedTexBuffer >= mRealStartMappedTexBuffer
		float* mStartMappedTexBuffer;
		float* mCurrentMappedTexBuffer;

		/// Bindable size left
		size_t mCurrentTexBufferSize;

		/** Holds the offset at which all tex. binds should start from.
			Resets every to zero every new buffer (@see unmapTexBuffer
			and @see mapNextTexBuffer).
		@remarks
			The texture buffer has three location we need to track for:
				* Where the buffer mapping starts (i.e. beginning of each render_pass)
					Tracked via mRealStartMappedTexBuffer
				* Where the buffer latest bind point starts (i.e. each time we exceed
					the HW limit for const buffers, usually 64kb; or rendering between
					render queue IDs). Tracked via mStartMappedTexBuffer.
				* How much data we've written so far for the current texture buffer,
					tracked via mTexLastOffset.
		*/
		size_t mTexLastOffset;

		/// Stores the offset to the last command buffer's binding command so we can
		/// write the amount of bytes that should be bound (which is only known after
		/// we've written them).
		size_t mLastTexBufferCmdOffset;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
