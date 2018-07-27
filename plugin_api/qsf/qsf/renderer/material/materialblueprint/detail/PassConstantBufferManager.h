// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/detail/RootSignatureTypes.h"

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class VaoManager;
	class SceneManager;
	class RenderSystem;
	class HlmsListener;
	class CommandBuffer;
	class ConstBufferPacked;
	class CompositorShadowNode;
}
namespace qsf
{
	class MaterialBlueprint;
	class MaterialBlueprintManager;
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
	*    Pass constant buffer manager
	*
	*  @note
	*    - Class name is singular, a pass is a pass is a pass
	*/
	class PassConstantBufferManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint8 MAXIMUM_NUMBER_OF_TEXTURE_UNITS = 16;	// Same as "OGRE_MAX_TEXTURE_LAYERS"

		struct PassData
		{
			Ogre::Matrix4		worldSpaceToClipSpaceMatrix;
			Ogre::Matrix4		worldSpaceToViewSpaceMatrix;
			Ogre::Matrix4		viewSpaceToWorldSpaceMatrix;
			const Ogre::Camera* ogreCamera;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		PassConstantBufferManager(const MaterialBlueprint& materialBlueprint, Ogre::RenderSystem& ogreRenderSystem, Ogre::VaoManager& ogreVaoManager);

		/**
		*  @brief
		*    Destructor
		*/
		~PassConstantBufferManager();

		/**
		*  @brief
		*    Fill the pass constant buffer
		*
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
		*/
		void fillBuffer(Ogre::SceneManager& ogreSceneManager, const Ogre::CompositorShadowNode* ogreCompositorShadowNode, bool casterPass, bool dualParaboloid, const Ogre::Texture* boundOgreTextures[MAXIMUM_NUMBER_OF_TEXTURE_UNITS]);

		/**
		*  @brief
		*    Return the pass data
		*
		*  @return
		*    The pass data filled inside "qsf::PassConstantBufferManager::fillBuffer()"
		*/
		const PassData& getPassData() const;

		/**
		*  @brief
		*    Bind the currently used pass constant buffer to the renderer
		*
		*  @param[in] ogreCommandBuffer
		*    OGRE command buffer to fill with instructions
		*  @param[in] slotIndex
		*    Slot index
		*/
		void bindToRenderer(Ogre::CommandBuffer& ogreCommandBuffer, uint16 slotIndex) const;

		/**
		*  @brief
		*    Reset current pass buffer
		*
		*  @note
		*    - Should be called once per frame
		*/
		void resetCurrentPassBuffer();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<Ogre::ConstBufferPacked*> ConstBufferPackedVec;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const MaterialBlueprint&		mMaterialBlueprint;
		const MaterialBlueprintManager& mMaterialBlueprintManager;
		Ogre::RenderSystem&				mOgreRenderSystem;
		Ogre::VaoManager&				mOgreVaoManager;
		Renderer::ShaderVisibility		mShaderVisibility;
		PassData						mPassData;
		ConstBufferPackedVec			mPassBuffers;
		uint32							mCurrentPassBuffer;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
