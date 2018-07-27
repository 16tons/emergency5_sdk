// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/serialization/QsfBinarySerializationFwd.h"

#include <qsf/reflection/CampClass.h>
#include <qsf/base/GetUninitialized.h>

namespace qsf
{
	// Use this to tell the compiler that you as programmer know that the floating point number is unsigned
	struct QSF_AI_API_EXPORT UncheckedUnsignedFloat
	{
		// not explicit by design
		UncheckedUnsignedFloat(float value);

		float value;
	};

	// Unsigned float data type.
	// Corrects invalid values to zero.
	// Default copy C'tor and assignment operator allowed by design.
	class QSF_AI_API_EXPORT UnsignedFloat
	{
	public:
		// necessary for private access for the binary serialization
		friend struct serialization::serializer<UnsignedFloat>;

		static const UnsignedFloat ZERO;
		static const UnsignedFloat ONE;

		// By default initialized to zero
		UnsignedFloat();

		// not explicit by design
		UnsignedFloat(float value);

		// not explicit by design
		UnsignedFloat(const UncheckedUnsignedFloat& value);

		// not explicit by design
		UnsignedFloat(const UnsignedFloat& value);

		virtual ~UnsignedFloat();

		// Getter and setter (for CAMP only)
		float getValue() const;
		void setValue(float value);

		// automatic conversion to regular float
		operator float() const;

		// Operations to stay inside the unsigned float safe zone
		UnsignedFloat operator+ (const UnsignedFloat& other) const;
		UnsignedFloat operator* (const UnsignedFloat& other) const;
		UnsignedFloat operator/ (const UnsignedFloat& other) const;
		const UnsignedFloat& operator+= (const UnsignedFloat& other);
		const UnsignedFloat& operator*= (const UnsignedFloat& other);
		const UnsignedFloat& operator/= (const UnsignedFloat& other);

		// manual conversion to regular float
		float operator* () const;

	private:
		float mValue;

		QSF_CAMP_RTTI();
	};

	// Necessary specialization for the getUninitialized template
	template <>
	UnsignedFloat getUninitialized<UnsignedFloat>();
}

QSF_CAMP_TYPE(qsf::UnsignedFloat);

#include "qsf_ai/base/UnsignedFloat-inl.h"
