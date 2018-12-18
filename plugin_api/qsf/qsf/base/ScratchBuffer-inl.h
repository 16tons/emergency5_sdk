// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	template<typename T>
	typename ScratchBuffer<T>::InstanceBuffer ScratchBuffer<T>::mInstanceBuffer;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T>
	inline ScratchBuffer<T>::ScratchBuffer() :
		mInstance(getInstance())
	{
		mInstance.mElement.clear();
	}

	template<typename T>
	inline ScratchBuffer<T>::ScratchBuffer(size_t reserveSize) :
		mInstance(getInstance())
	{
		mInstance.mElement.clear();
		mInstance.mElement.reserve(reserveSize);
	}

	template<typename T>
	inline ScratchBuffer<T>::~ScratchBuffer()
	{
		freeInstance(mInstance);
	}

	template<typename T>
	inline ScratchBuffer<T>::operator Element&()
	{
		return mInstance.mElement;
	}

	template<typename T>
	inline typename ScratchBuffer<T>::Element& ScratchBuffer<T>::operator*()
	{
		return mInstance.mElement;
	}

	template<typename T>
	inline typename ScratchBuffer<T>::Element* ScratchBuffer<T>::operator->()
	{
		return &mInstance.mElement;
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	template<typename T>
	typename ScratchBuffer<T>::Instance& ScratchBuffer<T>::getInstance()
	{
		// Is there an instance already?
		for (Instance& instance : mInstanceBuffer)
		{
			if (!instance.mInUse)
			{
				instance.mInUse = true;
				return instance;
			}
		}

		// Create new instance
		{
			mInstanceBuffer.emplace_back();

			Instance& instance = mInstanceBuffer.back();
			instance.mInUse = true;
			return instance;
		}
	}

	template<typename T>
	void ScratchBuffer<T>::freeInstance(Instance& instance)
	{
		instance.mInUse = false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T>
	inline void StdVectorScratchBuffer<T>::replaceWithBack(size_t index)
	{
		QSF_CHECK(index < this->mInstance.mElement.size(), "Invalid index " << index << " to remove", return);

		// Remove the element at given index by moving the last element here
		if (index < this->mInstance.mElement.size() - 1)
		{
			this->mInstance.mElement[index] = this->mInstance.mElement.back();
		}
		this->mInstance.mElement.pop_back();
	}

	template<typename T>
	inline typename ScratchBuffer<std::vector<T>>::Element::iterator StdVectorScratchBuffer<T>::begin()
	{
		return this->mInstance.mElement.begin();
	}

	template<typename T>
	inline typename ScratchBuffer<std::vector<T>>::Element::iterator StdVectorScratchBuffer<T>::end()
	{
		return this->mInstance.mElement.end();
	}

	template<typename T>
	inline bool StdVectorScratchBuffer<T>::empty() const
	{
		return this->mInstance.mElement.empty();
	}

	template<typename T>
	inline size_t StdVectorScratchBuffer<T>::size() const
	{
		return this->mInstance.mElement.size();
	}

	template<typename T>
	inline void StdVectorScratchBuffer<T>::reserve(size_t size)
	{
		return this->mInstance.mElement.reserve(size);
	}

	template<typename T>
	inline void StdVectorScratchBuffer<T>::push_back(const T& element)
	{
		this->mInstance.mElement.push_back(element);
	}

	template<typename T>
	template<class... argTs>
	inline void StdVectorScratchBuffer<T>::emplace_back(argTs&&... args)
	{
		this->mInstance.mElement.emplace_back(std::forward<argTs>(args)...);
	}

	template<typename T>
	inline StdVectorScratchBuffer<T>& StdVectorScratchBuffer<T>::operator=(const std::vector<T>& toCopy)
	{
		this->mInstance.mElement = toCopy;
		return this->mInstance.mElement;
	}

	template<typename T>
	inline T& StdVectorScratchBuffer<T>::operator[](size_t index)
	{
		return this->mInstance.mElement[index];
	}

	template<typename T>
	inline const T& StdVectorScratchBuffer<T>::operator[](size_t index) const
	{
		return this->mInstance.mElement[index];
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
