// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/StringHash.h"
#include "qsf/renderer/material/materialblueprint/detail/SamplerStateTypes.h"
#include "qsf/renderer/material/materialblueprint/detail/PipelineStateTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint64 GlobalAssetId;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material property value
	*
	*  @remarks
	*    Special property value types
	*    - Reference value types to have properties referencing other data
	*    - Declaration only property for value types were we don't need to store a material property value, but only need to know the value type
	*      (examples are float 3x3 and float 4x4 which would blow up the number of bytes required per material property value without a real usage)
	*/
	class QSF_API_EXPORT MaterialPropertyValue
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialProperty;	// Needs access to the constructor for the reference usage


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Value type
		*/
		enum class ValueType : uint8
		{
			UNKNOWN = 0,						///< Value type not known
			BOOLEAN,							///< Boolean value
			INTEGER,							///< Integer value
			INTEGER_2,							///< Integer value with two components
			INTEGER_3,							///< Integer value with three components
			INTEGER_4,							///< Integer value with four components
			FLOAT,								///< Float value
			FLOAT_2,							///< Float value with two components
			FLOAT_3,							///< Float value with three components
			FLOAT_4,							///< Float value with four components
			FLOAT_3_3,							///< Float value with nine components, declaration property only
			FLOAT_4_4,							///< Float value with sixteen components, declaration property only
			// For pipeline rasterizer state property usage
			FILL_MODE,							///< Pipeline rasterizer state fill mode with possible values: "WIREFRAME", "SOLID"
			CULL_MODE,							///< Pipeline rasterizer state cull mode with possible values: "NONE", "FRONT", "BACK"
			CONSERVATIVE_RASTERIZATION_MODE,	///< Pipeline rasterizer state conservative rasterization mode with possible values: "OFF", "ON"
			// For pipeline depth stencil state property usage
			DEPTH_WRITE_MASK,					///< Pipeline depth stencil state depth write mask with possible values: "ZERO", "ALL"
			STENCIL_OP,							///< Pipeline depth stencil state stencil function with possible values: "KEEP", "ZERO", "REPLACE", "INCR_SAT", "DECR_SAT", "INVERT", "INCREASE", "DECREASE"
			// For pipeline depth stencil state and sampler state property usage
			COMPARISON_FUNC,					///< Pipeline depth stencil state and sampler state comparison function with possible values: "NEVER", "LESS", "EQUAL", "LESS_EQUAL", "GREATER", "NOT_EQUAL", "GREATER_EQUAL", "ALWAYS"
			// For pipeline blend state property usage
			RENDER_TARGET_WRITE_MASK,			///< Pipeline blend state render target write mask, for example "RED | GREEN | BLUE | ALPHA"
			BLEND,								///< Pipeline blend state blend with possible values: "ZERO", "ONE", "SRC_COLOR", "INV_SRC_COLOR", "SRC_ALPHA", "INV_SRC_ALPHA", "DEST_ALPHA", "INV_DEST_ALPHA", "DEST_COLOR", "INV_DEST_COLOR", "SRC_ALPHA_SAT", "BLEND_FACTOR", "INV_BLEND_FACTOR", "SRC_1_COLOR", "INV_SRC_1_COLOR", "SRC_1_ALPHA", "INV_SRC_1_ALPHA"
			BLEND_OP,							///< Pipeline blend state blend operation with possible values: "ADD", "SUBTRACT", "REV_SUBTRACT", "MIN", "MAX"
			// For sampler state property usage
			FILTER_MODE,						///< Sampler state filter mode with possible values: "MIN_MAG_MIP_POINT", "MIN_MAG_POINT_MIP_LINEAR", "MIN_POINT_MAG_LINEAR_MIP_POINT", "MIN_POINT_MAG_MIP_LINEAR", "MIN_LINEAR_MAG_MIP_POINT", "MIN_LINEAR_MAG_POINT_MIP_LINEAR", "MIN_MAG_LINEAR_MIP_POINT", "MIN_MAG_MIP_LINEAR", "ANISOTROPIC", "COMPARISON_MIN_MAG_MIP_POINT", "COMPARISON_MIN_MAG_POINT_MIP_LINEAR", "COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT", "COMPARISON_MIN_POINT_MAG_MIP_LINEAR", "COMPARISON_MIN_LINEAR_MAG_MIP_POINT", "COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR", "COMPARISON_MIN_MAG_LINEAR_MIP_POINT", "COMPARISON_MIN_MAG_MIP_LINEAR", "COMPARISON_ANISOTROPIC"
			TEXTURE_ADDRESS_MODE,				///< Sampler state texture address mode with possible values: "WRAP", "MIRROR", "CLAMP", "BORDER", "MIRROR_ONCE"
			// For texture property usage
			GLOBAL_ASSET_ID,					///< Asset ID
			COMPOSITOR_TEXTURE_REFERENCE,		///< Compositor name and MRT-index ("Multi Render Target"), value as string example: "@<texture name>@<MRT-index>"
			RESOURCE_NAME						///< Resource name, don't use this inefficient thing if you don't have to
		};

		struct CompositorTextureReference
		{
			uint32 compositorTextureId;	///< For instance "Ogre::IdString("global_gbuffer")" for addressing the GBuffer MRT texture, we need to use the OGRE string hash because this ID is used with "Ogre::SceneManager::getCompositorTextures()"
			uint32 mrtIndex;			///< For instance 0 to index the GBuffer MRT texture containing diffuse information
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static uint32 getValueTypeNumberOfBytes(ValueType valueType);
		static MaterialPropertyValue fromString(ValueType valueType, const std::string& valueAsString);	// Throws an exception in case of an error
		inline static MaterialPropertyValue fromUnknown();
		inline static MaterialPropertyValue fromBoolean(bool value);
		inline static MaterialPropertyValue fromInteger(int value);
		inline static MaterialPropertyValue fromInteger2(int value0, int value1);
		inline static MaterialPropertyValue fromInteger3(int value0, int value1, int value2);
		inline static MaterialPropertyValue fromInteger4(int value0, int value1, int value2, int value3);
		inline static MaterialPropertyValue fromFloat(float value);
		inline static MaterialPropertyValue fromFloat2(float value0, float value1);
		inline static MaterialPropertyValue fromFloat3(float value0, float value1, float value2);
		inline static MaterialPropertyValue fromFloat4(float value0, float value1, float value2, float value3);
		inline static MaterialPropertyValue fromFloat3_3();	// Declaration property only
		inline static MaterialPropertyValue fromFloat4_4();	// Declaration property only
		// For pipeline rasterizer state property usage
		inline static MaterialPropertyValue fromFillMode(Renderer::FillMode value);
		inline static MaterialPropertyValue fromCullMode(Renderer::CullMode value);
		inline static MaterialPropertyValue fromConservativeRasterizationMode(Renderer::ConservativeRasterizationMode value);
		// For pipeline depth stencil state property usage
		inline static MaterialPropertyValue fromDepthWriteMask(Renderer::DepthWriteMask value);
		inline static MaterialPropertyValue fromStencilOp(Renderer::StencilOp value);
		// For pipeline depth stencil state and sampler state property usage
		inline static MaterialPropertyValue fromComparisonFunc(Renderer::ComparisonFunc value);
		// // For pipeline blend state property usage
		inline static MaterialPropertyValue fromRenderTargetWriteMask(Renderer::RenderTargetWriteMask value);
		inline static MaterialPropertyValue fromBlend(Renderer::Blend value);
		inline static MaterialPropertyValue fromBlendOp(Renderer::BlendOp value);
		// For sampler state property usage
		inline static MaterialPropertyValue fromFilterMode(Renderer::FilterMode value);
		inline static MaterialPropertyValue fromTextureAddressMode(Renderer::TextureAddressMode value);
		// For texture property usage
		inline static MaterialPropertyValue fromGlobalAssetId(GlobalAssetId value);
		inline static MaterialPropertyValue fromCompositorTextureReference(const CompositorTextureReference& value);
		static MaterialPropertyValue fromResourceName(const std::string& resourceName);	// Don't use this inefficient thing if you don't have to

		// Value to string conversion methods, throws an exception in case of an error
		static std::string getValueTypeAsString(ValueType valueType);
		static std::string getFillModeAsString(Renderer::FillMode fillMode);
		static std::string getCullModeAsString(Renderer::CullMode cullMode);
		static std::string getConservativeRasterizationModeAsString(Renderer::ConservativeRasterizationMode conservativeRasterizationMode);
		static std::string getDepthWriteMaskAsString(Renderer::DepthWriteMask depthWriteMask);
		static std::string getStencilOpAsString(Renderer::StencilOp stencilOp);
		static std::string getComparisonFuncAsString(Renderer::ComparisonFunc comparisonFunc);
		static std::string getRenderTargetWriteMaskAsString(Renderer::RenderTargetWriteMask renderTargetWriteMask);
		static std::string getBlendAsString(Renderer::Blend blend);
		static std::string getBlendOpModeAsString(Renderer::BlendOp blendOp);
		static std::string getFilterModeAsString(Renderer::FilterMode filterMode);
		static std::string getTextureAddressModeAsString(Renderer::TextureAddressMode textureAddressMode);

		// String to value conversion methods, throws an exception in case of an error
		static ValueType getValueTypeFromString(const std::string& valueAsString);
		static Renderer::FillMode getFillModeFromString(const std::string& valueAsString);
		static Renderer::CullMode getCullModeFromString(const std::string& valueAsString);
		static Renderer::ConservativeRasterizationMode getConservativeRasterizationModeFromString(const std::string& valueAsString);
		static Renderer::DepthWriteMask getDepthWriteMaskFromString(const std::string& valueAsString);
		static Renderer::StencilOp getStencilOpFromString(const std::string& valueAsString);
		static Renderer::ComparisonFunc getComparisonFuncFromString(const std::string& valueAsString);
		static Renderer::RenderTargetWriteMask getRenderTargetWriteMaskFromString(const std::string& valueAsString);
		static Renderer::Blend getBlendFromString(const std::string& valueAsString);
		static Renderer::BlendOp getBlendOpFromString(const std::string& valueAsString);
		static Renderer::FilterMode getFilterModeFromString(const std::string& valueAsString);
		static Renderer::TextureAddressMode getTextureAddressModeFromString(const std::string& valueAsString);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialPropertyValue();

		/**
		*  @brief
		*    Return the value type
		*
		*  @return
		*    The value type
		*/
		inline ValueType getValueType() const;

		//[-------------------------------------------------------]
		//[ Value getter                                          ]
		//[-------------------------------------------------------]
		inline const uint8* getData() const;
		inline bool getBooleanValue() const;
		inline int getIntegerValue() const;
		inline const int* getInteger2Value() const;
		inline const int* getInteger3Value() const;
		inline const int* getInteger4Value() const;
		inline float getFloatValue() const;
		inline const float* getFloat2Value() const;
		inline const float* getFloat3Value() const;
		inline const float* getFloat4Value() const;
		// inline const float* getFloat3_3Value() const;	// Declaration property only
		// inline const float* getFloat4_4Value() const;	// Declaration property only
		// For pipeline rasterizer state property usage
		inline Renderer::FillMode getFillModeValue() const;
		inline Renderer::CullMode getCullModeValue() const;
		inline Renderer::ConservativeRasterizationMode getConservativeRasterizationModeValue() const;
		// For pipeline depth stencil state property usage
		inline Renderer::DepthWriteMask getDepthWriteMaskValue() const;
		inline Renderer::StencilOp getStencilOpValue() const;
		// For pipeline depth stencil state and sampler state property usage
		inline Renderer::ComparisonFunc getComparisonFuncValue() const;
		// // For pipeline blend state property usage
		inline Renderer::RenderTargetWriteMask getRenderTargetWriteMaskValue() const;
		inline Renderer::Blend getBlendValue() const;
		inline Renderer::BlendOp getBlendOpValue() const;
		// For sampler state property usage
		inline Renderer::FilterMode getFilterMode() const;
		inline Renderer::TextureAddressMode getTextureAddressModeValue() const;
		// For texture property usage
		inline GlobalAssetId getGlobalAssetIdValue() const;
		inline const CompositorTextureReference& getCompositorTextureReference() const;
		const std::string& getResourceNameValue() const;	// Don't use this inefficient thing if you don't have to

		/**
		*  @brief
		*    Return the material property value as string
		*
		*  @return
		*    The property material value as string
		*
		*  @throw
		*   - Throws an exception in case of an error
		*
		*  @note
		*    - Don't use this inefficient method in performance critical use-cases
		*/
		std::string getValueAsString() const;

		//[-------------------------------------------------------]
		//[ Comparison operator                                   ]
		//[-------------------------------------------------------]
		bool operator ==(const MaterialPropertyValue& materialPropertyValue) const;
		inline bool operator !=(const MaterialPropertyValue& materialPropertyValue) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ValueType mValueType;

		/**
		*  @brief
		*    Value, depends on "qsf::MaterialProperty::ValueType"
		*/
		union Value
		{
			bool									Boolean;
			int										Integer;
			int										Integer2[2];
			int										Integer3[3];
			int										Integer4[4];
			float									Float;
			float									Float2[2];
			float									Float3[3];
			float									Float4[4];
			// float								Float3_3[9];	// Declaration property only
			// float								Float4_4[16];	// Declaration property only
			// For pipeline rasterizer state property usage
			Renderer::FillMode						FillMode;
			Renderer::CullMode						CullMode;
			Renderer::ConservativeRasterizationMode	ConservativeRasterizationMode;
			// For pipeline depth stencil state property usage
			Renderer::DepthWriteMask				DepthWriteMask;
			Renderer::StencilOp						StencilOp;
			// For pipeline depth stencil state and sampler state property usage
			Renderer::ComparisonFunc				ComparisonFunc;
			// // For pipeline blend state property usage
			Renderer::RenderTargetWriteMask			RenderTargetWriteMask;
			Renderer::Blend							Blend;
			Renderer::BlendOp						BlendOp;
			// For sampler state property usage
			Renderer::FilterMode					FilterMode;
			Renderer::TextureAddressMode			TextureAddressMode;
			// For texture property usage
			GlobalAssetId							GlobalAssetId;
			CompositorTextureReference				CompositorTextureReference;
			uint32									ResourceNameHash;
		} mValue;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline MaterialPropertyValue();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialPropertyValue-inl.h"
