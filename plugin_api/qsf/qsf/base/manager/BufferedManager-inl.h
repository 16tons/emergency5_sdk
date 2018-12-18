// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	// Local helper definition
	#define TEMPLATE_DEFINITION template <typename KEY, typename ELEMENT, typename CONTAINER>


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	TEMPLATE_DEFINITION	BufferedManager<KEY,ELEMENT,CONTAINER>::BufferedManager() :
		mWriteProtectionCount(0)
	{
		// Nothing here
	}

	TEMPLATE_DEFINITION	BufferedManager<KEY,ELEMENT,CONTAINER>::~BufferedManager()
	{
		// Nothing here
		// -> Note that remaining elements get not removed automatically!
	}

	TEMPLATE_DEFINITION	const typename BufferedManager<KEY,ELEMENT,CONTAINER>::ElementMap& BufferedManager<KEY,ELEMENT,CONTAINER>::getElements() const
	{
		return mElements;
	}

	TEMPLATE_DEFINITION	ELEMENT* BufferedManager<KEY,ELEMENT,CONTAINER>::findElement(const ElementKey& key)
	{
		// Search in element map
		typename ElementMap::iterator iterator = mElements.find(key);
		if (iterator == mElements.end())
		{
			// Search in insertion buffer
			iterator = mInsertionBuffer.find(key);
			if (iterator == mInsertionBuffer.end())
			{
				// Not found
				return nullptr;
			}
		}

		// Found
		return &iterator->second;
	}

	TEMPLATE_DEFINITION	const ELEMENT* BufferedManager<KEY,ELEMENT,CONTAINER>::findElement(const ElementKey& key) const
	{
		// Search in element map
		typename ElementMap::const_iterator iterator = mElements.find(key);
		if (iterator == mElements.end())
		{
			// Search in insertion buffer
			iterator = mInsertionBuffer.find(key);
			if (iterator == mInsertionBuffer.end())
			{
				// Not found
				return nullptr;
			}
		}

		// Found
		return &iterator->second;
	}

	TEMPLATE_DEFINITION	bool BufferedManager<KEY,ELEMENT,CONTAINER>::insertElement(const ElementKey& key, Element& element)
	{
		if (getWriteProtection())
		{
			// Write protection is enabled, so don't touch mElements now
			// -> Store the element in the insertion buffer first
			mInsertionBuffer.emplace(key, element);
		}
		else
		{
			// Insert the element directly into the element map
			mElements.emplace(key, element);

			// Call virtual method
			onElementInsertion(key, element);
		}

		// Done
		return true;
	}

	TEMPLATE_DEFINITION	bool BufferedManager<KEY,ELEMENT,CONTAINER>::removeElement(const ElementKey& key)
	{
		// Find the element to remove
		typename ElementMap::iterator iterator = mElements.find(key);
		if (iterator == mElements.end())
		{
			// Not found in the insertion buffer, too?
			iterator = mInsertionBuffer.find(key);
			if (iterator == mInsertionBuffer.end())
				return false;

			// Remove it from the insertion buffer again
			mInsertionBuffer.erase(iterator);
		}
		else
		{
			// Is write protection enabled?
			if (getWriteProtection())
			{
				// It is, so don't touch mElements!
				// -> Just mark the element for being removed
				mRemovalBuffer.push_back(key);
			}
			else
			{
				// First call virtual method
				onElementRemoval(key, iterator->second);

				// Remove the element
				mElements.erase(iterator);
			}
		}

		// Done
		return true;
	}

	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::removeAllElements()
	{
		// Make sure write protection is disabled, then both buffers are empty and can be ignored
		QSF_CHECK(!getWriteProtection(), "BufferedManager::removeAllElements() should not be called with write protection enabled!", QSF_REACT_NONE);

		// Loop through all elements
		for (typename ElementMap::iterator iterator = mElements.begin(); iterator != mElements.end(); ++iterator)
		{
			// Call virtual method
			onElementRemoval(iterator->first, iterator->second);
		}
		mElements.clear();
	}

	TEMPLATE_DEFINITION	bool BufferedManager<KEY,ELEMENT,CONTAINER>::getWriteProtection() const
	{
		return (mWriteProtectionCount > 0);
	}

	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::setWriteProtection(bool enable)
	{
		if (enable)
		{
			++mWriteProtectionCount;
		}
		else
		{
			--mWriteProtectionCount;

			QSF_CHECK(mWriteProtectionCount>=0, "Write protection can never be unbalanced and be smaller than 0", QSF_REACT_THROW);

			if (mWriteProtectionCount == 0)
			{
				// Insert new elements that have been added while write protection was active
				flushInsertionBuffer();

				// Remove elements that have been marked for removal while write protection was active
				flushRemovalBuffer();
			}
		}
	}

	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::flushInsertionBuffer()
	{
		// Anything to do?
		if (mInsertionBuffer.empty())
		{
			return;
		}

		// Loop through list of elements to be inserted
		for (typename ElementMap::iterator iterator = mInsertionBuffer.begin(); iterator != mInsertionBuffer.end(); ++iterator)
		{
			// Insert the element
			mElements.insert(*iterator);

			// Call virtual method
			onElementInsertion(iterator->first, iterator->second);
		}

		// All elements have been inserted, so clear this buffer
		mInsertionBuffer.clear();
	}

	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::flushRemovalBuffer()
	{
		// Anything to do?
		if (mRemovalBuffer.empty())
		{
			return;
		}

		// Loop through list of elements to be removed
		for (typename ElementKeyArray::const_iterator iterator = mRemovalBuffer.cbegin(); iterator != mRemovalBuffer.cend(); ++iterator)
		{
			// Find the element within the element map
			const typename ElementMap::iterator elementIterator = mElements.find(*iterator);
			if (elementIterator != mElements.cend())
			{
				// First call virtual method
				onElementRemoval(elementIterator->first, elementIterator->second);

				// Remove the element
				mElements.erase(elementIterator);
			}
		}

		// All elements have been removed, so clear this buffer
		mRemovalBuffer.clear();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BufferedManager methods        ]
	//[-------------------------------------------------------]
	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::onElementInsertion(const ElementKey& key, Element& element)
	{
		// Default implementation is empty
	}

	TEMPLATE_DEFINITION	void BufferedManager<KEY,ELEMENT,CONTAINER>::onElementRemoval(const ElementKey& key, Element& element)
	{
		// Default implementation is empty
	}


	// Remove local helper definition again
	#undef TEMPLATE_DEFINITION


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
