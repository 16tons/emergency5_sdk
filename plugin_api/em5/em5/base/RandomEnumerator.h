// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	namespace constructionpolicy
	{
		enum Policy
		{
			COPY,	///< Copy vector passed to RandomEnumerator constructor
			SWAP	///< Swap with vector passed to RandomEnumerator constructor, leaving it empty afterwards
		};
	}


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Helper class for iterating over a list of instances of a specific type, in a random order
	*
	*  @remarks
	*    No element will be returned twice
	*/
	template<typename T>
	class RandomEnumerator
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor to pass a vector of elements, which will be copied
		*/
		inline explicit RandomEnumerator(const std::vector<T*>& inputToCopy, uint32 maxTries = std::numeric_limits<uint32>::max()) :
			mTriesLeft(maxTries)
		{
			mElements = inputToCopy;
		}

		/**
		*  @brief
		*    Constructor to pass a vector of elements, with the option to swap contents
		*/
		inline RandomEnumerator(constructionpolicy::Policy policy, std::vector<T*>& inputToSwapWith, uint32 maxTries = std::numeric_limits<uint32>::max()) :
			mTriesLeft(maxTries)
		{
			if (policy == constructionpolicy::SWAP)
				mElements.swap(inputToSwapWith);
			else
				mElements = inputToSwapWith;
		}

		// This constructor must not be used, see comment inside
		inline explicit RandomEnumerator(std::vector<T*>& inputToCopy, uint32 maxTries = std::numeric_limits<uint32>::max()) :
			mTriesLeft(maxTries)
		{
			// If you get there, you used a RandomEnumerator constructor with a non-const std::vector reference as first argument.
			// In this case, use the constructor with "constructionpolicy::Policy" argument to specify whether to copy or swap contents with the given std::vector.
			// Both may be beneficial depending on your case:
			//  - constructionpolicy::COPY is safer, but produces overhead due to copying of data
			//  - constructionpolicy::SWAP will swap contents and consequently leave the given std::vector empty afterwards, which is fine if it is not used afterwards any more
			T::Used_ambiguous_RandomEnumerator_constructor;
		}

		/**
		*  @brief
		*    Check if all elements were read out already
		*/
		inline bool empty() const
		{
			return mElements.empty();
		}

		/**
		*  @brief
		*    Randomly choose an element that was not returned before
		*
		*  @return
		*    Pointer to another element, which gets randomly chosen from the list; or a null pointer in case we already had all elements
		*/
		inline T* getNext()
		{
			// Nothing left?
			if (mElements.empty() || mTriesLeft == 0)
				return nullptr;

			// Choose a random position and take element from there
			const size_t position = qsf::Random::getRandomSize(0, mElements.size() - 1);
			T* element = mElements[position];

			// Remove this element by moving the last element here
			if (position < mElements.size() - 1)
			{
				mElements[position] = mElements.back();
			}
			mElements.pop_back();

			// Done
			--mTriesLeft;
			return element;
		}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline explicit RandomEnumerator(uint32 maxTries = std::numeric_limits<uint32>::max()) :
			mTriesLeft(maxTries)
		{
			// Subclass using this constructor will have to fill mElements
		}


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<T*> mElements;
		uint32 mTriesLeft;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
