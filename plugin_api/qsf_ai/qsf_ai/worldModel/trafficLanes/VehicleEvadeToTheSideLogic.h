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
		* Specific logic that tries to plan an evade route for vehicles on a traffic lane world.
		*/
		class QSF_AI_API_EXPORT VehicleEvadeToTheSideLogic
		{
		public:
			VehicleEvadeToTheSideLogic(const NavigationComponent& evading);

			// Main calculation function that actually wraps all the work.
			// Returns whether an evade path could be found and was written to the reference parameter.
			// In case the entity is found to be already perfectly evaded, the path remains empty while there is still true returned
			bool tryCalculateEvadePath(Path& path);
		private:
			// Complex constructor work put into a function
			void initializeData();
			// Calculate an evade path in the form of waypoints that still need smoothing applied.
			// Returns whether the reference parameter was written with correct data
			bool createEvadePathFromCurrent(std::deque<Waypoint>& evadePath);
			// Calculate an evade path from scratch, based on the current entity position
			bool createEvadePathFromScratch(std::deque<Waypoint>& evadepath);
			// Takes an already calculated path, applies smoothing and writes it to the reference parameter
			void smoothPath(std::deque<Waypoint>& evadePath, Path& path) const;

			// Helper function to see whether we need a reduced braking distance if starting between two portals.
			// It returns an optional factor to apply to the braking distance in case there is a reason to reduce it.
			// If an uninitialized value is returned, the portals are not appropriate to make this decision
			// In case a zero is returned, the entity is already close enough to the side of the road to not evade at all
			boost::optional<Percentage> tryCalculateBrakingDistanceFactor(const DynamicPortal& previous, const DynamicPortal& next) const;

			const NavigationComponent& mEvadingNavi;
			glm::vec3 mStartPosition;
			glm::vec3 mStartOrientation;
			UnsignedFloat mBrakingDistance;
			boost::optional<DynamicPortal> mStartSupportPortal;
			boost::optional<DynamicPortal> mEndSupportPortal;
		};
	}
}
