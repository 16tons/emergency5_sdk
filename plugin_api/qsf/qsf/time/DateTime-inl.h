// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DateTime::DateTime() :
		mDateTimeInSecondsSinceEpoch(0)
	{
		// Nothing here
	}

	inline DateTime::DateTime(std::time_t dateTimeInSecondsSinceEpoch) :
		mDateTimeInSecondsSinceEpoch(dateTimeInSecondsSinceEpoch)
	{
		// Nothing here
	}

	inline DateTime::operator std::time_t() const
	{
		return mDateTimeInSecondsSinceEpoch;
	}

	// comparators
	inline bool DateTime::operator ==(const DateTime& other) const
	{
		return (mDateTimeInSecondsSinceEpoch == other.mDateTimeInSecondsSinceEpoch);
	}

	inline bool DateTime::operator !=(const DateTime& other) const
	{
		return !(*this == other);
	}

	inline bool DateTime::operator <(const DateTime& other) const
	{
		return (mDateTimeInSecondsSinceEpoch < other.mDateTimeInSecondsSinceEpoch);
	}

	inline bool DateTime::operator >(const DateTime& other) const
	{
		return (mDateTimeInSecondsSinceEpoch > other.mDateTimeInSecondsSinceEpoch);
	}

	inline bool DateTime::operator >=(const DateTime& other) const
	{
		return (mDateTimeInSecondsSinceEpoch >= other.mDateTimeInSecondsSinceEpoch);
	}

	inline bool DateTime::operator <=(const DateTime& other) const
	{
		return (mDateTimeInSecondsSinceEpoch <= other.mDateTimeInSecondsSinceEpoch);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
