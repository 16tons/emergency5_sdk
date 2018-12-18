// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	// Simple wrapper for an angle defined in radians.
	// Should later allow automatic conversion to degrees and stay within the range of [0, 2 * Pi)
	class Radians
	{
	public:
		explicit inline Radians(float value) :
			mValue(value)
		{}

		// Automatic and manual conversion to float
		inline operator float() const
		{
			return mValue;
		}

		inline float operator *() const
		{
			return mValue;
		}
	private:
		float mValue;
	};
}
