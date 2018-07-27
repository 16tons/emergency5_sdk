// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/interpolation/InterpolationPolicy.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<> struct InterpolationPolicy<Color4>
	{
		static void interpolate(const Color4& from, const Color4& to, float t, Color4& out)
		{
			// QSF_ASSERT(nullptr != out, "No nullptr has to be passed to interpolator policies!", QSF_REACT_THROW);	// TODO(co) Why is this commented without a comment why it's commented?

			out = from + (to - from) * t;
		}

		static void interpolate(const Color4& from, const Color4& to, const glm::vec4& t, Color4& out)
		{
			// QSF_ASSERT(nullptr != out, "No nullptr has to be passed to interpolator policies!", QSF_REACT_THROW);	// TODO(co) Why is this commented without a comment why it's commented?

			out.r = from.r + (to.r - from.r) * t.x;
			out.g = from.g + (to.g - from.g) * t.y;
			out.b = from.b + (to.b - from.b) * t.z;
			out.a = from.a + (to.a - from.a) * t.w;
		}

		/**
		*  @todo
		*    - TODO(oi) Rooms for improvement here (like SIMD?)
		*/
		static void bezierInterpolation(const Color4& from, const Color4& cp0, const Color4& cp1, const Color4& to, const glm::vec4& t, Color4& out)
		{
			// Calculate midpoint of FROM-CP0 / CP0-CP1 / CP1-TO
			Color4 m0, m1, m2;
			interpolate(from, cp0, t, m0);
			interpolate(cp0, cp1, t, m1);
			interpolate(cp1, to, t, m2);

			// Calculate midpoint of M0-M1, M1-M2
			Color4 k0, k1;
			interpolate(m0, m1, t, k0);
			interpolate(m1, m2, t, k1);

			// Finally calculate midpoint of K0-K1
			interpolate(k0, k1, t, out);
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
