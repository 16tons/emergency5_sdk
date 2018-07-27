// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficLanes/Lane.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"
#include "qsf_ai/worldModel/dynamicUpdate/ObstructingCollision.h"

#include <glm/fwd.hpp>


// file contains helper functionality for a segment of a single lane inside a traffic lane world

namespace qsf
{
	namespace ai
	{
		// Tiny helper class just combining a position and a segment index to avoid code repetition.
		// It describes an end for an lane that is only partially used by combining the exact end point and the entered segment.
		class LaneBound
		{
		public:
			LaneBound(unsigned int segmentId, const glm::vec3& position);

			unsigned int mSegmentId;
			glm::vec3 mPosition;
		};

		/* Calculate the four outer points of the trapezoid defined by two lane nodes.
		* By convention nodes [0] and [1] define the starting node edge while [2] and [3] define the end node edge
		* Returns false if it is unable to actually calculate a polygon, typically from a degenerated segment of two equal points
		*/
		bool tryCalculatePolygonPoints(const LaneNode3D& startNode, const LaneNode3D& endNode, glm::vec3 points[Lane::NUM_LANE_AREA_DEFINING_POINTS]);

		/*
		* Calculate the free space along the lane segment.
		*/
		UnsignedFloat calculateFreeSpaceAtIdealLane(const LaneNode3D& startNode, const LaneNode3D& endNode, Percentage lineFactor);

		/* Calculate the free space at the closest point on the lane between start and end.
		* The optional lane direction factor parameter allows returning where the closest point is on the segment.
		* It is written as as a factor to be multiplied to the direction from start to end.
		* The information is available anyway during the algorithm and needed by its client.
		* Be aware that the raw, unclamped value is written which may be outside the range [0, 1]
		*/
		UnsignedFloat calculateFreeSpaceAtIdealLane(const LaneNode3D& startNode, const LaneNode3D& endNode, const glm::vec3& closePoint, float* laneDirectionFactor = nullptr);

		/** Projects points p1 and p2 onto the line defined by start and goal.
		* Returns whether the movement from the projected p1 to the projected p2 is along the lane (from start to goal) or not.
		*/
		bool isMovementAlongIdealLane(const LaneNode3D& laneStart, const LaneNode3D& laneEnd, const glm::vec3& p1, const glm::vec3& p2);

		// Helper class encapsulating the result values of the estimation function below.
		// These are all the distance parts from a start to a goal along a specific lane.
		class FunnelDistanceEstimationResult
		{
		public:
			UnsignedFloat mDistanceFromStartToLane;
			UnsignedFloat mDistanceAlongLane;
			UnsignedFloat mRemainingDistanceToGoal;

			// Comfort access to the sum of the three distances contained
			UnsignedFloat getSummedDistances() const;
		};

		/**
		* Estimate the distance traveled along the laneNodes in the array given that we ultimately want to reach goal from start.
		* It roughly estimates what an ideal funnel smoothing would but as it is used inside the AStar as estimation function for a lot of nodes it needs to be much quicker than the regular algorithm.
		* It is therefore by design a very simple algorithm mimicking the funnel smoothing idea.
		* The nodes to use are defined by the interval [firstIndex, lastIndex] in the laneNodes array.
		* The direction is defined by whether endIndex is smaller or greater than beginIndex.
		*/
		FunnelDistanceEstimationResult estimateFunnelSmoothedDistanceAlongLane(const glm::vec3& start, const LaneNode3D* laneNodes, unsigned int firstIndex, unsigned int lastIndex, const glm::vec3& goal);

		/**
		* Retrieves the height value by linearly interpolating along the lane.
		*/
		float getHeightAlongLaneSegment(const LaneNode3D& start, const LaneNode3D& goal, float directionFactor);

		/**
		* Tests whether an obstructing obstacle is completely in front / behind a reference point given the ideal lane.
		* This is used for cases where we start and/or end inside this segment so we need to be clear which part of the segment is actually blocked.
		* All points are projected onto the ideal lane for this decision.
		* This is relevant for example to decide whether we need to take care of an obstacle or not.
		* The flag isUpperBound determines whether the lane bound is an upper or lower bound.
		*/
		bool isObstacleCompletelySeparated(const LaneNode3D* nodes, const ObstructingCollision& collision, const LaneBound& segmentBound, bool isUpperBound);


		// inline implementation of the helper class
		inline LaneBound::LaneBound(unsigned int segmentId, const glm::vec3& position) :
			mSegmentId(segmentId),
			mPosition(position)
		{}
	}
}
