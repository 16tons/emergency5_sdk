// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Color4.h"
#include "qsf/renderer/material/material/Material.h"
#include "qsf/renderer/material/materialblueprint/RootSignature.h"
#include "qsf/renderer/material/shaderblueprint/cache/ShaderProperties.h"

#include <OGRE/OgreHlmsCommon.h>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class RenderSystem;
	class CommandBuffer;
	struct QueuedRenderable;
	struct HlmsSamplerblock;
}
namespace qsf
{
	class ShaderBlueprint;
	class PipelineStateCacheManager;
	class PassConstantBufferManager;
	class MaterialsConstantBufferManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash						  MaterialBlueprintId;	///< The material blueprint ID is a result of "qsf::StringHash(<material blueprint name>)"
	typedef StringHash						  ShaderPropertyId;		///< Shader property identifier, internally just a POD "uint32", result of hashing the property name
	typedef std::map<StringHash, std::string> DynamicShaderPieces;	// TODO(co) Unordered map might perform better
	typedef std::vector<uint32>				  OgreHlmsCacheHashes;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material blueprint
	*
	*  @note
	*    - There are ease-of-use direct access methods for infrastructure resources
	*/
	class QSF_API_EXPORT MaterialBlueprint : public Material
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystemCacheManager;		///< Needs write access to "qsf::MaterialBlueprint::mSamplerStates"
		friend class MaterialBlueprintManager;			///< The material blueprint manager constructs material blueprints and hence needs access to some internals we better don't make public
		friend class JsonMaterialBlueprintSerializer;	///< Fills the data


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<ShaderPropertyId>					  ClearStaticProperties;
		typedef std::unordered_map<uint32, ClearStaticProperties> ClearStaticPropertiesByStaticPass;	///< Key = "qsf::ShaderPropertyId"
		typedef std::vector<ShaderPropertyId>					  MandatoryStaticCombinationShaderProperties;
		typedef std::vector<int32>								  ValidStaticIntegerValues;

		/**
		*  @brief
		*    Buffer usage
		*/
		enum class BufferUsage : uint8
		{
			UNKNOWN = 0,	///< Unknown buffer usage, supports the following "qsf::MaterialProperty::Usage": "UNKNOWN_REFERENCE", "GLOBAL_REFERENCE" as well as properties with simple values	// TODO(co) Reserved for future-use
			PASS,			///< Pass buffer usage, supports the following "qsf::MaterialProperty::Usage": "PASS_REFERENCE", "MATERIAL_REFERENCE" (material blueprint), "GLOBAL_REFERENCE" as well as properties with simple values
			MATERIALS,		///< Materials buffer usage, supports the following "qsf::MaterialProperty::Usage": "MATERIAL_REFERENCE", "GLOBAL_REFERENCE" as well as properties with simple values	// TODO(co) Reserved for future-use
			INSTANCES		///< Instances buffer usage, supports the following "qsf::MaterialProperty::Usage": "INSTANCE_REFERENCE", "GLOBAL_REFERENCE" as well as properties with simple values	// TODO(co) Reserved for future-use
		};

		typedef std::vector<MaterialProperty> ConstantBufferElementProperties;

		struct ConstantBuffer
		{
			uint32							rootParameterIndex;
			BufferUsage						bufferUsage;
			ConstantBufferElementProperties	constantBufferElementProperties;
			uint32							constantBufferNumberOfBytes;	///< Includes handling of packing rules for constant variables (see "Reference for HLSL - Shader Models vs Shader Profiles - Shader Model 4 - Packing Rules for Constant Variables" at https://msdn.microsoft.com/en-us/library/windows/desktop/bb509632%28v=vs.85%29.aspx )

			ConstantBuffer() :
				rootParameterIndex(getUninitialized<uint32>()),
				bufferUsage(BufferUsage::UNKNOWN),
				constantBufferNumberOfBytes(0)
			{
				// Nothing here
			}

		};
		typedef std::vector<ConstantBuffer> ConstantBuffers;

		struct TextureBuffer
		{
			uint32			 rootParameterIndex;
			BufferUsage		 bufferUsage;
			MaterialProperty value;

			TextureBuffer(uint32 rootParameterIndex, BufferUsage bufferUsage, const MaterialPropertyValue& materialPropertyValue) :
				rootParameterIndex(rootParameterIndex),
				bufferUsage(bufferUsage),
				value(MaterialProperty(0, getMaterialPropertyUsageFromBufferUsage(bufferUsage), materialPropertyValue))
			{
				// Nothing here
			}

		};
		typedef std::vector<TextureBuffer> TextureBuffers;

		class SamplerState
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class JsonMaterialBlueprintSerializer;	///< Fills the data


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			inline SamplerState();
			inline explicit SamplerState(const SamplerState& samplerState);
			inline ~SamplerState();
			inline const SamplerState& operator =(const SamplerState& samplerState);
			inline uint32 getRootParameterIndex() const;
			const Ogre::HlmsSamplerblock* getOgreHlmsSamplerblock() const;
			void clearOgreHlmsSamplerblock();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Given data
			uint32						 mRootParameterIndex;
			Color4						 mBorderColor;
			Renderer::TextureAddressMode mTextureAddressModeU;
			Renderer::TextureAddressMode mTextureAddressModeV;
			Renderer::TextureAddressMode mTextureAddressModeW;
			Renderer::FilterMode		 mFilterMode;
			bool						 mUseCompareFunction;	// TODO(co) Need to add "Renderer::CompareFunction" and then map to OGRE (default is mCompareFunction(NUM_COMPARE_FUNCTIONS) -> Off)

			// Derived data
			mutable const Ogre::HlmsSamplerblock* mOgreHlmsSamplerblock;	///< Can be a null pointer, we have to destroy it using "Ogre::HlmsManager::destroySamplerblock()"
			// TODO(co) For now "qsf::MaterialProperty::Usage::SAMPLER_STATE" is not supported because we currently don't need it. Reserved for future-use.


		};
		typedef std::vector<SamplerState> SamplerStates;

		/**
		*  @brief
		*    QSF renderable cache as replacement for the OGRE renderable cache
		*/
		struct RenderableCache
		{
			// Given
			ShaderProperties	shaderProperties;
			DynamicShaderPieces dynamicShaderPieces[Ogre::NumShaderTypes];
			// Derived
			bool				gpuSkinning;	// For performance reasons stored additionally, is also inside the shader properties so we don't need to take it into account for e.g. comparison
			uint32				hash;

			bool operator ==(const RenderableCache& _r) const
			{
				return (hash == _r.hash);
			}
		};
		typedef std::vector<RenderableCache> RenderableCaches;

		/**
		*  @brief
		*    QSF HLMS cache as extension for the OGRE HLMS cache
		*/
		struct HlmsCache : public Ogre::HlmsCache
		{
			MaterialBlueprintId materialBlueprintId;

			HlmsCache() :
				materialBlueprintId(getUninitialized<MaterialBlueprintId>())
			{
				// Nothing here
			};

			HlmsCache(MaterialBlueprintId _materialBlueprintId, uint32 _hash, Ogre::HlmsTypes _type) :
				Ogre::HlmsCache(_hash, _type),
				materialBlueprintId(_materialBlueprintId)
			{
				// Nothing here
			};
		};
		typedef std::vector<HlmsCache*> HlmsCaches;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static MaterialProperty::Usage getMaterialPropertyUsageFromBufferUsage(BufferUsage bufferUsage);

		// String to value conversion methods, throws an exception in case of an error
		static BufferUsage getBufferUsageFromString(const std::string& valueAsString);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprintId
		*    The material blueprint ID
		*/
		explicit MaterialBlueprint(MaterialBlueprintId materialBlueprintId);	// TODO(co) Should be private

		/**
		*  @brief
		*    Destructor
		*/
		~MaterialBlueprint();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the material blueprint ID
		*
		*  @return
		*    The material blueprint ID
		*/
		inline MaterialBlueprintId getMaterialBlueprintId() const;

		/**
		*  @brief
		*    Return the material blueprint name
		*
		*  @return
		*    The material blueprint name
		*/
		inline const std::string& getMaterialName() const;

		/**
		*  @brief
		*    Return the pipeline state cache manager instance
		*
		*  @return
		*    The pipeline state cache manager instance, do no destroy the returned instance
		*/
		inline PipelineStateCacheManager& getPipelineStateCacheManager() const;

		/**
		*  @brief
		*    Return the number of static shader combination properties
		*
		*  @return
		*    The number of static shader combination properties (material properties with usage set to "STATIC" or "STATIC_PASS", not the resulting number of shader combinations)
		*/
		uint32 getNumberOfStaticCombinationShaderProperties() const;

		/**
		*  @brief
		*    Return the mandatory static combination shader properties
		*
		*  @return
		*    The mandatory static combination shader properties
		*/
		inline const MandatoryStaticCombinationShaderProperties& getMandatoryStaticCombinationShaderProperties() const;

		/**
		*  @brief
		*    Return the valid static integer values of a requested shader property
		*
		*  @return
		*    The valid static integer values of a requested shader property
		*/
		const ValidStaticIntegerValues& getValidStaticIntegerValuesOfShaderProperty(ShaderPropertyId shaderPropertyId) const;

		/**
		*  @brief
		*    Return the number of static shader combinations
		*
		*  @return
		*    The number of static shader combinations
		*/
		uint32 getNumberOfStaticCombinations() const;

		/**
		*  @brief
		*    Return static properties to clear for a given static pass
		*
		*  @param[in] shaderPropertyId
		*   Shader property ID of the static pass to return the static properties to clear
		*
		*  @return
		*    Static properties to clear
		*
		*  @notes
		*    - For example, when rendering shadows we don't need color or emissive map and hence can reduce the number of required shader combinations by applying this knowledge
		*/
		const ClearStaticProperties& getClearStaticPropertiesByStaticPass(ShaderPropertyId shaderPropertyId) const;

		/**
		*  @brief
		*    Return the root signature
		*
		*  @return
		*    The root signature
		*/
		inline const RootSignature& getRootSignature() const;

		/**
		*  @brief
		*    Return the whether or not fill mode override is allowed
		*
		*  @return
		*    "true" if fill mode override is allowed, else "false"
		*/
		inline const bool getAllowFillModeOverride() const;

		/**
		*  @brief
		*    Return a shader blueprint
		*
		*  @param[in] ogreShaderType
		*    OGRE shader type to request the shader blueprint for
		*
		*  @return
		*    The requested shader blueprint, null pointer on error
		*/
		inline const ShaderBlueprint* getShaderBlueprint(Ogre::ShaderType ogreShaderType) const;

		/**
		*  @brief
		*    Create default pipeline state cache instances for this material blueprint
		*
		*  @see
		*    - See "qsf::MaterialBlueprintManager::createDefaultPipelineStateCaches()"
		*/
		void createDefaultPipelineStateCaches();

		/**
		*  @brief
		*    Bind the material blueprint to the renderer
		*
		*  @param[in] ogreCommandBuffer
		*    OGRE command buffer to fill with instructions
		*/
		void bindToRenderer(Ogre::CommandBuffer& ogreCommandBuffer);

		/**
		*  @brief
		*    Return the pass constant buffer manager
		*
		*  @return
		*    The pass constant buffer manager, can be a null pointer, don't destroy the instance
		*/
		inline PassConstantBufferManager* getPassConstantBufferManager() const;

		/**
		*  @brief
		*    Return the materials constant buffer manager
		*
		*  @return
		*    The materials constant buffer manager, can be a null pointer, don't destroy the instance
		*/
		inline MaterialsConstantBufferManager* getMaterialsConstantBufferManager() const;

		//[-------------------------------------------------------]
		//[ Resource                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the constant buffers
		*
		*  @return
		*    The constant buffers
		*/
		inline const ConstantBuffers& getConstantBuffers() const;

		/**
		*  @brief
		*    Return the texture buffers
		*
		*  @return
		*    The texture buffers
		*/
		inline const TextureBuffers& getTextureBuffers() const;

		/**
		*  @brief
		*    Return the sampler states
		*
		*  @return
		*    The sampler states
		*/
		inline const SamplerStates& getSamplerStates() const;

		//[-------------------------------------------------------]
		//[ Ease-of-use direct access                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the pass constant buffer
		*
		*  @return
		*    The pass constant buffer, can be a null pointer, don't destroy the instance
		*/
		inline const ConstantBuffer* getPassConstantBuffer() const;

		/**
		*  @brief
		*    Return the materials constant buffer
		*
		*  @return
		*    The materials constant buffer, can be a null pointer, don't destroy the instance
		*/
		inline const ConstantBuffer* getMaterialsConstantBuffer() const;

		/**
		*  @brief
		*    Return the instances constant buffer
		*
		*  @return
		*    The instances constant buffer, can be a null pointer, don't destroy the instance
		*/
		inline const ConstantBuffer* getInstancesConstantBuffer() const;

		/**
		*  @brief
		*    Return the instances texture buffer
		*
		*  @return
		*    The instances texture buffer, can be a null pointer, don't destroy the instance
		*/
		inline const TextureBuffer* getInstancesTextureBuffer() const;

		//[-------------------------------------------------------]
		//[ OGRE renderable cache                                 ]
		//[-------------------------------------------------------]
		inline int32 getVisualImportanceOfShaderProperty(ShaderPropertyId shaderPropertyId) const;
		void optimizeShaderProperties(ShaderProperties& shaderProperties) const;
		const RenderableCache& getRenderableCache(uint32 hash) const;
		uint32 addRenderableCache(const ShaderProperties& renderableShaderProperties, const DynamicShaderPieces* renderableDynamicShaderPieces, bool gpuSkinning);

		//[-------------------------------------------------------]
		//[ OGRE pass cache                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set current pass shader properties
		*
		*  @param[in] passShaderProperties
		*    The pass shader properties to set
		*/
		void setCurrentPassShaderProperties(const ShaderProperties& passShaderProperties);

		inline const Ogre::HlmsCache& getCurrentPassOgreHlmsCache() const;

		//[-------------------------------------------------------]
		//[ OGRE HLMS cache                                       ]
		//[-------------------------------------------------------]
		const Ogre::HlmsCache* getShaderCache(uint32 hash) const;
		const Ogre::HlmsCache* createShaderCacheEntry(uint32 renderableHash, const Ogre::HlmsCache& passCache, uint32 finalHash, const Ogre::QueuedRenderable& queuedRenderable);
		const Ogre::HlmsCache* addShaderCache(uint32 hash, Ogre::GpuProgramPtr& vertexShader, Ogre::GpuProgramPtr& geometryShader, Ogre::GpuProgramPtr& tesselationHullShader, Ogre::GpuProgramPtr& tesselationDomainShader, Ogre::GpuProgramPtr& pixelShader);
		void clearShaderCache();

		/**
		*  @brief
		*    Cleanup dirty cache parts
		*
		*  @param[in] deletedOgreHlmsCacheHashes
		*    The hashes of now invalid OGRE HLMS caches
		*/
		void cleanupDirtyCacheParts(const OgreHlmsCacheHashes& deletedOgreHlmsCacheHashes);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Set the material name
		*/
		inline void setMaterialName(const std::string& materialName);

		/**
		*  @brief
		*    Destroy all buffers
		*/
		void destroyAllBuffers();

		/**
		*  @brief
		*    Create the materials constant buffer manager
		*/
		void createMaterialsConstantBufferManager();

		/**
		*  @brief
		*    Called when a frame has ended
		*/
		void onFrameEnded();

		/**
		*  @brief
		*    Called when the render system has been changed
		*
		*  @param[in] ogreRenderSystem
		*    The new OGRE render system, can be a null pointer
		*/
		void onChangeRenderSystem(Ogre::RenderSystem* ogreRenderSystem);

		/**
		*  @brief
		*    Resolve material blueprint property value
		*
		*  @param[in] valueAsString
		*    Material blueprint property value to resolve as string, can contain "$<property name>" to reuse the value from another material blueprint property
		*
		*  @return
		*    The resolved material blueprint property value as string
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		std::string resolvePropertyValue(const std::string& valueAsString, MaterialProperty::Usage usage) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Ogre::HlmsPropertyVec>					 PassOgreHlmsCaches;
		typedef std::unordered_map<uint32, ValidStaticIntegerValues> ValidStaticIntegerValuesOfShaderProperties;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PipelineStateCacheManager*				   mPipelineStateCacheManager;					///< Pipeline state cache manager instance, always valid, destroy the instance if you no longer need it
		MandatoryStaticCombinationShaderProperties mMandatoryStaticCombinationShaderProperties;	///< See "qsf::MaterialBlueprintManager::createDefaultPipelineStateCaches()" for details
		ClearStaticPropertiesByStaticPass		   mClearStaticPropertiesByStaticPass;
		ShaderProperties						   mVisualImportanceOfShaderProperties;
		ValidStaticIntegerValuesOfShaderProperties mValidStaticIntegerValuesOfShaderProperties;

		// Name (mainly for debugging purposes)
		std::string mMaterialName;
		// Root signature
		RootSignature mRootSignature;
		// Pipeline state
		bool				   mAllowFillModeOverride;
		const ShaderBlueprint* mShaderBlueprint[Ogre::NumShaderTypes];	///< Shader blueprints, don't destroy the instances
		// Resources
		ConstantBuffers mConstantBuffers;
		TextureBuffers	mTextureBuffers;
		SamplerStates	mSamplerStates;
		// Ease-of-use direct access
		ConstantBuffer* mPassConstantBuffer;		///< Can be a null pointer, don't destroy the instance
		ConstantBuffer* mMaterialsConstantBuffer;	///< Can be a null pointer, don't destroy the instance
		ConstantBuffer* mInstancesConstantBuffer;	///< Can be a null pointer, don't destroy the instance
		TextureBuffer*  mInstancesTextureBuffer;	///< Can be a null pointer, don't destroy the instance
		// Buffer managers
		PassConstantBufferManager*		mPassConstantBufferManager;			///< Can be a null pointer, destroy the instance if no longer required
		MaterialsConstantBufferManager*	mMaterialsConstantBufferManager;	///< Can be a null pointer, destroy the instance if no longer required
		// OGRE caches
		RenderableCaches   mRenderableCaches;
		PassOgreHlmsCaches mPassOgreHlmsCaches;
		Ogre::HlmsCache    mCurrentPassOgreHlmsCache;
		HlmsCaches		   mHlmsCaches;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/MaterialBlueprint-inl.h"
