// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <ctime>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Data type for encapsulating date time information (in UTC)
	*
	*  @note
	*    - This represents a point in time
	*    - The date time is stored in seconds since epoch (epoch = 01.01.1970)
	*    - This is a simple version, there is currently now access to parts of an date time
	*/
	class QSF_API_EXPORT DateTime
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const DateTime EPOCH;	///< Holds the epoch date time


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the current system time as date time object
		*
		*  @return
		*    The current time as date time object
		*/
		static DateTime now();

		/**
		*  @brief
		*    Creates an date time object from string (in UTC)
		*
		*  @param[in] utcDateTimeString:
		*    The string from which an date time object should be created (expected format: %Y-%m-%d %H:%M:%S)
		*
		*  @return
		*    The UTC date time string as date time object
		*/
		static DateTime fromStringUtc(const std::string& utcDateTimeString);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor creates a EPOCH time instance
		*/
		inline DateTime();

		inline explicit DateTime(std::time_t dateTimeInSecondsSinceEpoch);

		/**
		*  @brief
		*    Return the date time as string (in UTC)
		*
		*  @return
		*    The date time as string "%Y-%m-%d %H:%M:%S" (don't use it directly as filename)
		*/
		std::string toStringUtc() const;

		/**
		*  @brief
		*    Return the date time as string (in local time)
		*
		*  @return
		*    The date time as string "%Y-%m-%d %H:%M:%S" (don't use it directly as filename)
		*/
		std::string toStringLocal() const;

		inline operator std::time_t() const;

		// comparators
		//@{
		bool operator ==(const DateTime& other) const;
		bool operator !=(const DateTime& other) const;
		bool operator < (const DateTime& other) const;
		bool operator > (const DateTime& other) const;
		bool operator >=(const DateTime& other) const;
		bool operator <=(const DateTime& other) const;
		//@}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::time_t mDateTimeInSecondsSinceEpoch;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/time/DateTime-inl.h"
