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
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<> struct InterpolationPolicy<glm::quat>
	{
		static void interpolate(const glm::quat& from, const glm::quat& to, float t, glm::quat& out)
		{
			// QSF_ASSERT(nullptr != out, "No nullptr has to be passed to interpolator policies!", QSF_REACT_THROW);	// TODO(co) Why is this commented without a comment why it's commented?

			out = glm::slerp(from, to, t);
		}

		/**
		*  @brief
		*    Bezier interpolating quaternions is really hard! Because of that, there is no implementation here.
		*
		*  @todo
		*    - TODO(oi) This is left to someone who knows his way around 4D-quat-arithmetics. I don't even know how I would represent the tangents in any meaningful way.
		*/
		static void bezierInterpolation(const glm::quat& from, const glm::quat& cp0, const glm::quat& cp1, const glm::quat& to, float t, glm::quat& out)
		{
			interpolate(from, to, t, out);
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
