// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline ShaderBlueprintManager& MaterialSystem::getShaderBlueprintManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mShaderBlueprintManager, "The shader blueprint manager instance is invalid", QSF_REACT_THROW);
		return *mShaderBlueprintManager;
	}

	inline MaterialBlueprintManager& MaterialSystem::getMaterialBlueprintManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialBlueprintManager, "The material blueprint manager instance is invalid", QSF_REACT_THROW);
		return *mMaterialBlueprintManager;
	}

	inline MaterialManager& MaterialSystem::getMaterialManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialManager, "The material manager instance is invalid", QSF_REACT_THROW);
		return *mMaterialManager;
	}

	inline MaterialVariationManager& MaterialSystem::getMaterialVariationManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialVariationManager, "The material variation manager instance is invalid", QSF_REACT_THROW);
		return *mMaterialVariationManager;
	}

	inline MaterialSystemCacheManager& MaterialSystem::getMaterialSystemCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialSystemCacheManager, "The material system cache manager instance is invalid", QSF_REACT_THROW);
		return *mMaterialSystemCacheManager;
	}

	inline HlmsBlueprint* MaterialSystem::getHlmsBlueprint() const
	{
		return mHlmsBlueprint;
	}

	inline PipelineStateCompiler& MaterialSystem::getPipelineStateCompiler() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPipelineStateCompiler, "The pipeline state compiler instance is invalid", QSF_REACT_THROW);
		return *mPipelineStateCompiler;
	}

	inline ShadowSetup* MaterialSystem::getShadowSetup() const
	{
		return mShadowSetup;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* MaterialSystem::getName() const
	{
		return "Material system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
