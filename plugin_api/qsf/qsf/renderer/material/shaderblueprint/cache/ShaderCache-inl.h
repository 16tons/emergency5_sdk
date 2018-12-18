// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderCache::ShaderCache(ShaderCacheId shaderCacheId) :
		mShaderCacheId(shaderCacheId),
		mShaderSourceCodeCacheId(0)
	{
		// Nothing here
	}

	inline ShaderCache::~ShaderCache()
	{
		// Nothing here
	}

	inline ShaderCacheId ShaderCache::getShaderCacheId() const
	{
		return mShaderCacheId;
	}

	inline ShaderSourceCodeCacheId ShaderCache::getShaderSourceCodeCacheId() const
	{
		return mShaderSourceCodeCacheId;
	}

	inline Ogre::HighLevelGpuProgramPtr ShaderCache::getOgreHighLevelGpuProgramPtr() const
	{
		return mOgreHighLevelGpuProgramPtr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
