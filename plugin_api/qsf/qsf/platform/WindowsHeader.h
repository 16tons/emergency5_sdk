// Copyright (C) 2012-2017 Promotion Software GmbH


// Do never ever include this header within another public header!


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// Set Windows version to Windows Vista (0x0600), we don't support Windows XP (0x0501)
#ifdef WINVER
	#undef WINVER
#endif
#define WINVER			0x0600
#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT	0x0600

// Exclude some stuff from "windows.h" to speed up compilation a bit
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOGDICAPMASKS
	#define NOGDICAPMASKS
#endif
#ifndef NOMENUS
	#define NOMENUS
#endif
#ifndef NOICONS
	#define NOICONS
#endif
#ifndef NOKEYSTATES
	#define NOKEYSTATES
#endif
#ifndef NORASTEROPS
	#define NORASTEROPS
#endif
#ifndef OEMRESOURCE
	#define OEMRESOURCE
#endif
#ifndef NOATOM
	#define NOATOM
#endif
#ifndef NOMEMMGR
	#define NOMEMMGR
#endif
#ifndef NOMETAFILE
	#define NOMETAFILE
#endif
#ifndef NOOPENFILE
	#define NOOPENFILE
#endif
#ifndef NOSCROLL
	#define NOSCROLL
#endif
#ifndef NOSERVICE
	#define NOSERVICE
#endif
#ifndef NOSOUND
	#define NOSOUND
#endif
#ifndef NOWH
	#define NOWH
#endif
#ifndef NOCOMM
	#define NOCOMM
#endif
#ifndef NOKANJI
	#define NOKANJI
#endif
#ifndef NOHELP
	#define NOHELP
#endif
#ifndef NOPROFILER
	#define NOPROFILER
#endif
#ifndef NODEFERWINDOWPOS
	#define NODEFERWINDOWPOS
#endif
#ifndef NOMCX
	#define NOMCX
#endif

// Disable warnings in external headers, we can't fix them
#pragma warning(push)
#pragma warning(disable: 4668)	// warning C4668: '<x>' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	#include <windows.h>
	#undef max		// See "GetUninitialized.h"
	#undef ERROR	// ... the usual fancy definition-hell, undefine this MS Windows "ERROR"-definition
	#undef DrawText
	#undef TRANSPARENT
#pragma warning(pop)
