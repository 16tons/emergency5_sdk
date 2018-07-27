// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	namespace ai
	{
		// Defines a height restriction to be used as filter for which obstacles are considered a collision for the voronoi blocker algorithm
		class HeightRestriction
		{
		public:
			// The min and max are automatically calculated internally from both extreme values passed here
			HeightRestriction(bool useTerrain, float height1, float height2);

			float getMinHeight() const;
			float getMaxHeight() const;

			// public data for this simple struct
			bool mUseTerrainHeight;
			float mMinHeight;
			UnsignedFloat mMaxAdditionalHeight; // this is the maximum offset to minHeight that is considered an obstacle
		};


		// inline implementation
		inline HeightRestriction::HeightRestriction(bool useTerrain, float height1, float height2) :
			mUseTerrainHeight(useTerrain),
			mMinHeight(std::min(height1, height2)),
			mMaxAdditionalHeight(std::max(height1, height2) - mMinHeight)
		{}

		inline float HeightRestriction::getMinHeight() const
		{
			return mMinHeight;
		}

		inline float HeightRestriction::getMaxHeight() const
		{
			return mMinHeight + mMaxAdditionalHeight;
		}
	}
}
