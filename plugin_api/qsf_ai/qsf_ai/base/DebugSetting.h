// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/time/Time.h>
#include <qsf/math/Color4.h>

#include <boost/optional.hpp>


namespace qsf
{
	namespace ai
	{
		// Very simple helper class that contains the general settings for debugging a specifc AI aspect graphically.
		// It holds the generic debug draw parameters like color and time to draw.
		class DebugParameters
		{
		public:
			DebugParameters(const Color4& color, const Time& time);

			Color4 mColor;
			Time mTime;
		};


		// inline implementation
		inline DebugParameters::DebugParameters(const Color4& color, const Time& time) :
			mColor(color),
			mTime(time)
		{}

		// Graphically debugging is always optional and so the parameters are always used inside an optional construction.
		typedef boost::optional<DebugParameters> DebugSetting;
	}
}
