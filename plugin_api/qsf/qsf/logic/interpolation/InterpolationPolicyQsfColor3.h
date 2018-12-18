// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/interpolation/InterpolationPolicy.h"
#include "qsf/math/Color3.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<> struct InterpolationPolicy<Color3>
	{
		static void interpolate(const Color3& from, const Color3& to, float t, Color3& out)
		{
			// QSF_ASSERT(nullptr != out, "No nullptr has to be passed to interpolator policies!", QSF_REACT_THROW);	// TODO(co) Why is this commented without a comment why it's commented?

			out = from + (to - from) * t;
		}

		static void interpolate(const Color3& from, const Color3& to, const glm::vec3& t, Color3& out)
		{
			// QSF_ASSERT(nullptr != out, "No nullptr has to be passed to interpolator policies!", QSF_REACT_THROW);	// TODO(co) Why is this commented without a comment why it's commented?

			out.r = from.r + (to.r - from.r) * t.x;
			out.g = from.g + (to.g - from.g) * t.y;
			out.b = from.b + (to.b - from.b) * t.z;
		}

		/**
		*  @todo
		*    - TODO(oi) Rooms for improvement here (like SIMD?)
		*/
		static void bezierInterpolation(const Color3& from, const Color3& cp0, const Color3& cp1, const Color3& to, const glm::vec3& s, Color3& out)
		{
			// Calculate midpoint of FROM-CP0 / CP0-CP1 / CP1-TO
			Color3 m0, m1, m2;
			interpolate(from, cp0, s, m0);
			interpolate(cp0, cp1, s, m1);
			interpolate(cp1, to, s, m2);

			// Calculate midpoint of M0-M1, M1-M2
			Color3 k0, k1;
			interpolate(m0, m1, s, k0);
			interpolate(m1, m2, s, k1);

			// Finally calculate midpoint of K0-K1
			interpolate(k0, k1, s, out);
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
