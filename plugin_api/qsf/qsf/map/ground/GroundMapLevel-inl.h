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
	inline GroundMapLevel::GroundMapLevel() :
		mWidth(0),
		mHeight(0),
		mMinimumY(0.0f),
		mRangeY(0.0f),
		mRangeYMultiplier(0.0f),
		mValidBlocksStride(0),
		mActive(true),
		mWalkableLevel(0)
	{
		// Nothing here
	}

	inline GroundMapLevel::SampleType* GroundMapLevel::operator*()
	{
		return &mSamples[0];
	}

	inline const GroundMapLevel::SampleType* GroundMapLevel::operator*() const
	{
		return &mSamples[0];
	}

	inline bool GroundMapLevel::empty() const
	{
		return mSamples.empty();
	}

	inline size_t GroundMapLevel::size() const
	{
		return mSamples.size();
	}

	inline float GroundMapLevel::getHeightForSample(SampleType sample) const
	{
		return mMinimumY + static_cast<float>(sample) * mRangeYMultiplier;
	}

	inline uint32 GroundMapLevel::getWidth() const
	{
		return mWidth;
	}

	inline uint32 GroundMapLevel::getHeight() const
	{
		return mHeight;
	}

	inline uint32 GroundMapLevel::getWalkableLevel() const
	{
		return mWalkableLevel;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
