// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GridCellSceneNode* GridSceneManager::getCellSceneNodeAtIndex(uint32 x, uint32 z) const
	{
		return mCellOgreSceneNodes[z * mCellsPerEdge + x];
	}

	inline const Ogre::Vector3& GridSceneManager::getCellSize() const
	{
		return mCellSize;
	}

	inline GridRootSceneNode* GridSceneManager::getGridRootSceneNode() const
	{
		return mGridRootSceneNode;
	}

	inline bool GridSceneManager::getAggressiveGridCulling() const
	{
		return mAggressiveGridCulling;
	}

	inline void GridSceneManager::setAggressiveGridCulling(bool aggressiveGridCulling)
	{
		mAggressiveGridCulling = aggressiveGridCulling;
		mDisableTerrainLod = aggressiveGridCulling;
	}

	inline bool GridSceneManager::getAggressiveRenderingDistanceGridCulling() const
	{
		return mAggressiveRenderingDistanceGridCulling;
	}

	inline void GridSceneManager::setAggressiveRenderingDistanceGridCulling(bool aggressiveRenderingDistanceGridCulling)
	{
		mAggressiveRenderingDistanceGridCulling = aggressiveRenderingDistanceGridCulling;
	}

	inline int32 GridSceneManager::getAggressiveGridCullingRadius() const
	{
		return mAggressiveGridCullingRadius;
	}

	inline void GridSceneManager::setAggressiveGridCullingRadius(int32 aggressiveGridCullingRadius)
	{
		mAggressiveGridCullingRadius = aggressiveGridCullingRadius;
	}

	inline bool GridSceneManager::getShowGrid() const
	{
		return mShowGrid;
	}

	inline void GridSceneManager::setShowGrid(bool showGrid)
	{
		mShowGrid = showGrid;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
