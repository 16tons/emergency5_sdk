// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/WorldModel.h"

#include <boost/thread/shared_mutex.hpp>

#include <thread>


namespace qsf
{
	namespace ai
	{
		/**
		* A managed navigation map write access is a thin wrapper around a world model that provides unique write access.
		* This contains a lock that denies any concurrent access.
		* This object needs to be kept alive for the time of the client accessing the navigation map inside.
		* Otherwise we risk data races.
		* Not copyable by design which is the purpose of the unique lock.
		*
		* There is a constructor variant allows to only try to acquire a lock which returns an invalid ReadAccess object.
		* This version is only used internally to allow locking several maps in an atomic way and objects with a failed lock should never leave the world model manager
		*/
		class QSF_AI_API_EXPORT ManagedNavigationMapWriteAccess
		{
		public:
			// Empty dummy class to allow using instances like a boolean Flag inside the constructor
			class TryLock
			{};

			// Default constructor enforcing the lock and waiting until granted as required
			ManagedNavigationMapWriteAccess(WorldModel& navigationMap, boost::shared_mutex& accessMutex);
			// Variant of the constructor that only tries to lock and returns with an object that failed to lock. Instances should never leave the world model manager
			ManagedNavigationMapWriteAccess(WorldModel& navigationMap, boost::shared_mutex& accessMutex, TryLock onlyTryToLock);

			operator WorldModel&();
			WorldModel& operator *();
			WorldModel& get();

			// Return whether the object was locked successfully. Should always return true outside the world model manager
			bool wasLocked() const;

		private:
			boost::upgrade_lock<boost::shared_mutex> mAccessLock;
			boost::upgrade_to_unique_lock<boost::shared_mutex> mUniqueAccessLock;
			WorldModel& mNavigationMap;
		};
	}
}
