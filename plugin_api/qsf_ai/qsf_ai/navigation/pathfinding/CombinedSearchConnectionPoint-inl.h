// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		// CombinedSearchConnectionPoint implementation
		inline CombinedSearchConnectionPoint::CombinedSearchConnectionPoint() :
		mId(getUninitialized(mId)),
		mStartingWorldElementId(getUninitialized(mStartingWorldElementId)),
		mStartingWorldSubElementIndex(getUninitialized(mStartingWorldSubElementIndex)),
		mReachedWorldElementId(getUninitialized(mReachedWorldElementId)),
		mReachedWorldSubElementIndex(getUninitialized(mReachedWorldSubElementIndex))
		{}

		inline CombinedSearchConnectionPoint::CombinedSearchConnectionPoint(unsigned int startingWorldElement, unsigned int startingWorldSubElement,
			unsigned int reachedWorldElement, unsigned int reachedWorldSubElement, const glm::vec3& switchPosition, UnsignedFloat freeSpaceAtSwitchPosition) :
		mId(getUninitialized(mId)),
		mStartingWorldElementId(startingWorldElement),
		mStartingWorldSubElementIndex(startingWorldSubElement),
		mReachedWorldElementId(reachedWorldElement),
		mReachedWorldSubElementIndex(reachedWorldSubElement),
		mSwitchPosition(switchPosition),
		mFreeSpaceAtSwitchPosition(freeSpaceAtSwitchPosition)
		{}

		inline void CombinedSearchConnectionPoint::swapReachedAndStartingData()
		{
			std::swap(mStartingWorldElementId, mReachedWorldElementId);
			std::swap(mStartingWorldSubElementIndex, mReachedWorldSubElementIndex);
		}

		inline SearchContactPoint CombinedSearchConnectionPoint::convertToSearchEntryPointOnStartWorld(const glm::vec3& switchPosition, const glm::vec3& switchDirection) const
		{
			return SearchContactPoint(switchPosition, switchDirection, switchPosition, switchDirection, UnsignedFloat::ZERO, mStartingWorldElementId, mStartingWorldSubElementIndex);
		}

		inline SearchContactPoint CombinedSearchConnectionPoint::convertToSearchEntryPointOnReachedWorld(const glm::vec3& switchPosition, const glm::vec3& switchDirection) const
		{
			return SearchContactPoint(switchPosition, switchDirection, switchPosition, switchDirection, UnsignedFloat::ZERO, mReachedWorldElementId, mReachedWorldSubElementIndex);
		}


		// UsedTransitionData implementation
		inline UsedTransitionData::UsedTransitionData(const CombinedSearchConnectionPoint* connection, const glm::vec3& position, const glm::vec3& direction, std::size_t searchStateOnStartingMap) :
		mCoreData(connection),
		mSwitchPosition(position),
		mSwitchOrientation(direction),
		mSearchStateOnStartingMap(searchStateOnStartingMap)
		{}

		inline SearchContactPoint UsedTransitionData::convertToSearchEntryPointOnStartWorld() const
		{
			SearchContactPoint result = mCoreData->convertToSearchEntryPointOnStartWorld(mSwitchPosition, mSwitchOrientation);
			result.mSearchStateIndex = mSearchStateOnStartingMap;

			return result;
		}

		inline SearchContactPoint UsedTransitionData::convertToSearchEntryPointOnReachedWorld() const
		{
			SearchContactPoint result = mCoreData->convertToSearchEntryPointOnReachedWorld(mSwitchPosition, mSwitchOrientation);

			return result;
		}
	}

	// serialization specialization
	namespace serialization
	{
		template <>
		struct serializer<ai::CombinedSearchConnectionPoint>
		{
			inline static void serialize(BinarySerializer& serializer, ai::CombinedSearchConnectionPoint& data)
			{
				serializer & data.mId;
				serializer & data.mStartingWorldElementId;
				serializer & data.mStartingWorldSubElementIndex;
				serializer & data.mReachedWorldElementId;
				serializer & data.mReachedWorldSubElementIndex;
				serializer & data.mSwitchPosition;
				serializer & data.mFreeSpaceAtSwitchPosition;
			}
		};
	}
}
