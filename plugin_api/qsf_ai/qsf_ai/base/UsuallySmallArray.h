// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** Very simple light-weight array class that combines heap- and local-storage.
	* For use cases where you'd like to use a C-style array of pretty small size
	* but still want to provide the (rare) possibility to grow.
	* Until there are <=MIN_SIZE elements the local fixed array will be used.
	* If MIN_SIZE is exceeded heap-space will be allocated (using new[]).
	* The class behaves like a simple C-array (for example, if you store pointers
	* there won't be any automatic delete on those on array destruction).
	* As being noted it is very simple. No automatic growing, just a resize method;
	* no bounds-checks; values don't survive a resize; iterator interface only contains
	* what was needed to use std::sort for the belowmentioned usecase.
	* It was created to improve locality of the AI AspectTags.
	*/
	template<typename T,typename SIZE_TYPE,SIZE_TYPE MIN_SIZE> class UsuallySmallArray
	{
	public:
		typedef UsuallySmallArray<T,SIZE_TYPE,MIN_SIZE> _ThisClass;

		UsuallySmallArray() :
			mSize(),
			mStartPtr(mLocalStorage)
		{
		}

		UsuallySmallArray(const _ThisClass& src) :
			mSize(),
			mStartPtr(mLocalStorage)
		{
			*this = src;
		}

		_ThisClass& operator=(const _ThisClass& rho)
		{
			const SIZE_TYPE size = rho.size();
			resize(size);
			for (SIZE_TYPE t = 0; t < size; ++t)
				mStartPtr[t] = rho[t];
			return *this;
		}

		~UsuallySmallArray()
		{
			if (mStartPtr != mLocalStorage)
				delete[] mStartPtr;
		}

		// Access to the array
		inline T& operator[](SIZE_TYPE i)
		{
			return mStartPtr[i];
		}

		// Access to the array
		inline const T& operator[](SIZE_TYPE i) const
		{
			return mStartPtr[i];
		}

		// Returns the array's current size.
		inline SIZE_TYPE size() const
		{
			return mSize;
		}

		// Returns the array's start-adress for use with iterators.
		inline T* begin()
		{
			return &mStartPtr[0];
		}

		// Returns the array's end-adress for use with iterators.
		inline T* end()
		{
			return &mStartPtr[mSize];
		}

		// Sets the array size to 0 and deletes heap-space if allocated.
		void clear()
		{
			mSize = SIZE_TYPE();
			if (mStartPtr != mLocalStorage)
			{
				delete[] mStartPtr;
				mStartPtr = mLocalStorage;
			}
		}

		// Resize the array.
		// If necessary (re)allocated heap space.
		// Note: heap-space will not shrink.
		// Note: If the array was using heap-space before and the new size allows it to use local storage again, it will do so,
		// invalidating all entries.
		// In general: never asume that the array's contents remain valid after a resize!
		void resize(SIZE_TYPE size)
		{
			if (size > mSize)
			{
				if (size > MIN_SIZE)
				{
					if (mStartPtr != mLocalStorage)
						delete[] mStartPtr;
					mStartPtr = new T[size];
				}
			}
			else
			{
				if (size <= MIN_SIZE)
				{
					if (mStartPtr != mLocalStorage)
					{
						delete[] mStartPtr;
						mStartPtr = mLocalStorage;
					}

				}
			}
			mSize = size;
		}

	private:
		SIZE_TYPE mSize;
		T* mStartPtr;
		T mLocalStorage[MIN_SIZE];
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
