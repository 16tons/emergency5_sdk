// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<class T> struct InterpolationPolicy
	{
		/**
		*  @brief
		*    Simple, default implementation: does not "interpolate" at all but just forward the "from" data to the output data
		*/
		static void interpolate(const T& from, const T& to, float t, T& out)
		{
			out = from;
		}

		/**
		*  @brief
		*    Default implementation for Bezier interpolation
		*/
		static void bezierInterpolation(const T& from, const T& outgoingTangent, const T& incomingTangent, const T& to, float t, T* out)
		{
			*out = from;
		}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
