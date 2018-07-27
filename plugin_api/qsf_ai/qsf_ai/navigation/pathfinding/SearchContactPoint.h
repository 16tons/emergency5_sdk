// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/math/GlmHelper.h>

#include <glm/glm.hpp>

#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		/** A search contact point is an extreme point of a search connected to the actual navigation map area.
		* It is used for start and goal positions both for the final ones as well as transition positions.
		* It tracks all the data for one point where the search on the graph may actually start or end and is connected to the real start or goal position.
		* Simple class that is copyable via the default mechanisms and is realized like a struct with public data members.
		*/
		class SearchContactPoint
		{
		public:
			// Very technical delta. We always want to use only the best navigation areas but due to round-off errors, some elements that should be equally close from a logical point of view have slightly different distances.
			// For the lane worlds this is most prominent when a point is a bit outside an outer node and thus is closest to the connecting circle around the node for example.
			// In this case it is imperative that the two lanes connecting to the node are considered equal because otherwise the entity might not find the close lane.
			// In general this is the highest delta that may be between distanceToLane and zero to consider the node 'inside' the legal space as well as between two solutions considered
			// equally far off the legal space.
			static const UnsignedFloat MAX_ROUND_OFF_DISTANCE_ERROR;

			SearchContactPoint(); // The default constructor initializes all single value members to illegal values and all vectors to the zero vector (which is illegal for an orientation)
			SearchContactPoint(const glm::vec3& originalPos, const boost::optional<glm::vec3>& originalOrientation, const glm::vec3& closestLegalPos, const boost::optional<glm::vec3>& orientationAtClosestLegalPos,
				UnsignedFloat distanceToLegalPosition, unsigned int worldElementId, unsigned int worldSubElementId, std::size_t searchStateIndex = getUninitialized<std::size_t>());

			// The input value for the position that lead to the calculation of the other values - the actual start or goal position that was requested in the first place
			glm::vec3 mOriginalPosition;
			// The input value for the original orientation
			boost::optional<glm::vec3> mOriginalOrientation;
			// The closest legal position found that is on the border of a world element if the original position was illegal or equal to the original position (ignoring height corrections) if it was legal
			glm::vec3 mClosestLegalPosition;
			// The orientation when reaching the closest legal position from the original position.
			boost::optional<glm::vec3> mOrientationAtClosestLegalPosition;

			// Distance that needs to be moved on the world abstraction to reach the closest legal position.
			// This may be != getDistance(mOriginalPosition, mClosestLegalPosition) because the distance along the height axis normally is ignored and does not lead to any movement
			UnsignedFloat mDistanceToMoveToLegalPosition;
			// The unique id of the world area that is closest to the original position
			unsigned int mWorldElementId;
			// The unique id of the world sub area that is closest to the original position.
			// Optional value for worlds whose areas are further split beyond the atomic search resolution.
			// These can use this additional index to specify the part that the closest point is connected to via this value.
			// The semantics of this value therefore changes with the actual navigation map implementation.
			unsigned int mWorldSubElementIndex;
			// The index of the associated search state in the used search space.
			std::size_t mSearchStateIndex;
		};


		// inline implementation
		inline SearchContactPoint::SearchContactPoint() :
			mDistanceToMoveToLegalPosition(getUninitialized(mDistanceToMoveToLegalPosition)),
			mWorldElementId(getUninitialized(mWorldElementId)),
			mWorldSubElementIndex(getUninitialized(mWorldSubElementIndex)),
			mSearchStateIndex(getUninitialized(mSearchStateIndex))
		{}

		inline SearchContactPoint::SearchContactPoint(const glm::vec3& originalPos, const boost::optional<glm::vec3>& originalOrientation, const glm::vec3& closestLegalPos,
			const boost::optional<glm::vec3>& orientationAtClosestLegalPos, UnsignedFloat distanceToLegalPosition, unsigned int worldElementId,
			unsigned int worldSubElementId, std::size_t searchStateIndex) :
		mOriginalPosition(originalPos),
		mOriginalOrientation(originalOrientation),
		mClosestLegalPosition(closestLegalPos),
		mOrientationAtClosestLegalPosition(orientationAtClosestLegalPos),
		mDistanceToMoveToLegalPosition(distanceToLegalPosition),
		mWorldElementId(worldElementId),
		mWorldSubElementIndex(worldSubElementId),
		mSearchStateIndex(searchStateIndex)
		{}

		// A weighted search contact point is just a combination of a search entry point and the costs associated with using it.
		// This should usually be the movement through illegal terrain distance and zero if the position was already legal.
		// But it is also possible to realize other types of penalties this way.
		template <typename SearchCost>
		class WeightedSearchContactPoint
		{
		public:
			WeightedSearchContactPoint(const SearchContactPoint& searchEntryPoint, const SearchCost& cost) :
			mSearchContactPoint(searchEntryPoint),
			mCosts(cost)
			{}

			SearchContactPoint mSearchContactPoint;
			SearchCost mCosts;
		};
	}
}
