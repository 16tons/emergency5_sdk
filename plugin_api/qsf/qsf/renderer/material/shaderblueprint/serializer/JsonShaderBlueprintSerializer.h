// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/forward/BoostPTree.h"
#include "qsf/renderer/material/shaderblueprint/serializer/ShaderBlueprintSerializer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderBlueprint;
	class ShaderSourceCodeBlueprintManager;
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
	*    JSON shader blueprint serializer
	*/
	class QSF_API_EXPORT JsonShaderBlueprintSerializer : public ShaderBlueprintSerializer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string SHADER_BLUEPRINT_FILE_EXTENSION;
		static const std::string SHADER_BLUEPRINT_FORMAT_TYPE;
		static const uint32		 SHADER_BLUEPRINT_FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load shader blueprints
		*
		*  @param[in] shaderBlueprintManager
		*    Shader blueprint manager to use
		*  @param[in] absoluteDirectory
		*    Absolute blueprint directory
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		static void loadShaderBlueprints(ShaderBlueprintManager& shaderBlueprintManager, const std::string& absoluteDirectory);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline JsonShaderBlueprintSerializer();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~JsonShaderBlueprintSerializer();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ShaderBlueprintSerializer methods ]
	//[-------------------------------------------------------]
	public:
		virtual void deserialize(ShaderBlueprintManager& shaderBlueprintManager, const std::string& absoluteFilename, std::istream& istream) const override;
		virtual void serialize(const ShaderBlueprintManager& shaderBlueprintManager, std::ostream& ostream) const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Load shader blueprint from a given Boost property tree
		*
		*  @param[in] shaderBlueprint
		*    Shader blueprint to load
		*  @param[in] shaderSourceCodeBlueprintManager
		*    Shader source code blueprint manager
		*  @param[in] absoluteDirectory
		*    Absolute blueprint directory
		*  @param[in] propertyTree
		*    Boost property tree to load from
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		void loadShaderBlueprintFromPTree(ShaderBlueprint& shaderBlueprint, ShaderSourceCodeBlueprintManager& shaderSourceCodeBlueprintManager, const std::string& absoluteDirectory, const boost::property_tree::ptree& propertyTree) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/serializer/JsonShaderBlueprintSerializer-inl.h"
