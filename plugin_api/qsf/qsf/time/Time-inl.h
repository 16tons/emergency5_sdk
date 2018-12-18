// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline Time Time::fromWeeks(float weeks)
	{
		return fromHours(weeks * 7.0f * 24.0f);
	}

	inline Time Time::fromWeeks(double weeks)
	{
		return fromHours(weeks * 7.0 * 24.0);
	}

	inline Time Time::fromWeeks(int64 weeks)
	{
		return fromHours(weeks * 7 * 24);
	}

	inline Time Time::fromDays(float days)
	{
		return fromHours(days * 24.0f);
	}

	inline Time Time::fromDays(double days)
	{
		return fromHours(days * 24.0);
	}

	inline Time Time::fromDays(int64 days)
	{
		return fromHours(days * 24);
	}

	inline Time Time::fromHours(float hours)
	{
		return fromMicroseconds(static_cast<int64>(hours * 3600.0f * 1e6f));
	}

	inline Time Time::fromHours(double hours)
	{
		return fromMicroseconds(static_cast<int64>(hours * 3600.0 * 1e6));
	}

	inline Time Time::fromHours(int64 hours)
	{
		return fromMicroseconds(hours * 3600 * 1000000);
	}

	inline Time Time::fromMinutes(float minutes)
	{
		return fromMicroseconds(static_cast<int64>(minutes * 60.0f * 1e6f));
	}

	inline Time Time::fromMinutes(double minutes)
	{
		return fromMicroseconds(static_cast<int64>(minutes * 60.0 * 1e6));
	}

	inline Time Time::fromMinutes(int64 minutes)
	{
		return fromMicroseconds(minutes * 60 * 1000000);
	}

	inline Time Time::fromSeconds(float seconds)
	{
		return fromMicroseconds(static_cast<int64>(seconds * 1e6f));
	}

	inline Time Time::fromSeconds(double seconds)
	{
		return fromMicroseconds(static_cast<int64>(seconds * 1e6));
	}

	inline Time Time::fromSeconds(int64 seconds)
	{
		return fromMicroseconds(seconds * 1000000);
	}

	inline Time Time::fromMilliseconds(int64 milliseconds)
	{
		return fromMicroseconds(milliseconds * 1000);
	}

	inline Time Time::fromMicroseconds(int64 microseconds)
	{
		return Time(microseconds);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Time::Time() :
		mMicroseconds(0)
	{}

	inline float Time::getWeeks() const
	{
		return getDays() / 7.0f;
	}

	inline float Time::getDays() const
	{
		return getHours() / 24.0f;
	}

	inline float Time::getHours() const
	{
		return getMinutes() / 60.0f;
	}

	inline float Time::getMinutes() const
	{
		return getSeconds() / 60.0f;
	}

	inline float Time::getSeconds() const
	{
		return static_cast<float>(getMicroseconds()) * 1e-6f;
	}

	inline int64 Time::getMilliseconds() const
	{
		return getMicroseconds() / 1000;
	}

	inline int64 Time::getMicroseconds() const
	{
		return mMicroseconds;
	}

	inline void Time::setWeeks(float weeks)
	{
		setHours(weeks * 7.0f * 24.0f);
	}

	inline void Time::setDays(float days)
	{
		setHours(days * 24.0f);
	}

	inline void Time::setHours(float hours)
	{
		mMicroseconds = static_cast<int64>(hours * 3600.0f * 1e6f);
	}

	inline void Time::setMinutes(float minutes)
	{
		mMicroseconds = static_cast<int64>(minutes * 60.0f * 1e6f);
	}

	inline void Time::setSeconds(float seconds)
	{
		mMicroseconds = static_cast<int64>(seconds * 1e6f);
	}

	inline void Time::setMilliseconds(int64 milliseconds)
	{
		mMicroseconds = milliseconds * 1000;
	}

	inline void Time::setMicroseconds(int64 microseconds)
	{
		mMicroseconds = microseconds;
	}

	// arithmetic operators
	inline Time Time::operator -() const
	{
		return Time(-mMicroseconds);
	}

	inline Time Time::operator +(const Time& other) const
	{
		return Time(mMicroseconds + other.getMicroseconds());
	}

	inline Time& Time::operator +=(const Time& other)
	{
		mMicroseconds += other.getMicroseconds();
		return *this;
	}

	inline Time Time::operator -(const Time& other) const
	{
		return Time(mMicroseconds - other.getMicroseconds());
	}

	inline Time& Time::operator -=(const Time& other)
	{
		mMicroseconds -= other.getMicroseconds();
		return *this;
	}

	inline Time Time::operator *(float multiplier) const
	{
		return Time((int64)((float)mMicroseconds * multiplier));
	}

	inline Time& Time::operator *=(float multiplier)
	{
		mMicroseconds = (int64)((float)mMicroseconds * multiplier);
		return *this;
	}

	inline Time Time::operator /(float divisor) const
	{
		return Time((int64)((float)mMicroseconds / divisor));
	}

	inline Time& Time::operator /=(float divisor)
	{
		mMicroseconds = (int64)((float)mMicroseconds / divisor);
		return *this;
	}

	inline Time Time::operator %(const Time& other) const
	{
		return Time(mMicroseconds % other.mMicroseconds);
	}

	inline Time& Time::operator %=(const Time& other)
	{
		mMicroseconds %= other.mMicroseconds;
		return *this;
	}

	inline float Time::operator /(const Time& divisor) const
	{
		return (float)((double)mMicroseconds / (double)divisor.mMicroseconds);
	}

	// comparators
	inline bool Time::operator ==(const Time& other) const
	{
		return mMicroseconds == other.getMicroseconds();
	}

	inline bool Time::operator !=(const Time& other) const
	{
		return !(*this == other);
	}

	inline bool Time::operator <(const Time& other) const
	{
		return mMicroseconds < other.getMicroseconds();
	}

	inline bool Time::operator >(const Time& other) const
	{
		return mMicroseconds > other.getMicroseconds();
	}

	inline bool Time::operator >=(const Time& other) const
	{
		return mMicroseconds >= other.getMicroseconds();
	}

	inline bool Time::operator <=(const Time& other) const
	{
		return mMicroseconds <= other.getMicroseconds();
	}


	namespace serialization
	{
		// specialization for use with the binary serializer
		template <>
		struct serializer<Time>
		{
			inline static void serialize(BinarySerializer& serializer, Time& time)
			{
				if (serializer.isReading())
				{
					int64 microseconds = 0;
					serializer & microseconds;
					time.setMicroseconds(microseconds);
				}
				else
				{
					int64 microseconds = time.getMicroseconds();
					serializer & microseconds;
				}
			}
		};
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline Time::Time(int64 microseconds) :
		mMicroseconds(microseconds)
	{}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
