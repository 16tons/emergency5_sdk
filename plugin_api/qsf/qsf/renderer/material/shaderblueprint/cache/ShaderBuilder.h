// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderProperties.h"
#include "qsf/renderer/material/shaderblueprint/cache/preprocessor/Preprocessor.h"

#include <boost/noncopyable.hpp>

#include <map>
#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderBlueprint;
	class ShaderPiecesManager;
	class PipelineStateSignature;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::map<StringHash, std::string> DynamicShaderPieces;	// TODO(co) Unordered map might perform better


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader builder
	*
	*  @note
	*   - Heavily basing on the OGRE 2.1 HLMS shader builder which is directly part of the OGRE class "Ogre::Hlms". So for syntax, have a look into the OGRE 2.1 documentation.
	*/
	class ShaderBuilder : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] shaderPiecesManager
		*    Shader pieces manager
		*/
		explicit ShaderBuilder(const ShaderPiecesManager& shaderPiecesManager);

		/**
		*  @brief
		*    Destructor
		*/
		~ShaderBuilder();

		/**
		*  @brief
		*    Build shader source code by using the given shader blueprint and shader properties
		*
		*  @param[in] shaderBlueprint
		*    Shader blueprint to use
		*  @param[in] pipelineStateSignature
		*    Pipeline state signature to use
		*  @param[out] buildSourceCode
		*    Will receive the build shader source code
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool buildShaderSourceCode(const ShaderBlueprint& shaderBlueprint, const PipelineStateSignature& pipelineStateSignature, std::string& buildSourceCode);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		ShaderBuilder(const ShaderBuilder&) = delete;
		ShaderBuilder& operator=(const ShaderBuilder&) = delete;
		bool parseMath(const std::string& inBuffer, std::string& outBuffer);
		bool parseForEach(const std::string& inBuffer, std::string& outBuffer) const;
		bool parseProperties(std::string& inBuffer, std::string& outBuffer) const;
		bool collectPieces(const std::string& inBuffer, std::string& outBuffer);
		bool insertPieces(std::string& inBuffer, std::string& outBuffer) const;
		bool parseCounter(const std::string& inBuffer, std::string& outBuffer);
		bool parse(const std::string& inBuffer, std::string& outBuffer) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const ShaderPiecesManager& mShaderPiecesManager;
		ShaderProperties		   mShaderProperties;
		DynamicShaderPieces		   mDynamicShaderPieces;
		std::string				   mInString;				///< Optimization: To avoid constant allocations/deallocations this is a class member
		Preprocessor			   mPreprocessor;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
