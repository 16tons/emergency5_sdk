// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/Math.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/SpatialConfiguration.h"
#include "qsf_ai/navigation/MovementQuality.h" // a bit ugly inclusion against the hierarchy
#include "qsf_ai/navigation/TurningConfiguration.h" // - " -

#include <qsf/math/PlanarProjector.h>

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <vector>

namespace qsf
{
	namespace ai
	{
		// Calculates the direction a tangent would have in point on a circle with the given midpoint depending on whether we would look around the circle clockwise or counter clockwise
		glm::vec2 calculateTangentDirection(const glm::vec2& pointOnCircle, const glm::vec2& midPoint, bool clockwiseTurn, const PlanarProjector<float>& projector);

		// Calculate the radius of a circle defined by one tangent point (position and direction) and another tangent point.
		// See drawing in document/personal/vs/CircleRadiusSketch.pdf for details
		// It will return +inf silently if fed with collinear values which don't create a valid circle
		template <typename Vector>
		UnsignedFloat calculateRadiusOfCircle(const Vector& tangentPosition, const Vector& tangentDirection, const Vector& secondTangentPosition);

		// Calculates the two points on the circle whose tangent direction is collinear to the direction passed.
		std::pair<glm::vec2, glm::vec2> calculateTangentPointsOnCircle(const glm::vec2& midPoint, UnsignedFloat radius, const glm::vec2& tangentDirection);

		// Calculate the point on this circle that has the given tangent direction.
		// Variant of the above function that selects among the two options the one that turns into the desired direction.
		// The bool clockwise governs which of the two possible locations is returned by defining the direction in which to move along the circle.
		QSF_AI_API_EXPORT glm::vec2 calculateTangentPointOnCircle(const glm::vec2& midPoint, UnsignedFloat radius, const glm::vec2& desiredDirection, bool clockwiseRotation,
			const PlanarProjector<float>& projector);

		/**
		 * Calculates the angle between two vectors in a given direction (cw or ccw).
		 * The values have to be in coordinates local to the turning midpoint.
		 * The result is always positive.
		 */
		QSF_AI_API_EXPORT Radians calculateAngleInDirection(const glm::vec2& firstPoint, const glm::vec2& secondPoint, bool clockwise, const PlanarProjector<float>& projector);

		/** Calculates the length along the outline of a circle.
		* Input is two points in local coordinates to the circle center, a boolean whether we move around the circle clockwise or counter clockwise and optionally a radius if available.
		* Otherwise the radius is calculated.
		* The two points are ordered so we calculate from the first to the second.
		* Be aware that all calculations are done in 2D, 3D coordinates are projected onto the ground plane and slope on the height axis is ignored.
		*/
		QSF_AI_API_EXPORT UnsignedFloat calculateLengthOnCircle(const glm::vec2& firstPoint, const glm::vec2& secondPoint, bool clockwise, const PlanarProjector<float>& projector,
			boost::optional<UnsignedFloat> radius = boost::optional<UnsignedFloat>());

		// Move a point along the circle outline. Works with global coordinates.
		QSF_AI_API_EXPORT glm::vec2 movePointOnCircle(const glm::vec2& current, const TurningConfiguration& turningConfig, UnsignedFloat distance, const PlanarProjector<float>& projector);

		// Calculates the intersections between two circles in 2D.
		// Return the number of intersection points between 0 and 2.
		// The results are optionally written to the intersection point parameters if these are passed.
		// The epsilon allows treating very close points as a one point solution.
		QSF_AI_API_EXPORT unsigned int calculateCircleCircleIntersection(const glm::vec2& center1, UnsignedFloat radius1, const glm::vec2& center2, UnsignedFloat radius2,
			glm::vec2* intersection1 = nullptr, glm::vec2* intersection2 = nullptr, UnsignedFloat epsilon = 1e-4f);

		// Calculates the intersections between a circle and a line in 2D.
		// Returns the number of intersections found and writes the solutions as direction factors from pointA to pointB to the optional output parameters, if these are provided
		QSF_AI_API_EXPORT unsigned int calculateCircleLineIntersectionAsDirectionFactor(const glm::vec2& center, UnsignedFloat radius, const Line2D& line,
			float* directionFactorSolution1 = nullptr, float* directionFactorSolution2 = nullptr);

		// Variant of the above function that calculates the final points from the direction factors
		QSF_AI_API_EXPORT unsigned int calculateCircleLineIntersection(const glm::vec2& center, UnsignedFloat radius, const Line2D& line,
			glm::vec2* solution1 = nullptr, glm::vec2* solution2 = nullptr);

		/** Calculate support points on a circle segment between the two tangent points passed.
		* Each point is defined by a position and a direction.
		* The outline is approximated by a number of linear line segments.
		* The additional parameter numSupportPointsPerWorldUnit lets you tweak the resolution of this approximation.
		* It defines how many points are per world units of linear distance between points p1 and p2.
		* Inherently assumes that the circle lies primarily in the ground plane and contains a projection to 2D.
		* Always contains p1 and p2 as first and last points in the result.
		* Forward defines whether to follow the direction given by the start and end orientations or in the opposite direction.
		*/
		QSF_AI_API_EXPORT std::vector<glm::vec3> calculatePartialCircleSupportPoints(
			const TurningConfiguration& turningConfig, const glm::vec3& start, const glm::vec3& goal, UnsignedFloat numSupportPointsPerWorldUnit = 4.f);

		// Helper classes that contains all the result variables of the TangentConnection calculation functions.
		//@{
		// Contains the values for a simple tangent connection with only a circle at the starting position and free orientation at the goal point
		class QSF_AI_API_EXPORT SimpleTangentConnectionResult
		{
		public:
			// C'tor for a non-degenerated case with real values
			SimpleTangentConnectionResult(const glm::vec2& leaveCircleAt, const glm::vec2& tangentDirection, UnsignedFloat distanceOnCircle, const TurningConfiguration& turningConfig);
			// C'tor for a case where the entity doesn't need to turn at all
			SimpleTangentConnectionResult(const glm::vec2& leaveCircleAt, const glm::vec2& tangentDirection);

			glm::vec2 mLeaveCircleAt; // equal to final goal position if the entity doesn't need to turn at all
			glm::vec2 mTangentDirection;
			UnsignedFloat mDistanceOnCircle; // zero if the entity doesn't need to turn at all
			boost::optional<TurningConfiguration> mTurningConfiguration; // no configuration set if the entity doesn't need to turn at all
		};

		// Contains the values for a tangent connection between two circles with a fixed orientation at the goal point
		class QSF_AI_API_EXPORT CircleTangentConnectionResult
		{
		public:
			CircleTangentConnectionResult(const SimpleTangentConnectionResult& startCircleData, const SimpleTangentConnectionResult& goalCircleData,
				move::Quality quality, UnsignedFloat totalDistance);

			SimpleTangentConnectionResult mStartCircleData;
			SimpleTangentConnectionResult mGoalCircleData;
			move::Quality mSolutionQuality;
			UnsignedFloat mTotalDistance;
		};
		//@}

		/**
		* Calculate the tangent connection between two circles defined by a start and an end spatial configuration.
		* Both are expected to be on a circle with the defined common radius and we calculate a connection between the two circles that goes straight from one circle to the other.
		* See http://www.gamasutra.com/view/feature/131505/toward_more_realistic_pathfinding.php?page=4 for the problem motivation.
		* It considers moving forward or backward around both circles.
		* The specific flag maxHalfGoalCircle disallows options where we move more than 50 % along the goal circle.
		* The reasoning is that we move in front of the goal location in these cases and this is not desirable in our primary use case.
		* The flag allowShrinkingRadius defines whether options that fail because of the radius are ignored or are investigated with a lowered radius.
		* As an additional constraint we may provide a pair of lines which define a corridor that should not be left during planning.
		*/
		QSF_AI_API_EXPORT boost::optional<CircleTangentConnectionResult> calculateTangentConnectionBetweenCircles(const SpatialConfiguration2D& start, const SpatialConfiguration2D& goal,
			UnsignedFloat turningRadius, bool maxHalfGoalCircle, bool allowShrinkingRadius, const PlanarProjector<float>& projector, const boost::optional<std::pair<Line2D, Line2D>>& corridor = boost::optional<std::pair<Line2D, Line2D>>());

		/**
		* Calculate the tangent connection from the start configuration to the goal.
		* Variant of the above function where there is no specific orientation required in the goal.
		*/
		QSF_AI_API_EXPORT boost::optional<SimpleTangentConnectionResult> calculateTangentConnection(const SpatialConfiguration2D& start, const glm::vec2& goal,
			UnsignedFloat turningRadius, bool allowShrinkingRadius, const PlanarProjector<float>& projector, const boost::optional<std::pair<Line2D, Line2D>>& corridor = boost::optional<std::pair<Line2D, Line2D>>());

		/**
		* calculate the tangent points of lines going through point and being tangents to the circle defined by midpoint and radius.
		* Returns the number of solutions written, either 0, 1 or 2.
		* In case there are one or two solutions, the tp1 and tp2 reference variables are written.
		* Zero solutions means the point is inside the circle.
		* One solution is returned if the point is on the circle or the radius is zero.
		* The epsilon is used as a margin for distance when the point snaps onto the circle.
		*/
		QSF_AI_API_EXPORT unsigned int calculateTangentPoints(const glm::vec2& circleCenter, UnsignedFloat radius, const glm::vec2& point, glm::vec2& tp1, glm::vec2& tp2, float epsilon = 1e-6f);
	}
}

#include "qsf_ai/base/CircleMath-inl.h"
