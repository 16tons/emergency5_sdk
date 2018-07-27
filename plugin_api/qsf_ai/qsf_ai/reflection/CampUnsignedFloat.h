// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/base/error/ErrorHandling.h>

#include <camp/valuemapper.hpp>


namespace camp_ext
{
	// contains interface functionality for exporting the unsigned float data type to CAMP
	template <>
	struct ValueMapper<qsf::UnsignedFloat>
	{
		// camp basic type definition
		static const int type = camp::userType;

		// conversion from UnsignedFloat into CAMP user object is trivial
		static camp::UserObject to(const qsf::UnsignedFloat& value)
		{
			return value; // automatic conversion because of operator float() defined
		}

		// conversion from any type to UnsignedFloat
		template <typename T>
		static qsf::UnsignedFloat from(const T& source)
		{
			return ValueMapper<float>::from(source);
		}

		// TODO(ca) Double check: replaced explicit specialization of static method by simple static method overload to conform to standard C++
		static qsf::UnsignedFloat from(const camp::UserObject& source)
		{
			// TODO(co) CAMP_ERROR(camp::BadType(camp::userType, camp::userType)));	// Types are not compatible (this implementation is required for "camp::Value::isCompatible<TYPE>()")
			static const camp::Class& qsfUnsignedFloatCampClass = camp::classByType<qsf::UnsignedFloat>();
			QSF_WARN_IF(source.getClass() != qsfUnsignedFloatCampClass, "Trying to create an unsigned float instance from " << source.getClass().name(),
				return qsf::UnsignedFloat());

			return *static_cast<qsf::UnsignedFloat*>(source.pointer());
		}
	};
}
