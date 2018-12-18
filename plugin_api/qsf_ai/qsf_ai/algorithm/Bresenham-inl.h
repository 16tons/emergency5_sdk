// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/CoordinateSystem.h>


namespace qsf
{
	namespace ai
	{
		namespace detail
		{
			// Extracted helper functions to avoid code repetition.
			// Calculates the cell index along one axis for the current value, with the cell intersection in local coordinates to the grid origin and the line goes towards connected and an assumed cellSize of 1
			inline int calculateCellIndex(float current, float connected)
			{
				const float currentCellFactor = current; // with cell size one and local coordinates, this is a direct copy
				const float currentCellFactorRounded = floor(currentCellFactor);
				int currentCellIndex = static_cast<int>(currentCellFactorRounded);
				if (currentCellFactor == currentCellFactorRounded && connected < current)
					--currentCellIndex;
					// Treatment for special cases (mainly unit tests) where we are exactly at a border. We may decide in which cell we start by looking at the target direction

				return currentCellIndex;
			}

			// Core version with local coordinates and a normalized cell size of one
			template <typename Action>
			void calculateIntersectedCells(const GridDelta& startIndices, const glm::vec2& localStartPosition, const GridDelta& goalIndices, const glm::vec2& localGoalPosition, Action& action, UnsignedFloat snapVertexDelta)
			{
				float currentX = localStartPosition.x;
				float currentY = localStartPosition.y;

				const float goalX = localGoalPosition.x;
				const float goalY = localGoalPosition.y;

				int currentXCellIndex = startIndices.x;
				int currentYCellIndex = startIndices.y;

				const int goalXCellIndex = goalIndices.x;
				const int goalYCellIndex = goalIndices.y;

				// (vs) Unable to use std::min /  std::max here because of an ambiguous resolution :-/
				const int minX = currentXCellIndex <= goalXCellIndex ? currentXCellIndex : goalXCellIndex;
				const int maxX = currentXCellIndex > goalXCellIndex ? currentXCellIndex : goalXCellIndex;;
				const int minY = currentYCellIndex <= goalYCellIndex ? currentYCellIndex : goalYCellIndex;
				const int maxY = currentYCellIndex > goalYCellIndex ? currentYCellIndex : goalYCellIndex;

				// determine the cell index increase / decrease
				const int cellDeltaX = (goalXCellIndex > currentXCellIndex) ? 1 : (goalXCellIndex < currentXCellIndex ? -1 : 0);
				const int cellDeltaY = (goalYCellIndex > currentYCellIndex) ? 1 : (goalYCellIndex < currentYCellIndex ? -1 : 0);

				// determine growth factors normalized to the delta x, if deltaX is not zero
				const float deltaX = std::abs(goalX - currentX);
				const float deltaY = std::abs(goalY - currentY);
				const float deltaXFactor = cellDeltaX ? 1.f : 0.f;
				const float deltaYFactor = cellDeltaY ? (cellDeltaX ? (deltaY / deltaX) : 1.f) : 0.f;

				do
				{
					if (currentXCellIndex >= 0 && currentYCellIndex >= 0) // legal coordinates
					{
						const bool continueAlgorithm = action.onGridCellIntersected(GridCoordinates(static_cast<unsigned int>(currentXCellIndex), static_cast<unsigned int>(currentYCellIndex)));
						if (!continueAlgorithm)
							break;
					}

					// break criteria is defined here
					if (currentXCellIndex == goalXCellIndex && currentYCellIndex == goalYCellIndex)
						break;
					// check whether we have left the legal range by accident to avoid an infinite loop
					QSF_CHECK(currentXCellIndex >= minX && currentXCellIndex <= maxX && currentYCellIndex >= minY && currentYCellIndex <= maxY, "Cell index out of range during bresenham traverse",
						break);

					// Initialize intermediate goals with values as if we would directly approach the final goal
					// absolute next values to reach
					float nextXGoal = goalX;
					float nextYGoal = goalY;

					if (cellDeltaX)
					{
						const int nextXCellIndex = currentXCellIndex + cellDeltaX;
						nextXGoal = static_cast<float>(nextXCellIndex + (cellDeltaX < 0 ? 1 : 0)); // add one if we approach the cell from the other side
					}
					if (cellDeltaY)
					{
						const int nextYCellIndex = currentYCellIndex + cellDeltaY;
						nextYGoal = static_cast<float>(nextYCellIndex + (cellDeltaY < 0 ? 1 : 0)); // add one if we approach the cell from the other side
					}

					// deltas towards the next goal
					const float towardsNextXGoal = nextXGoal - currentX;
					const float towardsNextYGoal = nextYGoal - currentY;

					// relative efforts towards the next goal, weighted by relative slope
					float effortTowardsNextXBorder = deltaXFactor ? std::abs(towardsNextXGoal / deltaXFactor) : std::numeric_limits<float>::max();
					float effortTowardsNextYBorder = deltaYFactor ? std::abs(towardsNextYGoal / deltaYFactor) : std::numeric_limits<float>::max();

					const bool increaseX = cellDeltaX && (effortTowardsNextXBorder < effortTowardsNextYBorder + snapVertexDelta);
					const bool increaseY = cellDeltaY && (effortTowardsNextYBorder < effortTowardsNextXBorder + snapVertexDelta);

					QSF_CHECK(increaseX || increaseY, "Logic error during bresenham algorithm",
						QSF_REACT_THROW);

					if (increaseX)
					{
						currentXCellIndex += cellDeltaX;
						if (effortTowardsNextXBorder < effortTowardsNextYBorder) // don't use the snap vertex delta here anymore
							currentX = nextXGoal;
						else // special case - we only increase X because we snapped a very close position
							currentX += (effortTowardsNextYBorder / effortTowardsNextXBorder) * towardsNextXGoal;
					}
					else if (towardsNextXGoal != 0.f) // only increase partially by the relative amount of effort to invest, if there is the need for change at all
						currentX += (effortTowardsNextYBorder / effortTowardsNextXBorder) * towardsNextXGoal;

					if (increaseY)
					{
						currentYCellIndex += cellDeltaY;
						if (effortTowardsNextYBorder < effortTowardsNextXBorder) // don't use the snap vertex delta here anymore
							currentY = nextYGoal;
						else // special case - we only increase X because we snapped a very close position
							currentY += (effortTowardsNextXBorder / effortTowardsNextYBorder) * towardsNextYGoal;
					}
					else if (towardsNextYGoal != 0.f) // only increase partially by the relative amount of effort to invest, if there is the need for change at all
						currentY += (effortTowardsNextXBorder / effortTowardsNextYBorder) * towardsNextYGoal;

				} while (true); // break criteria is that we have reached the target cell and it is contained in the loop body for code reduction reasons
			}
		}

		// Version that takes absolute world positions and the grid configuration as input and whose lines may start anywhere in a cell
		template <typename Action>
		void calculateIntersectedCells(glm::vec2 lineStart, glm::vec2 lineEnd, const GridConfiguration& gridConfig, Action& action, UnsignedFloat snapVertexDelta)
		{
			QSF_CHECK(gridConfig.mCellSize > 0.f, "Unable to calculate bresenham with a zero cellsize",
				QSF_REACT_THROW);

			const glm::vec2 gridOrigin2d = CoordinateSystem::getTopViewProjector().dropValue(gridConfig.mOrigin);

			// make the points local to the origin
			lineStart -= gridOrigin2d;
			lineEnd -= gridOrigin2d;

			// scale so we can work with cell size 1
			lineStart /= gridConfig.mCellSize;
			lineEnd /= gridConfig.mCellSize;

			// determine current cell indices
			int currentXCellIndex = detail::calculateCellIndex(lineStart.x, lineEnd.x);
			int currentYCellIndex = detail::calculateCellIndex(lineStart.y, lineEnd.y);
			// determine the same indices for the goal
			const int goalXCellIndex = detail::calculateCellIndex(lineEnd.x, lineStart.x);
			const int goalYCellIndex = detail::calculateCellIndex(lineEnd.y, lineStart.y);

			return detail::calculateIntersectedCells(GridDelta(currentXCellIndex, currentYCellIndex), lineStart, GridDelta(goalXCellIndex, goalYCellIndex), lineEnd, action, snapVertexDelta);
		}

		// Version which starts with grid coordinates and where the lines start in the middle positions of cells. Avoids some overhead this way
		template <typename Action>
		void calculateIntersectedCells(const GridCoordinates& startIndices, const GridCoordinates& goalIndices, Action& action, UnsignedFloat snapVertexDelta)
		{
			// Convert data to required format
			const GridDelta startIndicesAsDelta(static_cast<int>(startIndices.x), static_cast<int>(startIndices.y));
			const GridDelta goalIndicesAsDelta(static_cast<int>(goalIndices.x), static_cast<int>(goalIndices.y));

			const float startX = static_cast<float>(startIndices.x) + .5f;
			const float startY = static_cast<float>(startIndices.y) + .5f;
			const glm::vec2 startPos(startX, startY);

			const float goalX = static_cast<float>(goalIndices.x) + .5f;
			const float goalY = static_cast<float>(goalIndices.y) + .5f;
			const glm::vec2 goalPos(goalX, goalY);

			return detail::calculateIntersectedCells(startIndicesAsDelta, startPos, goalIndicesAsDelta, goalPos, action, snapVertexDelta);
		}
	}
}
