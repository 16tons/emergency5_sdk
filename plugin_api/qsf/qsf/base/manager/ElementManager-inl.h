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

	// Local helper definition
	#define TEMPLATE_DEFINITION template <typename KEY, typename ELEMENT, typename ALLOCATOR, typename ID_GENERATOR>


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TEMPLATE_DEFINITION	ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::ElementManager()
	{
		// Nothing here
	}

	TEMPLATE_DEFINITION	ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::~ElementManager()
	{
		// Destroy all elements
		destroyAllElements();
	}

	TEMPLATE_DEFINITION	const typename ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::ElementMap& ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::getElements() const
	{
		return mElements;
	}

	TEMPLATE_DEFINITION	ELEMENT* ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::findElement(const KEY& key) const
	{
		// Search in element map
		typename ElementMap::const_iterator iterator = mElements.find(key);
		return (mElements.end() != iterator) ? iterator->second : nullptr;
	}

	TEMPLATE_DEFINITION	void ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::reserveElements(size_t numberOfElements)
	{
		mElements.reserve(numberOfElements);
	}

	TEMPLATE_DEFINITION	ELEMENT* ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::createElement()
	{
		// Create the element instance and register it
		const KEY key = mIdGenerator.generate();
		ELEMENT* element = mAllocator.create(key);

		// Try to insert the element
		if (!insertElement(key, *element))
		{
			// Error!
			mAllocator.destroy(element);
			QSF_ERROR("Element key " << key << " is already in use!", QSF_REACT_THROW);
		}

		// Done
		return element;
	}

	TEMPLATE_DEFINITION	ELEMENT* ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::createElementById(const KEY& key)
	{
		// Ensure the ID is not used, yet
		typename ElementMap::iterator iterator = mElements.find(key);
		if (mElements.end() == iterator)
		{
			// Create the element instance and register it
			ELEMENT* element = mAllocator.create(key);
			mElements.emplace(key, element);

			// Call virtual method
			onElementInsertion(key, *element);

			// Done
			return element;
		}

		// Error!
		return nullptr;
	}

	TEMPLATE_DEFINITION	bool ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::insertElement(const KEY& key, ELEMENT& element)
	{
		// Ensure the ID is not used, yet
		typename ElementMap::iterator iterator = mElements.find(key);
		if (mElements.end() == iterator)
		{
			// Register the element instance
			mElements.emplace(key, &element);

			// Call virtual method
			onElementInsertion(key, element);

			// Done
			return true;
		}

		// Error!
		return false;
	}

	TEMPLATE_DEFINITION	bool ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::destroyElement(const KEY& key)
	{
		// Find the element to destroy
		typename ElementMap::iterator iterator = mElements.find(key);
		if (mElements.end() != iterator)
		{
			// First call virtual method
			onElementRemoval(key, *iterator->second);

			// Destroy the element
			mAllocator.destroy(iterator->second);
			mElements.erase(iterator);

			// Done
			return true;
		}

		// Error!
		return false;
	}

	TEMPLATE_DEFINITION	typename ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::ElementMap::iterator ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::destroyElement(typename ElementMap::iterator iterator)
	{
		// First call virtual method
		onElementRemoval(iterator->first, *iterator->second);

		// Destroy the element
		mAllocator.destroy(iterator->second);
		return mElements.erase(iterator);
	}

	TEMPLATE_DEFINITION	void ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::destroyAllElements()
	{
		// Loop through all elements
		for (typename ElementMap::iterator iterator = mElements.begin(); iterator != mElements.end(); ++iterator)
		{
			// Call virtual method
			onElementRemoval(iterator->first, *iterator->second);

			// Destroy the element
			mAllocator.destroy(iterator->second);
		}

		mElements.clear();
		mAllocator.clear();
	}

	TEMPLATE_DEFINITION	ELEMENT* ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::grabElement(const KEY& key)
	{
		// Find the element to remove
		typename ElementMap::iterator iterator = mElements.find(key);
		if (mElements.end() != iterator)
		{
			// Get the element
			ELEMENT* element = iterator->second;

			// Call virtual method
			onElementRemoval(key, *element);

			// Remove the element
			mElements.erase(iterator);

			// Done
			return element;
		}

		// Error!
		return nullptr;
	}

	TEMPLATE_DEFINITION KEY ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::generateId()
	{
		return mIdGenerator.generate();
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	TEMPLATE_DEFINITION	typename ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::ElementMap& ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::getElementsWritable()
	{
		return mElements;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::ElementManager methods         ]
	//[-------------------------------------------------------]
	TEMPLATE_DEFINITION	void ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::onElementInsertion(const KEY& key, ELEMENT& element)
	{
		// Default implementation is empty
	}

	TEMPLATE_DEFINITION	void ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::onElementRemoval(const KEY& key, ELEMENT& element)
	{
		// Default implementation is empty
	}


	// Destroy local helper definition again
	#undef TEMPLATE_DEFINITION


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
