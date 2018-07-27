// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


namespace qsf
{
	namespace ai
	{
		inline DistanceGrid::DistanceGrid(const GridConfiguration& configuration) :
			mConfiguration(configuration),
			mCells(new DistanceGridCell[configuration.mDimensions.x * configuration.mDimensions.y])
		{}

		inline const GridConfiguration& DistanceGrid::getConfiguration() const
		{
			return mConfiguration;
		}

		inline const DistanceGridCell& DistanceGrid::getCell(const GridCoordinates& position) const
		{
			return getCell(position.x, position.y);
		}

		inline const DistanceGridCell& DistanceGrid::getCell(unsigned int x, unsigned int y) const
		{
			// temporarily casting away const to avoid repeating the code
			return const_cast<DistanceGrid&>(*this).getCell(x, y);
		}

		inline const DistanceGridCell& DistanceGrid::getCell(unsigned int cellIndex) const
		{
			// temporarily casting away const to avoid repeating the code
			return const_cast<DistanceGrid&>(*this).getCell(cellIndex);
		}

		inline DistanceGridCell& DistanceGrid::getCell(const GridCoordinates& position)
		{
			return getCell(position.x, position.y);
		}

		inline DistanceGridCell& DistanceGrid::getCell(unsigned int x, unsigned int y)
		{
			QSF_CHECK(x < mConfiguration.mDimensions.x, "X value " << x << " out of bounds (max is " << mConfiguration.mDimensions.x << ") when accessing grid cell state",
				QSF_REACT_THROW);
			QSF_CHECK(y < mConfiguration.mDimensions.y, "Y value " << y << " out of bounds (max is " << mConfiguration.mDimensions.y << ") when accessing grid cell state",
				QSF_REACT_THROW);

			return mCells[(y * mConfiguration.mDimensions.x) + x];
		}

		inline DistanceGridCell& DistanceGrid::getCell(unsigned int cellIndex)
		{
			QSF_CHECK(cellIndex < mConfiguration.mDimensions.x * mConfiguration.mDimensions.y, "index out of bounds when accessing grid cell state",
				QSF_REACT_THROW);

			return mCells[cellIndex];
		}

		inline DistanceGridCell* DistanceGrid::tryGetCell(unsigned int x, unsigned int y)
		{
			if (x >= mConfiguration.mDimensions.x || y >= mConfiguration.mDimensions.y)
				return nullptr;

			return &mCells[(y * mConfiguration.mDimensions.x) + x];
		}

		inline DistanceGridCell* DistanceGrid::tryGetCell(const GridCoordinates& position)
		{
			return tryGetCell(position.x, position.y);
		}

		inline DistanceGridCell* DistanceGrid::tryGetCell(unsigned int cellIndex)
		{
			if (cellIndex < mConfiguration.mDimensions.x * mConfiguration.mDimensions.y)
				return nullptr;

			return &mCells[cellIndex];
		}

		inline const DistanceGridCell* DistanceGrid::tryGetCell(unsigned int x, unsigned int y) const
		{
			if (x >= mConfiguration.mDimensions.x || y >= mConfiguration.mDimensions.y)
				return nullptr;

			return &mCells[(y * mConfiguration.mDimensions.x) + x];
		}

		inline const DistanceGridCell* DistanceGrid::tryGetCell(const GridCoordinates& position) const
		{
			return tryGetCell(position.x, position.y);
		}

		inline const DistanceGridCell* DistanceGrid::tryGetCell(unsigned int cellIndex) const
		{
			if (cellIndex < mConfiguration.mDimensions.x * mConfiguration.mDimensions.y)
				return nullptr;

			return &mCells[cellIndex];
		}

		inline void DistanceGrid::setBlocked(unsigned int x, unsigned int y)
		{
			setBlocked(GridCoordinates(x, y));
		}

		inline void DistanceGrid::setBlocked(const GridCoordinates& position)
		{
			setBlocked(convertToCellIndex(position));
		}

		inline void DistanceGrid::setBlocked(unsigned int cellIndex)
		{
			DistanceGridCell& data = getCell(cellIndex);
			data.setClosestObstacleCellIndex(cellIndex); // set its own index as closest obstacle
		}

		inline bool DistanceGrid::containsUninitializedDistanceValue() const
		{
			for (unsigned int index = 0; index < mConfiguration.mDimensions.x * mConfiguration.mDimensions.y; ++index)
				if (!mCells[index].hasClosestKnownObstacle())
					return true;

			return false;
		}

		inline GridCoordinates DistanceGrid::convertToCoordinates(unsigned int cellIndex) const
		{
			const unsigned int x = cellIndex % mConfiguration.mDimensions.x;
			const unsigned int y = cellIndex / mConfiguration.mDimensions.x; // rounding down as desired

			return GridCoordinates(x, y);
		}

		inline unsigned int DistanceGrid::convertToCellIndex(const GridCoordinates& coordinates) const
		{
			return (coordinates.y * mConfiguration.mDimensions.x) + coordinates.x;
		}

		inline unsigned int DistanceGrid::calculateClosestDistanceSquared(unsigned int cellIndex) const
		{
			const unsigned int closestCellIndex = getCell(cellIndex).getClosestObstacleCellIndex();

			return calculateDistanceSquaredBetween(cellIndex, closestCellIndex);
		}

		inline unsigned int DistanceGrid::calculateClosestDistanceSquared(const GridCoordinates& coordinates) const
		{
			return calculateClosestDistanceSquared(convertToCellIndex(coordinates));
		}

		inline bool DistanceGrid::isBlocked(unsigned int cellIndex) const
		{
			const DistanceGridCell& cell = getCell(cellIndex);
			return cell.getClosestObstacleCellIndex() == cellIndex; // having its own index as closest obstacle is the indicator that a cell is occupied
		}

		inline bool DistanceGrid::isBlocked(const GridCoordinates& coordinates) const
		{
			return isBlocked(convertToCellIndex(coordinates));
		}
	}
}
