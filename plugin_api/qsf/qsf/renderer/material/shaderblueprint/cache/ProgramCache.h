// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <OGRE/OgreCommon.h>	// For "Ogre::NumShaderTypes"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderCache;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef uint32 ProgramCacheId;	///< Program cache identifier, internally just a POD "uint32", result of hashing the shader cache IDs of the referenced shaders


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Program cache
	*/
	class ProgramCache : private boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ProgramCacheManager;	///< Is creating and managing program cache instances


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline explicit ProgramCache(ProgramCacheId programCacheId);	// TODO(co) Should be private
		inline ~ProgramCache();	// TODO(co) Should be private

		/**
		*  @brief
		*    Return the program cache ID
		*
		*  @return
		*    The program cache ID
		*/
		inline ProgramCacheId getProgramCacheId() const;

		/**
		*  @brief
		*    Return a shader cache
		*
		*  @param[in] shaderType
		*    Shader type to request the shader cache for
		*
		*  @return
		*    The requested shader cache, can be a null pointer, do not destroy the instance
		*/
		inline const ShaderCache* getShaderCache(Ogre::ShaderType shaderType) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		ProgramCache(const ProgramCache&) = delete;
		ProgramCache& operator=(const ProgramCache&) = delete;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ProgramCacheId mProgramCacheId;
		ShaderCache*   mShaderCaches[Ogre::NumShaderTypes];


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/cache/ProgramCache-inl.h"
