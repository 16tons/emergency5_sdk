// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateSignature.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class HlmsBlueprint;
	class MaterialSystem;
	class PipelineStateCompiler;
	class ShaderBlueprintManager;
	class MaterialBlueprintManager;
	class MaterialSystemCacheMonitor;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderPiecesId;				///< The shader pieces ID is a result of "qsf::StringHash(<shader pieces name>)"
	typedef StringHash ShaderBlueprintId;			///< The shader blueprint ID is a result of "qsf::StringHash(<shader blueprint name>)"
	typedef StringHash ShaderSourceCodeBlueprintId;	///< The shader source code blueprint ID is a result of "qsf::StringHash(<shader source code blueprint name>)"
	typedef std::vector<ShaderPiecesId>				 ShaderPiecesIds;
	typedef std::vector<ShaderSourceCodeBlueprintId> ShaderSourceCodeBlueprintIds;
	typedef std::vector<ShaderBlueprintId>			 ShaderBlueprintIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material system cache manager
	*/
	class QSF_API_EXPORT MaterialSystemCacheManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;	// Creates and manages the material system cache instance


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<PipelineStateSignature> PipelineStateSignatures;

		/**
		*  @brief
		*    Structure holding information about out-of-date (aka "dirty") cache parts; used for incremental cache updates
		*/
		struct DirtyCacheParts
		{
			ShaderPiecesIds				 shaderPiecesIds;
			ShaderSourceCodeBlueprintIds shaderSourceCodeBlueprintIds;
			ShaderBlueprintIds			 shaderBlueprintIds;

			void clear()
			{
				shaderPiecesIds.clear();
				shaderSourceCodeBlueprintIds.clear();
				shaderBlueprintIds.clear();
			}
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return owner material system instance
		*
		*  @return
		*    The owner material system instance, do no destroy the returned instance
		*/
		inline MaterialSystem& getMaterialSystem() const;

		/**
		*  @brief
		*    Return the material system cache monitor instance
		*
		*  @return
		*    The material system cache monitor instance, do no destroy the returned instance
		*/
		inline MaterialSystemCacheMonitor& getMaterialSystemCacheMonitor() const;

		/**
		*  @brief
		*    Clear all OGRE HLMS samplerblocks
		*/
		void clearOgreHlmsSamplerblocks();

		/**
		*  @brief
		*    Explicitly clear all material and shader related caches
		*/
		void clearAllCaches();

		/**
		*  @brief
		*    Explicitly deserialize all material and shader related caches
		*/
		void deserializeAllCaches();

		/**
		*  @brief
		*    Explicitly serialize all material and shader related caches
		*/
		void serializeAllCaches();

		/**
		*  @brief
		*    Gather pipeline state signatures of the currently in-memory cache
		*
		*  @param[out] pipelineStateSignatures
		*    Receives the pipeline state signatures
		*/
		void gatherPipelineStateSignatures(PipelineStateSignatures& pipelineStateSignatures);

		/**
		*  @brief
		*    Create cache by requesting all given pipeline state signatures
		*
		*  @param[in] pipelineStateSignatures
		*    The pipeline state signatures to use for cache creation
		*/
		void createCacheByPipelineStateSignatures(const PipelineStateSignatures& pipelineStateSignatures);

		/**
		*  @brief
		*    Create default pipeline state caches
		*/
		void createDefaultPipelineStateCaches();

		/**
		*  @brief
		*    Explicitly reload all material and shader blueprints
		*
		*  @note
		*    - Usually one doesn't call this method but will use "qsf::MaterialSystemCacheMonitor" instead
		*/
		void reloadAllBlueprints();

		/**
		*  @brief
		*    Search for out-of-date (aka "dirty") cache parts; used for incremental cache updates
		*
		*  @param[out] dirtyCacheParts
		*    Receives the dirty cache parts, not cleared before filled
		*
		*  @return
		*    "true" if anything dirty was found, else "false"
		*/
		bool findDirtyCacheParts(DirtyCacheParts& dirtyCacheParts);

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] dirtyCacheParts
		*    The dirty cache parts
		*/
		void cleanupDirtyCacheParts(const DirtyCacheParts& dirtyCacheParts);

		/**
		*  @brief
		*    Mark file cache as dirty
		*/
		inline void markFileCacheAsDirty();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialSystem
		*    Owner material instance
		*/
		explicit MaterialSystemCacheManager(MaterialSystem& materialSystem);

		/**
		*  @brief
		*    Destructor
		*/
		~MaterialSystemCacheManager();

		/**
		*  @brief
		*    Destructor
		*
		*  @param[in] absoluteCacheFilename
		*    Absolute cache filename
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool deserializeAllCachesByFilename(const std::string& absoluteCacheFilename);

		HlmsBlueprint& getHlmsBlueprint();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialSystem&				mMaterialSystem;				///< The owner material system instance
		ShaderBlueprintManager&	    mShaderBlueprintManager;		///< Shader blueprint manager instance
		MaterialBlueprintManager&   mMaterialBlueprintManager;		///< Material blueprint manager instance
		HlmsBlueprint*				mHlmsBlueprint;					///< HLMS blueprint instance, always valid, don't destroy it
		PipelineStateCompiler&	    mPipelineStateCompiler;			///< Pipeline state compiler
		MaterialSystemCacheMonitor* mMaterialSystemCacheMonitor;	///< Material system cache monitor instance, always valid, we're responsible for destroying the instance in case we no longer need it
		bool						mDirtyFileCache;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/cache/MaterialSystemCacheManager-inl.h"
