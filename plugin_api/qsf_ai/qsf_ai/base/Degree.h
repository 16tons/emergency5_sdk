// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	// Simple wrapper for an angle defined in degree.
	// Should later allow automatic conversion to radians and stay within the range of [0, 360)
	class Degree
	{
	public:
		explicit Degree(float value);

		// Automatic and manual conversion to float
		operator float() const;
		float operator *()const;
	private:
		float mValue;
	};


	// inline implementation
	inline Degree::Degree(float value) :
		mValue(value)
	{}

	inline Degree::operator float() const
	{
		return mValue;
	}

	inline float Degree::operator *() const
	{
		return mValue;
	}
}
