// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/renderer/material/shaderblueprint/cache/ShaderProperties.h"

#include <OGRE/OgreCommon.h>	// For "Ogre::NumShaderTypes"

#include <boost/noncopyable.hpp>

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class MaterialBlueprint;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef std::map<StringHash, std::string> DynamicShaderPieces;		// TODO(co) Unordered map might perform better
	typedef StringHash						  MaterialBlueprintId;		///< The material blueprint ID is a result of "qsf::StringHash(<material blueprint name>)"
	typedef uint32							  PipelineStateSignatureId;	///< Pipeline state signature identifier, internally just a POD "uint32", result of hashing the referenced shaders as well as other pipeline state properties
	typedef uint32							  ShaderCombinationId;		///< Shader combination identifier, internally just a POD "uint32", result of hashing the shader combination generating shader blueprint, shader properties and dynamic shader pieces


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Pipeline state signature
	*
	*  @see
	*    - See "qsf::PipelineStateCacheManager" for additional information
	*/
	class QSF_API_EXPORT PipelineStateSignature : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PipelineStateCacheManager;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PipelineStateSignature();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialBlueprint
		*    Material blueprint to use
		*  @param[in] shaderProperties
		*    Shader properties to use
		*  @param[in] dynamicShaderPieces
		*    Dynamic via C++ generated shader pieces to use
		*  @param[in] ogreHash
		*    OGRE hash for usage with "qsf::MaterialBlueprint::getShaderCache()"/"qsf::MaterialBlueprint::addShaderCache()" in order to get an "Ogre::HlmsCache"-instance
		*/
		PipelineStateSignature(const MaterialBlueprint& materialBlueprint, const ShaderProperties& shaderProperties, const DynamicShaderPieces dynamicShaderPieces[Ogre::NumShaderTypes], uint32 ogreHash);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] pipelineStateSignature
		*    Pipeline state signature to copy from
		*/
		PipelineStateSignature(const PipelineStateSignature& pipelineStateSignature);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~PipelineStateSignature();

		PipelineStateSignature& operator=(const PipelineStateSignature& pipelineStateSignature);

		/**
		*  @brief
		*    Serialize the pipeline state signature
		*/
		void serializeCache(BinarySerializer& binarySerializer);

		//[-------------------------------------------------------]
		//[ Getter for input data                                 ]
		//[-------------------------------------------------------]
		inline MaterialBlueprintId getMaterialBlueprintId() const;
		inline const ShaderProperties& getShaderProperties() const;
		inline const DynamicShaderPieces& getDynamicShaderPieces(Ogre::ShaderType shaderType) const;
		inline uint32 getOgreHash() const;

		//[-------------------------------------------------------]
		//[ Getter for derived data                               ]
		//[-------------------------------------------------------]
		inline PipelineStateSignatureId getPipelineStateSignatureId() const;
		inline ShaderCombinationId getShaderCombinationId(Ogre::ShaderType shaderType) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input data
		MaterialBlueprintId mMaterialBlueprintId;
		ShaderProperties	mShaderProperties;
		DynamicShaderPieces mDynamicShaderPieces[Ogre::NumShaderTypes];
		uint32				mOgreHash;									///< OGRE hash for usage with "qsf::MaterialBlueprint::getShaderCache()"/"qsf::MaterialBlueprint::addShaderCache()" in order to get an "Ogre::HlmsCache"-instance
		// Derived data
		PipelineStateSignatureId mPipelineStateSignatureId;
		ShaderCombinationId		 mShaderCombinationId[Ogre::NumShaderTypes];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/cache/PipelineStateSignature-inl.h"
