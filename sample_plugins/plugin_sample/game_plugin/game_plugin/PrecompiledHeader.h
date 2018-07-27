// Copyright (C) 2012-2017 Promotion Software GmbH


// This here should recognize when you try to include multiple precompiled headers.
// Use this block without modification of "_QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_"
// in all precompiled headers of your project.
#ifdef _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
	#error Multiple precompiled headers included. The reason could be inclusion of a precompiled header in a public header file.
#else
	#define _QSF_BASED_PROJECT_PRECOMPILED_HEADER_INCLUDED_
#endif


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// QSF (stable)
#ifdef _MSC_VER
	#include <qsf/platform/msvc_no_sillywarnings_please.h> // Disables nasty VisualStudio warnings
#endif
#include <qsf/base/error/BoostAssertHandler.h>
#include <qsf/base/Memory.h>

// CAMP
#include <camp/class.hpp>

// GLM
#include <glm/glm.hpp>

// Boost
#include <boost/bind.hpp>
#include <boost/signals2.hpp>
#include <boost/random.hpp>
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/algorithm/string.hpp>

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
