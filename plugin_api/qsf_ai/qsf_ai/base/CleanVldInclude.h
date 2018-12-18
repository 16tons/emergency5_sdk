// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

#ifdef QSF_USE_VLD
	#define NOMINMAX
	#include <vld/vld.h>	// Visual Leak Detector for Visual C++ 2008/2010/2012/2013/2015
	#undef ERROR
#endif
