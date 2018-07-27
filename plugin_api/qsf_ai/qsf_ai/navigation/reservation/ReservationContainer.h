// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/reservation/Reservation.h"
#include "qsf_ai/Export.h"

#include <qsf/serialization/binary/BasicTypeSerialization.h>

#include <boost/optional.hpp>

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>


namespace qsf
{
	namespace ai
	{
		class ReservationDebugGroup;

		namespace detail
		{
			// helper struct, holds a reservation and an area index, only used in boost::multi_index structure
			struct ReservationContainerEntry
			{
				ReservationContainerEntry(const Reservation& reservation, const AreaConfiguration& entryIndex);
				ReservationContainerEntry(const Reservation& reservation, unsigned int mapId, unsigned int areaId);

				Reservation mReservation;
				AreaConfiguration mEntryIndex;
			};
		}

		/**
		*  @brief
		*    This defines the interface for a functor that you need to provide in your specific implementation.
		*    It is called every time there is a collision between a new reservation and older existing ones.
		*/
		struct ReservationConflictResolver
		{
			// Enum describing the relation certain areas have, to be passed to the function call below as additional information.
			// Used to identify flags in a bitmask, so choose values accordingly.
			enum AreaRelation
			{
				NONE = 0, // no specific relation
				AREAS_INTERSECT = 1 // this area is reserved because an intersecting area should be reserved
			};

			typedef std::vector<Reservation> ContainerType;
			virtual ~ReservationConflictResolver();

			// Encapsulates the result of an attempt to try to insert some reservation
			struct Result
			{
			public:
				explicit Result(bool allowsInsert, const boost::optional<ContainerType::const_iterator>& blockingReservationIndex = boost::optional<ContainerType::const_iterator>());

				bool mInsertionAllowed;
				boost::optional<ContainerType::const_iterator> mBlockingReservation; // Optionally an iterator to an reservation that was the reason for returning false in the end
			};

			// Returns whether the reservation should be added.
			// Needs to set the flags in the boolean vector passed to whether the conflicting reservations passed need to be removed.
			// A vector in the same size of the collisions collection is passed and the values are initialized to false which means nothing should be removed initially.
			// @note The return value has the opposite effect as the parameter flags because the initial situation is different.
			// The conflicts are already inside and the new reservation could be inserted. False means in both cases that nothing should be changed.
			// The results are applied independently in all cases which means you can write a resolver that removes collisions even if the new reservation is not added (returning false).
			virtual Result canInsertAfterResolve(const std::vector<ContainerType::const_iterator>& collisions, const Reservation& reservation, const AreaConfiguration& reservationId, std::vector<bool>& deleteFlags, unsigned int flags) = 0;
		};

		// Default resolving functor that can be used to resolve collisions.
		// This default implementation implements a first come first served semantics which always keeps existing reservations and only inserts new ones if they don't collide at all.
		struct QSF_AI_API_EXPORT AllowsNoConflictsResolver : public ReservationConflictResolver
		{
			//Default implementation returns true if all collisions are between same entity, otherwise not
			virtual Result canInsertAfterResolve(const std::vector<ContainerType::const_iterator>& collisions, const Reservation& reservation, const AreaConfiguration& reservationId, std::vector<bool>& deleteFlags, unsigned int flags) override;
		};

		struct AllowsAllConflictsResolver : public ReservationConflictResolver
		{
			// Returns true for whether the reservation should be added.
			virtual Result canInsertAfterResolve(const std::vector<ContainerType::const_iterator>& collisions, const Reservation& reservation, const AreaConfiguration& reservationId, std::vector<bool>& deleteFlags, unsigned int flags) override;
		};


		// Class holds Reservations and allows indexing them by area id.
		// example implementation using a map from area IDs to multisets of reservations.
		class QSF_AI_API_EXPORT ReservationContainer
		{
		public:
			friend struct qsf::serialization::serializer<ai::ReservationContainer>; // for binary serialization direct data access

			ReservationContainer();

			void eraseExpiredReservations(Time beforeTime);
			ReservationConflictResolver::Result canReservationBeEntered(const Reservation& reservation, const AreaConfiguration& resId, unsigned int flags = 0);
			void insertReservation(const Reservation& reservation, const AreaConfiguration& resId);
			void removeReservationsAlongPath(const NavigationComponent& reserver);
			void forceRemoveAllReservations(const NavigationComponent& reserver);
			void removeAllReservations();

			// Sanity check - verifies that the ordering is still correct among all reservations
			bool verifyOrdering() const;

			// Passes ownership of the resolver to the container and resets the parameter during the call
			void setResolvingFunctor(std::unique_ptr<ReservationConflictResolver>& resolver);

			// logs memory consumption
			void logMemoryConsumption() const;

			template <typename Action>
			void forAllOverlappingReservationsDo(const Reservation& reservation, const AreaConfiguration& resId, Action& action);

			typedef std::vector<Reservation> SingleAreaReservations;		// This was a multiset originally, but that's just overhead because we seldomly get more than 3 entries here
			typedef std::unordered_map<AreaConfiguration, SingleAreaReservations> ContainerType;

		private:
			// internal core method that tests as well as enters reservations based on the simulateOnly flag
			// Returns whether the reservation could be added and optionally writes a blocking reservation to the reference parameter
			ReservationConflictResolver::Result tryInsertReservation(const Reservation& reservation, const AreaConfiguration& resId, unsigned int flags = 0, bool simulateOnly = false);

			ContainerType mReservations;
			std::unique_ptr<ReservationConflictResolver> mResolveCollisions;

			size_t mOldCheckIndex;

			// Helper function encapsulates removing all reservations from one area for one reserver
			// Returns the number of entries that were removed
			unsigned int eraseAllReservedBy(SingleAreaReservations& reservations, const NavigationComponent* reserver);

			// Helper function to facilitate debugging.
			bool isReservationForCurrentArea(const Reservation& reservation, const AreaConfiguration& areaConfig) const;
		};
	}
}

//inline implementations
#include "qsf_ai/navigation/reservation/ReservationContainer-inl.h"
