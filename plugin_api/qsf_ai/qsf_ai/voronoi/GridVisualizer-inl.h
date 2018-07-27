// Copyright (C) 2012-2017 Promotion Software GmbH

#include "qsf_ai/voronoi/DistanceGrid.h"
#include "qsf_ai/voronoi/GridConfiguration.h"

#include <qsf/QsfHelper.h>
#include <qsf/debug/DebugDrawManager.h>
#include <qsf/debug/request/CompoundDebugDrawRequest.h>
#include <qsf/math/GlmHelper.h>


namespace qsf
{
	namespace ai
	{
		namespace voronoi
		{
			// Main algorithm implementation
			template <typename Filter, typename Visualizer>
			void visualizeCells(const Filter& filter, const DistanceGrid& grid, const Time& duration, const Visualizer& visualizer)
			{
				DebugDrawManager& ddm = QSF_DEBUGDRAW;
				CompoundDebugDrawRequest cddr;

				// Default lifetime data reused for all drawings in here currently
				DebugDrawLifetimeData ddld(duration);

				const unsigned int numCells = grid.getConfiguration().mDimensions.x * grid.getConfiguration().mDimensions.y;
				for (unsigned int cellIndex = 0; cellIndex < numCells; ++cellIndex)
				{
					const DistanceGridCell& cell = grid.getCell(cellIndex);
					if (!filter(cellIndex, cell))
						continue;

					const GridCoordinates& coordinates = grid.convertToCoordinates(cellIndex);
					visualizer(cell, coordinates, cddr);
				}

				ddm.requestDraw(cddr, ddld);
			}

			// Filter Functor implementation
			inline bool SelectObstacleFilter::operator () (unsigned int cellIndex, const DistanceGridCell& cell) const
			{
				return cell.getClosestObstacleCellIndex() == cellIndex;
			}

			inline SelectEquidistantCellsFilter::SelectEquidistantCellsFilter(const DynamicGraph& graph) :
				mGraph(graph)
			{}

			inline bool SelectEquidistantCellsFilter::operator () (unsigned int cellIndex, const DistanceGridCell& cell) const
			{
				return isInitialized(mGraph.mVoronoiLine[cellIndex]);
			}

			inline SelectSpecificDistanceFilter::SelectSpecificDistanceFilter(const DistanceGrid& grid, unsigned int distance) :
				mGrid(grid),
				mDistance(distance)
			{}

			inline bool SelectSpecificDistanceFilter::operator () (unsigned int cellIndex, const DistanceGridCell& cell) const
			{
				return mGrid.calculateClosestDistanceSquared(cellIndex) == mDistance;
			}

			inline SelectVoronoiEndFilter::SelectVoronoiEndFilter(const DynamicGraph& graph) :
				mGraph(graph)
			{}

			template <typename Container>
			SelectSpecificSetFilter<Container>::SelectSpecificSetFilter(const Container& cellIdsToShow) :
				mCellIdsToShow(cellIdsToShow)
			{}

			template <typename Container>
			bool SelectSpecificSetFilter<Container>::operator () (unsigned int cellIndex, const DistanceGridCell& cell) const
			{
				return std::find(mCellIdsToShow.begin(), mCellIdsToShow.end(), cellIndex) != mCellIdsToShow.end();
			}

			inline SelectVoronoiVicinityFilter::SelectVoronoiVicinityFilter(const GridCoordinates& corner1, const GridCoordinates& corner2, unsigned int extraSpace, const DynamicGraph& graph) :
				mMinCorner(getMinimum(corner1, corner2)),
				mMaxCorner(getMaximum(corner1, corner2)),
				mGraph(graph)
			{
				if (mMinCorner.x < extraSpace)
					mMinCorner.x = 0;
				else
					mMinCorner.x -= extraSpace;
				if (mMinCorner.y < extraSpace)
					mMinCorner.y = 0;
				else
					mMinCorner.y -= extraSpace;

				mMaxCorner += GridDelta(static_cast<int>(extraSpace), static_cast<int>(extraSpace));
			}

			inline bool SelectVoronoiVicinityFilter::operator ()(unsigned int cellIndex, const DistanceGridCell& cell) const
			{
				const GridCoordinates coords = mGraph.mGrid.convertToCoordinates(cellIndex);

				if (isUninitialized(mGraph.mVoronoiLine[cellIndex]))
					return false;

				return coords.x >= mMinCorner.x && coords.x <= mMaxCorner.x && coords.y >= mMinCorner.y && coords.y <= mMaxCorner.y;
			}

			// ---------------------------------
			// Visualizer functor implementation
			inline CellOutlineVisualizer::CellOutlineVisualizer(const GridConfiguration& gridConfig, const Color4& color, float fillOpacity) :
				mGridConfig(gridConfig),
				mColor(color),
				mFillOpacity(fillOpacity)
			{}

			inline void CellOutlineVisualizer::operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const
			{
				const std::pair<glm::vec3, glm::vec3> corners = mGridConfig.convertPositionToCorners(coordinates);
				cddr.mRectangles.emplace_back(corners.first, corners.second, mColor, mFillOpacity);
			}

			inline CellDiagonalVisualizer::CellDiagonalVisualizer(const GridConfiguration& gridConfig, const Color4& color) :
				mGridConfig(gridConfig),
				mColor(color)
			{}

			inline void CellDiagonalVisualizer::operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const
			{
				const auto corners = mGridConfig.convertPositionToCorners(coordinates);
				cddr.mSegments.emplace_back(Segment::fromTwoPoints(corners.first, corners.second), mColor);
			}

			inline CellInfluenceVisualizer::CellInfluenceVisualizer(const DistanceGrid& grid, const Color4& color) :
				mGrid(grid),
				mColor(color)
			{}

			inline void CellInfluenceVisualizer::operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const
			{
				if (!cell.hasClosestKnownObstacle())
					return;

				const glm::vec3 pos1 = mGrid.getConfiguration().convertPositionToMidpoint(coordinates);
				const glm::vec3 pos2 = mGrid.getConfiguration().convertPositionToMidpoint(mGrid.convertToCoordinates(cell.getClosestObstacleCellIndex()));

				cddr.mSegments.push_back(qsf::Segment::fromTwoPoints(pos1, pos2));
			}

			inline CellIndexVisualizer::CellIndexVisualizer(const DistanceGrid& grid) :
				mGrid(grid)
			{}

			inline void CellIndexVisualizer::operator ()(const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const
			{
				cddr.mTexts.emplace_back(std::to_string(mGrid.convertToCellIndex(coordinates)), mGrid.getConfiguration().convertPositionToMidpoint(coordinates));
			}
		}
	}
}
