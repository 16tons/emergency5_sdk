// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialPropertyValue.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash MaterialPropertyId;	///< Material property identifier, internally just a POD "uint32", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material property
	*/
	class MaterialProperty : public MaterialPropertyValue
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	public:
		friend class MaterialProperties;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Usage
		*/
		enum class Usage : uint8
		{
			UNKNOWN = 0,					///< Usage not known
			STATIC,							///< Property is considered to not change regularly and results in shader combinations
			STATIC_PASS,					///< Property is considered to only change per-pass (consider this really expensive) and results in shader combinations, for example depth only rendering or rendering with wet surfaces
			DYNAMIC,						///< Property is considered to change regularly and hence will be handled as shader constant managed in a combined constant buffer
			RASTERIZER_STATE,				///< Pipeline rasterizer state, property is considered to not change regularly
			DEPTH_STENCIL_STATE,			///< Pipeline depth stencil state, property is considered to not change regularly
			BLEND_STATE,					///< Pipeline blend state, property is considered to not change regularly
			SAMPLER_STATE,					///< Sampler state, property is considered to not change regularly	// TODO(co) Reserved for future-use
			TEXTURE_REFERENCE,				///< Property is a texture asset reference, property is considered to not change regularly
			COMPOSITOR_TEXTURE_REFERENCE,	///< Property is an automatic compositor texture reference, value type can be "INTEGER" (= by compositor quad input index) or "COMPOSITOR_TEXTURE_REFERENCE" (= by global/local compositor texture name, value syntax: "@<texture name>@<MRT-index>")
			SHADOW_TEXTURE_REFERENCE,		///< Property is an automatic shadow texture reference, value type can be "INTEGER" (= by shadow texture index)
			GLOBAL_REFERENCE,				///< Property is a global material property reference
			UNKNOWN_REFERENCE,				///< Property is an automatic unknown constant buffer property reference
			PASS_REFERENCE,					///< Property is an automatic pass constant buffer property reference
			MATERIAL_REFERENCE,				///< Property is a material constant buffer property reference
			INSTANCE_REFERENCE				///< Property is an automatic instance constant buffer property reference
		};

		// Fixed material property IDs for rasterizer state
		static const uint32 CULL_MODE_ID = 3810372182;	///< qsf::StringHash("CullMode")

		// Fixed material property IDs for depth stencil state
		static const uint32 DEPTH_ENABLE_ID			  = 3269242107;	///< qsf::StringHash("DepthEnable")
		static const uint32 DEPTH_WRITE_MASK_ID		  = 2741916219;	///< qsf::StringHash("DepthWriteMask")
		static const uint32 DEPTH_BIAS_CONSTANT_ID	  = 2118705953;	///< qsf::StringHash("DepthBiasConstant")
		static const uint32 DEPTH_BIAS_SLOPE_SCALE_ID = 657666036;	///< qsf::StringHash("DepthBiasSlopeScale")

		// Fixed material property IDs for blend state
		static const uint32 RENDER_TARGET_WRITE_MASK_ID	= 1484432449;	///< qsf::StringHash("RenderTargetWriteMask");
		static const uint32 SRC_BLEND_ID				= 165558300;	///< qsf::StringHash("SrcBlend")
		static const uint32 DEST_BLEND_ID				= 4049985944;	///< qsf::StringHash("DestBlend")


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		inline static MaterialPropertyValue materialPropertyValueFromReference(ValueType valueType, uint32 reference);

		/**
		*  @brief
		*    Return whether or not the provided material blueprint property usage is a reference to something else
		*
		*  @param[in] usage
		*    Usage to check
		*
		*  @return
		*    "true" if the provided material blueprint property usage is a reference to something else, else "false"
		*/
		inline static bool isReferenceUsage(Usage usage);

		/**
		*  @brief
		*    Get material property usage as string
		*
		*  @param[in] usage
		*    The material property usage
		*
		*  @return
		*    The material property usage as string
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static std::string getUsageAsString(Usage usage);

		/**
		*  @brief
		*    Get material property usage from string
		*
		*  @param[in] valueAsString
		*    The material property usage as string
		*
		*  @return
		*    The material property usage
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static Usage getUsageFromString(const std::string& valueAsString);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @note
		*    - For internal usage only
		*/
		inline MaterialProperty();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialPropertyId
		*    Material property ID
		*  @param[in] usage
		*    Material property usage
		*  @param[in] materialPropertyValue
		*    Material property value
		*/
		inline MaterialProperty(MaterialPropertyId materialPropertyId, Usage usage, const MaterialPropertyValue& materialPropertyValue);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~MaterialProperty();

		/**
		*  @brief
		*    Return the material property ID
		*
		*  @return
		*    The material property ID
		*/
		inline MaterialPropertyId getMaterialPropertyId() const;

		/**
		*  @brief
		*    Return the material blueprint property usage
		*
		*  @return
		*    The material blueprint property usage
		*/
		inline Usage getUsage() const;

		/**
		*  @brief
		*    Return whether or not this is an overwritten property
		*
		*  @return
		*    "true" if this is an overwritten property, else "false"
		*/
		inline bool isOverwritten() const;

		/**
		*  @brief
		*    Return whether or not the material blueprint property is a reference to something else
		*
		*  @return
		*    "true" if the material blueprint property is a reference to something else, else "false"
		*/
		inline bool isReferenceUsage() const;

		//[-------------------------------------------------------]
		//[ Value getter                                          ]
		//[-------------------------------------------------------]
		inline uint32 getReferenceValue() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialPropertyId mMaterialPropertyId;
		Usage			   mUsage;
		bool			   mOverwritten;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialProperty-inl.h"
