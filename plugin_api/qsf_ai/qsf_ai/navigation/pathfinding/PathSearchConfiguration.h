// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Percentage.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/navigation/pathfinding/PositionCorrection.h"
#include "qsf_ai/navigation/SearchStepToIgnore.h"

#include <qsf/base/GetUninitialized.h>

#include <glm/glm.hpp>

#include <vector>


namespace qsf
{
	class TransformComponent;

	namespace ai
	{
		class NavigationComponent;
		/**
		* A path search configuration holds all the parameters potentially relevant for tweaking a path search request.
		* Not every value might be used with all representations.
		* Since it is only a collection of largely independent values it is realized with public variables like a struct.
		*/
		class PathSearchConfiguration
		{
		public:
			friend class NavigationTaskThread; // needed to access the private default constructor during serialization

			// NavigationGoal is cloned during the process.
			// This constructor allows to set a selection of options where there is typically no good default value.
			PathSearchConfiguration(const glm::vec3& startPos, const glm::quat& startOrientation, const NavigationGoal& goal, unsigned int moverType, unsigned int primaryMapId);

			// Constructor for the typical use case triggered from the pathfinding system that collects all the data from the components available
			PathSearchConfiguration(const TransformComponent& mc, const NavigationComponent& nc);

			// Shortcuts to evaluate the movement options single flags
			//@{
			bool canMoveForward() const;
			bool canMoveBackwards() const;
			bool canManeuver() const;
			bool canShrinkTurningRadius() const;
			bool isPrioritySearch() const;
			bool shouldMoveToClosestInCaseOfFailure() const;
			//@}

			unsigned int mMoverType; // Generic type of moving entity with application specific meaning
			unsigned int mPrimaryMapId; // Unique id of the primary map to use for pathfinding. Needs to be valid
			unsigned int mSecondaryMapId; // Optional unique id of a secondary map to use for pathfinding if the path can't be found on the primary map alone.
			uint64 mSearchingEntityId; // Optional id of the searching entity. Uninitialized if no specific entity available. Allows ignoring yourself as an obstacle.
			glm::vec3 mStartPosition; // The position where the search should start at.
			glm::quat mStartOrientation; // The orientation at the initial position in world coordinates
			// TODO(sw) std::auto_ptr is deprecated, should be replaced by std::unique_ptr
			std::auto_ptr<NavigationGoal> mGoal; // always initialized, never a nullptr
			TurningConstraint mTurningConstraint; // Constraint according to the turning rate to be used
			// Multiplicative factor for penalizing costs when moving backwards. If set to uninitialized, driving backwards is not allowed
			// Already contains the global factor configured in the PathfindingSystem.
			UnsignedFloat mBackwardsCostMultiplier;
			// Optional hint to speed up the start location search.
			// If this id is provided we can often skip the brute force search and only investigate this area and its intersecting areas.
			unsigned int mLastKnownMapElementId;
			short mMovementOptions; // Defines the advanced options for moving. See NavigationComponent::mMovementOptions for details.
			position::CorrectionSettings mPositionCorrectionSettings; // The settings and options how to correct start and goal positions.
			bool mDebugPortals; // Flag to force a debug drawing of the crossed portals
			// Define an optional overwrite for the path smoothing method to apply. Temporary expert option.
			// See the similar property defined inside the movement mode for details
			boost::optional<bool> mShouldUseFunnelSmoothing;
			// Ignore these moves during your next search
			std::vector<SearchStepToIgnore> mSearchStepsToIgnore;

			// Helper method encapsulating the search inside the ignored steps vector
			bool shouldIgnoreSearchStep(unsigned int mapId, unsigned int areaId, unsigned int nodeId) const;

		private:
			// Mainly initialize the settings from the current navigation component state
			position::CorrectionSettings copyCorrectionSettingsFrom(const NavigationComponent& navi) const;

			PathSearchConfiguration(); // only needed during serialization inside the PathfindingSystem
		};
	}
}

#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration-inl.h"
