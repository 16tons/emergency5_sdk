// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <iosfwd>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	// additional helper functionality for working with common glm types
	template <typename Vector>
	Vector getScaled(const Vector& v, const typename Vector::value_type newLength)
	{
		Vector result(v);

		const typename Vector::value_type currentLength = glm::length(v);
		QSF_CHECK(currentLength != 0.0f, "trying to scale a zero vector",
			QSF_REACT_THROW);

		result *= newLength / currentLength;

		return result;
	}

	// The official dot definition of the glm does only allow floating point types.
	// The QSF version does also support integer types.
	template <typename Vector>
	typename Vector::value_type dot(const Vector& lhs, const Vector& rhs)
	{
		typename Vector::value_type result = 0;
		for (glm::length_t position = 0; position < lhs.length(); ++position)
			result += (lhs[position] * rhs[position]);

		return result;
	}

	template <typename Vector>
	typename Vector::value_type getDistanceSquared(const Vector& v1, const Vector& v2)
	{
		return glm::distance2(v1, v2);
	}

	template <typename Vector>
	typename Vector::value_type getDistance(const Vector& v1, const Vector& v2)
	{
		return glm::distance(v1, v2);
	}

	template <typename Vector>
	typename Vector::value_type getSquaredDistance2D(const Vector& v1, const Vector& v2)
	{
		// Ignore height (= y component) differences
		const float xDifference = v2.x - v1.x;
		const float zDifference = v2.z - v1.z;
		return xDifference * xDifference + zDifference * zDifference;
	}

	template <typename Vector>
	typename Vector::value_type getDistance2D(const Vector& v1, const Vector& v2)
	{
		// Ignore height (= y component) differences
		const float xDifference = v2.x - v1.x;
		const float zDifference = v2.z - v1.z;
		return std::sqrt(xDifference * xDifference + zDifference * zDifference);
	}

	template <typename Vector>
	typename Vector::value_type getLengthSquared(const Vector& v)
	{
		return dot(v, v);
	}

	template <typename Vector>
	typename Vector::value_type getLength(const Vector& v)
	{
		return std::sqrt(getLengthSquared(v));
	}

	// Creates a new vector object with the minimum values for each element position
	template <typename Vector>
	Vector getMinimum(const Vector& lhs, const Vector& rhs)
	{
		Vector result;
		for (glm::length_t position = 0; position < lhs.length(); ++position)
			result[position] = std::min(lhs[position], rhs[position]);

		return result;
	}

	// Creates a new vector object with the maximum values for each element position
	template <typename Vector>
	Vector getMaximum(const Vector& lhs, const Vector& rhs)
	{
		Vector result;
		for (glm::length_t position = 0; position < lhs.length(); ++position)
			result[position] = std::max(lhs[position], rhs[position]);

		return result;
	}

	template <typename Vector>
	bool areCollinear(const Vector& v1, const Vector& v2, typename Vector::value_type epsilon = 1e-3f)
	{
		// TODO(vs) Are borderline cases treated correctly?
		// A zero length leads to a division by zero which will become NaN iirc.
		// How does this compare to 1 minus epsilon?

		// TODO(dm) this can be optimized:
		// (getLength(v1)*getLength(v2)) coresponds to
		// std::sqrt(getLengthSquared(v1)*getLengthSquared(v2))
		// saving one std::sqrt
		return std::abs((dot(v1, v2) / (getLength(v1) * getLength(v2)))) > 1 - epsilon;
	}

	inline float getDeterminant(const glm::vec2& lhs, const glm::vec2& rhs)
	{
		return (lhs.x * rhs.y) - (lhs.y * rhs.x);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


// Writing glm::vec3 to a stream for debug output
template <typename T>
std::ostream& operator <<(std::ostream& s, const glm::tvec3<T, glm::defaultp>& v)
{
	return s << "(" << v.x << "," << v.y << "," << v.z << ")";
}

// Writing glm::vec2 to a stream for debug output
template <typename T>
std::ostream& operator <<(std::ostream& s, const glm::tvec2<T, glm::defaultp>& v)
{
	return s << "(" << v.x << "," << v.y << ")";
}
