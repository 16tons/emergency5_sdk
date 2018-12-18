// Copyright (C) 2012-2018 Promotion Software GmbH


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

namespace qsf
{
	// Use this to tell the compiler that you as programmer know that the floating point number is between 0 and 1
	struct QSF_AI_API_EXPORT UncheckedPercentage
	{
		// not explicit by design
		UncheckedPercentage(float value);

		float value;
	};

	// Data type representing a percentage as a float between 0 and 1.
	// Clamps invalid values into the valid range.
	// Default copy C'tor and assignment operator allowed by design.
	class QSF_AI_API_EXPORT Percentage
	{
	public:
		// necessary for private access for the binary serialization
		friend struct serialization::serializer<Percentage>;

		static const Percentage ZERO;
		static const Percentage MAX; // = 1.f

		inline static Percentage clampToPercentage(float value);

		// By default initialized to zero
		Percentage();

		// not explicit by design
		Percentage(float value);

		// not explicit by design
		Percentage(const UncheckedPercentage& value);

		// Copy constructor
		Percentage(const Percentage& other);

		virtual ~Percentage();

		// Getter and setter (for CAMP only)
		float getValue() const;
		void setValue(float value);

		// Assignment operator
		Percentage& operator=(const Percentage& other);

		// automatic conversion to regular float
		operator float() const;

		// manual conversion to regular float
		float operator* () const;

	private:
		float mValue;

		QSF_CAMP_RTTI();
	};
}

QSF_CAMP_TYPE(qsf::Percentage);

#include "qsf_ai/base/Percentage-inl.h"
