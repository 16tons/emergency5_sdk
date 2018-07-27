// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Paged allocator class template
	*
	*  @note
	*    - The create method currently supports up to 3 parameters; feel free to extend this if more parameters are needed
	*
	*  @todo
	*    - TODO(fw): Page size could be made changeable at any time (with effect for new pages only, of course) by saving the size of each page.
	*	 - TODO(fw): We could possibly use a bitfield or similar to store for each element whether it is used - that would improve performance for isUsed() and isUnused() checks.
	*/
	template<typename T, int PAGESIZE = 16>
	class PagedAllocator
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit PagedAllocator(size_t pageSize = PAGESIZE);

		/**
		*  @brief
		*    Destructor
		*/
		~PagedAllocator();

		/**
		*  @brief
		*    Clear all allocated memory, effectively destroying all instances
		*/
		void clear();

		/**
		*  @brief
		*    Check if there are any element currently registered
		*/
		bool empty() const;

		/**
		*  @brief
		*    Allocate a new element without calling its constructor
		*/
		T* alloc();

		/**
		*  @brief
		*    Free an element without calling its destructor
		*/
		void free(T* element);

		/**
		*  @brief
		*    Create a new element by its default constructor
		*/
		T* create();

		/**
		*  @brief
		*    Create a new element using a constructor with one argument
		*/
		template<typename A>
		T* create(A a);

		/**
		*  @brief
		*    Create a new element using a constructor with 2 arguments
		*/
		template<typename A, typename B>
		T* create(A a, B b);

		/**
		*  @brief
		*    Create a new element using a constructor with 3 arguments
		*/
		template<typename A, typename B, typename C>
		T* create(A a, B b, C c);

		/**
		*  @brief
		*    Create a new element using a constructor with 4 arguments
		*/
		template<typename A, typename B, typename C, typename D>
		T* create(A a, B b, C c, D d);

		/**
		*  @brief
		*    Create a new element using a constructor with 5 arguments
		*/
		template<typename A, typename B, typename C, typename D, typename E>
		T* create(A a, B b, C c, D d, E e);

		/**
		*  @brief
		*    Free an element, its destructor is called
		*/
		void destroy(T* element);

		/**
		*  @brief
		*    Check whether the given element is managed by this allocator, no matter if it is currently in use or not
		*/
		bool isManaged(const T* element) const;

		/**
		*  @brief
		*    Check whether the given element is currently used and managed by this allocator
		*/
		bool isUsed(const T* element) const;

		/**
		*  @brief
		*    Check whether the given element is currently not used and managed by this allocator
		*/
		bool isUnused(const T* element) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Page
		{
			size_t mSize;
			T* mElements;
		};
		typedef typename std::vector<Page> PageArray;
		typedef typename std::vector<T*> TPointerArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const size_t  mPageSize;		///< Number of components to be reserved per page; set by the constructor and must not be changed afterwards
		PageArray	  mPages;			///< Array of pages holding used and unused elements
		TPointerArray mFreeElements;	///< Array of currently unused elements


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/manager/PagedAllocator-inl.h"
