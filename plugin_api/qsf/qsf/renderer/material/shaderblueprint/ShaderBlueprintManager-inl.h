// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 ShaderBlueprintManager::getSourceCodeHash() const
	{
		return mSourceCodeHash;
	}

	inline ShaderPiecesManager& ShaderBlueprintManager::getShaderPiecesManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mShaderPiecesManager, "The shader pieces manager instance is invalid", QSF_REACT_THROW);
		return *mShaderPiecesManager;
	}

	inline ShaderSourceCodeBlueprintManager& ShaderBlueprintManager::getShaderSourceCodeBlueprintManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mShaderSourceCodeBlueprintManager, "The shader source code blueprint manager instance is invalid", QSF_REACT_THROW);
		return *mShaderSourceCodeBlueprintManager;
	}

	inline ProgramCacheManager& ShaderBlueprintManager::getProgramCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mProgramCacheManager, "The program cache manager instance is invalid", QSF_REACT_THROW);
		return *mProgramCacheManager;
	}

	inline ShaderCacheManager& ShaderBlueprintManager::getShaderCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mShaderCacheManager, "The shader cache manager instance is invalid", QSF_REACT_THROW);
		return *mShaderCacheManager;
	}

	inline ShaderSourceCodeCacheManager& ShaderBlueprintManager::getShaderSourceCodeCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mShaderSourceCodeCacheManager, "The shader source code cache manager instance is invalid", QSF_REACT_THROW);
		return *mShaderSourceCodeCacheManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
