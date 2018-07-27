// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateCache.h"

#include <OGRE/OgreCommon.h>	// For "Ogre::NumShaderTypes"

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderProperties;
	class BinarySerializer;
	class MaterialBlueprint;
	class ProgramCacheManager;
	class PipelineStateCompiler;
	class MaterialSystemCacheManager;
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
	typedef uint32							  ProgramCacheId;		///< Program cache identifier, internally just a POD "uint32", result of hashing the shader cache IDs of the referenced shaders
	typedef uint32							  PipelineStateCacheId;	///< Pipeline state cache identifier, internally just a POD "uint32", result of hashing the referenced program cache as well as other pipeline state properties
	typedef std::vector<ProgramCacheId>		  ProgramCacheIds;
	typedef std::vector<PipelineStateCacheId> PipelineStateCacheIds;
	typedef std::vector<uint32>				  OgreHlmsCacheHashes;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Pipeline state cache manager
	*
	*  @remarks
	*    The pipeline state cache is the top of the shader related cache hierarchy and maps to Direct3D 12, Vulkan,
	*    AMD Mantle, Apple Metal and other rendering APIs using pipeline state objects (PSO). The next cache hierarchy
	*    level is the program cache which maps to linked monolithic OpenGL programs and is also nice as a collection
	*    of shader compiler results which are fed into pipeline states. The next lowest cache hierarchy level is the
	*    shader cache (vertex shader, pixel shader etc.) which handles the binary results of the shader compiler.
	*    As of January 2016, although claimed to fulfill the OpenGL 4.1 specification, Apples OpenGL implementation used
	*    on Mac OS X lacks the feature of receiving the program binary in order to reuse it for the next time instead of
	*    fully compiling a program. Hence, at the lowest cache hierarchy, there's a shader source code cache for the build
	*    shader source codes so at least this doesn't need to be performed during each program execution.
	*
	*    Sum up of the cache hierarchy:
	*    - 0: "qsf::PipelineStateCacheManager": Maps to Direct3D 12, AMD Mantle, Apple Metal etc.; managed by material blueprint
	*    - 1: "qsf::ProgramCacheManager": Maps to linked monolithic OpenGL programs; managed by shader blueprint manager
	*    - 2: "qsf::ShaderCacheManager": Maps to Direct3D 9 - 11, separate OpenGL shader objects and is still required for Direct3D 12
	*      and other similar designed APIs because the binary shaders are required when creating pipeline state objects;
	*      managed by shader blueprint manager
	*    - 3: "qsf::ShaderSourceCodeCacheManager": Shader source code cache for the build shader source codes, used for e.g. Apples
	*      OpenGL implementation lacking of binary program support; managed by shader blueprint manager
	*
	*    The pipeline state cache has two types of IDs:
	*    - "qsf::PipelineStateSignatureId" -> Result of hashing the material blueprint ID and the shader combination generating shader properties and dynamic shader pieces
	*    - "qsf::PipelineStateCacheId" -> Includes the hashing the build shader source code
	*    Those two types of IDs are required because it's possible that different "qsf::PipelineStateSignatureId" result in one and the
	*    same build shader source code of references shaders.
	*
	*  @note
	*    - One pipeline state cache manager per material blueprint instance
	*
	*  @todo
	*    - TODO(co) For DirectX 12, Vulkan, AMD Mantle and Apple Metal the pipeline state object instance will be managed in here (OGRE currently has no pipeline state support)
	*    - TODO(co) Direct3D 12: Pipeline state object: Add support for "GetCachedBlob" (super efficient material cache), see https://github.com/Microsoft/DirectX-Graphics-Samples/blob/master/Samples/D3D12PipelineStateCache/src/PSOLibrary.cpp
	*/
	class QSF_API_EXPORT PipelineStateCacheManager : public ElementManager<PipelineStateCacheId, PipelineStateCache, detail::DirectAllocator<PipelineStateCache>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialBlueprint;	///< Is creating and using a pipeline cache manager instance


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Request a pipeline state cache instance by combination
		*
		*  @param[in] materialBlueprint
		*    Material blueprint to use
		*  @param[in] shaderProperties
		*    Shader properties to use
		*  @param[in] dynamicShaderPieces
		*    Dynamic via C++ generated shader pieces to use
		*  @param[in] ogreHash
		*    OGRE hash for usage with "qsf::MaterialBlueprint::getShaderCache()"/"qsf::MaterialBlueprint::addShaderCache()" in order to get an "Ogre::HlmsCache"-instance
		*  @param[in] allowEmergencySynchronousCompilation
		*    Allow emergency synchronous compilation if no fallback could be found? This will result in a runtime hiccup instead of graphics artifacts.
		*
		*  @return
		*    The requested pipeline state cache instance, null pointer on error, do not destroy the instance
		*/
		PipelineStateCache* getPipelineStateCacheByCombination(const MaterialBlueprint& materialBlueprint, const ShaderProperties& shaderProperties, const DynamicShaderPieces dynamicShaderPieces[Ogre::NumShaderTypes], uint32 ogreHash, bool allowEmergencySynchronousCompilation);

		/**
		*  @brief
		*    Clear the pipeline state cache manager
		*/
		void clearCache();

		/**
		*  @brief
		*    Delete a given pipeline state cache
		*
		*  @param[in] pipelineStateCache
		*    Pipeline state cache to delete
		*/
		void deletePipelineStateCache(PipelineStateCache& pipelineStateCache);

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] deletedProgramCacheIds
		*    IDs of the deleted program caches
		*  @param[out] deletedPipelineStateCacheIds
		*    Receives the IDs of the deleted pipeline state caches
		*  @param[out] deletedOgreHlmsCacheHashes
		*    Receives the hashes of now invalid OGRE HLMS caches
		*/
		void cleanupDirtyCacheParts(const ProgramCacheIds& deletedProgramCacheIds, PipelineStateCacheIds& deletedPipelineStateCacheIds, OgreHlmsCacheHashes& deletedOgreHlmsCacheHashes);

		/**
		*  @brief
		*    Serialize the pipeline state cache manager
		*/
		void serializeCache(BinarySerializer& binarySerializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit PipelineStateCacheManager(MaterialBlueprint& materialBlueprint);
		inline ~PipelineStateCacheManager();
		PipelineStateCacheManager(const PipelineStateCacheManager&) = delete;
		PipelineStateCacheManager& operator=(const PipelineStateCacheManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, PipelineStateCache*> PipelineStateCacheByPipelineStateSignatureId;	// TODO(co) Key should be "qsf::PipelineStateSignatureId"


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialSystemCacheManager&						mMaterialSystemCacheManager;
		MaterialBlueprint&								mMaterialBlueprint;								///< Owner material blueprint
		PipelineStateCompiler&							mPipelineStateCompiler;							///< Used pipeline state compiler instance, we're not owning it
		ProgramCacheManager&							mProgramCacheManager;							///< Used program cache manager instance, we're not owning it
		PipelineStateCacheByPipelineStateSignatureId	mPipelineStateCacheByPipelineStateSignatureId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateCacheManager-inl.h"
