// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/detail/PassConstantBufferManager.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Matrix4;
	class SceneManager;
	class RenderSystem;
	class CompositorShadowNode;
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
	*    Abstract material blueprint listener interface
	*/
	class IMaterialBlueprintListener : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends methods                                       ]
	//[-------------------------------------------------------]
		friend class MaterialBlueprint;			///< Is calling the private interface methods
		friend class InstancesBufferManager;	///< Is calling the private interface methods
		friend class HlmsBlueprintDatablock;	///< Is calling the private interface methods
		friend class PassConstantBufferManager;	///< Is calling the private interface methods
		friend class MaterialBlueprintManager;	///< Is calling the private interface methods


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline IMaterialBlueprintListener();
		inline virtual ~IMaterialBlueprintListener();
		IMaterialBlueprintListener(const IMaterialBlueprintListener&) = delete;
		IMaterialBlueprintListener& operator=(const IMaterialBlueprintListener&) = delete;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::IMaterialBlueprintListener methods ]
	//[-------------------------------------------------------]
	private:
		virtual void onStartup(MaterialBlueprintManager& materialBlueprintManager) = 0;
		virtual void onShutdown(MaterialBlueprintManager& materialBlueprintManager) = 0;

		virtual void beginFillUnknown() = 0;
		virtual bool fillUnknownValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) = 0;

		/**
		*  @brief
		*    Begin of pass constant buffer filling
		*
		*  @param[in] ogreRenderSystem
		*    OGRE render system to use
		*  @param[in] ogreSceneManager
		*    OGRE scene manager to use
		*  @param[in] ogreCompositorShadowNode
		*    OGRE compositor shadow node
		*  @param[in] casterPass
		*    Caster pass?
		*  @param[in] dualParaboloid
		*    Dual paraboloid?
		*  @param[in] boundOgreTextures
		*    OGRE textures bound to texture units
		*  @param[out] passData
		*    Pass data which also is of interest to the rest of the world
		*/
		virtual void beginFillPass(Ogre::RenderSystem& ogreRenderSystem, Ogre::SceneManager& ogreSceneManager, const Ogre::CompositorShadowNode* ogreCompositorShadowNode, bool casterPass, bool dualParaboloid, const Ogre::Texture* boundOgreTextures[PassConstantBufferManager::MAXIMUM_NUMBER_OF_TEXTURE_UNITS], PassConstantBufferManager::PassData& passData) = 0;

		virtual bool fillPassValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) = 0;
		virtual bool fillMaterialValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) = 0;
		virtual uint32 getNumberOfInstanceValueBytes(uint32 referenceValue, bool casterPass) = 0;
		virtual bool fillInstanceValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes, const Ogre::Matrix4& ogreObjectSpaceToWorldSpaceMatrix, const PassConstantBufferManager::PassData& passData, const Ogre::Renderable& ogreRenderable) = 0;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/listener/IMaterialBlueprintListener-inl.h"
