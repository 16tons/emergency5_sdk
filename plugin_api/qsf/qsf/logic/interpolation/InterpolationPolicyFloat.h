// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/interpolation/InterpolationPolicy.h"
#include "qsf/platform/PlatformTypes.h"		//< int32 et. al.

#include <limits>		//< For the Bezier value finder
#include <algorithm>	//< For the Bezier value finder


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<> struct InterpolationPolicy<float>
	{
		static void interpolate(const float& from, const float& to, float t, float& out)
		{
			out = from + (to - from) * t;
		}

		/**
		*  @brief
		*    Really simple Bezier interpolation algorithm in here. This one could be improved by A LOT in terms of performance.
		*
		*  @param from
		*    The first point to interpolate from, reached at t=0
		*  @param cp0
		*    The value of the control point next to the first point
		*  @param cp1
		*    The value of the second control point, just before the second point
		*  @param to
		*    The second (target) point reached at t=1
		*  @param t
		*    The interpolation vector in the range of [0.0f, 1.0f]
		*  @param[out] out
		*    Interpolated value is written to that point
		*
		*  @todo
		*    - TODO(oi) Have some smart mathematician improve performance of this algorithm
		*/
		static void bezierInterpolation(const float& from, const float& cp0, const float& cp1, const float& to, float t, float& out)
		{
			// This algorithm is effectively de-casteljau style recursive subdivision on a quadratic Bezier curve

			// Calculate midpoint of FROM-CP0 / CP0-CP1 / CP1-TO
			float m0 = 0.0f, m1 = 0.0f, m2 = 0.0f;
			interpolate(from, cp0, t, m0);
			interpolate(cp0, cp1, t, m1);
			interpolate(cp1, to, t, m2);

			// Calculate midpoint of M0-M1, M1-M2
			float k0 = 0.0f, k1 = 0.0f;
			interpolate(m0, m1, t, k0);
			interpolate(m1, m2, t, k1);

			// Finally calculate midpoint of K0-K1
			interpolate(k0, k1, t, out);
		}

		/**
		*  @todo
		*   - TODO(oi) This is the best I can do for proper Bezier interpolation. This algorithm looks for the interpolation
		*     value that can be passed as t to the Bezier interpolator method by analyzing the time axis and doing
		*     a binary search. As you couldn't already tell, that is slow as hell. Someone MIGHT have a clue how
		*     to speed Bezier interpolation up by eliminating this.
		*/
		static float approximateBezierInterpolationTime(float searchedT, float p0t, float cp0t, float cp1t, float p1t)
		{
			static const int32 MAXIMUM_ITERATIONS = 100;
			static const float APPROXIMATION_EPSILON = 1.0e-04f;
			static const float VERYSMALL = 1.0e-20f;

			if (searchedT - p0t < VERYSMALL)
			{
				return 0.0f;
			}

			if (p1t - searchedT < VERYSMALL)
			{
				return 1.0f;
			}

			int32 iterationStep = 0;

			float u = 0.0f;
			float v = 1.0f;

			// Iteratively apply subdivision to approach value atX
			while (iterationStep < MAXIMUM_ITERATIONS)
			{
				// de Casteljau Subdivision
				const float a = (p0t + cp0t) * 0.5f;
				const float b = (cp0t + cp1t) * 0.5f;
				const float c = (cp1t + p1t) * 0.5f;
				const float d = (a + b) * 0.5f;
				const float e = (b + c) * 0.5f;
				const float f = (d + e) * 0.5f;	// This one is on the curve!

				// The curve point is close enough to our wanted atX
				if (std::abs(f - searchedT) < APPROXIMATION_EPSILON)
				{
					return std::max<float>(0.0f, std::min<float>(1.0f, (u + v) * 0.5f));
				}

				// Dichotomy
				if (f < searchedT)
				{
					p0t = f;
					cp0t = e;
					cp1t = c;
					u = (u + v) * 0.5f;
				}
				else
				{
					cp0t = a;
					cp1t = d;
					p1t = f;
					v = (u + v) * 0.5f;
				}

				++iterationStep;
			}

			return std::max(0.0f, std::min(1.0f, (u + v) * 0.5f));
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
