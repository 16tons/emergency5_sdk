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
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline MaterialPropertyValue MaterialPropertyValue::fromUnknown()
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType = ValueType::UNKNOWN;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromBoolean(bool value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	 = ValueType::BOOLEAN;
		materialPropertyValue.mValue.Boolean = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromInteger(int value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	 = ValueType::INTEGER;
		materialPropertyValue.mValue.Integer = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromInteger2(int value0, int value1)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType		 = ValueType::INTEGER_2;
		materialPropertyValue.mValue.Integer2[0] = value0;
		materialPropertyValue.mValue.Integer2[1] = value1;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromInteger3(int value0, int value1, int value2)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType		 = ValueType::INTEGER_3;
		materialPropertyValue.mValue.Integer3[0] = value0;
		materialPropertyValue.mValue.Integer3[1] = value1;
		materialPropertyValue.mValue.Integer3[2] = value2;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromInteger4(int value0, int value1, int value2, int value3)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType		 = ValueType::INTEGER_4;
		materialPropertyValue.mValue.Integer4[0] = value0;
		materialPropertyValue.mValue.Integer4[1] = value1;
		materialPropertyValue.mValue.Integer4[2] = value2;
		materialPropertyValue.mValue.Integer4[3] = value3;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat(float value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType   = ValueType::FLOAT;
		materialPropertyValue.mValue.Float = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat2(float value0, float value1)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	   = ValueType::FLOAT_2;
		materialPropertyValue.mValue.Float2[0] = value0;
		materialPropertyValue.mValue.Float2[1] = value1;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat3(float value0, float value1, float value2)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	   = ValueType::FLOAT_3;
		materialPropertyValue.mValue.Float3[0] = value0;
		materialPropertyValue.mValue.Float3[1] = value1;
		materialPropertyValue.mValue.Float3[2] = value2;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat4(float value0, float value1, float value2, float value3)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	   = ValueType::FLOAT_4;
		materialPropertyValue.mValue.Float4[0] = value0;
		materialPropertyValue.mValue.Float4[1] = value1;
		materialPropertyValue.mValue.Float4[2] = value2;
		materialPropertyValue.mValue.Float4[3] = value3;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat3_3()
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType = ValueType::FLOAT_3_3;
		materialPropertyValue.mValue.Boolean = false;	// Declaration property only
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFloat4_4()
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType = ValueType::FLOAT_4_4;
		materialPropertyValue.mValue.Boolean = false;	// Declaration property only
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFillMode(Renderer::FillMode value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	  = ValueType::FILL_MODE;
		materialPropertyValue.mValue.FillMode = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromCullMode(Renderer::CullMode value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	  = ValueType::CULL_MODE;
		materialPropertyValue.mValue.CullMode = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromConservativeRasterizationMode(Renderer::ConservativeRasterizationMode value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType						   = ValueType::CONSERVATIVE_RASTERIZATION_MODE;
		materialPropertyValue.mValue.ConservativeRasterizationMode = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromDepthWriteMask(Renderer::DepthWriteMask value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType			= ValueType::DEPTH_WRITE_MASK;
		materialPropertyValue.mValue.DepthWriteMask = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromStencilOp(Renderer::StencilOp value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	   = ValueType::STENCIL_OP;
		materialPropertyValue.mValue.StencilOp = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromComparisonFunc(Renderer::ComparisonFunc value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType			= ValueType::COMPARISON_FUNC;
		materialPropertyValue.mValue.ComparisonFunc = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromRenderTargetWriteMask(Renderer::RenderTargetWriteMask value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType				   = ValueType::RENDER_TARGET_WRITE_MASK;
		materialPropertyValue.mValue.RenderTargetWriteMask = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromBlend(Renderer::Blend value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType   = ValueType::BLEND;
		materialPropertyValue.mValue.Blend = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromBlendOp(Renderer::BlendOp value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType	 = ValueType::BLEND_OP;
		materialPropertyValue.mValue.BlendOp = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromFilterMode(Renderer::FilterMode value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType		= ValueType::FILTER_MODE;
		materialPropertyValue.mValue.FilterMode = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromTextureAddressMode(Renderer::TextureAddressMode value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType				= ValueType::TEXTURE_ADDRESS_MODE;
		materialPropertyValue.mValue.TextureAddressMode = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromGlobalAssetId(GlobalAssetId value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType		   = ValueType::GLOBAL_ASSET_ID;
		materialPropertyValue.mValue.GlobalAssetId = value;
		return materialPropertyValue;
	}

	inline MaterialPropertyValue MaterialPropertyValue::fromCompositorTextureReference(const CompositorTextureReference& value)
	{
		MaterialPropertyValue materialPropertyValue;
		materialPropertyValue.mValueType						= ValueType::COMPOSITOR_TEXTURE_REFERENCE;
		materialPropertyValue.mValue.CompositorTextureReference = value;
		return materialPropertyValue;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MaterialPropertyValue::~MaterialPropertyValue()
	{
		// Nothing here
	}

	inline MaterialPropertyValue::ValueType MaterialPropertyValue::getValueType() const
	{
		return mValueType;
	}

	inline const uint8* MaterialPropertyValue::getData() const
	{
		return reinterpret_cast<const uint8*>(&mValue);
	}

	inline bool MaterialPropertyValue::getBooleanValue() const
	{
		QSF_ASSERT(ValueType::BOOLEAN == mValueType, "qsf::MaterialPropertyValue::getBooleanValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.Boolean;
	}

	inline int MaterialPropertyValue::getIntegerValue() const
	{
		QSF_ASSERT(ValueType::INTEGER == mValueType, "qsf::MaterialPropertyValue::getIntegerValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.Integer;
	}

	inline const int* MaterialPropertyValue::getInteger2Value() const
	{
		QSF_ASSERT(ValueType::INTEGER_2 == mValueType, "qsf::MaterialPropertyValue::getInteger2Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Integer2[0];
	}

	inline const int* MaterialPropertyValue::getInteger3Value() const
	{
		QSF_ASSERT(ValueType::INTEGER_3 == mValueType, "qsf::MaterialPropertyValue::getInteger3Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Integer3[0];
	}

	inline const int* MaterialPropertyValue::getInteger4Value() const
	{
		QSF_ASSERT(ValueType::INTEGER_4 == mValueType, "qsf::MaterialPropertyValue::getInteger4Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Integer4[0];
	}

	inline float MaterialPropertyValue::getFloatValue() const
	{
		QSF_ASSERT(ValueType::FLOAT == mValueType, "qsf::MaterialPropertyValue::getFloatValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.Float;
	}

	inline const float* MaterialPropertyValue::getFloat2Value() const
	{
		QSF_ASSERT(ValueType::FLOAT_2 == mValueType, "qsf::MaterialPropertyValue::getFloat2Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Float2[0];
	}

	inline const float* MaterialPropertyValue::getFloat3Value() const
	{
		QSF_ASSERT(ValueType::FLOAT_3 == mValueType, "qsf::MaterialPropertyValue::getFloat3Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Float3[0];
	}

	inline const float* MaterialPropertyValue::getFloat4Value() const
	{
		QSF_ASSERT(ValueType::FLOAT_4 == mValueType, "qsf::MaterialPropertyValue::getFloat4Value(): Invalid internal type", QSF_REACT_NONE);
		return &mValue.Float4[0];
	}

	inline Renderer::FillMode MaterialPropertyValue::getFillModeValue() const
	{
		QSF_ASSERT(ValueType::FILL_MODE == mValueType, "qsf::MaterialPropertyValue::getFillModeValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.FillMode;
	}

	inline Renderer::CullMode MaterialPropertyValue::getCullModeValue() const
	{
		QSF_ASSERT(ValueType::CULL_MODE == mValueType, "qsf::MaterialPropertyValue::getCullModeValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.CullMode;
	}

	inline Renderer::ConservativeRasterizationMode MaterialPropertyValue::getConservativeRasterizationModeValue() const
	{
		QSF_ASSERT(ValueType::CONSERVATIVE_RASTERIZATION_MODE == mValueType, "qsf::MaterialPropertyValue::getConservativeRasterizationModeValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.ConservativeRasterizationMode;
	}

	inline Renderer::DepthWriteMask MaterialPropertyValue::getDepthWriteMaskValue() const
	{
		QSF_ASSERT(ValueType::DEPTH_WRITE_MASK == mValueType, "qsf::MaterialPropertyValue::getDepthWriteMaskValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.DepthWriteMask;
	}

	inline Renderer::StencilOp MaterialPropertyValue::getStencilOpValue() const
	{
		QSF_ASSERT(ValueType::STENCIL_OP == mValueType, "qsf::MaterialPropertyValue::getStencilOpValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.StencilOp;
	}

	inline Renderer::ComparisonFunc MaterialPropertyValue::getComparisonFuncValue() const
	{
		QSF_ASSERT(ValueType::COMPARISON_FUNC == mValueType, "qsf::MaterialPropertyValue::getComparisonFuncValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.ComparisonFunc;
	}

	inline Renderer::RenderTargetWriteMask MaterialPropertyValue::getRenderTargetWriteMaskValue() const
	{
		QSF_ASSERT(ValueType::RENDER_TARGET_WRITE_MASK == mValueType, "qsf::MaterialPropertyValue::getRenderTargetWriteMaskValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.RenderTargetWriteMask;
	}

	inline Renderer::Blend MaterialPropertyValue::getBlendValue() const
	{
		QSF_ASSERT(ValueType::BLEND == mValueType, "qsf::MaterialPropertyValue::getBlendValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.Blend;
	}

	inline Renderer::BlendOp MaterialPropertyValue::getBlendOpValue() const
	{
		QSF_ASSERT(ValueType::BLEND_OP == mValueType, "qsf::MaterialPropertyValue::getBlendOpValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.BlendOp;
	}

	inline Renderer::FilterMode MaterialPropertyValue::getFilterMode() const
	{
		QSF_ASSERT(ValueType::FILTER_MODE == mValueType, "qsf::MaterialPropertyValue::getFilterMode(): Invalid internal type", QSF_REACT_NONE);
		return mValue.FilterMode;
	}

	inline Renderer::TextureAddressMode MaterialPropertyValue::getTextureAddressModeValue() const
	{
		QSF_ASSERT(ValueType::TEXTURE_ADDRESS_MODE == mValueType, "qsf::MaterialPropertyValue::getTextureAddressModeValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.TextureAddressMode;
	}

	inline GlobalAssetId MaterialPropertyValue::getGlobalAssetIdValue() const
	{
		QSF_ASSERT(ValueType::GLOBAL_ASSET_ID == mValueType, "qsf::MaterialPropertyValue::getGlobalAssetIdValue(): Invalid internal type", QSF_REACT_NONE);
		return mValue.GlobalAssetId;
	}

	inline const MaterialPropertyValue::CompositorTextureReference& MaterialPropertyValue::getCompositorTextureReference() const
	{
		QSF_ASSERT(ValueType::COMPOSITOR_TEXTURE_REFERENCE == mValueType, "qsf::MaterialPropertyValue::getCompositorTextureReference(): Invalid internal type", QSF_REACT_NONE);
		return mValue.CompositorTextureReference;
	}

	inline bool MaterialPropertyValue::operator !=(const MaterialPropertyValue& materialPropertyValue) const
	{
		return !(*this == materialPropertyValue);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline MaterialPropertyValue::MaterialPropertyValue()
	{
		// Nothing here, no member initialization by intent in here (see "qsf::MaterialPropertyValue::fromBoolean()" etc.)
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
