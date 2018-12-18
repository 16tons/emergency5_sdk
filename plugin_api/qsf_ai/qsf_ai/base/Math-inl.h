// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/CoordinateSystem.h>
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	inline glm::vec3 convertQuaternionToDirectionVector(const glm::quat& orientation)
	{
		return orientation * CoordinateSystem::getIn();
	}

	inline boost::optional<float> calculateIntersectionAsDirectionFactor(const Line2D& lineA, const Line2D& lineB)
	{
		const glm::vec2& deltaLineA = lineA.getDirection();
		const glm::vec2& deltaLineB = lineB.getDirection();

		const glm::vec2 perpDeltaLineB(deltaLineB.y, -deltaLineB.x);
		const float determinant = glm::dot(deltaLineA, perpDeltaLineB);
		if (determinant == 0.f)
			return boost::optional<float>();

		const glm::vec2 deltaFirstNodes = lineB.getOrigin() - lineA.getOrigin();
		return glm::dot(deltaFirstNodes, perpDeltaLineB) / determinant;
	}

	inline float calculateClosestPointOnLineAsDirectionFactor(const Line2D& line, const glm::vec2& p)
	{
		const glm::vec2 perpToLine = getPerpendicular(line.getDirection());
		return *calculateIntersectionAsDirectionFactor(line, Line2D::fromOriginAndDirection(p, perpToLine));
	}

	inline glm::vec2 calculateClosestPointOnLine(const Line2D& line, const glm::vec2& p)
	{
		const float factor = calculateClosestPointOnLineAsDirectionFactor(line, p);
		return line.getOrigin() + (line.getDirection()) * factor;
	}

	inline UnsignedFloat calculateSquaredDistanceToLine(const Line2D& line, const glm::vec2& p)
	{
		return UncheckedUnsignedFloat(getDistanceSquared(calculateClosestPointOnLine(line, p), p));
	}

	inline UnsignedFloat calculateDistanceToLine(const Line2D& line, const glm::vec2& p)
	{
		return UncheckedUnsignedFloat(getDistance(calculateClosestPointOnLine(line, p), p));
	}

	inline glm::vec2 calculateClosestPointOnLineSegment(const Line2D& segment, const glm::vec2& p)
	{
		const float factor = calculateClosestPointOnLineAsDirectionFactor(segment, p);
		// dedicated handling of borders instead of clamp to avoid round-off errors
		if (factor <= 0.f)
			return segment.getOrigin();
		if (factor >= 1.f)
			return segment.getOrigin() + segment.getDirection();

		return segment.getOrigin() + (segment.getDirection()) * factor;
	}

	namespace detail
	{
		// See https://gist.github.com/volkansalma/2972237 and http://www.dspguru.com/dsp/tricks/fixed-point-atan2-with-self-normalization

		#define PI_FLOAT 3.14159265f
		#define PIBY2_FLOAT 1.5707963f

		inline float atan2_approximation2(float y, float x)
		{
			if (x == 0.0f)
			{
				if (y > 0.0f)
					return PIBY2_FLOAT;
				else if (y == 0.0f)
					return 0.0f;
				else
					return -PIBY2_FLOAT;
			}

			float atan;
			float z = y/x;
			if (fabs(z) < 1.0f)
			{
				atan = z / (1.0f + 0.28f*z*z);
				if (x < 0.0f)
				{
					if (y < 0.0f)
						return atan - PI_FLOAT;
					else
						return atan + PI_FLOAT;
				}
			}
			else
			{
				atan = PIBY2_FLOAT - z / (z*z + 0.28f);
				if (y < 0.0f)
					return atan - PI_FLOAT;
			}
			return atan;
		}
	}

	inline Radians calculateOrientedAngle(const glm::vec2& vec1, const glm::vec2& vec2)
	{
		// vec2 rotated by 90°
		const glm::vec2 rotatedVec2(vec2.y, -vec2.x);

		// Direct dot product = cosine of angle
		const float sharedPart = glm::dot(vec1, vec2);

		// Dot product of vec1 with rotated vec2 = sine of angle
		const float orthogonalPart = glm::dot(vec1, rotatedVec2);

		// Use an approximation for atan2
		return Radians(detail::atan2_approximation2(orthogonalPart, sharedPart));
	}

	template <typename Vector>
	std::pair<Vector, Vector> calculateBoundingBox(const Vector* first, const Vector* last)
	{
		QSF_CHECK(first, "Nullptr not allowed as vector range value",
			QSF_REACT_THROW);

		QSF_CHECK(last, "Nullptr not allowed as vector range value",
			QSF_REACT_THROW);

		std::pair<Vector, Vector> result;
		// Initialize with extreme values
		for (glm::length_t i = 0; i < first->length(); ++i)
			result.first[i] = std::numeric_limits<typename Vector::value_type>::max();

		for (glm::length_t i = 0; i < first->length(); ++i)
			result.second[i] = -std::numeric_limits<typename Vector::value_type>::max();

		for (const Vector* v = first; v <= last; ++v)
		{
			for (glm::length_t i = 0; i < first->length(); ++i)
			{
				result.first[i] = std::min(result.first[i], (*v)[i]);
				result.second[i] = std::max(result.second[i], (*v)[i]);
			}
		}

		return result;
	}

	inline area::PointRelation calculatePointToTwoLinesRelation(const glm::vec2& point, const Line2D& lineA, const Line2D& lineB)
	{
		const glm::vec2 lineAPerp = getPerpendicular(lineA.getDirection());
		const Line2D lineThroughPoint = Line2D::fromOriginAndDirection(point, lineAPerp);
		const auto intersectionTowardsPoint = calculateIntersectionAsDirectionFactor(lineThroughPoint, lineA);
		QSF_CHECK(intersectionTowardsPoint, "Unable to intersect a line with its perpendicular",
			QSF_REACT_THROW);

		if (*intersectionTowardsPoint == 0.f)
			return area::ON_THE_BORDER;

		const auto intersectionTowardsLineB = calculateIntersectionAsDirectionFactor(lineThroughPoint, lineB);
		if (!intersectionTowardsLineB)
			return area::OUTSIDE;

		if (*intersectionTowardsLineB == 0.f)
			return area::ON_THE_BORDER;

		// else return inside if the two intersection factors point to different sides from the point
		return (*intersectionTowardsPoint > 0.f != *intersectionTowardsLineB > 0) ? area::INSIDE : area::OUTSIDE;
	}
}
