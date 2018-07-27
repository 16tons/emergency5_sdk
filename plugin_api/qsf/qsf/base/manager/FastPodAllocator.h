// From http://www.niksula.hut.fi/~hkankaan/Homepages/fastallocation.html


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"

#include <boost/pool/pool.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/// Make the allocation/deallocation faster for small NON-POLYMORPHIC objects.
	///
	/// Usage:
	///
	/// class A { };
	/// =>
	/// class A : public qsf::FastPODAllocator<A> { };
	///
	/// Note that FastPODAllocator<> is not thread safe.
	/// Use as you wish.
	/// @see http://www.boost.org/
	/// @author Hannu Kankaanp‰‰ (hkankaan@cc.hut.fi)
	template <class T>
	class FastPodAllocator
	{
	public:
		static void* operator new(size_t size)
		{
			// Just in case someone tries to misuse this class, at least a run-time error is shown
			QSF_ASSERT(size == sizeof(T), "qsf::FastPodAllocator: Invalid allocation size", QSF_REACT_NONE);
			return s_memPool.malloc();
		}

		static void operator delete(void* deletable, size_t size)
		{
			// Don't delete null pointers
			if (deletable)
				s_memPool.free(deletable);
		}

	protected:
		~FastPodAllocator() {}

	private:
		// Each FastAllocator specialization has its own memory pool
		static boost::pool<> s_memPool;
	};

	// The static variable s_memPool is defined here. It's constructor is passed the object size.
	template <class T>
	boost::pool<> FastPodAllocator<T>::s_memPool(sizeof(T));


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
