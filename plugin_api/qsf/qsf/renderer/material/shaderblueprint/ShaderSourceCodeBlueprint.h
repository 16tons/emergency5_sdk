// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"

#include <boost/noncopyable.hpp>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderPropertyId;			///< Shader property identifier, internally just a POD "uint32", result of hashing the property name
	typedef StringHash ShaderSourceCodeBlueprintId;	///< The shader source code blueprint ID is a result of "qsf::StringHash(<shader source code blueprint name>)"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader source code blueprint
	*
	*  @note
	*    - One shader source code blueprint can be shared by multiple shader blueprints
	*/
	class ShaderSourceCodeBlueprint : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprint;					///< Needs to be able to set the properties after creation
		friend class ShaderBlueprintManager;			///< Needs to be able to call "qsf::ShaderSourceCodeBlueprint::loadSourceCode()"
		friend class MaterialSystemCacheManager;		///< Needs to be able to call "qsf::ShaderSourceCodeBlueprint::loadSourceCode()"
		friend class ShaderSourceCodeBlueprintManager;	///< The shader source code blueprint manager constructs shader source code blueprints and hence needs access to some internals we better don't make public
		friend class JsonShaderBlueprintSerializer;		///< Fills the data


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::unordered_set<uint32> ReferencedShaderProperties;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] shaderSourceCodeBlueprintId
		*    The shader source code blueprint ID
		*/
		explicit ShaderSourceCodeBlueprint(ShaderSourceCodeBlueprintId shaderSourceCodeBlueprintId);	// TODO(co) Should be private

		/**
		*  @brief
		*    Destructor
		*/
		~ShaderSourceCodeBlueprint();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the shader source code blueprint ID
		*
		*  @return
		*    The shader source code blueprint ID
		*/
		inline ShaderSourceCodeBlueprintId getShaderSourceCodeBlueprintId() const;

		/**
		*  @brief
		*    Return the shader source code blueprint filename as read from the shader blueprint
		*
		*  @return
		*    The shader source code blueprint filename as read from the shader blueprint
		*/
		inline const std::string& getSourceCodeFilename() const;

		/**
		*  @brief
		*    Return the shader source code blueprint
		*
		*  @return
		*    The shader ASCII source code blueprint
		*/
		inline const std::string& getSourceCode() const;

		/**
		*  @brief
		*    Return the shader source code blueprint hash
		*
		*  @return
		*    The shader source code blueprint hash
		*/
		inline uint32 getSourceCodeHash() const;

		/**
		*  @brief
		*    Calculate shader source code blueprint hash from file
		*
		*  @return
		*    The calculated shader source code blueprint hash from file
		*/
		uint32 calculateSourceCodeHashFromFile() const;

		/**
		*  @brief
		*    Return whether or not the given shader property ID is known to the shader source code blueprint
		*
		*  @param[in] shaderPropertyId
		*    Shader property ID to check for
		*
		*  @return
		*    "true" if the given shader property ID is known to the shader source code blueprint, else "false"
		*/
		inline bool isShaderPropertyReferenced(ShaderPropertyId shaderPropertyId) const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Load in the ASCII shader source code
		*
		*  @throw
		*   - Throws an exception in case of an error
		*/
		void loadSourceCode();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		ShaderSourceCodeBlueprintId mShaderSourceCodeBlueprintId;	///< Shader source code blueprint ID
		std::string					mSourceCodeFilename;			///< Shader source code blueprint filename as read from the shader blueprint
		std::string					mAbsoluteSourceCodeFilename;	///< Constructed absolute shader source code blueprint filename
		std::string					mSourceCode;					///< Shader ASCII source code blueprint
		uint32						mSourceCodeHash;				///< Shader source code hash
		ReferencedShaderProperties	mReferencedShaderProperties;	///< IDs of shader properties known to the shader source code blueprint


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderSourceCodeBlueprint-inl.h"
