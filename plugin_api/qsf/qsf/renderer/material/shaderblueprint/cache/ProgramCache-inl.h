// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ProgramCache::ProgramCache(ProgramCacheId programCacheId) :
		mProgramCacheId(programCacheId)
	{
		memset(&mShaderCaches[0], 0, sizeof(ShaderCache*) * Ogre::NumShaderTypes);
	}

	inline ProgramCache::~ProgramCache()
	{
		// Nothing here
	}

	inline ProgramCacheId ProgramCache::getProgramCacheId() const
	{
		return mProgramCacheId;
	}

	inline const ShaderCache* ProgramCache::getShaderCache(Ogre::ShaderType shaderType) const
	{
		return mShaderCaches[shaderType];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
