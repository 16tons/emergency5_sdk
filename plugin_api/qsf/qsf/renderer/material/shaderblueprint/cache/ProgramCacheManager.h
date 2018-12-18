// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/cache/ProgramCache.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class MaterialBlueprint;
	class ShaderCacheManager;
	class PipelineStateSignature;
	class ShaderBlueprintManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash					ShaderCacheId;	///< Shader cache identifier, internally just a POD "uint32", result of hashing the build shader source code (meaning identical to "qsf::ShaderSourceCodeCacheId")
	typedef std::vector<ShaderCacheId>	ShaderCacheIds;
	typedef std::vector<ProgramCacheId> ProgramCacheIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/*
	*  @brief
	*    Program cache manager
	*
	*  @note
	*    - The program cache manager is owned by the shader blueprint manager
	*
	*  @see
	*    - See "qsf::PipelineStateCacheManager" for additional information
	*/
	class ProgramCacheManager : public ElementManager<ProgramCacheId, ProgramCache, detail::DirectAllocator<ProgramCache>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprintManager;	///< Is creating and using a program cache manager instance


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get program cache by pipeline state signature
		*
		*  @param[in] materialBlueprint
		*    Material blueprint to use
		*  @param[in] pipelineStateSignature
		*    Pipeline state signature to use
		*
		*  @return
		*    The program cache, null pointer on error, do not destroy the instance
		*/
		ProgramCache* getProgramCacheByPipelineStateSignature(const MaterialBlueprint& materialBlueprint, const PipelineStateSignature& pipelineStateSignature);

		/**
		*  @brief
		*    Clear the program cache manager
		*/
		void clearCache();

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] deletedShaderCacheIds
		*    IDs of the deleted shader caches
		*  @param[out] deletedProgramCacheIds
		*    Receives the IDs of the deleted program caches
		*/
		void cleanupDirtyCacheParts(const ShaderCacheIds& deletedShaderCacheIds, ProgramCacheIds& deletedProgramCacheIds);

		/**
		*  @brief
		*    Serialize the program cache manager
		*/
		void serializeCache(BinarySerializer& binarySerializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		explicit ProgramCacheManager(ShaderBlueprintManager& shaderBlueprintManager);
		inline ~ProgramCacheManager();
		ProgramCacheManager(const ProgramCacheManager&) = delete;
		ProgramCacheManager& operator=(const ProgramCacheManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderCacheManager& mShaderCacheManager;	///< Shader cache manager instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ProgramCacheManager-inl.h"
