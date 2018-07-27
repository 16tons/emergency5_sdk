// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Percentage.h"

#include <qsf/base/error/ErrorHandling.h>

#include <camp/valuemapper.hpp>

namespace camp_ext
{
	// contains interface functionality for exporting the percentage data type to CAMP
	template <>
	struct ValueMapper<qsf::Percentage>
	{
		// camp basic type definition
		static const int type = camp::userType;

		// conversion from Percentage into CAMP user object is trivial
		static camp::UserObject to(const qsf::Percentage& value)
		{
			return value; // automatic conversion because of operator float() defined
		}

		// conversion from any type to Percentage
		template <typename T>
		static qsf::Percentage from(const T& source)
		{
			return ValueMapper<float>::from(source);
		}

		// TODO(ca) Double check: replaced explicit specialization of static method by simple static method overload to conform to standard C++
		static qsf::Percentage from(const camp::UserObject& source)
		{
			// TODO(co) CAMP_ERROR(camp::BadType(camp::userType, camp::userType))); // Types are not compatible (this implementation is required for "camp::Value::isCompatible<TYPE>()")
			static const camp::Class& qsfPercentageCampClass = camp::classByType<qsf::Percentage>();
			QSF_WARN_IF(source.getClass() != qsfPercentageCampClass, "Trying to create a percentage instance from " << source.getClass().name(),
				return qsf::Percentage());

			return *static_cast<qsf::Percentage*>(source.pointer());
		}
	};
}
