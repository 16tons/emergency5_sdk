// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T, int PAGESIZE>
	inline PagedAllocator<T, PAGESIZE>::PagedAllocator(size_t pageSize) :
		mPageSize(pageSize)
	{
		// Nothing here
	}

	template<typename T, int PAGESIZE>
	inline PagedAllocator<T, PAGESIZE>::~PagedAllocator()
	{
		// Free all reserved memory
		clear();
	}

	template<typename T, int PAGESIZE>
	void PagedAllocator<T, PAGESIZE>::clear()
	{
		// Free all pages
		for (size_t i = 0; i < mPages.size(); ++i)
			::free(mPages[i].mElements);

		// Clear arrays of pages and free elements
		mPages.clear();
		mFreeElements.clear();
	}

	template<typename T, int PAGESIZE>
	bool PagedAllocator<T, PAGESIZE>::empty() const
	{
		// Get total number of reserved elements
		size_t totalNumberOfElements = 0;
		for (size_t i = 0; i < mPages.size(); ++i)
			totalNumberOfElements += mPages[i].mSize;

		// Compare to the number of unused elements
		return (mFreeElements.size() == totalNumberOfElements);
	}

	template<typename T, int PAGESIZE>
	T* PagedAllocator<T, PAGESIZE>::alloc()
	{
		// Is there still a free element?
		if (mFreeElements.empty())
		{
			// Create a new page
			Page page;
			page.mSize = mPageSize;

			const size_t memorySize = sizeof(T) * page.mSize;
			page.mElements = static_cast<T*>(::malloc(memorySize));

			QSF_ASSERT(nullptr != page.mElements, "Page could not be created!", QSF_REACT_THROW);

			// Store page
			mPages.push_back(page);

			// Add newly created elements to the array of free elements
			mFreeElements.resize(page.mSize);
			for (size_t i = 0; i < page.mSize; ++i)
				mFreeElements[i] = &page.mElements[page.mSize-i-1];
		}

		// Get the next free element
		T* result = mFreeElements.back();
		mFreeElements.pop_back();

		// Done
		return result;
	}

	template<typename T, int PAGESIZE>
	void PagedAllocator<T, PAGESIZE>::free(T* element)
	{
		// These checks are quite expensive, so only do this in Debug builds at all
#ifdef DEBUG
		QSF_ASSERT(isManaged(element), "Tried to free an element not created by this allocator!", QSF_REACT_THROW);
		QSF_ASSERT(!isUnused(element), "Tried to free an element twice!", QSF_REACT_THROW);
#endif

		// Free an element by simply adding it to the array of free elements
		mFreeElements.push_back(element);
	}

	template<typename T, int PAGESIZE>
	T* PagedAllocator<T, PAGESIZE>::create()
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T();

		// Done
		return element;
	}

	template<typename T, int PAGESIZE> template<typename A>
	T* PagedAllocator<T, PAGESIZE>::create(A a)
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T(a);

		// Done
		return element;
	}

	template<typename T, int PAGESIZE> template<typename A, typename B>
	T* PagedAllocator<T, PAGESIZE>::create(A a, B b)
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T(a, b);

		// Done
		return element;
	}

	template<typename T, int PAGESIZE> template<typename A, typename B, typename C>
	T* PagedAllocator<T, PAGESIZE>::create(A a, B b, C c)
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T(a, b, c);

		// Done
		return element;
	}

	template<typename T, int PAGESIZE> template<typename A, typename B, typename C, typename D>
	T* PagedAllocator<T, PAGESIZE>::create(A a, B b, C c, D d)
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T(a, b, c, d);

		// Done
		return element;
	}

	template<typename T, int PAGESIZE> template<typename A, typename B, typename C, typename D, typename E>
	T* PagedAllocator<T, PAGESIZE>::create(A a, B b, C c, D d, E e)
	{
		// Get an unused element
		T* element = alloc();

		// Call its constructor
		new (static_cast<void*>(element)) T(a, b, c, d, e);

		// Done
		return element;
	}

	template<typename T, int PAGESIZE>
	void PagedAllocator<T, PAGESIZE>::destroy(T* element)
	{
		// Call element's destructor
		element->~T();

		// Free the element
		free(element);
	}

	template<typename T, int PAGESIZE>
	bool PagedAllocator<T, PAGESIZE>::isManaged(const T* element) const
	{
		// Check if the given element is managed by this allocator
		for (size_t i = 0; i < mPages.size(); ++i)
		{
			const int offset = static_cast<int>((char*)element - (char*)&(mPages[i].mElements[0]));
			if (offset >= 0 && (size_t)offset < sizeof(T) * mPages[i].mSize && offset % sizeof(T) == 0)
				return true;
		}
		return false;
	}

	template<typename T, int PAGESIZE>
	bool PagedAllocator<T, PAGESIZE>::isUsed(const T* element) const
	{
		// Check if the given element is currently in use
		return (isManaged(element) && !isUnused());
	}

	template<typename T, int PAGESIZE>
	bool PagedAllocator<T, PAGESIZE>::isUnused(const T* element) const
	{
		// Check if the given element is in the free elements list
		for (typename TPointerArray::const_iterator iterator = mFreeElements.begin(); iterator != mFreeElements.end(); ++iterator)
		{
			if (*iterator == element)
				return true;
		}
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
