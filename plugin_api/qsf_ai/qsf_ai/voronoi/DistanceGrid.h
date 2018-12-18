// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/voronoi/GridConfiguration.h"
#include "qsf_ai/voronoi/DistanceGridCell.h"
#include "qsf_ai/serialization/SerializationHelper.h"

#include <qsf/platform/PlatformTypes.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

#include <boost/noncopyable.hpp>
#include <boost/scoped_array.hpp>

#include <cstddef>


namespace qsf
{
	namespace ai
	{
		/**
		* Defines a regular 2D grid structure that tracks distances to obstacles during the Voronoi graph creation process.
		* Non copyable because these structures are usually very big.
		*/
		class QSF_AI_API_EXPORT DistanceGrid : public boost::noncopyable
		{
		public:
			friend struct qsf::serialization::serializer<ai::DistanceGrid>; // to allow serializing the data

			DistanceGrid(const GridConfiguration& configuration);

			const GridConfiguration& getConfiguration() const;

			// Accessing a single cell.
			//@{
			const DistanceGridCell& getCell(unsigned int x, unsigned int y) const;
			const DistanceGridCell& getCell(const GridCoordinates& position) const;
			const DistanceGridCell& getCell(unsigned int cellIndex) const;
			DistanceGridCell& getCell(unsigned int x, unsigned int y);
			DistanceGridCell& getCell(const GridCoordinates& position);
			DistanceGridCell& getCell(unsigned int cellIndex);
			DistanceGridCell* tryGetCell(unsigned int x, unsigned int y);
			DistanceGridCell* tryGetCell(const GridCoordinates& position);
			DistanceGridCell* tryGetCell(unsigned int cellIndex);
			const DistanceGridCell* tryGetCell(unsigned int x, unsigned int y) const;
			const DistanceGridCell* tryGetCell(const GridCoordinates& position) const;
			const DistanceGridCell* tryGetCell(unsigned int cellIndex) const;
			//@}

			// Mark a single cell as being directly blocked by an obstacle.
			// This obliterates all older states even a previous obstacle marking.
			//@{
			void setBlocked(const GridCoordinates& position);
			void setBlocked(unsigned int cellIndex);
			void setBlocked(unsigned int x, unsigned int y);
			//@}

			// Returns whether the cell with the given index is actually blocked directly
			//@{
			bool isBlocked(const GridCoordinates& coordinates) const;
			bool isBlocked(unsigned int cellIndex) const;
			//@}

			// Debugging helper function to check for failed distance calculations.
			// Returns true if at least one cell has an uninitialized distance value.
			bool containsUninitializedDistanceValue() const;

			// Sets all distance values to uninitialized again
			void resetDistanceValues();

			// Calculate the distance squared between the two cells with the given indices
			unsigned int calculateDistanceSquaredBetween(unsigned int cellAIndex, unsigned int cellBIndex) const;
			unsigned int calculateDistanceSquaredBetween(const GridCoordinates& cellA, const GridCoordinates& cellB) const;
			// Calculate the distance squared to the closest obstacle detected
			unsigned int calculateClosestDistanceSquared(unsigned int cellIndex) const;
			unsigned int calculateClosestDistanceSquared(const GridCoordinates& coordinates) const;
			// Returns whether cell A is in the 8-connected neighbourhood of another cell
			//@{
			bool isEightAdjacent(unsigned int cellAIndex, unsigned int cellBIndex) const;
			bool isEightAdjacent(const GridCoordinates& cellA, const GridCoordinates& cellB) const;
			//@}

			// Helper for converting between indices and grid coordinates
			//@{
			GridCoordinates convertToCoordinates(unsigned int cellIndex) const;
			unsigned int convertToCellIndex(const GridCoordinates& coordinates) const;
			//@}

			std::size_t calculateMemoryConsumption() const;

		private:
			GridConfiguration mConfiguration;

			// The actual cells making up the grid.
			// Cell for position (X, Y) being at index (mXDimension * Y) + X
			boost::scoped_array<DistanceGridCell> mCells;
		};
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::DistanceGrid>
		{
			static void serialize(qsf::BinarySerializer& serializer, ai::DistanceGrid& grid)
			{
				serializer & grid.mConfiguration;
				const std::size_t numCells = grid.mConfiguration.mDimensions.x * grid.mConfiguration.mDimensions.y;

				if (serializer.isReading())
					grid.mCells.reset(new ai::DistanceGridCell[numCells]);

				// To optimize memory, only the blocked cells are stored as a bitfield.
				// That should be a minimal amount of information to be able to recreate the state
				std::vector<bool> blockedFlags(numCells, false);

				if (serializer.isWriting())
				{
					// Store blocked == whether the own cell is the closest
					for (unsigned int cellIndex = 0; cellIndex < numCells; ++cellIndex)
						blockedFlags[cellIndex] = (grid.getCell(cellIndex).getClosestObstacleCellIndex() == cellIndex);
				}

				serializeVectorOfBools(serializer, blockedFlags); // read or write the vector here

				if (serializer.isReading())
				{
					// Apply the just read blocked flagging to the grid
					for (unsigned int cellIndex = 0; cellIndex < numCells; ++cellIndex)
					{
						if (blockedFlags[cellIndex])
							grid.setBlocked(cellIndex);
					}
				}
			}
		};
	}
}

#include "qsf_ai/voronoi/DistanceGrid-inl.h"
