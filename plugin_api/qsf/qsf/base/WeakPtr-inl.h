// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool WeakPtrBase::valid() const
	{
		return (nullptr != mTarget);
	}

	inline void WeakPtrBase::clear()
	{
		// Anything to do?
		if (nullptr != mTarget)
		{
			// Unregister at target
			unregisterFromTarget();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline WeakPtrTarget& WeakPtrBase::getSafe() const
	{
		QSF_CHECK(nullptr != mTarget, "WeakPtr target is not valid", QSF_REACT_THROW);
		return *mTarget;
	}

	inline void WeakPtrBase::set(WeakPtrTarget& t)
	{
		if (nullptr != mTarget)
		{
			if (&t == mTarget)
			{
				// Don't react if there's no change
				return;
			}
			else
			{
				// Remove old registration first
				unregisterFromTarget();
			}
		}

		// Register at target, insert at front
		registerAtTarget(t);
	}

	inline void WeakPtrBase::set(WeakPtrTarget* t)
	{
		if (nullptr == t)
		{
			clear();
		}
		else
		{
			set(*t);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline WeakPtrBase::WeakPtrBase() :
		mTarget(nullptr),
		mPrevious(nullptr),
		mNext(nullptr)
	{
		// Nothing here
	}

	inline WeakPtrBase::~WeakPtrBase()
	{
		clear();
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template <typename T>
	inline WeakPtr<T>::WeakPtr()
	{
		// Nothing here
	}

	template <typename T>
	inline WeakPtr<T>::WeakPtr(T& t)
	{
		set(t);
	}

	template <typename T>
	inline WeakPtr<T>::WeakPtr(T* t)
	{
		set(t);
	}

	template <typename T>
	inline WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
	{
		if (nullptr != other.mTarget)
		{
			set(*static_cast<T*>(other.mTarget));
		}
	}

	template <typename T>
	inline WeakPtr<T>::~WeakPtr()
	{
		// Nothing here
	}

	template <typename T>
	inline T* WeakPtr<T>::get() const
	{
		return static_cast<T*>(mTarget);
	}

	template <typename T>
	inline T& WeakPtr<T>::getSafe() const
	{
		return static_cast<T&>(WeakPtrBase::getSafe());
	}

	template <typename T>
	inline void WeakPtr<T>::set(T& t)
	{
		WeakPtrBase::set(static_cast<WeakPtrTarget&>(t));
	}

	template <typename T>
	inline void WeakPtr<T>::set(T* t)
	{
		WeakPtrBase::set(static_cast<WeakPtrTarget*>(t));
	}

	template <typename T>
	inline void WeakPtr<T>::operator=(T& t)
	{
		WeakPtrBase::set(static_cast<WeakPtrTarget&>(t));
	}

	template <typename T>
	inline void WeakPtr<T>::operator=(T* t)
	{
		WeakPtrBase::set(static_cast<WeakPtrTarget*>(t));
	}

	template <typename T>
	inline void WeakPtr<T>::operator=(const WeakPtr<T>& other)
	{
		WeakPtrBase::set(static_cast<WeakPtrTarget*>(other.mTarget));
	}

	template <typename T>
	inline T& WeakPtr<T>::operator*() const
	{
#ifdef RETAIL
		return *get();
#else
		// Additional safety check
		return getSafe();
#endif
	}

	template <typename T>
	inline T* WeakPtr<T>::operator->() const
	{
#ifdef RETAIL
		return get();
#else
		// Additional safety check
		return &getSafe();
#endif
	}

	template <typename T>
	inline bool WeakPtr<T>::operator==(const WeakPtr<T>& other) const
	{
		return (mTarget == other.mTarget);
	}

	template <typename T>
	inline bool WeakPtr<T>::operator!=(const WeakPtr<T>& other) const
	{
		return (mTarget != other.mTarget);
	}

	template <typename T>
	inline bool WeakPtr<T>::operator==(const T* other) const
	{
		return (static_cast<T*>(mTarget) == other);
	}

	template <typename T>
	inline bool WeakPtr<T>::operator!=(const T* other) const
	{
		return (static_cast<T*>(mTarget) != other);
	}

	template <typename T>
	inline bool WeakPtr<T>::operator==(const T& other) const
	{
		return (static_cast<T*>(mTarget) == &other);
	}

	template <typename T>
	inline bool WeakPtr<T>::operator!=(const T& other) const
	{
		return (static_cast<T*>(mTarget) != &other);
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline WeakPtrTarget::WeakPtrTarget() :
		mWeakPtrList(nullptr)
	{
		// Nothing here
	}

	inline WeakPtrTarget::WeakPtrTarget(const WeakPtrTarget&) :
		mWeakPtrList(nullptr)
	{
		// Don't even think of copying the weak pointer list
	}

	inline WeakPtrTarget::~WeakPtrTarget()
	{
		if (nullptr != mWeakPtrList)
		{
			invalidateAllWeakPtr();
		}
	}

	inline void WeakPtrTarget::invalidateAllWeakPtr()
	{
		// Invalidate all weak pointers referencing this instance
		for (WeakPtrBase* weakPtr = mWeakPtrList; nullptr != weakPtr; )
		{
			WeakPtrBase* next = weakPtr->mNext;
			weakPtr->mTarget = nullptr;
			weakPtr->mPrevious = nullptr;
			weakPtr->mNext = nullptr;
			weakPtr = next;
		}
		mWeakPtrList = nullptr;
	}

	inline void WeakPtrTarget::operator=(const WeakPtrTarget&)
	{
		// Don't even think of copying the weak pointer list
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
