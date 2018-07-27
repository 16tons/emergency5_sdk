// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	* @brief Data type for encapsulating time information.
	*  It might represents points in time and time spans.
	*  The native resolution is currently microseconds with one microsecond being the minimal representable value.
	*  These objects are lightweight and are copyable via the default mechanisms.
	*/
	class QSF_API_EXPORT Time
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const Time ZERO;	///< A zero value for time
		static const Time MAX;	///< A maximum value for time


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the current system time as string (In UTC)
		*
		*  @return
		*    The current time as string "%Y-%m-%d %H:%M:%S" (don't use it directly as filename)
		*/
		static std::string currentSystemTimeAsString();

		/**
		*  @brief
		*    Return the current time as filename string (In UTC)
		*
		*  @return
		*    The current time as filename string "%Y-%m-%d-%H-%M-%S"
		*/
		static std::string currentSystemTimeAsFilenameString();

		/**
		*  @brief
		*    Return the current local time as string
		*
		*  @return
		*    The current time as string "%Y-%m-%d %H:%M:%S" (don't use it directly as filename)
		*/
		static std::string currentLocalTimeAsString();

		/**
		*  @brief
		*    Return the current local time as filename string
		*
		*  @return
		*    The current time as filename string "%Y-%m-%d-%H-%M-%S"
		*/
		static std::string currentLocalTimeAsFilenameString();

		/**
		*  @brief
		*    Return a Time object representing the current time with medium (~10ms) precision
		*
		*  @return
		*    A Time object representing the current time
		*/
		static Time now();

		/**
		*  @brief
		*    Return a Time object representing the current time with high (microsecond) precision
		*
		*  @return
		*    A Time object representing the current time
		*/
		static Time highResolutionNow();

		// Named constructors to avoid any ambiguity when creating times.
		//@{
		inline static Time fromWeeks(float weeks);
		inline static Time fromWeeks(double weeks);
		inline static Time fromWeeks(int64 weeks);
		inline static Time fromDays(float days);
		inline static Time fromDays(double days);
		inline static Time fromDays(int64 days);
		inline static Time fromHours(float hours);
		inline static Time fromHours(double hours);
		inline static Time fromHours(int64 hours);
		inline static Time fromMinutes(float minutes);
		inline static Time fromMinutes(double minutes);
		inline static Time fromMinutes(int64 minutes);
		inline static Time fromSeconds(float seconds);
		inline static Time fromSeconds(double seconds);
		inline static Time fromSeconds(int64 seconds);
		inline static Time fromMilliseconds(int64 milliseconds);
		inline static Time fromMicroseconds(int64 microseconds);
		//@}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// Default constructor creates a ZERO time instance.
		inline Time();

		// Accessing the stored time in different granularities.
		//@{
		inline float getWeeks() const;
		inline float getDays() const;
		inline float getHours() const;
		inline float getMinutes() const;
		inline float getSeconds() const;
		inline int64 getMilliseconds() const;
		inline int64 getMicroseconds() const;
		//@}

		// Set time directly.
		//@{
		inline void setWeeks(float weeks);
		inline void setDays(float days);
		inline void setHours(float hours);
		inline void setMinutes(float minutes);
		inline void setSeconds(float seconds);
		inline void setMilliseconds(int64 milliseconds);
		inline void setMicroseconds(int64 microseconds);
		//@}

		// arithmetic operators
		//@{
		inline Time operator - () const;
		inline Time operator + (const Time& other) const;
		inline Time& operator +=(const Time& other);
		inline Time operator - (const Time& other) const;
		inline Time& operator -=(const Time& other);
		inline Time operator *(float multiplier) const;
		inline Time& operator *=(float multiplier);
		inline Time operator /(float divisor) const;
		inline Time& operator /=(float divisor);
		inline Time operator % (const Time& other) const;
		inline Time& operator %=(const Time& other);
		inline float operator /(const Time& divisor) const;
		//@}

		// comparators
		//@{
		inline bool operator ==(const Time& other) const;
		inline bool operator !=(const Time& other) const;
		inline bool operator < (const Time& other) const;
		inline bool operator > (const Time& other) const;
		inline bool operator >=(const Time& other) const;
		inline bool operator <=(const Time& other) const;
		//@}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Private constructor from microseconds.
		// Use named constructors to avoid any ambiguity.
		inline explicit Time(int64 microseconds);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int64 mMicroseconds; // native resolution
		// TODO(vs) use ticks as native resolution delaying the expensive conversion towards microseconds until it is needed for output for example.


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/Time-inl.h"
