// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/worldModel/AreaConfiguration.h"

#include <qsf/time/Time.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>


namespace qsf
{
	namespace ai
	{
		/** Specific class to note a reservation of a world area for a certain time period.
		* Reservations are used to guide traffic along the maps and avoid collision situations.
		*/
		class Reservation
		{
		public:
			/** Constructs reservation from a begin time until an inclusive end time and by a navigation component that occupies the area.
			*  Note that time begin has to be smaller or equal to time end.
			* A nullptr is allowed for technical reasons though it is not really useful to use this option for real reservations.
			*/
			Reservation(Time begin, Time end, const NavigationComponent* reserver);
			explicit Reservation(NavigationComponent* reserver = nullptr);

			static Reservation unionOf(const Reservation& lhs, const Reservation& rhs);
			static bool intersect(const Reservation& lhs, const Reservation& rhs);

			Time mBegin;
			Time mEnd;
			const NavigationComponent* mReserver;
		};
	}

	//binary serialization specialization
	namespace serialization
	{
		template <>
		struct serializer<ai::Reservation>
		{
			static void serialize(BinarySerializer& serializer, ai::Reservation& data);
		};
	}
}

#include "qsf_ai/navigation/reservation/Reservation-inl.h"
