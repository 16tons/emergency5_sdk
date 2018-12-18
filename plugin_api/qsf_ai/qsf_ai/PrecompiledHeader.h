// Copyright (C) 2012-2018 Promotion Software GmbH


// Do never ever include this optional precompiled header within another public header!
// The precompiled header is only used to speed up compilation, it's no replacement for
// a decent header design. Be clever, don't use precompiled headers like a sledgehammer
// or you increase instead of decrease the compile time faster as you can say "bad idea".


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
// We can't use pragma once here since precompiled headers get special treatment on Mac OS X that is incompatible with #pragma once
#ifndef _QSF_AI_PRECOMPILED_HEADER_H_
#define _QSF_AI_PRECOMPILED_HEADER_H_


// This here should recognize when you try to include multiple precompiled headers.
// Use this block without modification of "_QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_"
// in all precompiled headers of your project.
#ifdef _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
	#error Multiple precompiled headers included. The reason could be inclusion of a precompiled header in a public header file.
#else
	#define _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// QSF (stable)
#ifdef _MSC_VER
	#include <qsf/platform/msvc_no_sillywarnings_please.h> // Disables nasty VisualStudio warnings
#endif
#include <qsf/base/Memory.h>
#include <qsf/base/profiler/Profiler.h>

#include "qsf_ai/base/CleanXutilityForBoostContainer.h"

// CAMP
#include <camp/class.hpp>

// GLM
#include <glm/glm.hpp>

// Boost
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>

// C++ standard headers
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <deque>
#include <vector>
#include <string>
#include <limits>
#include <memory>


//[-------------------------------------------------------]
//[ Compile-time flags                                    ]
//[-------------------------------------------------------]

#ifndef ENDUSER
	// Enable this only for AI debugging, otherwise some checks are active that quite often trigger, even though it's not critical
	//#define QSF_AI_PERFORM_ALL_CHECKS
#endif


#endif // _QSF_AI_PRECOMPILED_HEADER_H_
