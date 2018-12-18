// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& ShaderSourceCodeCacheManager::getDebugOutputPath() const
	{
		return mDebugOutputPath;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline ShaderSourceCodeCacheManager::ShaderSourceCodeCacheManager(const ShaderPiecesManager& shaderPiecesManager) :
		mShaderPiecesManager(shaderPiecesManager),
		mEnableDebugOutput(false)
	{
		// Nothing here
	}

	inline ShaderSourceCodeCacheManager::~ShaderSourceCodeCacheManager()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
