// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/math/Math.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{

	inline UncheckedPercentage::UncheckedPercentage(float value) :
		value(value)
	{}

	inline Percentage Percentage::clampToPercentage(float value)
	{
		return Percentage(glm::clamp(value, 0.0f, 1.0f));
	}

	inline Percentage::Percentage() :
		mValue(0.f)
	{}

	inline Percentage::Percentage(float value) :
		mValue(Math::clamp(value, 0.f, 1.f))
	{
		QSF_CHECK(value >= 0.f && value <= 1.f, "Invalid percentage float value " << value << " clamped to " << mValue,
			QSF_REACT_NONE);
	}

	inline Percentage::Percentage(const UncheckedPercentage& other) :
		mValue(other.value)
	{}

	inline Percentage::Percentage(const Percentage& other) :
		mValue(other.mValue)
	{}

	inline Percentage::~Percentage()
	{
		// Nothing to do in here
	}

	inline float Percentage::getValue() const
	{
		return mValue;
	}

	inline void Percentage::setValue(float value)
	{
		mValue = Math::clamp(value, 0.f, 1.f);
		QSF_CHECK(value >= 0.f && value <= 1.f, "Invalid percentage float value " << value << " clamped to " << mValue,
			QSF_REACT_NONE);
	}

	inline Percentage& Percentage::operator=(const Percentage& other)
	{
		mValue = other.mValue;
		return *this;
	}

	inline Percentage::operator float() const
	{
		return mValue;
	}

	inline float Percentage::operator *() const
	{
		return mValue;
	}

	namespace serialization
	{
		// specialization for the binary serialization
		template <>
		struct serializer<Percentage>
		{
			inline static void serialize(BinarySerializer& serializer, Percentage& data)
			{
				serializer & data.mValue;
			}
		};
	}
}
