// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PipelineStateCache::PipelineStateCache(PipelineStateCacheId pipelineStateCacheId) :
		mPipelineStateCacheId(pipelineStateCacheId),
		mIsUsingFallback(false),
		mProgramCache(nullptr),
		mOgreHlmsCache(nullptr)
	{
		// Nothing here
	}

	inline PipelineStateCache::~PipelineStateCache()
	{
		// Nothing here
	}

	inline PipelineStateCacheId PipelineStateCache::getPipelineStateCacheId() const
	{
		return mPipelineStateCacheId;
	}

	inline const PipelineStateSignature& PipelineStateCache::getPipelineStateSignature() const
	{
		return mPipelineStateSignature;
	}

	inline bool PipelineStateCache::isUsingFallback() const
	{
		return mIsUsingFallback;
	}

	inline const ProgramCache* PipelineStateCache::getProgramCache() const
	{
		return mProgramCache;
	}

	inline const Ogre::HlmsCache* PipelineStateCache::getOgreHlmsCache() const
	{
		return mOgreHlmsCache;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
