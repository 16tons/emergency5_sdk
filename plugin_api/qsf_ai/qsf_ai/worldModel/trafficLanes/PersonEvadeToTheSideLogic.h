// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/Export.h"

#include <deque>


namespace qsf
{
	namespace ai
	{
		/**
		* Specific logic that tries to plan an evade route for persons on a traffic lane world.
		*/
		class QSF_AI_API_EXPORT PersonEvadeToTheSideLogic
		{
		public:
			// The evasion config is made up of the position for the entity to evade and its movement direction
			PersonEvadeToTheSideLogic(const NavigationComponent& evading, const SpatialConfiguration3D& approachingConfig, UnsignedFloat approachingLateralFreeSpaceRequired);

			// Main calculation function that actually wraps all the work.
			// Returns whether an evade path could be found and was written to the reference parameter.
			// In case the entity is found to be already perfectly evaded, the path remains empty while there is still true returned
			bool tryCalculateEvadePath(Path& path);
		private:
			// Helper function to create a slightly randomized offset for how far we want to evade beyond just exactly avoiding the impact
			static UnsignedFloat getRandomEvasionSafetyDistance();

			const NavigationComponent& mEvadingNavi;
			glm::vec3 mCurrentPosition;
			const SpatialConfiguration3D mApproachingConfig;
			const UnsignedFloat mApproachingLateralFreeSpaceRequired;
		};
	}
}
