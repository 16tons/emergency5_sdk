/*=============================================================================
Description: the memory pool manager template class

Revision history:
* Created by Lin Luo, November 3, 2005
=============================================================================*/

#pragma once

#include <queue>
#include <mutex>

#pragma push_macro("new")
#undef new

#ifdef WIN32
#	include <tchar.h>
#else
typedef char TCHAR;
#   define _T(x) x
#endif

#ifdef _DEBUG
#	include <iomanip>
#	include <iostream>
#endif

typedef const TCHAR *LPCTSTR;

#define T_FILE _T(__FILE__)

template<typename T>
class CMemoryPool {
public:
	//!< default constructor
	CMemoryPool()
	{
#ifdef DEBUG_MEMORY
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif // DEBUG_MEMORY

		T* p = (T*)malloc( sizeof(T) );
		mFreeList.push(p);
	}

	//!< constructor
	explicit CMemoryPool(size_t initialCapacity_)
	{
#ifdef DEBUG_MEMORY
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF );
#endif // DEBUG_MEMORY

		if (initialCapacity_ == 0)
			initialCapacity_ = 1;

		// build the free list
		for (size_t i = 0; i < initialCapacity_; ++i)
		{
			T* p = (T*)malloc( sizeof(T) );
			mFreeList.push(p);
		}
	}

	//!< destructor
	~CMemoryPool()
	{
		std::lock_guard<std::mutex> lock(mPoolMutex);

		bool needAssert = false;
		if ( !mUsedList.empty() )
			needAssert = true;
		while ( !mUsedList.empty() )
		{
#ifdef _DEBUG
#define DEC(i) std::dec << i
#define HEX(i) std::hex << std::setw(2*sizeof(i)) << std::setfill(_T('0')) << i
			typename std::map<T*, FILELINE>::iterator itor = mUsedList.begin();
			std::basic_stringstream<TCHAR> ss;
			ss << itor->second.file << _T("(") << itor->second.line << _T(") : object at ") << HEX(itor->first) << _T(" not freed!") << std::endl;
#ifdef _UNICODE
			std::wcout << ss.str();
#else // _UNICODE
			std::cout << ss.str();
#endif // _UNICODE
#else // _DEBUG
			typename std::map<T*, int>::iterator itor = mUsedList.begin();
#endif // _DEBUG
			T* p = itor->first;
			mUsedList.erase(itor);
			free(p);
		}
		if (needAssert)
			assert( !_T("mUsedList is not empty!") );

		while ( !mFreeList.empty() )
		{
			T* p = mFreeList.front();
			mFreeList.pop();
			free(p);
		}
	}

	//!< allocates a free element from the pool
	T* Allocate(LPCTSTR file_ = nullptr, int line_ = 0)
	{
		std::lock_guard<std::mutex> lock(mPoolMutex);

		// recharge the free list if empty
		if ( mFreeList.empty() )
		{
			for ( size_t i = 0; i < mUsedList.size(); ++i )
			{
				T* p = (T*)malloc( sizeof(T) );
				mFreeList.push(p);
			}
		}

		// get the first element from the free list
		T* p = mFreeList.front();
		mFreeList.pop();

#ifdef _DEBUG
		if (file_ == nullptr)
			file_ = _T("unknown");
		FILELINE fl = {file_, line_};
		mUsedList[p] = fl;
#else // _DEBUG
		// register it to the used list
		mUsedList[p] = 0;
#endif // _DEBUG

		// construct the object and return it to caller
		return new(p) T;
	}

	//!< frees an element into the pool
	void Free(T* p_)
	{
		if (p_ == nullptr)
			return;

		std::lock_guard<std::mutex> lock(mPoolMutex);

#ifdef _DEBUG
		typename std::map<T*, FILELINE>::iterator itor = mUsedList.find(p_);
#else // _DEBUG
		// a pointer is valid only when it is registered in the used list
		typename std::map<T*, int>::iterator itor = mUsedList.find(p_);
#endif // _DEBUG
		if ( itor == mUsedList.end() )
			return;

		// the pointer seems valid, unregister it from the used list
		mUsedList.erase(itor);

		// return it back to the free list
		mFreeList.push(p_);

		// destruct the object with the lock held (why?)
		p_->~T();
	}

private:
#ifdef _DEBUG
	struct FILELINE
	{
		std::basic_string<TCHAR> file;
		int line;
	};
	std::map<T*, FILELINE> mUsedList;
#else // _DEBUG
	// a map of pointers that point to all the elements allocated so far
	std::map<T*, int> mUsedList;
#endif // _DEBUG

	// a FIFO list of pointers that point to the free elements to be allocated
	std::queue<T*> mFreeList;

	std::mutex mPoolMutex;
};

#pragma pop_macro("new")

#ifdef _DEBUG
#define MEMORYPOOL_ALLOCATE(MemoryPool) MemoryPool.Allocate(T_FILE, __LINE__)
#else // _DEBUG
#define MEMORYPOOL_ALLOCATE(MemoryPool) MemoryPool.Allocate()
#endif // _DEBUG

