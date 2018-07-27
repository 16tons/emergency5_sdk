// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool RendererSystem::isRunning() const
	{
		return mRunning;
	}

	inline MaterialSystem& RendererSystem::getMaterialSystem() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialSystem, "The material system instance is invalid", QSF_REACT_THROW);
		return *mMaterialSystem;
	}

	inline TextureStreamingManager& RendererSystem::getTextureStreamingManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mTextureStreamingManager, "The texture streaming manager instance is invalid", QSF_REACT_THROW);
		return *mTextureStreamingManager;
	}

	inline ParticleManager& RendererSystem::getParticleManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mParticleManager, "The particle manager instance is invalid", QSF_REACT_THROW);
		return *mParticleManager;
	}

	inline CompositorWorkspaceConfiguratorManager& RendererSystem::getCompositorWorkspaceConfiguratorManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mCompositorWorkspaceConfiguratorManager, "The compositor workspace configurator manager instance is invalid", QSF_REACT_THROW);
		return *mCompositorWorkspaceConfiguratorManager;
	}

	inline SkeletonManager& RendererSystem::getSkeletonManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSkeletonManager, "The skeleton manager instance is invalid", QSF_REACT_THROW);
		return *mSkeletonManager;
	}

	inline OgreResourceManagerHelper& RendererSystem::getResourceManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mOgreResourceManagerHelper, "The ogre resource manager helper instance is invalid", QSF_REACT_THROW);
		return *mOgreResourceManagerHelper;
	}

	inline Ogre::Root* RendererSystem::getOgreRoot() const
	{
		return mOgreRoot;
	}

	inline MeshVertexFormatConverter* RendererSystem::getMeshVertexFormatConverter() const
	{
		return mMeshVertexFormatConverter;
	}

	inline RenderWindow* RendererSystem::getRenderWindowDummy() const
	{
		return mRenderWindowDummy;
	}

	inline void RendererSystem::setRenderWindowDummyLock(bool lock)
	{
		mRenderWindowDummyLock = lock;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* RendererSystem::getName() const
	{
		return "Renderer system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
