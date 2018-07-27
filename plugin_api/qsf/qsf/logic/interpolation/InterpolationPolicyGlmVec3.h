// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/interpolation/InterpolationPolicy.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<> struct InterpolationPolicy<glm::vec3>
	{
		static void interpolate(const glm::vec3& from, const glm::vec3& to, float t, glm::vec3& out)
		{
			out = from + (to - from) * t;
		}

		// Overload for the Bezier midpoint interpolation evaluation
		static void interpolate(const glm::vec3& from, const glm::vec3& to, const glm::vec3& t, glm::vec3& out)
		{
			out = from + (to - from) * t;
		}

		/**
		*  @note
		*    Each element in the vectors is treated as an independent curve, that's also the reason why the time parameter is a vector, too.
		*    The reason for this decision is that this is the only way to properly resolve a Bezier curve as long as
		*    the time t does not line up with the interpolation vector s
		*
		*  @todo
		*    - TODO(oi) Rooms for improvement here (like SIMD?)
		*/
		static void bezierInterpolation(const glm::vec3& from, const glm::vec3& cp0, const glm::vec3& cp1, const glm::vec3& to, const glm::vec3& s, glm::vec3& out)
		{
			// Calculate midpoints of FROM-CP0 / CP0-CP1 / CP1-TO
			glm::vec3 m0, m1, m2;
			interpolate(from, cp0, s, m0);
			interpolate(cp0, cp1, s, m1);
			interpolate(cp1, to, s, m2);

			// Calculate midpoints of M0-M1, M1-M2
			glm::vec3 k0, k1;
			interpolate(m0, m1, s, k0);
			interpolate(m1, m2, s, k1);

			// Finally calculate midpoints of K0-K1
			interpolate(k0, k1, s, out);
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
