// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/base/GetUninitialized.h"
#include "qsf/base/manager/PagedAllocator.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
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
	*    Prototype hash map data structure class
	*
	*  @remarks
	*    This class combines two main needs we have regarding prototype management:
	*     a) Fast finding a prototype by its ID
	*         -> An "std::map" or "std::unordered_map" would provide this as well, but we found these
	*             to have inferior performance, da memory management or other drawbacks
	*         -> The solution is an own implementation of a hash map, with full control over memory
	*             management and the hashing algorithm
	*     b) Iteration in original insertion order, but with fast insertion and removal
	*         -> We're using a double-chained list embedded into the hash map for this
	*/
	class QSF_API_EXPORT PrototypeHashMap : boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BasePrototypeManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Entry
		{
			uint64	   mId;				///< Unique prototype ID
			Prototype* mPrototype;		///< Prototype pointer; if this is a null pointer, the entry is invalid / not used
			Entry*	   mNextEntry;		///< Next entry in chained list for a hash table index
			Entry*	   mFormerInOrder;	///< Former entry in list representing the original prototype insertion order
			Entry*	   mNextInOrder;	///< Next entry in list representing the original prototype insertion

			Entry() : mId(getUninitialized<uint64>()), mPrototype(nullptr), mNextEntry(nullptr), mFormerInOrder(nullptr), mNextInOrder(nullptr) {}
		};

		// Iterator for cycling through all prototypes in the original insertion order
		struct iterator
		{
		public:
			typedef size_t difference_type;
			typedef Prototype* value_type;
			typedef Prototype*& reference;
			typedef Prototype* pointer;
			typedef std::bidirectional_iterator_tag iterator_category;

			inline iterator(Entry* entry = nullptr) : mEntry(entry) {}

			inline bool operator==(const iterator& other) const	 { return mEntry == other.mEntry; }
			inline bool operator!=(const iterator& other) const	 { return mEntry != other.mEntry; }
			inline iterator& operator--()	 { if (nullptr != mEntry) mEntry = mEntry->mFormerInOrder; return *this; }
			inline iterator& operator++()	 { if (nullptr != mEntry) mEntry = mEntry->mNextInOrder;   return *this; }
			inline iterator& operator--(int) { if (nullptr != mEntry) mEntry = mEntry->mFormerInOrder; return *this; }
			inline iterator& operator++(int) { if (nullptr != mEntry) mEntry = mEntry->mNextInOrder;   return *this; }
			inline Prototype* operator*()	 { return mEntry->mPrototype; }

		protected:
			Entry* mEntry;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PrototypeHashMap();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~PrototypeHashMap();

		/**
		*  @brief
		*    Does the hash map contain any prototypes at all?
		*
		*  @return
		*    "true" if there are no prototypes, "false" if there is at least one
		*/
		inline bool empty() const;

		/**
		*  @brief
		*    Return the number of prototypes in the hash map
		*
		*  @return
		*    The number of prototypes
		*/
		inline size_t size() const;

		/**
		*  @brief
		*    Clear the hash map
		*
		*  @note
		*    - This will not destroy any prototype instance, just unregister them all
		*/
		void clear();

		/**
		*  @brief
		*    Prepare the hash map to hold at least the given number of prototypes
		*
		*  @param[in] size
		*    Number of prototypes
		*
		*  @note
		*    - This has no effect if the reserved size is already larger or equal the given number
		*    - As a consequence, a hash map can only grow, not shrink - except with a "clear" method call
		*/
		void reserve(size_t size);

		/**
		*  @brief
		*    Insert a new prototype into the hash map
		*
		*  @param[in] id
		*    ID of the prototype
		*  @param[in] prototype
		*    The prototype instance
		*
		*  @note
		*    - Warning: There is no check to prevent double insertion
		*/
		void insert(uint64 id, Prototype& prototype);

		/**
		*  @brief
		*    Remove a prototype by its ID
		*
		*  @param[in] id
		*    ID of the prototype
		*
		*  @return
		*    The prototype instance just removed, or a null pointer if the ID was not found
		*/
		Prototype* remove(uint64 id);

		/**
		*  @brief
		*    Find a prototype by its ID
		*
		*  @param[in] id
		*    ID of the prototype
		*
		*  @return
		*    The prototype instance, or a null pointer if the ID was not found
		*/
		inline Prototype* find(uint64 id) const;

		/**
		*  @brief
		*    Return first prototype in original insertion order
		*/
		inline Prototype* front() const;

		/**
		*  @brief
		*    Return last inserted prototype
		*/
		inline Prototype* back() const;

		inline iterator begin() const;
		inline iterator end() const;
		inline iterator rbegin() const;
		inline iterator rend() const;

		inline const Entry* getFirstEntry() const;
		inline const Entry* getLastEntry() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline size_t getHashTableIndexById(uint64 id) const;

		void pushLastPrototypeToFront();
		void removeEntry(Entry& entry, Entry* formerEntry);
		void consistencyCheck();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<Entry>		 mHashTable;
		PagedAllocator<Entry,64> mEntryAllocator;
		size_t					 mNumEntries;
		uint64					 mHashBitmask;
		Entry*					 mFirstInOrder;
		Entry*					 mLastInOrder;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/prototype/data/PrototypeHashMap-inl.h"
