// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/cache/ShaderSourceCodeCache.h"

#include <mutex>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderBlueprint;
	class ShaderProperties;
	class BinarySerializer;
	class ShaderPiecesManager;
	class PipelineStateSignature;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32								 ShaderCombinationId;	///< Shader combination identifier, internally just a POD "uint32", result of hashing the shader combination generating shader blueprint, shader properties and dynamic shader pieces
	typedef std::map<StringHash, std::string>	 DynamicShaderPieces;	// TODO(co) Unordered map might perform better
	typedef std::vector<ShaderBlueprintId>		 ShaderBlueprintIds;
	typedef std::vector<ShaderSourceCodeCacheId> ShaderSourceCodeCacheIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/*
	*  @brief
	*    Shader source code cache manager
	*
	*  @remarks
	*    The shader source code cache has two types of IDs:
	*    - "qsf::ShaderCombinationId" -> Result of hashing the shader combination generating shader blueprint, shader properties and dynamic shader pieces
	*    - "qsf::ShaderSourceCodeCacheId" -> Result of hashing the build shader source code
	*    Those two types of IDs are required because it's possible that different "qsf::ShaderCombinationId" result in one and the same build shader source code.
	*
	*  @note
	*    - The shader source code cache manager is owned by the shader blueprint manager
	*/
	class ShaderSourceCodeCacheManager : public ElementManager<ShaderSourceCodeCacheId, ShaderSourceCodeCache, detail::DirectAllocator<ShaderSourceCodeCache>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprintManager;	///< Is creating and using a shader source code cache manager instance


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Generate shader combination ID
		*
		*  @param[in] shaderBlueprint
		*    The used shader blueprint
		*  @param[in] shaderProperties
		*    Shader properties to use
		*  @param[in] dynamicShaderPieces
		*    Dynamic via C++ generated shader pieces to use
		*
		*  @return
		*    The generated shader combination ID
		*/
		static ShaderCombinationId generateShaderCombinationId(const ShaderBlueprint& shaderBlueprint, const ShaderProperties& shaderProperties, const DynamicShaderPieces& dynamicShaderPieces);

		/**
		*  @brief
		*    Generate shader source code cache ID
		*
		*  @param[in] buildShaderSourceCode
		*    Build ASCII shader source code to use
		*
		*  @return
		*    The generated shader source code cache ID
		*/
		static ShaderSourceCodeCacheId generateShaderSourceCodeCacheId(const std::string& buildShaderSourceCode);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the debug output path
		*
		*  @return
		*    The debug output path
		*
		*  @see
		*    - "qsf::ShaderSourceCodeCacheManager::setDebugOutputPath()"
		*/
		inline const std::string& getDebugOutputPath() const;

		/**
		*  @brief
		*    Call to output the automatically generated shaders (which are usually made from templates) on the given folder for inspection, analyzing, debugging, etc.
		*
		*  @param[in] enableDebugOutput
		*    Whether to enable or disable dumping the shaders into a folder
		*  @param[in] debugOutputPath
		*    Path location on where to dump it. Should end with slash for proper concatenation (i.e. C:/path/ instead of C:/path; or /home/user/ instead of /home/user)
		*
		*  @remarks
		*    The shader will be dumped when it is generated, not when this function gets called. You should call this function at start up.
		*/
		void setDebugOutputPath(bool enableDebugOutput, const std::string& debugOutputPath);

		/**
		*  @brief
		*    Get shader source code cache by pipeline state signature
		*
		*  @param[in] shaderBlueprint
		*    Shader blueprint to use
		*  @param[in] pipelineStateSignature
		*    Pipeline state signature to use
		*
		*  @return
		*    The shader source code cache, null pointer on error, do not destroy the instance
		*
		*  @note
		*    - This method is multi-threading safe because it must be called from the main renderer thread as well as the asynchronous pipeline state compiler
		*/
		ShaderSourceCodeCache* getShaderSourceCodeCacheByPipelineStateSignature(const ShaderBlueprint& shaderBlueprint, const PipelineStateSignature& pipelineStateSignature);

		/**
		*  @brief
		*    Clear the shader source code cache manager
		*/
		void clearCache();

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] dirtyShaderBlueprintIds
		*    IDs of the dirty shader blueprint
		*  @param[out] deletedShaderSourceCodeCacheIds
		*    Receives the IDs of the deleted shader source code caches
		*/
		void cleanupDirtyCacheParts(const ShaderBlueprintIds& dirtyShaderBlueprintIds, ShaderSourceCodeCacheIds& deletedShaderSourceCodeCacheIds);

		/**
		*  @brief
		*    Serialize the shader source code cache manager
		*/
		void serializeCache(BinarySerializer& binarySerializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline explicit ShaderSourceCodeCacheManager(const ShaderPiecesManager& shaderPiecesManager);
		inline ~ShaderSourceCodeCacheManager();
		ShaderSourceCodeCacheManager(const ShaderSourceCodeCacheManager&) = delete;
		ShaderSourceCodeCacheManager& operator=(const ShaderSourceCodeCacheManager&) = delete;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, ShaderSourceCodeCache*> ShaderSourceCodeCacheByShaderCombinationId;	// TODO(co) Key should be "qsf::ShaderCombinationId"


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const ShaderPiecesManager&				   mShaderPiecesManager;
		bool									   mEnableDebugOutput;
		std::string								   mDebugOutputPath;
		ShaderSourceCodeCacheByShaderCombinationId mShaderSourceCodeCacheByShaderCombinationId;
		std::mutex								   mMutex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderSourceCodeCacheManager.inl"
