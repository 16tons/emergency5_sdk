// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/listener/IMaterialBlueprintListener.h"
#include "qsf/job/JobProxy.h"

#include <OGRE/OgreMatrix4.h>

#include <random>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class Texture;
	class Viewport;
	class CompositorShadowNode;
}
namespace qsf
{
	class Map;
	class JobArguments;
	class MaterialProperties;
	class RendererSettingsGroup;
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
	*    Material blueprint listener
	*/
	class MaterialBlueprintListener : public IMaterialBlueprintListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MaterialBlueprintListener();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MaterialBlueprintListener();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		MaterialBlueprintListener(const MaterialBlueprintListener&) = delete;
		MaterialBlueprintListener& operator=(const MaterialBlueprintListener&) = delete;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::IMaterialBlueprintListener methods ]
	//[-------------------------------------------------------]
	private:
		virtual void onStartup(MaterialBlueprintManager& materialBlueprintManager) override;
		virtual void onShutdown(MaterialBlueprintManager& materialBlueprintManager) override;
		inline virtual void beginFillUnknown() override;
		inline virtual bool fillUnknownValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) override;
		virtual void beginFillPass(Ogre::RenderSystem& ogreRenderSystem, Ogre::SceneManager& ogreSceneManager, const Ogre::CompositorShadowNode* ogreCompositorShadowNode, bool casterPass, bool dualParaboloid, const Ogre::Texture* boundOgreTextures[PassConstantBufferManager::MAXIMUM_NUMBER_OF_TEXTURE_UNITS], PassConstantBufferManager::PassData& passData) override;
		virtual bool fillPassValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) override;
		virtual bool fillMaterialValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes) override;
		virtual uint32 getNumberOfInstanceValueBytes(uint32 referenceValue, bool casterPass) override;
		virtual bool fillInstanceValue(uint32 referenceValue, uint8* buffer, uint32 numberOfBytes, const Ogre::Matrix4& ogreObjectSpaceToWorldSpaceMatrix, const PassConstantBufferManager::PassData& passData, const Ogre::Renderable& ogreRenderable) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Main update function that is passed to the job manager, for updates using real-time timing
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateRealtimeJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Main update function that is passed to the job manager, for updates using animation timing (e.g. game speed applied or similar)
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateAnimationJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Update the shared shader parameter for wind animation (on water and vegetation)
		*/
		void updateWindAnimationParameters();

		/**
		*  @brief
		*    Update the shared shader parameter for highlight pulse animation
		*/
		void updateHighlightPulseParameters();

		/**
		*  @brief
		*    Update the shared shader parameter for global animation (uv scrolling, map border, shine through, object clipping ec.)
		*/
		void updateGlobalAnimationParameters();

		/**
		*  @brief
		*    Update the shared shader parameter for vegetation animation
		*/
		void updateVegetationAnimationParameters();

		/**
		*  @brief
		*    Update the shared shader parameter for water animation
		*/
		void updateWaterAnimationParameters();

		/**
		*  @brief
		*    Update the shared shader parameter for fire animation
		*/
		void updateFireAnimationParameters();

		Map& getMapByOgreCamera(const Ogre::Camera& ogreCamera) const;
		bool fillPassTextureSizeValue(uint8* buffer, uint32 numberOfBytes, uint8 textureUnit, bool inverse) const;
		bool fillPassShadowTextureDepthRangeValue(uint8* buffer, uint32 numberOfBytes, uint8 shadowTextureIndex) const;
		bool fillPassViewSpaceToShadowTextureSpaceValue(uint8* buffer, uint32 numberOfBytes, uint8 shadowTextureIndex, const Ogre::Matrix4& viewSpaceToWorldSpace) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const RendererSettingsGroup*		 mRendererSettingsGroup;	///< Can be a null pointer, don't destroy the instance
		MaterialProperties*					 mGlobalMaterialProperties;	///< Global material properties, can be a null pointer, don't destroy the instance
		PassConstantBufferManager::PassData* mPassData;					///< Memory address received via "qsf::MaterialBlueprintListener::beginFillPass()", can be a null pointer outside the correct scope, don't destroy the memory
		Ogre::Matrix4						 mOgreProjectionMatrix;
		const Ogre::Viewport*				 mOgreViewport;				///< Currently used OGRE viewport, can be a null pointer, do not destroy the instance
		const Ogre::Camera*					 mOgreCamera;				///< Currently used OGRE camera, can be a null pointer, do not destroy the instance
		const Ogre::CompositorShadowNode*	 mOgreCompositorShadowNode;
		bool								 mCasterPass;
		const Ogre::Texture**				 mBoundOgreTextures;
		std::mt19937						 mRandomGenerator;
		Ogre::Matrix4						 mGuiSpaceToClipSpaceMatrix;
		// Job proxy
		JobProxy mRealtimeUpdateJobProxy;	///< Update job proxy, called once a frame with time passed measured in real time
		JobProxy mAnimationUpdateJobProxy;	///< Update job proxy, called once a frame with time passed measured in game time
		// Animation timer
		float mPulseAnimationTimer;			///< Highlight pulse animation timer
		float mGlobalAnimationTimer;		///< Global animation timer
		float mVegetationAnimationTimer;	///< Vegetation animation timer
		float mWaterAnimationTimer;			///< Water animation timer
		float mFireAnimationTimer;			///< Fire animation timer


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/materialblueprint/listener/MaterialBlueprintListener-inl.h"
