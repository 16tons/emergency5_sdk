// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/DebugSetting.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Percentage.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/worldModel/trafficLanes/LaneNode.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/navigation/MovementQuality.h"
#include "qsf_ai/worldModel/trafficLanes/PathUnderConstruction.h"
#include "qsf_ai/navigation/pathfinding/smoothing/EdgePortal.h"
#include "qsf_ai/worldModel/WorldElementStateCollection.h"

#include <qsf/math/PlanarProjector.h>

#include <boost/optional.hpp>

#include <vector>


namespace qsf
{
	class CompoundDebugDrawRequest;

	// The functionality defined in this file often serves a double purpose.
	// It tests whether some element can be used given a specific turning restriction of some entity.
	// To do this they all have to calculate the points where the entity needs to move.
	// So the same functions are used to retrieve the calculated positions when it comes to creating the final path at the end as well as checks which nodes to expand during pathfinding at all.
	namespace ai
	{
		class LaneCollection;

		typedef SpatialConfiguration<glm::vec3> TurningPoint;

		// Minor helper class for results to curve planning queries that are provided in this file.
		// These curves are always defined by three points
		class QSF_AI_API_EXPORT CurveTurningResult
		{
		public:
			CurveTurningResult(move::Quality quality, const TurningConfiguration& turningConfiguration, Percentage curveStartFactor, Percentage curveEndFactor);

			move::Quality mResultQuality; // The quality of the movement which means basically which constraints are violated
			Percentage mCurveStartFactor; // a factor to be applied to the segment from first to second (middle) point of the curve that defines where the curve starts
			Percentage mCurveEndFactor; // a factor to be applied to the segment from second (middle) to third point that defines where the curve ends
			TurningConfiguration mTurningConfig; // The curve configuration itself

			// Comfort function to convert the factors to a spatial configuration in two or three dimensions
			template <typename Vector>
			SpatialConfiguration<Vector> calculateTurningPosition(const Vector& from, const Vector& to, Percentage factor) const;
		};

		/**
		* Calculate the actual free space needed when taking a turn with the given radius and turning constraint at the outer side of the curve.
		* This scales between lateral and max free space needed depending on the maximum translation when using the curve.
		*/
		UnsignedFloat calculateFreeSpaceNeeded(const TurningConstraint& constraint, UnsignedFloat radius);

		/** Determines whether a single curve defined by three consecutive lane nodes in 2D can be used by some entity with the given movement restriction
		* The bool allowReducingTheTurningRadius defines whether the turning radius is a hard or soft constraint.
		* In the latter case, it may actually create results that don't pay attention to the radius if there is no such solution.
		*/
		CurveTurningResult QSF_AI_API_EXPORT calculateTurningPointsForCurve2D(const glm::vec2& start, const LaneNode2D& middle, const glm::vec2& end, const TurningConstraint& turningConstraint, const PlanarProjector<float>& projector);

		/** 3D variant of calculation for a single curve.
		*/
		CurveTurningResult calculateTurningPointsForCurve3D(const glm::vec3& curveStart3D, const LaneNode3D& curveMiddle3D, const glm::vec3& curveEnd3D, const TurningConstraint& turningConstraint);

		/** Calculate the portal data for a turning configuration.
		* The portals are currently not optimized for the upcoming funnel algorithm but remain a roughly correct sketch.
		* The first portal is the ending portal for the part from start to middle while the second portal is the beginning portal for the part from middle to end.
		*/
		std::pair<EdgePortal, EdgePortal> calculateSegmentPortals(const glm::vec3& curveStart3D, const LaneNode3D& curveMiddle3D, const glm::vec3& curveEnd3D, const TurningConstraint& constraints);

		/** Tests whether a single lane can be used by an entity with the given the turning restriction.
		* Tests all the inter segment transitions in succession.
		*/
		move::Quality canLaneBeUsed(const LaneCollection& lanes, unsigned int laneIndex, const TurningConstraint& turningConstraint, CompoundDebugDrawRequest* drawBlockedParts = nullptr);

		/** Tests whether a partial single lane can be used.
		* Variant of the canLaneBeUsed call that allows testing only a partial lane.
		* The part to be used is defined by the id of an entered segment and a boolean flag for the direction in which we follow the lane.
		* This has nothing to do with the orientation of the entity.
		*/
		move::Quality canPartialLaneBeUsed(const LaneCollection& lanes, const SearchContactPoint& sep, unsigned int connectedNodeId, const TurningConstraint& turningConstraint, CompoundDebugDrawRequest* drawBlockedParts = nullptr);
		move::Quality canPartialLaneBeUsed(const LaneCollection& lanes, unsigned int laneIndex, unsigned int firstNodeToTest, unsigned int lastNodeToTest, const TurningConstraint& turningConstraint, CompoundDebugDrawRequest* drawBlockedParts = nullptr);

		/** Tests whether a single transition between two lane ends can be used by some entity with the given movement restriction.
		* The previous position is passed as relative offset from the node position.
		* The optional output maneuver space is written independent of the return value if it is provided at all.
		*/
		move::Quality canTransitionBeUsed(const LaneCollection& lanes, unsigned int middleNodeId, const glm::vec3& previousPositionOffset, unsigned int followingLaneId,
			const PathSearchConfiguration& searchConfig, UnsignedFloat* maneuverSpace = nullptr);

		/** Tests the lanes(s) passed whether these can be used by an entity with the given the turning restriction.
		* Stores the result with the given constraint index.
		*/
		void calculateUsabilityForAllLanes(LaneCollection& lanes, const TurningConstraint& turningConstraint, unsigned int constraintIndex);
		void calculateUsabilityForLane(LaneCollection& lanes, unsigned int laneId, const TurningConstraint& turningConstraint, unsigned int constraintIndex, CompoundDebugDrawRequest* debugDrawCollector = nullptr);

		/** Writes the gateways that define the part of a lane that is crossed when using the search entry point towards the connected node into the reference parameter vector.
		* Intermediate step of the smoothing algorithm that collects all nodes first.
		* Used for collecting the nodes from the start and finish parts.
		* The bool towards contact point signals whether we are using the search entry point as starting or goal role.
		* Returns the supporting portal which is the one just behind the first node written.
		*/
		DynamicPortal writeNodesForPartialLane(const LaneCollection& lanes, const SearchContactPoint& searchEntryPoint, unsigned int connectedNodeId,
			bool towardsContactPoint, bool onPrimaryMap, bool movingForward, const PathSearchConfiguration& searchConfig, PathUnderConstruction& result);

		/** Variant of the above function that collects the nodes used in a case where neither lane node is actually reached.
		* Both search entry points need to indicate the same lane for this function to work.
		* Returns both supporting portals, first is the one on the start side while second is the one on the goal side.
		*/
		std::pair<DynamicPortal, DynamicPortal> writeNodesForPartialLane(const LaneCollection& lanes, const SearchContactPoint& start, const SearchContactPoint& goal,
			bool onPrimaryMap, bool movingForward, const PathSearchConfiguration& searchConfig, PathUnderConstruction& result);

		/** Writes the temporary path nodes that define a lane into the reference parameter vector.
		* Intermediate step of the smoothing algorithm that collects all nodes first.
		* Used for collecting the nodes from the whole lanes that are crossed.
		* The bool towards end defines in which direction we are moving along the lane - towards the end node or the start node.
		* The id of the node we move towards determines in which direction we move along the lane.
		* It needs to be one of the end nodes of the lane obviously.
		* The bool onPrimaryMap is mainly needed to channel the information into the waypoints written.
		*/
		void writeAllNodesForLane(const LaneCollection& lanes, unsigned int laneIndex, unsigned int towardsNodeId, bool moveForward, bool onPrimaryMap, PathUnderConstruction& result);


		// inline implementation of helper classes
		inline CurveTurningResult::CurveTurningResult(move::Quality quality, const TurningConfiguration& turningConfiguration, Percentage curveStartFactor, Percentage curveEndFactor) :
		mResultQuality(quality),
		mCurveStartFactor(curveStartFactor),
		mCurveEndFactor(curveEndFactor),
		mTurningConfig(turningConfiguration)
		{}

		template <typename Vector>
		SpatialConfiguration<Vector> CurveTurningResult::calculateTurningPosition(const Vector& from, const Vector& to, Percentage factor) const
		{
			const Vector pos = from + (to - from) * *factor;
			const Vector dir = to - from;
			return SpatialConfiguration<Vector>(pos, dir);
		}
	}
}
