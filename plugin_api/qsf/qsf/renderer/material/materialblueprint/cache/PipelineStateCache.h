// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateSignature.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ProgramCache;
	class MaterialBlueprint;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32 PipelineStateCacheId;	///< Pipeline state cache identifier, internally just a POD "uint32", result of hashing the referenced program cache as well as other pipeline state properties


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Pipeline state cache
	*
	*  @note
	*    - TODO(co) For DirectX 12, Vulkan, AMD Mantle and Apple Metal the pipeline state object instance will be stored in here (OGRE currently has no pipeline state support)
	*
	*  @see
	*    - See "qsf::PipelineStateCacheManager" for additional information
	*/
	class PipelineStateCache : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PipelineStateCacheManager;	///< Is creating and managing pipeline state cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit PipelineStateCache(PipelineStateCacheId pipelineStateCacheId);	// TODO(co) Should be private
		inline ~PipelineStateCache();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the pipeline state cache ID
		*
		*  @return
		*    The pipeline state cache ID
		*/
		inline PipelineStateCacheId getPipelineStateCacheId() const;

		/**
		*  @brief
		*    Return the pipeline state signature of the cache
		*
		*  @return
		*    The pipeline state signature of the cache
		*/
		inline const PipelineStateSignature& getPipelineStateSignature() const;

		/**
		*  @brief
		*    Return whether or not the pipeline state cache is currently using fallback data due to asynchronous compilation
		*
		*  @return
		*    If "true", this pipeline state cache is currently using fallback data because it's in asynchronous compilation
		*/
		inline bool isUsingFallback() const;

		/**
		*  @brief
		*    Return the program cache
		*
		*  @return
		*    The program cache, can be a null pointer, do not destroy the instance, do not cache this pointer because it might change later on due to asynchronous compilation
		*/
		inline const ProgramCache* getProgramCache() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the OGRE HLMS cache
		*
		*  @return
		*    The OGRE HLMS cache, null pointer on error, do not destroy the returned instance
		*/
		inline const Ogre::HlmsCache* getOgreHlmsCache() const;

		void setFinalPipelineStateCacheProgramCache(const ProgramCache* programCache, MaterialBlueprint& materialBlueprint);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		PipelineStateCache(const PipelineStateCache&) = delete;
		PipelineStateCache& operator=(const PipelineStateCache&) = delete;
		void setPipelineStateCacheProgramCache(const ProgramCache* programCache, MaterialBlueprint& materialBlueprint);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PipelineStateCacheId   mPipelineStateCacheId;
		PipelineStateSignature mPipelineStateSignature;
		bool				   mIsUsingFallback;			///< If "true", this pipeline state cache is currently using fallback data because it's in asynchronous compilation
		const ProgramCache*	   mProgramCache;				///< Program cache, can be a null pointer, do not destroy the instance
		const Ogre::HlmsCache* mOgreHlmsCache;				///< Contains also the OGRE hash for usage with "qsf::MaterialBlueprint::getShaderCache()"/"qsf::MaterialBlueprint::addShaderCache()" in order to get an "Ogre::HlmsCache"-instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateCache-inl.h"
