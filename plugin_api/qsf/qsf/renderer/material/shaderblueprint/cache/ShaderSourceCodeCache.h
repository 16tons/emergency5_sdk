// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderBlueprint.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderSourceCodeCacheId;	///< Shader source code cache identifier, internally just a POD "uint32", result of hashing the build shader source code


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader source code cache
	*
	*  @see
	*    - "qsf::ShaderSourceCodeCacheManager"
	*/
	class ShaderSourceCodeCache : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderSourceCodeCacheManager;	///< Is creating and managing shader source code cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit ShaderSourceCodeCache(ShaderSourceCodeCacheId shaderSourceCodeCacheId);	// TODO(co) Should be private
		inline ~ShaderSourceCodeCache();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the shader source code cache ID
		*
		*  @return
		*    The shader source code cache ID
		*/
		inline ShaderSourceCodeCacheId getShaderSourceCodeCacheId() const;

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
		inline ShaderBlueprint::Type getShaderBlueprintType() const;

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
		*    Return the build shader source code
		*
		*  @return
		*    The build shader ASCII source code
		*/
		inline const std::string& getBuildSourceCode() const;

		/**
		*  @brief
		*    Return the relative debug filename
		*
		*  @return
		*    The relative debug ASCII filename
		*/
		inline const std::string& getDebugFilename() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		ShaderSourceCodeCache(const ShaderSourceCodeCache&) = delete;
		ShaderSourceCodeCache& operator=(const ShaderSourceCodeCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderSourceCodeCacheId mShaderSourceCodeCacheId;	///< The shader source code cache ID
		ShaderBlueprintId		mShaderBlueprintId;			///< Shader blueprint ID
		ShaderBlueprint::Type	mShaderBlueprintType;		///< Shader blueprint type
		std::string				mHlslProfile;				///< HLSL profile
		std::string				mBuildSourceCode;			///< Build shader ASCII source code
		std::string				mDebugFilename;				///< Relative ASCII debug filename


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderSourceCodeCache.inl"
