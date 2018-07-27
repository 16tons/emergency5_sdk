/*=============================================================================
Description: common declarations

Revision history:
* Created by Lin Luo, November 3, 2005
=============================================================================*/

#pragma once

#include "MemoryPool.h"

// uncomment the following line will enable CRT memory debugging, extremely slow!
//#define DEBUG_MEMORY

#ifdef DEBUG_MEMORY
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif // DEBUG_MEMORY

typedef unsigned char byte;

/*
	network data transmission interface will be bit stream based. internally, bit stream buffer is composed of
	a sequential list of fixed-sized buckets. fixed-sized bucket memory management involves little unnecessary
	dynamic memory allocation, and it is the best way to avoid memory fragmentation when using non-fixed-sized
	dynamic memory allocation scheme. extra packaging is not needed by utilizing win32 scatter-gather asynchronous
	IO scheme. the low level networking will also use the same bucket scheme to retrieve incoming network data,
	and the corresponding bit stream will be constructed.
*/

#define BUCKETSIZE 32 // 32

struct BUCKET
{
	BUCKET()
	{
		memset(buffer, 0, BUCKETSIZE);
	}

	byte buffer[BUCKETSIZE];
};

#pragma warning(disable: 4554)

#define BITS2BYTES(x) ( ((x) + 7) >> 3 )
#define BYTES2BUCKETS(x) ( ( (x) + BUCKETSIZE - 1 ) / BUCKETSIZE )
#define BITS2BUCKETS(x) ( BYTES2BUCKETS( BITS2BYTES(x) ) )
#define BITS2BYTEOFFSET(x) ( BITS2BYTES(x) % BUCKETSIZE )

// the global bucket pool used everywhere
extern CMemoryPool<BUCKET> GBucketPool;

extern std::wstring convert(const char* s_);
extern std::string convert(const wchar_t* s_);

