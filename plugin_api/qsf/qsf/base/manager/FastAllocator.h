// From http://www.niksula.hut.fi/~hkankaan/Homepages/fastallocation.html


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/pool/pool.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	#ifndef QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE
		#define QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE 64
	#endif


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Manages QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE number of memory pools (no much cost for empty pools)
	*
	*  @note
	*    -It selects the appropriate memory pool depending on how much memory is being allocated/deallocated.
	*/
	class PoolManager
	{
	public:
		/// singleton access
		static PoolManager& getInstance()
		{
			static PoolManager instance;
			return instance;
		}

		void* allocateMemory(size_t size)
		{
			if (size > QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE)
				return ::operator new(size);

			return m_pools[size - 1]->malloc();
		}

		void releaseMemory(void* deletable, size_t size)
		{
			if (size > QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE)
			{
				// Use normal delete
				::operator delete(deletable);
			}
			else
			{
				// Use pool free. Don't delete null pointers
				if (deletable)
					m_pools[size - 1]->free(deletable);
			}
		}

		~PoolManager()
		{
			for (int i = 0; i < QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE; ++i)
				delete m_pools[i];
		}

	private:
		PoolManager()
		{
			for (int i = 0; i < QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE; ++i)
				m_pools[i] = new boost::pool<>(i + 1);
		}

		// No copying of a singleton:
		PoolManager(const PoolManager&);
		const PoolManager& operator=(const PoolManager&);

		/// Memory pool array. m_pools[n] corresponds to pool with objectSize==n+1.
		boost::pool<>* m_pools[QSF_BOOST_POOL_MAX_SMALL_OBJECT_SIZE];
	};

	/// Make the allocation/deallocation faster for small objects. Only the root of class hierarchy should
	/// be derived from GMS::FastAllocator.
	///
	/// Usage:
	///
	/// class A { };
	/// =>
	/// class A : public qsf::FastAllocator { };
	///
	/// Note that FastAllocator is not thread safe.
	/// Use as you wish.
	/// @see: http://www.boost.org/
	/// @author: Hannu Kankaanp‰‰ (hkankaan@cc.hut.fi)
	class FastAllocator
	{
	public:
		static void* operator new(size_t size)
		{
			return PoolManager::getInstance().allocateMemory(size);
		}

		static void operator delete(void* deletable, size_t size)
		{
			PoolManager::getInstance().releaseMemory(deletable, size);
		}

		virtual ~FastAllocator() {}
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
