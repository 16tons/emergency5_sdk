// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class WeakPtrTarget;
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
	*    Lightweight weak pointer base class
	*
	*  @remarks
	*    See documentation at "qsf::WeakPtr" just below.
	*/
	class QSF_API_EXPORT WeakPtrBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class WeakPtrTarget;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Check if this instance contains is a valid pointer
		*
		*  @return
		*    "true" if a target object was set and still exists, "false" otherwise
		*/
		inline bool valid() const;

		/**
		*  @brief
		*    Clear the pointer, i.e. set it to a null pointer
		*/
		inline void clear();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline WeakPtrBase();

		/**
		*  @brief
		*    Delete the move constructor, as WeakPtrBase would need a proper move constructor-implementation that ensures that its pointer gets updated in the WeakPtrTarget as well
		*/
		inline WeakPtrBase(WeakPtrBase&&) = delete;

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~WeakPtrBase();

		inline WeakPtrTarget& getSafe() const;

		inline void set(WeakPtrTarget& target);
		inline void set(WeakPtrTarget* target);

		void registerAtTarget(WeakPtrTarget& target);
		void unregisterFromTarget();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		WeakPtrTarget* mTarget;		///< Target pointer; may be a null pointer in case this WeakPtr is not valid
		WeakPtrBase*   mPrevious;	///< Previous element in linked list; may be a null pointer in case this one is the first already
		WeakPtrBase*   mNext;		///< Next element in linked list; may be a null pointer in case this one is the last already


	};


	/**
	*  @brief
	*    Lightweight weak pointer implementation
	*
	*  @remarks
	*    A weak pointer is guaranteed to get invalid in a safe way as soon as its target gets destroyed.
	*    This can be checked with the "valid()" method.
	*
	*  @note
	*    - This weak pointer implementation can only be used with targets derived from "qsf::WeakPtrTarget".
	*    - The target has to store references to the weak pointer instances currently referencing it.
	*       To make the weak pointer mechanism as lightweight as possible for the target, it only stores
	*       a pointer to the first WeakPtr instance, which is the first element in a linked list.
	*/
	template <typename T>
	class WeakPtr : public WeakPtrBase
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline WeakPtr();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] t
		*    Instance to point to
		*/
		inline WeakPtr(T& target);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] t
		*    Instance to point to; may be a null pointer
		*/
		inline WeakPtr(T* target);

		/**
		*  @brief
		*    Copy constructor
		*/
		inline WeakPtr(const WeakPtr<T>& other);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~WeakPtr();

		/**
		*  @brief
		*    Get the internal pointer
		*
		*  @return
		*    The pointer currently set; may be a null pointer
		*/
		inline T* get() const;

		/**
		*  @brief
		*    Get the internal pointer as reference
		*
		*  @return
		*    The pointer currently set as reference
		*
		*  @throw
		*    Throws an exception if the pointer is invalid
		*/
		inline T& getSafe() const;

		/**
		*  @brief
		*    Set the pointer to the given target object
		*
		*  @param[in] target
		*    The object to set the pointer to
		*/
		inline void set(T& target);

		/**
		*  @brief
		*    Set the pointer to the given target object or null pointer
		*
		*  @param[in] target
		*    The object to set the pointer to, a null pointer is allowed
		*/
		inline void set(T* target);

		/**
		*  @brief
		*    Set the pointer to the given target object
		*
		*  @param[in] target
		*    The object to set the pointer to
		*/
		inline void operator=(T& target);

		/**
		*  @brief
		*    Set the pointer to the given target object or null pointer
		*
		*  @param[in] target
		*    The object to set the pointer to, a null pointer is allowed
		*/
		inline void operator=(T* target);

		/**
		*  @brief
		*    Set the pointer to the same target as the given weak pointer instance
		*
		*  @param[in] other
		*    The weak pointer whose target to set
		*/
		inline void operator=(const WeakPtr<T>& other);

		/**
		*  @brief
		*    The pointer currently set as reference
		*
		*  @throw
		*    - Throws an exception if the target is not valid
		*/
		inline T& operator*() const;

		/**
		*  @brief
		*    Access to public methods and members of the target
		*
		*  @throw
		*    - Throws an exception if the target is not valid
		*/
		inline T* operator->() const;

		/**
		*  @brief
		*    Compare with weak pointer
		*
		*  @param[in] other
		*    The weak pointer to compare with
		*
		*  @return
		*    "true" on if both weak pointers have the same target or both are invalid, "false" otherwise
		*/
		inline bool operator==(const WeakPtr<T>& other) const;
		inline bool operator!=(const WeakPtr<T>& other) const;

		/**
		*  @brief
		*    Compare with pointer
		*
		*  @param[in] other
		*    The pointer to compare with
		*
		*  @return
		*    "true" on if weak pointer target is the same as the given pointer, "false" otherwise
		*/
		inline bool operator==(const T* other) const;
		inline bool operator!=(const T* other) const;

		/**
		*  @brief
		*    Compare with reference
		*
		*  @param[in] other
		*    The reference to compare with
		*
		*  @return
		*    "true" on if weak pointer target is valid and points to the given reference, "false" otherwise
		*/
		inline bool operator==(const T& other) const;
		inline bool operator!=(const T& other) const;


	};


	// Comparison operators
	template<typename T> static bool operator==(const T& a, const WeakPtr<T>& b)  { return b == a; }
	template<typename T> static bool operator!=(const T& a, const WeakPtr<T>& b)  { return b != a; }
	template<typename T> static bool operator==(const T* a, const WeakPtr<T>& b)  { return b == a; }
	template<typename T> static bool operator!=(const T* a, const WeakPtr<T>& b)  { return b != a; }


	/**
	*  @brief
	*    Weak pointer target base class
	*/
	class QSF_API_EXPORT WeakPtrTarget
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class WeakPtrBase;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline WeakPtrTarget();

		/**
		*  @brief
		*    Copy constructor
		*/
		inline WeakPtrTarget(const WeakPtrTarget& other);

		/**
		*  @brief
		*    Delete the move constructor, as WeakPtrTarget would need a proper move constructor-implementation that ensures that its pointer gets updated in the WeakPtrBase as well
		*/
		inline WeakPtrTarget(WeakPtrTarget&&) = delete;

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~WeakPtrTarget();

		/**
		*  @brief
		*    Invalidate all weak pointers to this object
		*/
		inline void invalidateAllWeakPtr();

		/**
		*  @brief
		*    Copy operator
		*/
		inline void operator=(const WeakPtrTarget& other);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void weakPtrConsistencyCheck();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		WeakPtrBase* mWeakPtrList;		///< First element in linked list of weak pointers that are referencing this target; may be a null pointer in case there is none


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/WeakPtr-inl.h"
