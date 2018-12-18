// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/forward/BoostPTree.h"
#include "qsf/renderer/material/materialblueprint/serializer/MaterialBlueprintSerializer.h"
#include "qsf/renderer/material/materialblueprint/MaterialBlueprint.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderProperties;
	class MaterialBlueprintManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    JSON material blueprint serializer
	*/
	class QSF_API_EXPORT JsonMaterialBlueprintSerializer : public MaterialBlueprintSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string MATERIAL_BLUEPRINT_FILE_EXTENSION;
		static const std::string MATERIAL_BLUEPRINT_FORMAT_TYPE;
		static const uint32		 MATERIAL_BLUEPRINT_FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load material blueprints
		*
		*  @param[in] materialBlueprintManager
		*    Material blueprint manager to use
		*  @param[in] absoluteDirectory
		*    Absolute blueprint directory
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static void loadMaterialBlueprints(MaterialBlueprintManager& materialBlueprintManager, const std::string& absoluteDirectory, const ShaderBlueprintManager& shaderBlueprintManager);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonMaterialBlueprintSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonMaterialBlueprintSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::MaterialBlueprintSerializer methods ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(MaterialBlueprint& materialBlueprint, const ShaderBlueprintManager& shaderBlueprintManager, std::istream& istream) const override;
		virtual void serialize(const MaterialBlueprint& materialBlueprint, const ShaderBlueprintManager& shaderBlueprintManager, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void loadFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& materialBlueprintTree, const ShaderBlueprintManager& shaderBlueprintManager);
		static void loadPropertiesFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& propertiesPTree, uint32 iterationNumber, const std::string& iterationCounterVariable);
		static void loadRootSignatureFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& rootSignaturePTree);
		static void loadRootSignatureParametersFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& rootParametersPTree, uint32 iterationNumber, const std::string& iterationCounterVariable);
		static void loadPipelineStateFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& pipelineStatePTree, const ShaderBlueprintManager& shaderBlueprintManager);
		static void loadRasterizerStateFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& rasterizerStatePTree);
		static void loadDepthStencilStateFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& depthStencilStatePTree);
		static void loadBlendStateFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& blendStatePTree);
		static void loadResourcesFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& resourcesPTree);
		static void loadConstantBuffersFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& constantBuffersPTree);
		static void loadConstantBufferElementsFromPTree(MaterialBlueprint& materialBlueprint, const boost::property_tree::ptree& elementPropertiesPTree, MaterialBlueprint::ConstantBufferElementProperties& constantBufferElementProperties, uint32 iterationNumber, const std::string& iterationCounterVariable);
		static void loadTextureBuffersFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& textureBuffersPTree);
		static void loadSamplerStatesFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& samplerStatesPTree);
		static void loadTexturesFromPTree(MaterialBlueprint& materialBlueprint, ShaderProperties& shaderProperties, const boost::property_tree::ptree& texturesPTree, uint32 iterationNumber, const std::string& iterationCounterVariable);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/serializer/JsonMaterialBlueprintSerializer-inl.h"
