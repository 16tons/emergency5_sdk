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
	inline Map& MapSceneManager::getMap() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMap, "The QSF map instance is invalid", QSF_REACT_THROW);
		return *mMap;
	}

	inline bool MapSceneManager::getUpdateAnimationsAndTransforms() const
	{
		return mUpdateAnimationsAndTransforms;
	}

	inline void MapSceneManager::setUpdateAnimationsAndTransforms(bool updateAnimationsAndTransforms)
	{
		mUpdateAnimationsAndTransforms = updateAnimationsAndTransforms;
	}

	inline bool MapSceneManager::getAllowShowBoundingBoxes() const
	{
		return mAllowShowBoundingBoxes;
	}

	inline void MapSceneManager::setAllowShowBoundingBoxes(bool allowShowBoundingBoxes)
	{
		mAllowShowBoundingBoxes = allowShowBoundingBoxes;
	}

	inline MovableObjectRenderingListenerManager& MapSceneManager::getMovableObjectRenderingListenerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMovableObjectRenderingListenerManager, "The QSF movable object rendering listener manager instance is invalid", QSF_REACT_THROW);
		return *mMovableObjectRenderingListenerManager;
	}

	inline OgreManualObjectCacheManager& MapSceneManager::getOgreManualObjectCacheManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mOgreManualObjectCacheManager, "The OGRE manual object cache manager instance is invalid", QSF_REACT_THROW);
		return *mOgreManualObjectCacheManager;
	}

	inline FlareManager& MapSceneManager::getFlareManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFlareManager, "The QSF flare manager instance is invalid", QSF_REACT_THROW);
		return *mFlareManager;
	}

	inline ParticleUniverseParticleSystemInstanceManager& MapSceneManager::getParticleUniverseParticleSystemInstanceManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mParticleUniverseParticleSystemInstanceManager, "The QSF Particle Universe particle system manager instance is invalid", QSF_REACT_THROW);
		return *mParticleUniverseParticleSystemInstanceManager;
	}

	inline bool MapSceneManager::isSceneRenderingInProgress() const
	{
		return mSceneRenderingInProgress;
	}

	inline Ogre::Camera* MapSceneManager::getMainOgreCamera() const
	{
		return mMainOgreCamera;
	}

	inline const Time& MapSceneManager::getLastRenderedTimestamp() const
	{
		return mLastRenderedTimestamp;
	}

	inline const MapSceneManager::MovableObjectWithListenerArray& MapSceneManager::getMovableObjectWithListenerArray() const
	{
		return mMovableObjectWithListenerArray;
	}

	inline const Ogre::FrameStats& MapSceneManager::getFrameStats() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFrameStats, "The frame statistics instance is invalid", QSF_REACT_THROW);
		return *mFrameStats;
	}

	inline uint32 MapSceneManager::getNumberOfRenderedOgreMovableObjects() const
	{
		return mNumberOfRenderedOgreMovableObjects;
	}

	inline uint32 MapSceneManager::getNumberOfRenderedBatches() const
	{
		return mNumberOfRenderedBatches;
	}

	inline uint32 MapSceneManager::getNumberOfRenderedTriangles() const
	{
		return mNumberOfRenderedTriangles;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
