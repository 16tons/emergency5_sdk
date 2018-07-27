// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Archive;
	typedef std::vector<Archive*> ArchiveVec;
}
namespace qsf
{
	class ShadowSetup;
	class HlmsBlueprint;
	class ParameterGroup;
	class CameraComponent;
	class MaterialManager;
	class PipelineStateCompiler;
	class ShaderBlueprintManager;
	class MaterialBlueprintManager;
	class MaterialVariationManager;
	class MaterialSystemCacheManager;
	namespace detail
	{
		class OgreHlmsManagerListener;
	}
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
	*    Material system
	*/
	class QSF_API_EXPORT MaterialSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystemCacheManager;	// Needs access to "qsf::MaterialSystem::getHlmsOgreArchiveAndArchiveVec()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MaterialSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MaterialSystem();

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the shader blueprint manager instance
		*
		*  @return
		*    The shader blueprint manager instance, do no destroy the returned instance
		*/
		inline ShaderBlueprintManager& getShaderBlueprintManager() const;

		/**
		*  @brief
		*    Return the material blueprint manager instance
		*
		*  @return
		*    The material blueprint manager instance, do no destroy the returned instance
		*/
		inline MaterialBlueprintManager& getMaterialBlueprintManager() const;

		/**
		*  @brief
		*    Return the material manager instance
		*
		*  @return
		*    The material manager instance, do no destroy the returned instance
		*/
		inline MaterialManager& getMaterialManager() const;

		/**
		*  @brief
		*    Return the material variation manager instance
		*
		*  @return
		*    The material variation manager instance, do no destroy the returned instance
		*/
		inline MaterialVariationManager& getMaterialVariationManager() const;

		/**
		*  @brief
		*    Return the material system cache manager instance
		*
		*  @return
		*    The material system cache manager instance, do no destroy the returned instance
		*/
		inline MaterialSystemCacheManager& getMaterialSystemCacheManager() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the HLMS blueprint instance
		*
		*  @return
		*    The HLMS blueprint instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access this if you don't have to
		*/
		inline HlmsBlueprint* getHlmsBlueprint() const;

		/**
		*  @brief
		*    Return the pipeline state compiler instance
		*
		*  @return
		*    The pipeline state compiler instance, do no destroy the returned instance
		*/
		inline PipelineStateCompiler& getPipelineStateCompiler() const;

		inline ShadowSetup* getShadowSetup() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Ogre::Archive* getHlmsOgreArchiveAndArchiveVec(Ogre::ArchiveVec& ogreArchiveVec) const;
		void registerHlms();
		void unregisterHlms();
		void setShaderSourceCodeCacheManagerDebugOutputPathAndState() const;
		void createOrRecreateShadowSetup();
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the debug group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onDebugGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Managers
		ShaderBlueprintManager*			 mShaderBlueprintManager;		///< Shader blueprint manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MaterialBlueprintManager*		 mMaterialBlueprintManager;		///< Material blueprint manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MaterialManager*				 mMaterialManager;				///< Material manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MaterialVariationManager*		 mMaterialVariationManager;		///< Material variation manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MaterialSystemCacheManager*		 mMaterialSystemCacheManager;	///< Material system cache manager, always valid, we're responsible for destroying the instance in case we no longer need it
		// Miscellaneous
		detail::OgreHlmsManagerListener* mOgreHlmsManagerListener;		///< OGRE HLMS manager listener instance, always valid, we're responsible for destroying the instance in case we no longer need it
		HlmsBlueprint*					 mHlmsBlueprint;				///< HLMS blueprint instance, always valid, we're responsible for destroying the instance in case we no longer need it
		PipelineStateCompiler*			 mPipelineStateCompiler;		///< Pipeline state compiler, always valid, we're responsible for destroying the instance in case we no longer need it
		ShadowSetup*					 mShadowSetup;					///< Shadow setup instance, can be a null pointer, destroy the instance if you no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/MaterialSystem-inl.h"
