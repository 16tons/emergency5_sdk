// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline bool operator==(const Reservation& lhs, const Reservation& rhs)
		{
			return lhs.mBegin == rhs.mBegin &&
				   lhs.mEnd == rhs.mEnd &&
				   lhs.mReserver == rhs.mReserver;
		}

		inline Reservation::Reservation(Time begin, Time end, const NavigationComponent* reserver) :
			mBegin(begin),
			mEnd(end),
			mReserver(reserver)
		{
			QSF_CHECK(mBegin <= mEnd, "Constructing a reservation with negative duration", QSF_REACT_THROW);
		}

		inline Reservation::Reservation(NavigationComponent* reserver) :
			mBegin(Time::ZERO),
			mEnd(Time::ZERO),
			mReserver(reserver)
		{}

		inline Reservation Reservation::unionOf(const Reservation& lhs, const Reservation& rhs)
		{
			QSF_CHECK(lhs.mReserver == rhs.mReserver, "uniting two reservations reserved for different navigation components!", QSF_REACT_THROW);
			return Reservation(std::min(lhs.mBegin, rhs.mBegin), std::max(lhs.mEnd, rhs.mEnd), lhs.mReserver);
		}

		inline bool Reservation::intersect(const Reservation& lhs, const Reservation& rhs)
		{
			return lhs.mEnd >= rhs.mBegin && lhs.mBegin <= rhs.mEnd;
		}
	}
}
