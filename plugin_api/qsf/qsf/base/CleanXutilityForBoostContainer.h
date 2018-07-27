// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

// include this before including a boost container header to avoid a warning

QSF_WARNING_PUSH;
QSF_WARNING_DISABLE_MSVC(4996);

#ifdef _MSC_VER
	// Include this before it's included by other headers
	#include <xutility>
#endif

QSF_WARNING_POP;
