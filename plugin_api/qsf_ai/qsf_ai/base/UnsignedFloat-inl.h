// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{

	inline UncheckedUnsignedFloat::UncheckedUnsignedFloat(float value) :
		value(value)
	{}

	inline UnsignedFloat::UnsignedFloat() :
		mValue(0.f)
	{}

	inline UnsignedFloat::UnsignedFloat(float value) :
		mValue(value)
	{
		QSF_WARN_IF_ONCE(value < 0.0f, "Invalid unsigned float value " << value << " corrected to zero", mValue = 0.0f);
	}

	inline UnsignedFloat::UnsignedFloat(const UncheckedUnsignedFloat& value) :
		mValue(value.value)
	{}

	inline UnsignedFloat::UnsignedFloat(const UnsignedFloat& value) :
		mValue(value)
	{}

	inline UnsignedFloat::~UnsignedFloat()
	{
		// Nothing to do in here
	}

	inline float UnsignedFloat::getValue() const
	{
		return mValue;
	}

	inline void UnsignedFloat::setValue(float value)
	{
		mValue = value;
		QSF_WARN_IF_ONCE(value < 0.0f, "Invalid unsigned float value " << value << " corrected to zero", mValue = 0.0f);
	}

	inline UnsignedFloat::operator float() const
	{
		return mValue;
	}

	inline float UnsignedFloat::operator *() const
	{
		return mValue;
	}

	inline UnsignedFloat UnsignedFloat::operator+ (const UnsignedFloat& other) const
	{
		return UncheckedUnsignedFloat(mValue + other.mValue);
	}

	inline UnsignedFloat UnsignedFloat::operator* (const UnsignedFloat& other) const
	{
		return UncheckedUnsignedFloat(mValue * other.mValue);
	}

	inline UnsignedFloat UnsignedFloat::operator/ (const UnsignedFloat& other) const
	{
		return UncheckedUnsignedFloat(mValue / other.mValue);
	}

	inline const UnsignedFloat& UnsignedFloat::operator+= (const UnsignedFloat& other)
	{
		mValue += other.mValue;
		return *this;
	}

	inline const UnsignedFloat& UnsignedFloat::operator*= (const UnsignedFloat& other)
	{
		mValue *= other.mValue;
		return *this;
	}

	inline const UnsignedFloat& UnsignedFloat::operator/= (const UnsignedFloat& other)
	{
		mValue /= other.mValue;
		return *this;
	}

	// specialization for getUninitialized
	template <>
	inline UnsignedFloat getUninitialized<UnsignedFloat>()
	{
		return UncheckedUnsignedFloat(std::numeric_limits<float>::max());
	}

	namespace serialization
	{
		// specialization for binary serialization
		template <>
		struct serializer<UnsignedFloat>
		{
			inline static void serialize(BinarySerializer& serializer, UnsignedFloat& data)
			{
				serializer & data.mValue;
			}
		};
	}
}
