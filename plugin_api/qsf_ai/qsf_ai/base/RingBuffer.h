// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
}


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
	*    Very simple ring-buffer for small number of items.
	*/
	template<typename T,typename SIZE_TYPE,SIZE_TYPE SIZE> class RingBuffer
	{
	public:
		inline RingBuffer() :
			mCount(0),
			mStart(0)
		{
		}

		inline SIZE_TYPE getCount() const
		{
			return mCount;
		}

		inline void setCount(SIZE_TYPE count)
		{
			mCount = std::min<SIZE_TYPE>(count, SIZE);
		}

		inline T& operator[](SIZE_TYPE i)
		{
			return items[getRealIndex(i)];
		}

		inline const T& operator[](SIZE_TYPE i) const
		{
			return items[getRealIndex(i)];
		}

		inline void clear()
		{
			mStart = mCount = 0;
		}

		inline void removeFirst()
		{
			if (mCount)
			{
				--mCount;
				++mStart;
			}
		}

		inline void removeLast()
		{
			if (mCount)
			{
				--mCount;
			}
		}

		void add(T& item)
		{
			SIZE_TYPE i;
			if (mCount < SIZE)
			{
				i = getRealIndex(mCount);
				++mCount;
			}
			else
			{
				++mStart;
				i = getRealIndex(SIZE);
			}
			items[i] = item;
		}

		void serialize(BinarySerializer& serializer)
		{
			serializer & mCount;
			for (SIZE_TYPE i = 0; i < mCount; ++i)
			{
				items[getRealIndex(i)].serialize(serializer);
			}
		}

	private:
		inline SIZE_TYPE getRealIndex(SIZE_TYPE i) const
		{
			return ((mStart + i) % SIZE);
		}

	private:
		SIZE_TYPE mCount;
		SIZE_TYPE mStart;
		T items[SIZE];
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
