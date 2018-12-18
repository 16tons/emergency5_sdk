// Copyright (C) 2012-2018 Promotion Software GmbH


// Do never ever include this optional precompiled header within another public header!
// The precompiled header is only used to speed up compilation, it's no replacement for
// a decent header design. Be clever, don't use precompiled headers like a sledgehammer
// or you increase instead of decrease the compile time faster as you can say "bad idea".


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
// We can't use pragma once here since precompiled headers get a special treatment on Mac OS X that is incompatible with #pragma once
#ifndef _QSF_PRECOMPILED_HEADER_H_
#define _QSF_PRECOMPILED_HEADER_H_


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
	#include "qsf/platform/msvc_no_sillywarnings_please.h" // Disables nasty VisualStudio warnings
#endif
#include "qsf/base/Memory.h"
#include "qsf/platform/PlatformTypes.h"
#include "qsf/base/profiler/Profiler.h"
#include "qsf/base/CleanXutilityForBoostContainer.h"	// Include this first to eliminate the compiler warning "warning C4996: 'std::_Copy_impl': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'"

#ifdef WIN32
	// Include the windows header with our precautions before others include it themselves (looking at you, Ogre)
	#include "qsf/platform/WindowsHeader.h"
#endif

// Include this before it's included by other headers
#include <algorithm>

// Let loose the heavy OGRE
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSubEntity.h>
#include <OGRE/OgreTagPoint.h>
#include <OGRE/OgreFileSystem.h>
#include <OGRE/OgreArchiveManager.h>
#include <OGRE/OgreAnimationState.h>
#include <OGRE/OgreTechnique.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreTexture.h>
#include <OGRE/OgreTextureManager.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSubMesh.h>
#include <OGRE/OgreMeshManager.h>
#include <OGRE/OgreMeshSerializer.h>
#include <OGRE/OgreRenderWindow.h>
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <OGRE/OgreBillboard.h>
#include <OGRE/OgreBillboardSet.h>
#include <OGRE/OgreRenderObjectListener.h>
#include <OGRE/OgreRenderQueueListener.h>
#include <OGRE/OgreRenderTargetListener.h>
#include <OGRE/OgreHardwareBufferManager.h>
#include <OGRE/OgreHardwareOcclusionQuery.h>
#include <OGRE/Animation/OgreBone.h>
#include <OGRE/Animation/OgreSkeletonManager.h>

// OGRE includes an OS header resulting in the usual fancy definition-hell, undefine these evil MS Windows definitions
#undef ERROR
#undef TRANSPARENT

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

#endif // _QSF_PRECOMPILED_HEADER_H_
