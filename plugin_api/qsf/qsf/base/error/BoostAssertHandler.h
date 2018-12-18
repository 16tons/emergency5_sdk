// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


// Delegate the boost assert to our own so that we can handle all errors in the same way.
namespace boost
{
	inline void assertion_failed_msg(char const*, char const* msg, const char*, const char*, long)
	{
		// TODO(vs) we are unable to use all these information because our own logging system does not support them at the moment
		QSF_ERROR(msg, QSF_REACT_THROW); // boost uses asserts for situations where execution can't just continue executing. We risk crashes if we don't stop execution on a failed assert.
	}

	inline void assertion_failed(char const* expr, char const*, char const*, long)
	{
		// TODO(vs) we are unable to use all these information because our own logging system does not support them at the moment
		QSF_ERROR("boost assertion failed:" << expr, QSF_REACT_THROW); // boost uses asserts for situations where execution can't just continue executing. We risk crashes if we don't stop execution on a failed assert.
	}
}
