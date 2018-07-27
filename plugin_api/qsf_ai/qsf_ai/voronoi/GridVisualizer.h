// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/voronoi/GridCoordinates.h"
#include "qsf_ai/voronoi/DynamicVoronoiGraph.h"

#include <qsf/time/Time.h>
#include <qsf/math/Color4.h>

#include <vector>


namespace qsf
{
	class CompoundDebugDrawRequest;

	namespace ai
	{
		namespace voronoi
		{
			// Contains helper functions for graphically debugging the Voronoi grid state.
			// The Filter is applied to GridCell objects and determines whether this cell is actually visualized or not.
			// The Visualizer defines what will actually be drawn for the cells that pass the filter.
			// Example implementations are defined at the bottom of this file
			template <typename Filter, typename Visualizer>
			void visualizeCells(const Filter& filter, const DistanceGrid& grid, const Time& duration, const Visualizer& visualizer);

			// Some filters to be used with the above iteration
			// A filter to select only obstacles identified by the closest distance being zero
			class SelectObstacleFilter
			{
			public:
				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;
			};

			// A filter to select only cells which are equidistant to at least two obstacles
			class SelectEquidistantCellsFilter
			{
			public:
				SelectEquidistantCellsFilter(const DynamicGraph& graph);

				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;

			private:
				const DynamicGraph& mGraph;
			};

			// A filter to select only cells with a specific distance
			class SelectSpecificDistanceFilter
			{
			public:
				SelectSpecificDistanceFilter(const DistanceGrid& grid, unsigned int distance);

				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;

			private:
				const DistanceGrid& mGrid;
				unsigned int mDistance;
			};

			// A filter to select the cells that are end points of the voronoi graph.
			// This tests whether at most one node in the eight connected neighbourhood is part of the voronoi graph.
			class SelectVoronoiEndFilter
			{
			public:
				SelectVoronoiEndFilter(const DynamicGraph& graph);

				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;

			private:
				const DynamicGraph& mGraph;
			};

			// Filter by precalculated selection of cell ids
			template <typename Container>
			class SelectSpecificSetFilter
			{
			public:
				SelectSpecificSetFilter(const Container& cellsIdsToShow);

				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;

			private:
				const Container& mCellIdsToShow;
			};

			// Filter to display the vicinity of a specific problematic region
			class SelectVoronoiVicinityFilter
			{
			public:
				SelectVoronoiVicinityFilter(const GridCoordinates& minCorner, const GridCoordinates& maxCorner, unsigned int extraSpace, const DynamicGraph& graph);

				bool operator () (unsigned int cellIndex, const DistanceGridCell& cell) const;

			private:
				GridCoordinates mMinCorner;
				GridCoordinates mMaxCorner;

				const DynamicGraph& mGraph;
			};

			//--------------------------------------------------
			// Some visualizers that draw data for a single cell
			// Visualizer that draws the a box around the cell outline.
			class CellOutlineVisualizer
			{
			public:
				CellOutlineVisualizer(const GridConfiguration& gridConfig, const Color4& color, float fillOpacity = .5f);

				void operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const;

			private:
				const GridConfiguration& mGridConfig;
				const Color4 mColor;
				const float mFillOpacity;
			};

			// Visualizer that draws a diagonal through the cell just to reduce the number of single lines to draw in case the performance is bad with the outline visualizer
			class CellDiagonalVisualizer
			{
			public:
				CellDiagonalVisualizer(const GridConfiguration& gridConfig, const Color4& color);

				void operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const;

			private:
				const GridConfiguration& mGridConfig;
				const Color4 mColor;
			};

			// Visualizer to draw lines to the influencing obstacles
			class CellInfluenceVisualizer
			{
			public:
				CellInfluenceVisualizer(const DistanceGrid& grid, const Color4& color);

				void operator () (const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const;

			private:
				const DistanceGrid& mGrid;
				const Color4 mColor;
			};

			// Visualizer to display the cell index above each cell
			class CellIndexVisualizer
			{
			public:
				CellIndexVisualizer(const DistanceGrid& grid);

				void operator ()(const DistanceGridCell& cell, const GridCoordinates& coordinates, CompoundDebugDrawRequest& cddr) const;
			private:
				const DistanceGrid& mGrid;
				const Color4 mColor;
			};
		}
	}
}

#include "qsf_ai/voronoi/GridVisualizer-inl.h"
