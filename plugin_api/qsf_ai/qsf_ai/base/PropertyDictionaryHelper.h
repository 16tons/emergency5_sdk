// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/base/PropertyDictionary.h>

namespace qsf
{
	// specializations of tryToSetFrom for some common types that are not directly supported
	template <>
	inline bool getValueFromPropertyDictionary<unsigned int>(unsigned int& variable, const camp::Value& value)
	{
		int intermediateResult = 0;
		if (!getValueFromPropertyDictionary(intermediateResult, value))
			return false;

		QSF_CHECK(intermediateResult >= 0, "unable to read unsigned int variable from negative value " << intermediateResult,
			return false);

		variable = static_cast<unsigned int>(intermediateResult);
		return true;
	}

	template <>
	inline bool getValueFromPropertyDictionary<UnsignedFloat>(UnsignedFloat& variable, const camp::Value& value)
	{
		float intermediateResult = 0;
		if (!getValueFromPropertyDictionary(intermediateResult, value))
			return false;

		QSF_CHECK(intermediateResult >= 0.f, "unable to read unsigned float variable from negative value " << intermediateResult,
			return false);

		variable = intermediateResult;
		return true;
	}
}
