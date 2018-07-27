// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"

#include <OGRE/OgrePrerequisites.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash ShaderCacheId;			///< Shader cache identifier, internally just a POD "uint32", result of hashing the build shader source code (meaning identical to "qsf::ShaderSourceCodeCacheId")
	typedef StringHash ShaderSourceCodeCacheId;	///< Shader source code cache identifier, internally just a POD "uint32", result of hashing the build shader source code


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader cache
	*/
	class ShaderCache : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ShaderCacheManager;	///< Is creating and managing shader cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit ShaderCache(ShaderCacheId shaderCacheId);	// TODO(co) Should be private
		inline ~ShaderCache();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the shader cache ID
		*
		*  @return
		*    The shader cache ID
		*/
		inline ShaderCacheId getShaderCacheId() const;

		/**
		*  @brief
		*    Return the shader source cache ID
		*
		*  @return
		*    The shader source code cache ID
		*/
		inline ShaderSourceCodeCacheId getShaderSourceCodeCacheId() const;

		/**
		*  @brief
		*    Return the OGRE high level GPU program pointer
		*
		*  @return
		*    The OGRE high level GPU program pointer
		*/
		inline Ogre::HighLevelGpuProgramPtr getOgreHighLevelGpuProgramPtr() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		ShaderCache(const ShaderCache&) = delete;
		ShaderCache& operator=(const ShaderCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ShaderCacheId				 mShaderCacheId;
		ShaderSourceCodeCacheId		 mShaderSourceCodeCacheId;
		Ogre::HighLevelGpuProgramPtr mOgreHighLevelGpuProgramPtr;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ShaderCache.inl"
