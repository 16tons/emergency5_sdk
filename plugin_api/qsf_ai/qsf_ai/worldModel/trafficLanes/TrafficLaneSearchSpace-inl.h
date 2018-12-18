// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline TrafficLaneSearchSpace::~TrafficLaneSearchSpace()
		{
			for (auto entry : mStatesByUniqueIndex)
				delete entry;
		}

		inline std::size_t TrafficLaneSearchSpace::getIndex(const TrafficLaneSearchState& state) const
		{
			return state.getIndex();
		}

		inline std::size_t TrafficLaneSearchSpace::getNumExpandedSearchStates() const
		{
			return mStatesByUniqueIndex.size();
		}

		inline TrafficLaneSearchState& TrafficLaneSearchSpace::getStateByUniqueIndex(std::size_t index) const
		{
			QSF_CHECK(index < mStatesByUniqueIndex.size(), "Index " << index << " out of bounds when accessing search state",
				QSF_REACT_THROW);

			return *mStatesByUniqueIndex[index];
		}
	}
}
