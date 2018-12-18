// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficLanes/LaneSegmentHelper.h"

#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/GlmHelper.h>
#include <qsf/serialization/binary/BinarySerializer.h>

namespace qsf
{
	namespace ai
	{
		inline unsigned int LaneCollection::getNumLanes() const
		{
			return static_cast<unsigned int>(mLanes.size());
		}

		inline unsigned int LaneCollection::getIntersectingLanesStart(unsigned int lane) const
		{
			return lane == 0 ? 0 : mLanes[lane - 1].getIntersectingLaneEndIndex();
		}

		inline void LaneCollection::addIntersectionIfNecessary(unsigned int laneA, unsigned int laneB)
		{
			addIntersectionUnidirectionalIfNecessary(laneA, laneB);
			addIntersectionUnidirectionalIfNecessary(laneB, laneA);
		}

		template <typename Functor>
		void LaneCollection::visitAllAreas(unsigned int laneIndex, Functor& f) const
		{
			const Lane& lane = getLane(laneIndex);
			const auto& nodes = lane.getNodes();
			unsigned int numNodes = lane.getNumNodes();

			for (unsigned int nodeIndex = 1; nodeIndex < numNodes; ++nodeIndex)
			{
				const LaneNode3D& lastNode = nodes[nodeIndex - 1];
				const LaneNode3D& currentNode = nodes[nodeIndex];

				glm::vec3 points[Lane::NUM_LANE_AREA_DEFINING_POINTS];
				if (!tryCalculatePolygonPoints(lastNode, currentNode, points))
					continue;

				// This call defines the interface for the functor to pass.
				if (!f.accept(nodeIndex - 1, lastNode, currentNode, points))
					break;
			}
		}

		inline LaneCollection::SingleClosestPointOption::SingleClosestPointOption() :
		mDistanceToLane(UncheckedUnsignedFloat(std::numeric_limits<float>::max())),
		mRelationToLane(BEFORE_START)
		{}

		inline bool LaneCollection::SingleClosestPointOption::isInsideIdealLane() const
		{
			return mRelationToLane == IDEAL_BETWEEN_NODES;
		}

		inline void LaneCollection::setLaneChangeOptions(const LaneChangeOptionContainer& options)
		{
			mLaneChangeOptions = options;
		}

		inline const LaneCollection::LaneChangeOptionContainer& LaneCollection::getLaneChangeOptions() const
		{
			return mLaneChangeOptions;
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::LaneCollection>
		{
			inline static void serialize(BinarySerializer& serializer, ai::LaneCollection& lanes)
			{
				serializer.serialize(lanes.mLanes);
				serializer.serialize(lanes.mIntersectingLaneIds);
				serializer.serialize(lanes.mLaneChangeOptions);
			}
		};
	}
}
