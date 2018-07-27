// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Radians.h"
#include "qsf_ai/base/NormalizedVector.h"
#include "qsf_ai/base/Line2D.h"

#include <qsf/math/GlmHelper.h>
#include <qsf/math/Math.h>

#include <boost/optional.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>


namespace qsf
{
	namespace area
	{
		// Define the relation of one point to an area.
		// It can be either inside, outside or on the border
		enum PointRelation
		{
			OUTSIDE = -1,
			ON_THE_BORDER = 0,
			INSIDE = 1,
		};
	}

	namespace line
	{
		// Define the relation of one point to a line interpreted as a direction.
		// It can be either left, right or on the line
		enum PointRelation
		{
			LEFT = -1,
			ON_THE_LINE = 0,
			RIGHT = 1,
		};
	}

	template <typename T>
	bool isClose(const T& lhs, const T& rhs, const T& maxDelta)
	{
		return (lhs > rhs) ? (lhs - rhs <= maxDelta) : (rhs - lhs <= maxDelta);
	}

	template <typename T>
	T getSign(T value)
	{
		if (value < static_cast<T>(0))
			return static_cast<T>(-1);
		if (value > static_cast<T>(0))
			return static_cast<T>(1);
		return static_cast<T>(0);
	}

	// Helper function for sorting glm vectors.
	// Each component value is compared and the first that is different found determines whether the lhs is smaller (true) or not (false).
	// Returns an uninitialized value for identical vectors.
	template <typename Vector>
	boost::optional<bool> isSmallerByFirstDifferentSingleComponent(const Vector& lhs, const Vector& rhs)
	{
		for (int component = 0; component < lhs.length(); ++component)
		{
			if (lhs[component] < rhs[component])
				return true;
			if (lhs[component] > rhs[component])
				return false;
		}

		return boost::optional<bool>();
	}

	// Test for any glm vector to be zero
	template <typename Vector>
	bool isZero(const Vector& v)
	{
		for (glm::length_t component = 0; component < v.length(); ++component)
			if (v[component] != static_cast<typename Vector::value_type>(0))
				return false;

		return true;
	}

	// TODO(vs) To be migrated to QSF once we have a proper Line2D
	// Calculate an intersection in 2D between two lines.
	// Return the result as a factor multiplied to the direction of line A.
	boost::optional<float> calculateIntersectionAsDirectionFactor(const Line2D& lineA, const Line2D& lineB);

	// TODO(vs) To be migrated to QSF once we have a proper Line2D
	// Calculate an intersection in 2D between two lines A.
	// Return the result as a position.
	inline boost::optional<glm::vec2> calculateIntersection(const Line2D& lineA, const Line2D& lineB)
	{
		const boost::optional<float> directionFactor = calculateIntersectionAsDirectionFactor(lineA, lineB);
		if (!directionFactor)
			return boost::optional<glm::vec2>();

		return lineA.getOrigin() + (lineA.getDirection() * *directionFactor);
	}

	/**
	* Returns a factor to be multiplied to the direction from lineA to lineB to calculate the closest point on the line to point p.
	*/
	float calculateClosestPointOnLineAsDirectionFactor(const Line2D& line, const glm::vec2& p);
	glm::vec2 calculateClosestPointOnLine(const Line2D& line, const glm::vec2& p);
	glm::vec2 calculateClosestPointOnLineSegment(const Line2D& segment, const glm::vec2& p); // We should introduce a LineSegment2D for this. Treating a 2D line as a segment during this function
	UnsignedFloat calculateSquaredDistanceToLine(const Line2D& line, const glm::vec2& p);
	UnsignedFloat calculateDistanceToLine(const Line2D& line, const glm::vec2& p);

	// Calculate an axis aligned bounding box around the set of vectors provided as a range
	// It is returned as a pair with first being the lower and second being the upper corner
	// The range is both inclusive [first, last]
	template <typename Vector>
	std::pair<Vector, Vector> calculateBoundingBox(const Vector* first, const Vector* last);

	// This function returns any perpendicular when the direction doesn't matter
	template <typename Value>
	glm::tvec2<Value, glm::defaultp> getPerpendicular(const glm::tvec2<Value, glm::defaultp>& v)
	{
		return glm::tvec2<Value, glm::defaultp>(v.y, -v.x);
	}

	template <typename Vector>
	bool isPositionClose(const Vector& lhs, const Vector& rhs, typename Vector::value_type epsilon)
	{
		return getDistanceSquared(lhs, rhs) < epsilon * epsilon;
	}

	template <typename Vector>
	bool isDirectionClose(const Vector& lhs, const Vector& rhs)
	{
		const bool lhsIsZero = (lhs == Vector());
		const bool rhsIsZero = (rhs == Vector());
		if (lhsIsZero && rhsIsZero)
			return true;

		if (lhsIsZero || rhsIsZero)
			return false;

		// Experiments have shown that the necessary epsilon grows as the vectors get smaller.
		// Therefore we define an epsilon based on the vector length here.
		// These epsilons are defined by actual examples that we run into during the EM5 development
		typedef typename Vector::value_type Units;
		const Units desiredLengthSquared = 1e-5f;
		const Units minimaLengthSquared = 1e-9f;

		const Units lhsLengthSquared = getLengthSquared(lhs);
		const Units rhsLengthSquared = getLengthSquared(rhs);
		const Units minimumOfLengthsSquared = std::min(lhsLengthSquared, rhsLengthSquared);
		const Units epsilon = minimumOfLengthsSquared < minimaLengthSquared ? 2e-1f : (minimumOfLengthsSquared < desiredLengthSquared) ? 1e-1f : 1e-2f;

		// copied from glm::normalize because we already needed to calculate the length before to determine the epsilon
		const Vector normalizedLhs = lhs * glm::inversesqrt(lhsLengthSquared);
		const Vector normalizedRhs = rhs * glm::inversesqrt(rhsLengthSquared);

		return isPositionClose(normalizedLhs, normalizedRhs, epsilon);
	}

	// Calculates the average direction
	// Parameters will be normalized internally.
	// Result is not normalized.
	// Returns zero as the result of two zero inputs but also as the result of two completely inverse inputs which is undesired.
	template <typename Vector>
	Vector calculateAverageDirection(const Vector& dir1, const Vector& dir2)
	{
		return (glm::normalize(dir1) + glm::normalize(dir2)) * .5f;
	}

	// Helper functions to convert locally to a higher precision version of the vector data where required
	//@{
	inline glm::tvec2<double, glm::defaultp> getHighPrecisionVersion(const glm::vec2& v)
	{
		return glm::tvec2<double, glm::defaultp>(v);
	}
	inline glm::tvec3<double, glm::defaultp> getHighPrecisionVersion(const glm::vec3& v)
	{
		return glm::tvec3<double, glm::defaultp>(v);
	}
	//@}

	// An own getAngle function that avoids internal errors with a dot product of two normalized vectors being outside the range [-1, 1] due to round-off errors.
	template <typename Vector>
	Radians calculateAngle(const Normalized<Vector>& v1, const Normalized<Vector>& v2)
	{
		if (*v1 == *v2)
			return Radians(0.f);

		// Casting to double locally because we need the precision here.
		// Using floats here lead to disappearing deltas up to 0.05f
		const auto pv1 = getHighPrecisionVersion(*v1);
		const auto pv2 = getHighPrecisionVersion(*v2);

		double projected = dot(pv1, pv2);
		projected = Math::clamp(projected, -1., 1.); // due to round-off errors, it may actually be possible for the values to be slightly outside the range which wreaks havoc when fead as an argument to acos

		return Radians(static_cast<float>(acos(projected)));
	}

	// An own orientedAngle function to avoid the dumb hardcoded 0.01 angle limit within glm.
	// The parameters do not need to be normalized.
	Radians calculateOrientedAngle(const glm::vec2& vec1, const glm::vec2& vec2);

	/**
	* Convert a quaternion to a direction vector dropping the roll aspect.
	*/
	glm::vec3 convertQuaternionToDirectionVector(const glm::quat& orientation);

	/**
	* Returns whether the point is between the two lines.
	* It is defined by intersecting the perpendicular from one line onto the node with the other line.
	* If the intersection is on the same side as the node from the first line the node is defined to be between the two lines.
	*/
	area::PointRelation calculatePointToTwoLinesRelation(const glm::vec2& point, const Line2D& lineA, const Line2D& lineB);

}

#include "qsf_ai/base/Math-inl.h"
