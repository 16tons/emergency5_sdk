// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/reservation/ReservationContainer.h"

#include "qsf_ai/Export.h"
#include "qsf_ai/base/StandardSystem.h"

#include <qsf/reflection/CampDefines.h>

namespace qsf
{
	namespace ai
	{
		/** AI system to handle reservations for world areas
		* Noncopyable by inheritance because Job as well as System are noncopyable.
		*/
		class QSF_AI_API_EXPORT ReservationSystem : public StandardSystem<NavigationComponent, ReservationSystem>
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier
			static const char* NAME;

			// Dummy struct used as a named flag to influence some methods to only simulate the behavior
			struct SimulateOnly{};

			ReservationSystem(Prototype* prototype);

			// If you want to overwrite the reservation conflict resolving functor, set it here.
			// Otherwise a default implementation is used.
			void setResolvingFunctor(std::unique_ptr<ReservationConflictResolver>& functor);

			// Public template interface requirement of qsf::ai::StandardSystem
			//@{
			effort::Indicator updateComponent(NavigationComponent& navi, const JobArguments& arguments) const;
			void createDebugOutput(const NavigationComponent& navi) const;
			void updateGlobals(const JobArguments&);
			//@}

			// public Component implementation override
			//@{
			virtual void onSetSimulating(bool simulating) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Tries to insert a reservation for the specified area. Uses the resolving functor to resolve
			// collisions, and returns whether the reservation could be inserted. Does not perform changes
			// flags supplied by steering are combinations of the SteeringSystem::AreaRelation enum.
			ReservationConflictResolver::Result canReservationBeEntered(const Reservation& reservation, const AreaConfiguration& resId, unsigned int flags = 0);

			// Inserts all reservations by this reserver for the given area. Uses functor collision resolving to
			// resolve collisions, but ignores the case in which an area can not be reserved (this has to be checked
			// using tryInsertReservation before calling this method)
			void insertReservation(const Reservation& reservation, const AreaConfiguration& resId);

			// Removes all registered reservations for one reserver along its path according to the reservation range
			void removeReservationsAlongPath(const NavigationComponent& navi);

			// makes a brute force search to be sure to remove all reservations.
			// Is known to be inefficient and it is only an emergency solution.
			// It would be better to rely solely on removeReservationsAlongPath but according to the crashes we get it seems we sometimes fail to delete some reservation.
			// Replacing the raw pointers with smart pointers to detect the situation proved to be a performance problem on the other side.
			void forceRemoveAllReservations(const NavigationComponent& navi);

			// logs memory consumption
			void logMemoryConsumption() const;

			// Performs an action for all reservations at resId that overlap with reservation.
			template <typename Action>
			void forAllOverlappingReservationsDo(const Reservation& reservation, const AreaConfiguration& resId, Action& action);
		private:
			ReservationContainer mReservations;
			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::ReservationSystem);

#include "qsf_ai/navigation/reservation/ReservationSystem-inl.h"
