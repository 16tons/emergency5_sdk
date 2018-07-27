// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/cache/ShaderCache.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class HlmsBlueprint;
	class ShaderBlueprint;
	class BinarySerializer;
	class ShaderSourceCodeCache;
	class ShaderBlueprintManager;
	class PipelineStateSignature;
	class ShaderSourceCodeCacheManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::vector<ShaderCacheId>			 ShaderCacheIds;
	typedef std::vector<ShaderSourceCodeCacheId> ShaderSourceCodeCacheIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/*
	*  @brief
	*    Shader cache manager
	*
	*  @note
	*    - The shader cache manager is owned by the shader blueprint manager
	*/
	class ShaderCacheManager : public ElementManager<ShaderCacheId, ShaderCache, detail::DirectAllocator<ShaderCache>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprintManager;	///< Is creating and using a shader cache manager instance


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get shader cache by pipeline state signature
		*
		*  @param[in] shaderBlueprint
		*    Shader blueprint to use
		*  @param[in] pipelineStateSignature
		*    Pipeline state signature to use
		*
		*  @return
		*    The shader cache, null pointer on error, do not destroy the instance
		*/
		ShaderCache* getShaderCacheByPipelineStateSignature(const ShaderBlueprint& shaderBlueprint, const PipelineStateSignature& pipelineStateSignature);

		/**
		*  @brief
		*    Clear the shader cache manager
		*/
		void clearCache();

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] deletedShaderSourceCodeCacheIds
		*    IDs of the deleted shader source code caches
		*  @param[out] deletedShaderCacheIds
		*    Receives the IDs of the deleted shader caches
		*/
		void cleanupDirtyCacheParts(const ShaderSourceCodeCacheIds& deletedShaderSourceCodeCacheIds, ShaderCacheIds& deletedShaderCacheIds);

		/**
		*  @brief
		*    Serialize the shader cache manager
		*/
		void serializeCache(BinarySerializer& binarySerializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ShaderCacheManager(ShaderBlueprintManager& shaderBlueprintManager);
		inline ~ShaderCacheManager();
		ShaderCacheManager(const ShaderCacheManager&) = delete;
		ShaderCacheManager& operator=(const ShaderCacheManager&) = delete;
		void createOgreHighLevelGpuProgram(ShaderCache& shaderCache, const ShaderSourceCodeCache& shaderSourceCodeCache);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderSourceCodeCacheManager& mShaderSourceCodeCacheManager;	///< Shader source code cache manager
		HlmsBlueprint*				  mHlmsBlueprint;					///< HLMS blueprint instance, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderCacheManager.inl"
