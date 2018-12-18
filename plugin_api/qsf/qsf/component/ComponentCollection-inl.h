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


	//[-------------------------------------------------------]
	//[ Public qsf::ComponentCollection::iterator methods     ]
	//[-------------------------------------------------------]
	inline ComponentCollection::iterator::iterator(const ComponentCollection& collection) :
		mPair(collection.getFirstBaseChildPair()),
		mIndex(0)
	{
		// Nothing here
	}

	inline bool ComponentCollection::iterator::operator==(const iterator& other) const
	{
		return (mPair == other.mPair && mIndex == other.mIndex);
	}

	inline bool ComponentCollection::iterator::operator!=(const iterator& other) const
	{
		return (mPair != other.mPair || mIndex != other.mIndex);
	}

	inline ComponentCollection::iterator& ComponentCollection::iterator::operator++()
	{
		QSF_CHECK(nullptr != mPair, "Trying to increment uninitialized or end iterator of component collection", QSF_REACT_THROW);

		// Go to next component in child collection
		++mIndex;
		if (static_cast<size_t>(mIndex) >= mPair->mChild.mComponentArray.size())
		{
			// Go to next child collection
			mPair = mPair->mBase.getNextBaseChildPair(*mPair);
			mIndex = 0;
		}
		return *this;
	}

	inline ComponentCollection::iterator& ComponentCollection::iterator::operator++(int)
	{
		return operator++();
	}

	inline Component* ComponentCollection::iterator::operator*() const
	{
		QSF_CHECK(nullptr != mPair, "Trying to dereference uninitialized or end iterator of component collection", QSF_REACT_THROW);
		ComponentCollection& collection = mPair->mChild;

		QSF_CHECK(static_cast<size_t>(mIndex) < collection.mComponentArray.size(), "Trying to access invalid index " << mIndex << " by component collection iterator", QSF_REACT_THROW);
		return collection.mComponentArray[mIndex];
	}


	//[-------------------------------------------------------]
	//[ Public qsf::ComponentCollection::ComponentList methods]
	//[-------------------------------------------------------]
	template<typename T>
	inline typename ComponentCollection::ComponentList<T>::iterator ComponentCollection::ComponentList<T>::begin() const
	{
		return iterator(mCollection);
	}

	template<typename T>
	inline typename ComponentCollection::ComponentList<T>::iterator ComponentCollection::ComponentList<T>::end() const
	{
		return iterator();
	}

	template<typename T>
	inline bool ComponentCollection::ComponentList<T>::empty() const
	{
		return mCollection.empty();
	}

	template<typename T>
	inline size_t ComponentCollection::ComponentList<T>::size() const
	{
		return mCollection.size();
	}

	template<typename T>
	inline std::vector<T*> ComponentCollection::ComponentList<T>::copyToVector() const
	{
		std::vector<T*> outVector;
		copyToVector(outVector, false);
		return outVector;
	}

	template<typename T>
	void ComponentCollection::ComponentList<T>::copyToVector(std::vector<T*>& outVector, bool clearOutput) const
	{
		if (clearOutput)
			outVector.clear();

		outVector.reserve(outVector.size() + size());
		for (T* component : *this)
		{
			outVector.push_back(component);
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 ComponentCollection::getComponentId() const
	{
		return mComponentId;
	}

	inline const ComponentCollection::ComponentList<Component>& ComponentCollection::getComponents() const
	{
		return mComponentList;
	}

	template<typename T>
	inline const ComponentCollection::ComponentList<T>& ComponentCollection::getComponentList() const
	{
		// The reinterpret_cast is needed here, but does no harm
		return reinterpret_cast<const ComponentList<T>&>(mComponentList);
	}

	inline bool ComponentCollection::empty() const
	{
		return (mSize == 0);
	}

	inline size_t ComponentCollection::size() const
	{
		return mSize;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
