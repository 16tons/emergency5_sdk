// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/detail/SamplerStateTypes.h"
#include "qsf/renderer/material/materialblueprint/detail/PipelineStateTypes.h"

#include <cstring> // For "strcpy()"
#undef DOMAIN	// "math.h" is defining a macro with this name making it impossible to use it as enum name


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	/**
	*  @brief
	*    Own renderer related definitions since OGRE doesn't have a root signature
	*/
	namespace Renderer
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		enum class DescriptorRangeType
		{
			SRV     = 0,
			UAV     = SRV + 1,
			CBV     = UAV + 1,
			SAMPLER = CBV + 1
		};
		struct DescriptorRange
		{
			static const uint32 NAME_LENGTH = 32;
			DescriptorRangeType rangeType;
			uint32			    numberOfDescriptors;
			uint32			    baseShaderRegister;
			uint32			    registerSpace;
			uint32			    offsetInDescriptorsFromTableStart;
			char				baseShaderRegisterName[NAME_LENGTH];
			uint32			    samplerRootParameterIndex;
		};
		struct DescriptorRangeBuilder : public DescriptorRange
		{
			static const uint32 OFFSET_APPEND	= 0xffffffff;
			DescriptorRangeBuilder()
			{
			}
			explicit DescriptorRangeBuilder(const DescriptorRangeBuilder&)
			{
			}
			DescriptorRangeBuilder(
				DescriptorRangeType _rangeType,
				uint32 _numberOfDescriptors,
				uint32 _baseShaderRegister,
				const char _baseShaderRegisterName[NAME_LENGTH],
				uint32 _samplerRootParameterIndex,
				uint32 _registerSpace = 0,
				uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
			{
				initialize(_rangeType, _numberOfDescriptors, _baseShaderRegister, _baseShaderRegisterName, _samplerRootParameterIndex, _registerSpace, _offsetInDescriptorsFromTableStart);
			}
			inline void initializeSampler(
				uint32 _numberOfDescriptors,
				uint32 _baseShaderRegister,
				uint32 _registerSpace = 0,
				uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
			{
				initialize(*this, DescriptorRangeType::SAMPLER, _numberOfDescriptors, _baseShaderRegister, "", 0, _registerSpace, _offsetInDescriptorsFromTableStart);
			}
			inline void initialize(
				DescriptorRangeType _rangeType,
				uint32 _numberOfDescriptors,
				uint32 _baseShaderRegister,
				const char _baseShaderRegisterName[NAME_LENGTH],
				uint32 _samplerRootParameterIndex,
				uint32 _registerSpace = 0,
				uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
			{
				initialize(*this, _rangeType, _numberOfDescriptors, _baseShaderRegister, _baseShaderRegisterName, _samplerRootParameterIndex, _registerSpace, _offsetInDescriptorsFromTableStart);
			}
			static inline void initialize(
				DescriptorRange& range,
				DescriptorRangeType _rangeType,
				uint32 _numberOfDescriptors,
				uint32 _baseShaderRegister,
				const char _baseShaderRegisterName[NAME_LENGTH],
				uint32 _samplerRootParameterIndex,
				uint32 _registerSpace = 0,
				uint32 _offsetInDescriptorsFromTableStart = OFFSET_APPEND)
			{
				range.rangeType = _rangeType;
				range.numberOfDescriptors = _numberOfDescriptors;
				range.baseShaderRegister = _baseShaderRegister;
				range.registerSpace = _registerSpace;
				range.offsetInDescriptorsFromTableStart = _offsetInDescriptorsFromTableStart;
				strcpy(range.baseShaderRegisterName, _baseShaderRegisterName);
				range.samplerRootParameterIndex = _samplerRootParameterIndex;
			}
		};
		struct RootDescriptorTable
		{
			uint32				   numberOfDescriptorRanges;
			const DescriptorRange* descriptorRanges;
		};
		struct RootDescriptorTableBuilder : public RootDescriptorTable
		{
			RootDescriptorTableBuilder()
			{
			}
			explicit RootDescriptorTableBuilder(const RootDescriptorTableBuilder&)
			{
			}
			RootDescriptorTableBuilder(
				uint32 _numberOfDescriptorRanges,
				const DescriptorRange* _descriptorRanges)
			{
				initialize(_numberOfDescriptorRanges, _descriptorRanges);
			}
			inline void initialize(
				uint32 _numberOfDescriptorRanges,
				const DescriptorRange* _descriptorRanges)
			{
				initialize(*this, _numberOfDescriptorRanges, _descriptorRanges);
			}
			static inline void initialize(
				RootDescriptorTable& rootDescriptorTable,
				uint32 _numberOfDescriptorRanges,
				const DescriptorRange* _descriptorRanges)
			{
				rootDescriptorTable.numberOfDescriptorRanges = _numberOfDescriptorRanges;
				rootDescriptorTable.descriptorRanges = _descriptorRanges;
			}
		};
		enum class RootParameterType
		{
			DESCRIPTOR_TABLE = 0,
			CONSTANTS_32BIT  = DESCRIPTOR_TABLE + 1,
			CBV              = CONSTANTS_32BIT + 1,
			SRV              = CBV + 1,
			UAV              = SRV + 1
		};
		struct RootConstants
		{
			uint32 shaderRegister;
			uint32 registerSpace;
			uint32 numberOf32BitValues;
		};
		struct RootConstantsBuilder : public RootConstants
		{
			RootConstantsBuilder()
			{
			}
			explicit RootConstantsBuilder(const RootConstantsBuilder&)
			{
			}
			RootConstantsBuilder(
				uint32 _numberOf32BitValues,
				uint32 _shaderRegister,
				uint32 _registerSpace = 0)
			{
				initialize(_numberOf32BitValues, _shaderRegister, _registerSpace);
			}
			inline void initialize(
				uint32 _numberOf32BitValues,
				uint32 _shaderRegister,
				uint32 _registerSpace = 0)
			{
				initialize(*this, _numberOf32BitValues, _shaderRegister, _registerSpace);
			}
			static inline void initialize(
				RootConstants& rootConstants,
				uint32 _numberOf32BitValues,
				uint32 _shaderRegister,
				uint32 _registerSpace = 0)
			{
				rootConstants.numberOf32BitValues = _numberOf32BitValues;
				rootConstants.shaderRegister = _shaderRegister;
				rootConstants.registerSpace = _registerSpace;
			}
		};
		struct RootDescriptor
		{
			uint32 shaderRegister;
			uint32 registerSpace;
		};
		struct RootDescriptorBuilder : public RootDescriptor
		{
			RootDescriptorBuilder()
			{
			}
			explicit RootDescriptorBuilder(const RootDescriptorBuilder&)
			{
			}
			RootDescriptorBuilder(
				uint32 _shaderRegister,
				uint32 _registerSpace = 0)
			{
				initialize(_shaderRegister, _registerSpace);
			}
			inline void initialize(
				uint32 _shaderRegister,
				uint32 _registerSpace = 0)
			{
				initialize(*this, _shaderRegister, _registerSpace);
			}
			static inline void initialize(RootDescriptor& table, uint32 _shaderRegister, uint32 _registerSpace = 0)
			{
				table.shaderRegister = _shaderRegister;
				table.registerSpace = _registerSpace;
			}
		};
		enum class ShaderVisibility
		{
			ALL      = 0,
			VERTEX   = 1,
			HULL     = 2,
			DOMAIN   = 3,
			GEOMETRY = 4,
			PIXEL    = 5
		};
		struct RootParameter
		{
			RootParameterType		parameterType;
			union
			{
				RootDescriptorTable	descriptorTable;
				RootConstants		constants;
				RootDescriptor		descriptor;
			};
			ShaderVisibility		shaderVisibility;
		};
		struct RootParameterBuilder : public RootParameter
		{
			RootParameterBuilder()
			{
			}
			explicit RootParameterBuilder(const RootParameterBuilder&)
			{
			}
			static inline void initializeAsDescriptorTable(
				RootParameter& rootParam,
				uint32 numberOfDescriptorRanges,
				const DescriptorRange* descriptorRanges,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				rootParam.parameterType = RootParameterType::DESCRIPTOR_TABLE;
				rootParam.shaderVisibility = visibility;
				RootDescriptorTableBuilder::initialize(rootParam.descriptorTable, numberOfDescriptorRanges, descriptorRanges);
			}
			static inline void initializeAsConstants(
				RootParameter& rootParam,
				uint32 numberOf32BitValues,
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				rootParam.parameterType = RootParameterType::CONSTANTS_32BIT;
				rootParam.shaderVisibility = visibility;
				RootConstantsBuilder::initialize(rootParam.constants, numberOf32BitValues, shaderRegister, registerSpace);
			}
			static inline void initializeAsConstantBufferView(
				RootParameter& rootParam,
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				rootParam.parameterType = RootParameterType::CBV;
				rootParam.shaderVisibility = visibility;
				RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
			}
			static inline void initializeAsShaderResourceView(
				RootParameter& rootParam,
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				rootParam.parameterType = RootParameterType::SRV;
				rootParam.shaderVisibility = visibility;
				RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
			}
			static inline void initializeAsUnorderedAccessView(
				RootParameter& rootParam,
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				rootParam.parameterType = RootParameterType::UAV;
				rootParam.shaderVisibility = visibility;
				RootDescriptorBuilder::initialize(rootParam.descriptor, shaderRegister, registerSpace);
			}
			inline void initializeAsDescriptorTable(
				uint32 numberOfDescriptorRanges,
				const DescriptorRange* descriptorRanges,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				initializeAsDescriptorTable(*this, numberOfDescriptorRanges, descriptorRanges, visibility);
			}
			inline void initializeAsConstants(
				uint32 numberOf32BitValues,
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				initializeAsConstants(*this, numberOf32BitValues, shaderRegister, registerSpace, visibility);
			}
			inline void initializeAsConstantBufferView(
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				initializeAsConstantBufferView(*this, shaderRegister, registerSpace, visibility);
			}
			inline void initializeAsShaderResourceView(
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				initializeAsShaderResourceView(*this, shaderRegister, registerSpace, visibility);
			}
			inline void initializeAsUnorderedAccessView(
				uint32 shaderRegister,
				uint32 registerSpace = 0,
				ShaderVisibility visibility = ShaderVisibility::ALL)
			{
				initializeAsUnorderedAccessView(*this, shaderRegister, registerSpace, visibility);
			}
		};
		struct RootSignatureFlags
		{
			enum Enum
			{
				NONE                               = 0,
				ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = 0x1,
				DENY_VERTEX_SHADER_ROOT_ACCESS     = 0x2,
				DENY_HULL_SHADER_ROOT_ACCESS       = 0x4,
				DENY_DOMAIN_SHADER_ROOT_ACCESS     = 0x8,
				DENY_GEOMETRY_SHADER_ROOT_ACCESS   = 0x10,
				DENY_PIXEL_SHADER_ROOT_ACCESS      = 0x20,
				ALLOW_STREAM_OUTPUT                = 0x40
			};
		};
		enum class StaticBorderColor
		{
			TRANSPARENT_BLACK = 0,
			OPAQUE_BLACK      = TRANSPARENT_BLACK + 1,
			OPAQUE_WHITE      = OPAQUE_BLACK + 1
		};
		struct StaticSampler
		{
			FilterMode			filter;
			TextureAddressMode	addressU;
			TextureAddressMode	addressV;
			TextureAddressMode	addressW;
			float				mipLodBias;
			uint32				maxAnisotropy;
			ComparisonFunc		comparisonFunc;
			StaticBorderColor	borderColor;
			float				minLod;
			float				maxLod;
			uint32				shaderRegister;
			uint32				registerSpace;
			ShaderVisibility	shaderVisibility;
		};
		struct RootSignature
		{
			uint32					 numberOfParameters;
			const RootParameter*	 parameters;
			uint32					 numberOfStaticSamplers;
			const StaticSampler*	 staticSamplers;
			RootSignatureFlags::Enum flags;
		};
		struct RootSignatureBuilder : public RootSignature
		{
			RootSignatureBuilder()
			{
			}
			explicit RootSignatureBuilder(const RootSignatureBuilder&)
			{
			}
			RootSignatureBuilder(
				uint32 _numberOfParameters,
				const RootParameter* _parameters,
				uint32 _numberOfStaticSamplers = 0,
				const StaticSampler* _staticSamplers = nullptr,
				RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
			{
				initialize(_numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
			}
			inline void initialize(
				uint32 _numberOfParameters,
				const RootParameter* _parameters,
				uint32 _numberOfStaticSamplers = 0,
				const StaticSampler* _staticSamplers = nullptr,
				RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
			{
				initialize(*this, _numberOfParameters, _parameters, _numberOfStaticSamplers, _staticSamplers, _flags);
			}
			static inline void initialize(
				RootSignature& rootSignature,
				uint32 _numberOfParameters,
				const RootParameter* _parameters,
				uint32 _numberOfStaticSamplers = 0,
				const StaticSampler* _staticSamplers = nullptr,
				RootSignatureFlags::Enum _flags = RootSignatureFlags::NONE)
			{
				rootSignature.numberOfParameters = _numberOfParameters;
				rootSignature.parameters = _parameters;
				rootSignature.numberOfStaticSamplers = _numberOfStaticSamplers;
				rootSignature.staticSamplers = _staticSamplers;
				rootSignature.flags = _flags;
			}

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // Renderer
} // qsf
