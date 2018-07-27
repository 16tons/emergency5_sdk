// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager.h"
#include "qsf/renderer/material/shaderblueprint/ShaderBlueprint.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ShaderCacheManager;
	class ProgramCacheManager;
	class ShaderPiecesManager;
	class ShaderSourceCodeCacheManager;
	class ShaderSourceCodeBlueprintManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash								 ShaderSourceCodeBlueprintId;	///< The shader source code blueprint ID is a result of "qsf::StringHash(<shader source code blueprint name>)"
	typedef std::vector<ShaderPiecesId>				 ShaderPiecesIds;
	typedef std::vector<ShaderSourceCodeBlueprintId> ShaderSourceCodeBlueprintIds;


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Shader blueprint manager
	*/
	class ShaderBlueprintManager : public ElementManager<ShaderBlueprintId, ShaderBlueprint, detail::DirectAllocator<ShaderBlueprint>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;				///< The material system is the master of this class
		friend class MaterialSystemCacheManager;	///< Needs access to "qsf::ShaderBlueprintManager::mSourceCodeHash"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Loaded shader source codes                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the shader source code blueprint hash
		*
		*  @return
		*    The hash of the shader source code blueprints which are currently loaded in RAM
		*
		*  @see
		*    - "qsf::ShaderBlueprintManager::calculateSourceCodeHashFromFile()"
		*/
		inline uint32 getSourceCodeHash() const;

		/**
		*  @brief
		*    Calculate shader source code blueprint hash from file
		*
		*  @remarks
		*    The shader ASCII source code hash incorporates
		*    - ASCII source codes from OGRE HLMS library pieces
		*    - ASCII source codes from shader blueprints
		*
		*  @return
		*    The calculated shader source code blueprint hash from file
		*/
		uint32 calculateSourceCodeHashFromFile() const;

		/**
		*  @brief
		*    Search for out-of-date (aka "dirty") cache parts; used for incremental cache updates
		*
		*  @param[out] shaderPiecesIds
		*    Receives the dirty shader pieces IDs, not cleared before filled
		*  @param[out] shaderSourceCodeBlueprintIds
		*    Receives the dirty shader source code blueprint IDs, not cleared before filled
		*
		*  @return
		*    "true" if anything dirty was found, else "false"
		*/
		bool findDirtyCacheParts(ShaderPiecesIds& shaderPiecesIds, ShaderSourceCodeBlueprintIds& shaderSourceCodeBlueprintIds);

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the shader pieces manager instance
		*
		*  @return
		*    The shader pieces manager instance, do no destroy the returned instance
		*/
		inline ShaderPiecesManager& getShaderPiecesManager() const;

		/**
		*  @brief
		*    Return the shader source code blueprint manager instance
		*
		*  @return
		*    The shader source code blueprint manager instance, do no destroy the returned instance
		*/
		inline ShaderSourceCodeBlueprintManager& getShaderSourceCodeBlueprintManager() const;

		//[-------------------------------------------------------]
		//[ Cache manager                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the program cache manager instance
		*
		*  @return
		*    The program cache manager instance, do no destroy the returned instance
		*/
		inline ProgramCacheManager& getProgramCacheManager() const;

		/**
		*  @brief
		*    Return the shader cache manager instance
		*
		*  @return
		*    The shader cache manager instance, do no destroy the returned instance
		*/
		inline ShaderCacheManager& getShaderCacheManager() const;

		/**
		*  @brief
		*    Return the shader source code cache manager instance
		*
		*  @return
		*    The shader source code cache manager instance, do no destroy the returned instance
		*/
		inline ShaderSourceCodeCacheManager& getShaderSourceCodeCacheManager() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		ShaderBlueprintManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ShaderBlueprintManager();

		/**
		*  @brief
		*    Reload all shader blueprints
		*/
		void reloadAllBlueprints();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Loaded shader source codes
		uint32							  mSourceCodeHash;						///< Hash of the shader source code blueprints which are currently loaded in RAM
		ShaderPiecesManager*			  mShaderPiecesManager;					///< Shader pieces manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ShaderSourceCodeBlueprintManager* mShaderSourceCodeBlueprintManager;	///< Shader source code blueprint manager instance, always valid, we're responsible for destroying the instance in case we no longer need it

		// Cache managers in reversed order because they access each other
		ShaderSourceCodeCacheManager* mShaderSourceCodeCacheManager;	///< Shader source code cache manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ShaderCacheManager*			  mShaderCacheManager;				///< Shader cache manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ProgramCacheManager*		  mProgramCacheManager;				///< Program cache manager instance, always valid, we're responsible for destroying the instance in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/shaderblueprint/ShaderBlueprintManager-inl.h"
