// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/algorithm/clamp.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline qsf::Map* MiniMapRenderer::getMap()
	{
		return mMap;
	}

	inline const MiniMapRenderer::LiveIconStates& MiniMapRenderer::getLiveIconStates() const
	{
		return mLiveIconStates;
	}

	inline Rocket::Core::Texture* MiniMapRenderer::getMiniMapTexture() const
	{
		return mRocketTexture;
	}

	inline bool MiniMapRenderer::getCameraQuadVisible() const
	{
		return mCameraQuadVisible;
	}

	inline void MiniMapRenderer::setPathVisualizationEntityId(uint64 entityId)
	{
		mPathVisualizationEntityId = entityId;
	}

	inline float MiniMapRenderer::getZoomFactor() const
	{
		return mZoomFactor;
	}

	inline float MiniMapRenderer::getMinZoomFactor() const
	{
		return mMinZoomFactor;
	}

	inline void MiniMapRenderer::setMinZoomFactor(float zoomFactor)
	{
		mMinZoomFactor = zoomFactor;
	}

	inline float MiniMapRenderer::getMaxZoomFactor() const
	{
		return mMaxZoomFactor;
	}

	inline void MiniMapRenderer::setMaxZoomFactor(float zoomFactor)
	{
		mMaxZoomFactor = zoomFactor;
	}

	inline void MiniMapRenderer::zoomIn()
	{
		setZoomFactor(mZoomFactor * 2.0f);
	}

	inline void MiniMapRenderer::zoomOut()
	{
		setZoomFactor(mZoomFactor * 0.5f);
	}

	inline bool MiniMapRenderer::getUseViewportSizeBlackBars() const
	{
		return mUseViewportSizeBlackBars;
	}

	inline void MiniMapRenderer::setUseViewportSizeBlackBars(bool useViewportSizeBlackBars)
	{
		mUseViewportSizeBlackBars = useViewportSizeBlackBars;
		viewportSizeUpdated();
		updateCamera();
	}

	inline bool MiniMapRenderer::getUnitOfDifferentPlayerHandling() const
	{
		return mUnitOfDifferentPlayerHandling;
	}

	inline void MiniMapRenderer::setUnitOfDifferentPlayerHandling(bool unitOfDifferentPlayerHandling)
	{
		mUnitOfDifferentPlayerHandling = unitOfDifferentPlayerHandling;
	}

	inline bool MiniMapRenderer::getViewportFollowsGameCamera() const
	{
		return mViewportFollowsGameCamera;
	}

	inline void MiniMapRenderer::setViewportFollowsGameCamera(bool viewportFollowsGameCamera)
	{
		mViewportFollowsGameCamera = viewportFollowsGameCamera;
	}

	const glm::vec2& MiniMapRenderer::getCurrentMinimapCameraPosition() const
	{
		return mCameraPosition;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
