// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool PrototypeHashMap::empty() const
	{
		return (mNumEntries == 0);
	}

	inline size_t PrototypeHashMap::size() const
	{
		return mNumEntries;
	}

	inline Prototype* PrototypeHashMap::find(uint64 id) const
	{
		if (mNumEntries > 0)
		{
			// Start in right hash table entry
			const Entry* entry = &mHashTable[getHashTableIndexById(id)];

			// Search in chained list of entries
			for (;;)
			{
				if (entry->mId == id)
				{
					// Found right entry
					return entry->mPrototype;
				}
				else if (nullptr == entry->mNextEntry)
				{
					// Reached end of list, ID not found
					return nullptr;
				}

				entry = entry->mNextEntry;
			}
		}

		// No entries
		return nullptr;
	}

	inline Prototype* PrototypeHashMap::front() const
	{
		return (nullptr != mFirstInOrder) ? mFirstInOrder->mPrototype : nullptr;
	}

	inline Prototype* PrototypeHashMap::back() const
	{
		return (nullptr != mLastInOrder) ? mLastInOrder->mPrototype : nullptr;
	}

	inline PrototypeHashMap::iterator PrototypeHashMap::begin() const
	{
		return iterator(mFirstInOrder);
	}

	inline PrototypeHashMap::iterator PrototypeHashMap::end() const
	{
		return iterator();
	}

	inline PrototypeHashMap::iterator PrototypeHashMap::rbegin() const
	{
		return iterator(mLastInOrder);
	}

	inline PrototypeHashMap::iterator PrototypeHashMap::rend() const
	{
		return iterator();
	}

	inline const PrototypeHashMap::Entry* PrototypeHashMap::getFirstEntry() const
	{
		return mFirstInOrder;
	}

	inline const PrototypeHashMap::Entry* PrototypeHashMap::getLastEntry() const
	{
		return mLastInOrder;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline size_t PrototypeHashMap::getHashTableIndexById(uint64 id) const
	{
		// Randomly generated prototype IDs have an approximately uniform distribution in the upper 32 bits
		return static_cast<size_t>((id + (id >> 32)) & mHashBitmask);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
