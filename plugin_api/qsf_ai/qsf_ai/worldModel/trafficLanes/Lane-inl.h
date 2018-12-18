// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/MemoryDebugHelper.h"

#include <qsf/base/error/ErrorHandling.h>
#include <qsf/base/GetUninitialized.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline Lane::Lane() :
			mStartNodeId(getUninitialized(mStartNodeId)),
			mEndNodeId(getUninitialized(mEndNodeId)),
			mLength(getUninitialized<UnsignedFloat>()),
			mUsableByEntityConfigurationFlags(true),
			mTypeId(getUninitialized(mTypeId)),
			mIntersectingLaneEndIndex(getUninitialized(mIntersectingLaneEndIndex)),
			mAxisAlignedBoundingBox(),
			mNodesInBoundingBox(0)
		{
			mAxisAlignedBoundingBox.setToNumericLimit();
		}

		inline Lane::Lane(unsigned int startNode, unsigned int endNode, const std::vector<LaneNode3D>& positions, UnsignedFloat length, unsigned int typeId) :
			mStartNodeId(startNode),
			mEndNodeId(endNode),
			mPositions(positions),
			mLength(length),
			mTypeId(typeId),
			mIntersectingLaneEndIndex(0),
			mAxisAlignedBoundingBox(),
			mNodesInBoundingBox(0)
		{
			mAxisAlignedBoundingBox.setToNumericLimit();
		}

		inline const std::vector<LaneNode3D>& Lane::getNodes() const
		{
			return mPositions;
		}

		inline unsigned int Lane::getNumNodes() const
		{
			return static_cast<unsigned int>(mPositions.size());
		}

		inline unsigned int Lane::getStartNodeId() const
		{
			return mStartNodeId;
		}

		inline unsigned int Lane::getEndNodeId() const
		{
			return mEndNodeId;
		}

		inline unsigned int Lane::getTypeId() const
		{
			return mTypeId;
		}

		inline UnsignedFloat Lane::getLength() const
		{
			return mLength;
		}

		inline unsigned int Lane::getIntersectingLaneEndIndex() const
		{
			return mIntersectingLaneEndIndex;
		}

		inline void Lane::onIntersectionAdded()
		{
			++mIntersectingLaneEndIndex;
		}

		inline void Lane::onIntersectionRemoved()
		{
			QSF_CHECK(mIntersectingLaneEndIndex > 0, "Index out of range when trying to adapt intersected element counter",
				QSF_REACT_THROW);

			--mIntersectingLaneEndIndex;
		}

		inline unsigned int Lane::getOtherEndNodeId(unsigned int nodeId) const
		{
			if (nodeId == mStartNodeId)
				return mEndNodeId;
			if (nodeId == mEndNodeId)
				return mStartNodeId;

			QSF_ERROR("The node id passed is no end node for this lane, unable to determine opposing end node",
				QSF_REACT_THROW);
		}

		inline bool Lane::connects(unsigned int nodeA, unsigned int nodeB, direction::OneDimensional dir) const
		{
			// Check regular direction usage
			if (dir != direction::BACKWARD && mStartNodeId == nodeA && mEndNodeId == nodeB)
				return true;

			// backwards - check opposite direction usage
			return (dir != direction::FORWARD && mStartNodeId == nodeB && mEndNodeId == nodeA);
		}

		inline bool Lane::canBeUsedByEntityConfiguration(unsigned int index) const
		{
			return mUsableByEntityConfigurationFlags.get(index);
		}

		inline void Lane::setUsableByEntityConfiguration(unsigned int index, bool value)
		{
			mUsableByEntityConfigurationFlags.set(index, value);
		}

		inline void Lane::tweakIntersectiongLaneEndIndex(int delta)
		{
			const int newLaneConnectionEndIndex = static_cast<int>(mIntersectingLaneEndIndex) + delta;
			QSF_CHECK(newLaneConnectionEndIndex >= 0, "Connection index out of range",
				QSF_REACT_THROW);

			mIntersectingLaneEndIndex = static_cast<unsigned int>(newLaneConnectionEndIndex);
		}

		inline const AxisAlignedBoundingBox& Lane::getAxisAlignedBoundingBox() const
		{
			if (mNodesInBoundingBox != getNumNodes())
			{
				// We need to update the BoundingBox, we are not "really" changing the data, if it was inconsistent in the first place
				const_cast<Lane*>(this)->updateAxisAlignedBoundingBox();
			}

			return mAxisAlignedBoundingBox;
		}

		inline void Lane::updateAxisAlignedBoundingBox()
		{
			const UnsignedFloat offset = 0.2f;	// Offset is added to our bounding box

			mNodesInBoundingBox = 0;
			mAxisAlignedBoundingBox.setToNumericLimit();

			for (const LaneNode3D& node : getNodes())
			{
				const glm::vec3 nodeCenter(node.mPosition);
				const glm::vec3 nodeHalfExtent(node.mFreeSpace + offset, offset, node.mFreeSpace + offset);

				AxisAlignedBoundingBox nodeAABB(nodeCenter - nodeHalfExtent, nodeCenter + nodeHalfExtent);

				if (mNodesInBoundingBox == 0)
				{
					mAxisAlignedBoundingBox = nodeAABB;
				}
				else
				{
					mAxisAlignedBoundingBox.combineAABoxes(nodeAABB);
				}
				mNodesInBoundingBox++;
			}
		}

		inline void Lane::eraseState()
		{
			setUninitialized(mStartNodeId);
			setUninitialized(mEndNodeId);
			mPositions.clear();
			mLength = 0.f;
			// mUsableByEntityConfigurationFlags will be recalculated later on
			setUninitialized(mTypeId);
			// mIntersectingLaneEndIndex; needs to be tweaked externally, should be equal to predecessor end index
			mAxisAlignedBoundingBox.setToNumericLimit();
			mNodesInBoundingBox = 0;
		}

		inline std::size_t Lane::calculateMemoryConsumption() const
		{
			std::size_t allocatedMemory = 0;

			allocatedMemory += sizeof(mStartNodeId);
			allocatedMemory += sizeof(mEndNodeId);
			allocatedMemory += memory::calculateMemoryConsumption(mPositions);
			allocatedMemory += sizeof(mLength);
			allocatedMemory += sizeof(mUsableByEntityConfigurationFlags);
			allocatedMemory += sizeof(mTypeId);
			allocatedMemory += sizeof(mIntersectingLaneEndIndex);
			allocatedMemory += sizeof(mAxisAlignedBoundingBox);
			allocatedMemory += sizeof(mNodesInBoundingBox);

			return allocatedMemory;
		}
	}

	// specialization for binary serialization
	namespace serialization
	{
		template <>
		struct serializer<ai::Lane>
		{
			inline static void serialize(BinarySerializer& serializer, ai::Lane& lane)
			{
				serializer & lane.mStartNodeId;
				serializer & lane.mEndNodeId;
				serializer & lane.mPositions;
				serializer & lane.mLength;
				serializer & lane.mTypeId;
				serializer & lane.mIntersectingLaneEndIndex;

				// The bounding box is not serialized, it can be calculated fast and cashed
				// serializer & lane.mAxisAlignedBoundingBox
				// serializer & lane.mNodesInBoundingBox
			}
		};
	}
}
