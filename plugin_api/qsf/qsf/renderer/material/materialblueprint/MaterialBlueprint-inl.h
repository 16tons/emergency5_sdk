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
	inline MaterialBlueprint::SamplerState::SamplerState() :
		mRootParameterIndex(getUninitialized<uint32>()),
		mBorderColor(Color4::WHITE),	// This is the default value OGRE is using, see "Ogre::HlmsSamplerblock::mBorderColour(Ogre::ColourValue::White)" (while OpenGL is using black)
		mTextureAddressModeU(Renderer::TextureAddressMode::CLAMP),
		mTextureAddressModeV(Renderer::TextureAddressMode::CLAMP),
		mTextureAddressModeW(Renderer::TextureAddressMode::CLAMP),
		mFilterMode(Renderer::FilterMode::DEFAULT),
		mUseCompareFunction(false),
		mOgreHlmsSamplerblock(nullptr)
	{
		// Nothing here
	}

	inline MaterialBlueprint::SamplerState::SamplerState(const SamplerState& samplerState) :
		mRootParameterIndex(samplerState.mRootParameterIndex),
		mBorderColor(samplerState.mBorderColor),
		mTextureAddressModeU(samplerState.mTextureAddressModeU),
		mTextureAddressModeV(samplerState.mTextureAddressModeV),
		mTextureAddressModeW(samplerState.mTextureAddressModeW),
		mFilterMode(samplerState.mFilterMode),
		mUseCompareFunction(samplerState.mUseCompareFunction),
		mOgreHlmsSamplerblock(nullptr)
	{
		// Nothing here
	}

	inline MaterialBlueprint::SamplerState::~SamplerState()
	{
		clearOgreHlmsSamplerblock();
	}

	inline const MaterialBlueprint::SamplerState& MaterialBlueprint::SamplerState::operator =(const SamplerState& samplerState)
	{
		clearOgreHlmsSamplerblock();

		// Copy data
		mRootParameterIndex  = samplerState.mRootParameterIndex;
		mBorderColor		 = samplerState.mBorderColor;
		mTextureAddressModeU = samplerState.mTextureAddressModeU;
		mTextureAddressModeV = samplerState.mTextureAddressModeV;
		mTextureAddressModeW = samplerState.mTextureAddressModeW;
		mFilterMode			 = samplerState.mFilterMode;
		mUseCompareFunction  = samplerState.mUseCompareFunction;

		// Done
		return *this;
	}

	inline uint32 MaterialBlueprint::SamplerState::getRootParameterIndex() const
	{
		return mRootParameterIndex;
	}

	inline MaterialBlueprintId MaterialBlueprint::getMaterialBlueprintId() const
	{
		return mMaterialId;
	}

	inline const std::string& MaterialBlueprint::getMaterialName() const
	{
		return mMaterialName;
	}

	inline PipelineStateCacheManager& MaterialBlueprint::getPipelineStateCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mPipelineStateCacheManager, "The pipeline state cache manager instance is invalid", QSF_REACT_THROW);
		return *mPipelineStateCacheManager;
	}

	inline const MaterialBlueprint::MandatoryStaticCombinationShaderProperties& MaterialBlueprint::getMandatoryStaticCombinationShaderProperties() const
	{
		return mMandatoryStaticCombinationShaderProperties;
	}

	inline const RootSignature& MaterialBlueprint::getRootSignature() const
	{
		return mRootSignature;
	}

	inline const bool MaterialBlueprint::getAllowFillModeOverride() const
	{
		return mAllowFillModeOverride;
	}

	inline const ShaderBlueprint* MaterialBlueprint::getShaderBlueprint(Ogre::ShaderType ogreShaderType) const
	{
		QSF_ASSERT(ogreShaderType != Ogre::NumShaderTypes, "qsf::MaterialBlueprint::getShaderBlueprint(): Invalid OGRE shader type", QSF_REACT_NONE);
		return mShaderBlueprint[ogreShaderType];
	}

	inline PassConstantBufferManager* MaterialBlueprint::getPassConstantBufferManager() const
	{
		return mPassConstantBufferManager;
	}

	inline MaterialsConstantBufferManager* MaterialBlueprint::getMaterialsConstantBufferManager() const
	{
		return mMaterialsConstantBufferManager;
	}

	inline const MaterialBlueprint::ConstantBuffers& MaterialBlueprint::getConstantBuffers() const
	{
		return mConstantBuffers;
	}

	inline const MaterialBlueprint::TextureBuffers& MaterialBlueprint::getTextureBuffers() const
	{
		return mTextureBuffers;
	}

	inline const MaterialBlueprint::SamplerStates& MaterialBlueprint::getSamplerStates() const
	{
		return mSamplerStates;
	}

	inline const MaterialBlueprint::ConstantBuffer* MaterialBlueprint::getPassConstantBuffer() const
	{
		return mPassConstantBuffer;
	}

	inline const MaterialBlueprint::ConstantBuffer* MaterialBlueprint::getMaterialsConstantBuffer() const
	{
		return mMaterialsConstantBuffer;
	}

	inline const MaterialBlueprint::ConstantBuffer* MaterialBlueprint::getInstancesConstantBuffer() const
	{
		return mInstancesConstantBuffer;
	}

	inline const MaterialBlueprint::TextureBuffer* MaterialBlueprint::getInstancesTextureBuffer() const
	{
		return mInstancesTextureBuffer;
	}

	inline int32 MaterialBlueprint::getVisualImportanceOfShaderProperty(ShaderPropertyId shaderPropertyId) const
	{
		return mVisualImportanceOfShaderProperties.getPropertyValueUnsafe(shaderPropertyId);
	}

	inline const Ogre::HlmsCache& MaterialBlueprint::getCurrentPassOgreHlmsCache() const
	{
		return mCurrentPassOgreHlmsCache;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void MaterialBlueprint::setMaterialName(const std::string& materialName)
	{
		mMaterialName = materialName;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
