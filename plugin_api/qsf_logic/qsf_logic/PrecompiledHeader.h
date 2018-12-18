// Copyright (C) 2012-2018 Promotion Software GmbH


// Do never ever include this optional precompiled header within another public header!
// The precompiled header is only used to speed up compilation, it's no replacement for
// a decent header design. Be clever, don't use precompiled headers like a sledgehammer
// or you increase instead of decrease the compile time faster as you can say "bad idea".


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
// We can't use pragma once here since precompiled headers get a special treatment on Mac OS X that is incompatible with #pragma once
#ifndef _QSF_LOGIC_PRECOMPILED_HEADER_H_
#define _QSF_LOGIC_PRECOMPILED_HEADER_H_


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

#ifdef _MSC_VER
	// Include this Windows only header before it's included by other headers
	#include <xutility>
#endif

// CAMP
#include <camp/class.hpp>

// GLM
#include <glm/glm.hpp>

// Boost
#include <boost/bind.hpp>
// TODO(co) I was unable to resolve warning C4996 occurring in some external headers and I was also unable to pinpoint the disabling of the warning more accurate. Find a better solution if somehow possible.
#if defined(WIN32) || defined(_DEBUG)
	#pragma warning(push)
	#pragma warning(disable: 4996)	// Disable warning "1>c:\program files (x86)\microsoft visual studio 11.0\vc\include\xutility(2176): warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"
	#include <boost/signals2.hpp>
	#pragma warning(pop)
#endif
#include <boost/optional.hpp>
#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/nowide/convert.hpp>

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

#endif // _QSF_LOGIC_PRECOMPILED_HEADER_H_
