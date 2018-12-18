// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// TODO(co) Complete the "qsf::ShaderBlueprint" implementation


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderSourceCodeBlueprint;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderBlueprintId;	///< The shader blueprint ID is a result of "qsf::StringHash(<shader blueprint name>)"
	typedef StringHash ShaderPiecesId;		///< The shader pieces ID is a result of "qsf::StringHash(<shader pieces name>)"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader blueprint
	*/
	class ShaderBlueprint : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderBlueprintManager;		///< The shader blueprint manager constructs shader blueprints and hence needs access to some internals we better don't make public
		friend class JsonShaderBlueprintSerializer;	///< Fills the data


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Shader blueprint type
		*/
		enum Type
		{
			VERTEX = 0,	///< Vertex shader
			PIXEL,		///< Pixel shader
			GEOMETRY,	///< Geometry shader
			HULL,		///< Hull shader
			DOMAIN,		///< Domain shader
			COMPUTE,	///< Compute shader
			UNKNOWN		///< Unknown shader type
		};

		typedef std::vector<ShaderPiecesId> ShaderPiecesIds;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] shaderBlueprintId
		*    The shader blueprint ID
		*/
		explicit ShaderBlueprint(ShaderBlueprintId shaderBlueprintId);	// TODO(co) Should be private

		/**
		*  @brief
		*    Destructor
		*/
		~ShaderBlueprint();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the shader blueprint ID
		*
		*  @return
		*    The shader blueprint ID
		*/
		inline ShaderBlueprintId getShaderBlueprintId() const;

		/**
		*  @brief
		*    Return the shader blueprint type
		*
		*  @return
		*    The shader blueprint type
		*/
		inline Type getType() const;

		/**
		*  @brief
		*    Return the HLSL profile
		*
		*  @return
		*    The HLSL profile
		*/
		inline const std::string& getHlslProfile() const;

		/**
		*  @brief
		*    Return the shader source code blueprint
		*
		*  @return
		*    The shader source code blueprint, can be a null pointer, do not destroy the instance
		*/
		inline ShaderSourceCodeBlueprint* getShaderSourceCodeBlueprint() const;

		/**
		*  @brief
		*    Return the referenced shader pieces
		*
		*  @return
		*    The referenced shader pieces
		*/
		inline const ShaderPiecesIds& getReferencedShaderPiecesIds() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderBlueprintId			mShaderBlueprintId;			///< Shader blueprint ID
		Type						mType;						///< Shader blueprint type
		std::string					mHlslProfile;				///< HLSL profile
		ShaderSourceCodeBlueprint*	mShaderSourceCodeBlueprint;	///< Shader source code blueprint instance, can be a null pointer, do not destroy the instance
		ShaderPiecesIds				mReferencedShaderPiecesIds;	///< Referenced shader pieces IDs


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderBlueprint-inl.h"
