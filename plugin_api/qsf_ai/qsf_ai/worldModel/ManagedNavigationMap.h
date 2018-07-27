// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/WorldModel.h"
#include "qsf_ai/worldModel/ManagedNavigationMapReadAccess.h"
#include "qsf_ai/worldModel/ManagedNavigationMapWriteAccess.h"

#include <thread>
#include <memory>


namespace qsf
{
	namespace ai
	{
		/**
		* A managed navigation map is a thin wrapper around a world model.
		* It mainly contains access control structures to make sure than no concurrent writes are tried.
		* It has ownership for the navigation map passed.
		*/
		class QSF_AI_API_EXPORT ManagedNavigationMap
		{
		public:
			ManagedNavigationMap(WorldModel* navigationMap);

			// Grants non-exclusive read only access to the navigation map until the requesting resource destroys the shared pointer.
			// When this function returns, no write access is possible until this object is destroyed
			// Otherwise, the client sleeps inside this function until he gets access rights
			std::unique_ptr<ManagedNavigationMapReadAccess> acquireReadAccess();
			// Variant of the above function that returns a read access that was not locked instead of sleeping in case the map is already locked for writing.
			// Necessary to break up multiple interleaved access requests.
			std::unique_ptr<ManagedNavigationMapReadAccess> acquireReadAccess(ManagedNavigationMapReadAccess::TryLock onlyTryToLock);

			// Grants exclusive write access to the navigation map until the requesting resource destroys the shared pointer.
			// When this function returns, no concurrent access is possible until this object is destroyed
			// Otherwise, the client sleeps inside this function until he gets access rights
			std::unique_ptr<ManagedNavigationMapWriteAccess> acquireWriteAccess();
			// Variant of the above function that returns a read access that was not locked instead of sleeping in case the map is already locked for writing.
			// Necessary to break up multiple interleaved access requests.
			std::unique_ptr<ManagedNavigationMapWriteAccess> acquireWriteAccess(ManagedNavigationMapWriteAccess::TryLock onlyTryToLock);

		private:
			std::unique_ptr<WorldModel> mNavigationMap;
			mutable boost::shared_mutex mCommunicationMutex; // for synchronization writes and reads access to the resource
		};
	}
}
