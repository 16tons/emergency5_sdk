// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderProperties.h"

#include <OGRE/OgreHlms.h>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Archive;
	class Texture;
	class VaoManager;
	typedef std::vector<Archive*> ArchiveVec;
	struct QueuedRenderable;
	class CompositorShadowNode;
}
namespace qsf
{
	class MaterialSystem;
	class MaterialTexture;
	class TextureStreamer;
	class MaterialBlueprint;
	class HlmsBlueprintDatablock;
	class InstancesBufferManager;
	class MaterialBlueprintManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::map<StringHash, std::string> DynamicShaderPieces;	// TODO(co) Unordered map might perform better


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF HLMS blueprint implementation
	*
	*  @remarks
	*    Sadly, while the core concept is nice, the OGRE HLMS is poorly implemented. Main flaws (no-goes):
	*    - OGRE compositor is using the old OGRE materials. This means there are two shader shader approaches simultaneously and the OGRE
	*      material shaders are rather overcomplicated to write.
	*    - New material types have to be implemented by writing thousands lines of complicated C++ code, limited to <8 material types.
	*      Since we use one shader system for everything (including compositor), we have 35> material types inside our products. Additionally,
	*      artists can't introduce new material types like a sky material without the help of a C++ programmer.
	*    - No asynchronous shader complication (!)
	*    - More a usage flow of OGRE HLMS and shader pieces: One has to write shaders for each single shader language. In real-world projects
	*      this is not manageable with reasonable (human-)resource investments. Design shaders in a way so one only has one shader which can
	*      be cross-complied into other languages.
	*
	*    We had to introduce our own stuff because the OGRE HLMS stuff is often overcomplicated, while at the same time only offering limited
	*    functionality
	*    - "Ogre::HlmsPropertyVec" is a simple type definition with complicated access copy'n'pasted all over the OGRE codes, "qsf::ShaderProperties"
	*      is much more handy
	*    - Inside OGRE, "Ogre::HlmsPropertyVec" is wildly cleared, set and get all over the place, not good for overview or asynchronous shader
	*      complication (which OGRE doesn't offer)
	*    - "Ogre::Hlms" directly contains the shader builder with it's source code distributed all over the place intermixed with none shader builder
	*      code. Not good for overview or asynchronous shader complication (which OGRE doesn't offer).
	*
	*  @note
	*    - We don't support "Ogre::HlmsListener()" because it's not required with the blueprint concept. Instead there's
	*      "qsf::MaterialBlueprintListener()" which is more high-level.
	*/
	class QSF_API_EXPORT HlmsBlueprint : public Ogre::Hlms
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PipelineStateCache;			// Must be able to call "qsf::MaterialBlueprint::addShaderCache()"
		friend class MaterialSystemCacheManager;	// Must be able to call "qsf::MaterialBlueprint::clearShaderCache()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Ogre::HlmsTypes HLMS_TYPE;	// Using "Ogre::HLMS_USER0"

		typedef std::unordered_set<std::string> AbsolutePieceFilenames;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		HlmsBlueprint(Ogre::Archive* dataFolder, Ogre::ArchiveVec* libraryFolders, MaterialSystem& materialSystem);
		HlmsBlueprint(Ogre::Archive* dataFolder, Ogre::ArchiveVec* libraryFolders, Ogre::HlmsTypes type, const Ogre::String& typeName, MaterialSystem& materialSystem);
		virtual ~HlmsBlueprint();
		const std::string& getShaderProfile() const;	// "glsl", "glsles", "hlsl"
		const std::string* getShaderTarget(Ogre::ShaderType shaderType) const;
		const AbsolutePieceFilenames& getAbsolutePieceFilenames() const;
		MaterialBlueprint* getCreateDatablockMaterialBlueprint() const;
		void setCreateDatablockMaterialBlueprint(MaterialBlueprint* materialBlueprint);	// TODO(co) Horrible hack, maybe extend the OGRE interface with custom data?
		void notifyShowWireframesSetting() const;
		void flushAllRenderables() const;


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::Hlms methods                     ]
	//[-------------------------------------------------------]
	public:
		virtual void calculateHashFor(Ogre::Renderable* renderable, uint32& outHash, uint32& outCasterHash) override;
		virtual Ogre::HlmsCache preparePassHash(const Ogre::CompositorShadowNode* shadowNode, bool casterPass, bool dualParaboloid, Ogre::SceneManager* sceneManager) override;
		virtual const Ogre::HlmsCache* getMaterial(const Ogre::HlmsCache* lastReturnedValue, const Ogre::HlmsCache& passCache, const Ogre::QueuedRenderable& queuedRenderable, bool casterPass) override;
		virtual void preCommandBufferExecution(Ogre::CommandBuffer* commandBuffer) override;
		virtual void postCommandBufferExecution(Ogre::CommandBuffer* commandBuffer) override;
		virtual uint32 fillBuffersFor(const Ogre::HlmsCache* cache, const Ogre::QueuedRenderable& queuedRenderable, bool casterPass, uint32 lastCacheHash, uint32 lastTextureHash) override;
		virtual uint32 fillBuffersForV1(const Ogre::HlmsCache* cache, const Ogre::QueuedRenderable& queuedRenderable, bool casterPass, uint32 lastCacheHash, Ogre::CommandBuffer* commandBuffer) override;
		virtual uint32 fillBuffersForV2(const Ogre::HlmsCache* cache, const Ogre::QueuedRenderable& queuedRenderable, bool casterPass, uint32 lastCacheHash, Ogre::CommandBuffer* commandBuffer) override;
		virtual void frameEnded() override;
		virtual void _changeRenderSystem(Ogre::RenderSystem *newRs) override;


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::Hlms methods                  ]
	//[-------------------------------------------------------]
	protected:
		virtual void enumeratePieceFiles() override;
		virtual const Ogre::HlmsCache* createShaderCacheEntry(uint32 renderableHash, const Ogre::HlmsCache& passCache, uint32 finalHash, const Ogre::QueuedRenderable& queuedRenderable) override;
		virtual Ogre::HlmsDatablock* createDatablockImpl(Ogre::IdString datablockName, const Ogre::HlmsMacroblock* macroblock, const Ogre::HlmsBlendblock* blendblock, const Ogre::HlmsParamVec& paramVec) override;
		virtual Ogre::HlmsDatablock* createDefaultDatablock() override;
		virtual void calculateHashForPreCreate(Ogre::Renderable* renderable, PiecesMap* inOutPieces) override;
		virtual void calculateHashForPreCaster(Ogre::Renderable* renderable, PiecesMap* inOutPieces) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void destroyAllBuffers();
		FORCEINLINE uint32 fillBuffersFor(const Ogre::HlmsCache* cache, const Ogre::QueuedRenderable& queuedRenderable, bool casterPass, uint32 lastCacheHash, Ogre::CommandBuffer* commandBuffer, bool isV1);
		void calculateHashForPreCreate(Ogre::Renderable& ogreRenderable, DynamicShaderPieces inOutDynamicShaderPieces[Ogre::NumShaderTypes]);
		void calculateHashForPreCaster(Ogre::Renderable& ogreRenderable, DynamicShaderPieces inOutDynamicShaderPieces[Ogre::NumShaderTypes]);
		void addCbTextureCommand(const MaterialTexture& materialTexture, Ogre::Texture& ogreTexture, Ogre::CommandBuffer& ogreCommandBuffer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialBlueprintManager& mMaterialBlueprintManager;
		Ogre::VaoManager*		  mOgreVaoManager;
		AbsolutePieceFilenames	  mAbsolutePieceFilenames;
		TextureStreamer&		  mTextureStreamer;
		MaterialBlueprint*		  mCreateDatablockMaterialBlueprint;			///< Material blueprint which will be assigned to created datablocks, can but shouldn't be a null pointer, don't destroy the instance
		ShaderProperties		  mTransportShaderProperties;					///< For the transport of shader properties from "qsf::HlmsBlueprint::calculateHashForPreCreate()" into "qsf::HlmsBlueprint::calculateHashFor()" (OGRE style)
		uint32					  mLastTextureHash;
		float					  mLastTextureHashCachedDistanceToCamera;
		const Ogre::Texture*	  mBoundOgreTextures[OGRE_MAX_TEXTURE_LAYERS];	///< We need to keep track of the OGRE textures bound to texture units, when filling the pass constant buffer this information might be requested. Additionally, e.g. "Ogre::D3D11RenderSystem::_setTexture()" does not perform redundancy checks.
		MaterialBlueprint*		  mLastMaterialBlueprint;
		bool					  mUseTextureBuffer;
		bool					  mNeedToSetTextureWithSampler;					///< Tell the renderer about the sampler states each time setting a texture?
																				// -> DirectX 11 is using explicit sampler objects inside shaders and the material blueprint can just set them once to rule all following calls
																				// -> OpenGL binds sampler objects to texture units and a "glBindTexture()" call resets the sampler objects bound via "glBindSampler()".
																				//    Meaning the material blueprint can't just set the sampler object once, but has to do this on OpenGL for each and every texture set call.

		// Per pass changing data
		const Ogre::CompositorShadowNode* mOgreCompositorShadowNode;
		bool							  mDualParaboloid;
		Ogre::SceneManager*				  mOgreSceneManager;

		// Buffer managers
		InstancesBufferManager* mInstancesBufferManager;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
