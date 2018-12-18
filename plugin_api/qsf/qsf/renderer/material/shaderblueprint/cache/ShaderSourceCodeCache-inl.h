// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderSourceCodeCache::ShaderSourceCodeCache(ShaderSourceCodeCacheId shaderSourceCodeCacheId) :
		mShaderSourceCodeCacheId(shaderSourceCodeCacheId),
		mShaderBlueprintId(0),
		mShaderBlueprintType(ShaderBlueprint::Type::UNKNOWN)
	{
		// Nothing here
	}

	inline ShaderSourceCodeCache::~ShaderSourceCodeCache()
	{
		// Nothing here
	}

	inline ShaderSourceCodeCacheId ShaderSourceCodeCache::getShaderSourceCodeCacheId() const
	{
		return mShaderSourceCodeCacheId;
	}

	inline ShaderBlueprintId ShaderSourceCodeCache::getShaderBlueprintId() const
	{
		return mShaderBlueprintId;
	}

	inline ShaderBlueprint::Type ShaderSourceCodeCache::getShaderBlueprintType() const
	{
		return mShaderBlueprintType;
	}

	inline const std::string& ShaderSourceCodeCache::getHlslProfile() const
	{
		return mHlslProfile;
	}

	inline const std::string& ShaderSourceCodeCache::getBuildSourceCode() const
	{
		return mBuildSourceCode;
	}

	inline const std::string& ShaderSourceCodeCache::getDebugFilename() const
	{
		return mDebugFilename;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
