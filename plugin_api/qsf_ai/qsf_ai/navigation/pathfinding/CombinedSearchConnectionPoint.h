// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/pathfinding/SearchContactPoint.h"

#include <glm/glm.hpp>

#include <cstddef>


namespace qsf
{
	namespace ai
	{
		/**
		* Very simple class holding all static data for a connection point between two maps during a combined search.
		* This connection is directed and distinguishes between the starting and the reached world.
		* Copyable via the default mechanisms.
		*/
		class CombinedSearchConnectionPoint
		{
		public:
			// The default constructor is needed for technical reasons for using it inside STL containers and the serialization
			CombinedSearchConnectionPoint();
			CombinedSearchConnectionPoint(unsigned int startingWorldElement, unsigned int startingWorldSubElement, unsigned int reachedWorldElement, unsigned int reachedWorldSubElement,
				const glm::vec3& switchPosition, UnsignedFloat freeSpaceAtSwitchPosition);

			// Swap all starting values with their reached counterparts
			void swapReachedAndStartingData();

			// Inner class holding the exact values for the transition spatial data that is only calculated at runtime.
			// The position here is an exact position and may deviate from the static position which is a midpoint for an area.
			// Very simple struct with public data
			class SpatialData
			{
			public:
				glm::vec3 mPosition;
				glm::vec3 mDirection;
				UnsignedFloat mDistanceOnStartWorld;
				UnsignedFloat mDistanceOnReachedWorld;
			};

			// Create a search entry point from the combination of the stored and passed data
			//@{
			SearchContactPoint convertToSearchEntryPointOnStartWorld(const glm::vec3& switchPosition, const glm::vec3& switchDirection) const;
			SearchContactPoint convertToSearchEntryPointOnReachedWorld(const glm::vec3& switchPosition, const glm::vec3& switchDirection) const;
			//@}

			// public data
			unsigned int mId; // Unique id of the transitions between the two connected maps.Is created automatically when adding connections so there is no use setting it in a constructor

			unsigned int mStartingWorldElementId; // The id of the world element we are moving through to the switch position on the starting world
			unsigned int mStartingWorldSubElementIndex; // An optional sub element index inside the world element on the starting world

			unsigned int mReachedWorldElementId; // The id of the world element we are moving through from the switch position on the reached world
			unsigned int mReachedWorldSubElementIndex; // An optional sub element index inside the world element on the reached world

			glm::vec3 mSwitchPosition;
			UnsignedFloat mFreeSpaceAtSwitchPosition;
		};


		/**
		* Simple class holding all static and dynamic (entity specific) data for a connection point between two maps during a combined search.
		* This connection is directed and distinguishes between the starting and the reached world.
		* Copyable via the default mechanisms.
		*/
		class UsedTransitionData
		{
		public:
			UsedTransitionData(const CombinedSearchConnectionPoint* connection, const glm::vec3& position, const glm::vec3& direction, std::size_t searchStateOnStartingMap);

			// Create a search entry point from the combination of the stored and passed data
			//@{
			SearchContactPoint convertToSearchEntryPointOnStartWorld() const;
			SearchContactPoint convertToSearchEntryPointOnReachedWorld() const;
			//@}

			const CombinedSearchConnectionPoint* mCoreData;
			glm::vec3 mSwitchPosition;
			glm::vec3 mSwitchOrientation;
			std::size_t mSearchStateOnStartingMap;
		};
	}
}

#include "qsf_ai/navigation/pathfinding/CombinedSearchConnectionPoint-inl.h"
