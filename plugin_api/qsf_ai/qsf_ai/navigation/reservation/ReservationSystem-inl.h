// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent.h"

namespace qsf
{
	namespace ai
	{
		inline ReservationSystem::ReservationSystem(Prototype* prototype) :
			StandardSystem<NavigationComponent, ReservationSystem>(prototype, NAME)
		{}

		inline void ReservationSystem::createDebugOutput(const NavigationComponent&) const
		{}

		inline effort::Indicator ReservationSystem::updateComponent(NavigationComponent&, const JobArguments&) const
		{
			return effort::TRIVIAL;
		}

		inline ReservationConflictResolver::Result ReservationSystem::canReservationBeEntered(const Reservation& reservation, const AreaConfiguration& resId, unsigned int flags)
		{
			return mReservations.canReservationBeEntered(reservation, resId, flags);
		}

		inline void ReservationSystem::insertReservation(const Reservation& reservation, const AreaConfiguration& resId)
		{
			mReservations.insertReservation(reservation, resId);
		}

		inline void ReservationSystem::removeReservationsAlongPath(const NavigationComponent& navi)
		{
			mReservations.removeReservationsAlongPath(navi);
		}

		inline void ReservationSystem::forceRemoveAllReservations(const NavigationComponent& navi)
		{
			mReservations.forceRemoveAllReservations(navi);
		}

		template <typename Action>
		inline void ReservationSystem::forAllOverlappingReservationsDo(const Reservation& reservation, const AreaConfiguration& resId, Action& action)
		{
			mReservations.forAllOverlappingReservationsDo(reservation, resId, action);
		}

		inline void ReservationSystem::serialize(BinarySerializer& serializer)
		{
			Component::serialize(serializer);

			serializer & mReservations;
		}
	}
}
