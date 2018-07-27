// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PipelineStateSignature::PipelineStateSignature() :
		mMaterialBlueprintId(getUninitialized<MaterialBlueprintId>()),
		mOgreHash(getUninitialized<uint32>()),
		mPipelineStateSignatureId(getUninitialized<PipelineStateSignatureId>()),
		mShaderCombinationId{getUninitialized<ShaderCombinationId>(), getUninitialized<ShaderCombinationId>(), getUninitialized<ShaderCombinationId>(), getUninitialized<ShaderCombinationId>(), getUninitialized<ShaderCombinationId>()}
	{
		// Nothing here
	}

	inline PipelineStateSignature::~PipelineStateSignature()
	{
		// Nothing here
	}

	inline MaterialBlueprintId PipelineStateSignature::getMaterialBlueprintId() const
	{
		return mMaterialBlueprintId;
	}

	inline const ShaderProperties& PipelineStateSignature::getShaderProperties() const
	{
		return mShaderProperties;
	}

	inline const DynamicShaderPieces& PipelineStateSignature::getDynamicShaderPieces(Ogre::ShaderType shaderType) const
	{
		return mDynamicShaderPieces[shaderType];
	}

	inline uint32 PipelineStateSignature::getOgreHash() const
	{
		return mOgreHash;
	}

	inline PipelineStateSignatureId PipelineStateSignature::getPipelineStateSignatureId() const
	{
		return mPipelineStateSignatureId;
	}

	inline ShaderCombinationId PipelineStateSignature::getShaderCombinationId(Ogre::ShaderType shaderType) const
	{
		return mShaderCombinationId[shaderType];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
