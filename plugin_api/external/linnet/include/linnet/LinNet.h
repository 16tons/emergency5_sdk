/*=============================================================================
Description: the precompiled header for LinNet

Revision history:
* Created by Lin Luo, December 19, 2005
=============================================================================*/

#pragma once


#include <stdint.h>
#include <iomanip>
#include <wchar.h>
#include <cassert>
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <queue>
#include <iostream>
#include <mutex>

#ifdef WIN32
#	include <tchar.h>
#else
	typedef char TCHAR;
#   define _T(x) x
#endif

typedef const TCHAR *LPCTSTR;

#include "MemoryPool.h"
#include "Common.h"
#include "BitStream.h"
#include "DirectNet.h"
#include "AdaptiveHuffmanEncoding.h"
