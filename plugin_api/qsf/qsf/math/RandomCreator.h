// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <random>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	// Class providing random numbers for real types.
	template <typename Real>
	class RandomRealCreator
	{
	public:
		RandomRealCreator(Real min, Real max, unsigned int seed);

		Real create();
	private:
		std::mt19937 mGenerator;
		std::uniform_real_distribution<Real> mRange;
	};


	// inline implementation
	template <typename Real>
	RandomRealCreator<Real>::RandomRealCreator(Real min, Real max, unsigned int seed) :
		mRange(std::min(min, max), std::max(min, max)) // boost needs ordered input
	{
		mGenerator.seed(seed);
	}

	template <typename Real>
	Real RandomRealCreator<Real>::create()
	{
		if (mRange.max() - mRange.min() < 0.01f) // random getter seems to fail with very small distance ending in an infinite loop
			return (mRange.max() + mRange.min()) * .5f;

		return mRange(mGenerator); // regular case
	}


	// Class providing random numbers for integer types
	template <typename Integer>
	class RandomIntegerCreator
	{
	public:
		RandomIntegerCreator(Integer min, Integer max, unsigned int seed);

		Integer create();
	private:
		std::mt19937 mGenerator;
		std::uniform_int_distribution<Integer> mRange;
	};

	// inline implementation
	template <typename Integer>
	RandomIntegerCreator<Integer>::RandomIntegerCreator(Integer min, Integer max, unsigned int seed) :
		mRange(std::min(min, max), std::max(min, max))
	{
		mGenerator.seed(seed);
	}

	template <typename Integer>
	Integer RandomIntegerCreator<Integer>::create()
	{
		// Hopefully the issue with the infinite loop doesn't occur with integers - otherwise we should use a similar shortcut here
		return mRange(mGenerator);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
