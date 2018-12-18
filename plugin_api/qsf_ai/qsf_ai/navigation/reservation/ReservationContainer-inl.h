// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/debug/DebugDrawManager.h>
#include <qsf/debug/group/DebugGroupManager.h>
#include <qsf/debug/request/CompoundDebugDrawRequest.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		inline ReservationContainer::ReservationContainer() :
			mResolveCollisions(new AllowsNoConflictsResolver()),
			mOldCheckIndex(0)
		{
		}

		inline void ReservationContainer::setResolvingFunctor(std::unique_ptr<ReservationConflictResolver>& resolver)
		{
			QSF_CHECK(resolver.get(), "Trying to set nullptr as conflict resolver!",
				return);

			mResolveCollisions.swap(resolver);
			resolver.reset(); // We should not have the old implementation suddenly on the outside - perhaps a default implementation that we never provided ourselves
		}

		inline ReservationConflictResolver::~ReservationConflictResolver()
		{}

		template <typename Action>
		inline void ReservationContainer::forAllOverlappingReservationsDo(const Reservation& reservation, const AreaConfiguration& resId, Action& action)
		{
			auto& areaRange = mReservations[resId];
			// Unable to use the sorting via end times here - we need to go through the reservations manually

			for (const auto& r : areaRange)
			{
				// TODO(tl): Implemented a hack to ignore inactive navigation components, so we just need to set entities to inactive to disable all their reservations
				// TODO(vs) This !isRunning check is pretty ugly and should not be necessary but it seems some reservations are invalidated in between the ReservationSystem global update and the steering system component update
				if (!r.mReserver || !r.mReserver->isRunning() || !r.mReserver->isActive())
					continue; // deprecated entry, should not be here but currently happens to often to assert

				if (Reservation::intersect(r, reservation))
					action(r);
			}
		}

		inline void ReservationContainer::removeAllReservations()
		{
			mReservations.clear();
		}

		inline ReservationConflictResolver::Result ReservationContainer::canReservationBeEntered(const Reservation& reservation, const AreaConfiguration& resId, unsigned int flags)
		{
			return tryInsertReservation(reservation, resId, flags, true);
		}

		inline ReservationConflictResolver::Result AllowsAllConflictsResolver::canInsertAfterResolve(const std::vector<ContainerType::const_iterator>&, const Reservation&, const AreaConfiguration&, std::vector<bool>&, unsigned int)
		{
			//dummy implementation does always insert
			return Result(true);
		}

		inline ReservationConflictResolver::Result::Result(bool allowsInsert, const boost::optional<ContainerType::const_iterator>& blockingReservationIndex) :
			mInsertionAllowed(allowsInsert),
			mBlockingReservation(blockingReservationIndex)
		{}

		namespace detail
		{
			inline ReservationContainerEntry::ReservationContainerEntry(const Reservation& reservation, const AreaConfiguration& entryIndex) :
				mReservation(reservation),
				mEntryIndex(entryIndex)
			{}

			inline ReservationContainerEntry::ReservationContainerEntry(const Reservation& reservation, unsigned int mapId, unsigned int areaId) :
				mReservation(reservation),
				mEntryIndex(AreaConfiguration(mapId, areaId))
			{}
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::detail::ReservationContainerEntry>
		{
			inline static void serialize(BinarySerializer& serializer, ai::detail::ReservationContainerEntry& data)
			{
				serializer & data.mEntryIndex;
				serializer & data.mReservation;
			}
		};

		template <>
		struct serializer<ai::ReservationContainer>
		{
			inline static void serialize(BinarySerializer& serializer, ai::ReservationContainer& data)
			{
				serializer & data.mReservations;
			}
		};
	}
}
