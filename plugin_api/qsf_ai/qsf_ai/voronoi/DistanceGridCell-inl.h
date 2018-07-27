// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


namespace qsf
{
	namespace ai
	{
		inline DistanceGridCell::DistanceGridCell() :
			mClosestObstacleCell(getUninitialized(mClosestObstacleCell))
		{}

		inline void DistanceGridCell::setClosestObstacleCellIndex(unsigned int index)
		{
			mClosestObstacleCell = index;
		}

		inline void DistanceGridCell::clearClosestObstacleCellIndex()
		{
			setUninitialized(mClosestObstacleCell);
		}

		inline unsigned int DistanceGridCell::getClosestObstacleCellIndex() const
		{
			return mClosestObstacleCell;
		}

		inline bool DistanceGridCell::hasClosestKnownObstacle() const
		{
			return isInitialized(mClosestObstacleCell);
		}
	}
}
