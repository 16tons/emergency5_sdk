// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <camp/valuemapper.hpp>

#include <boost/algorithm/string.hpp>

#include <stdio.h>	// For "sprintf_s()"/"sprintf()"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::Time
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::Time as a string type
	*/
	template <>
	struct ValueMapper<qsf::Time>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::Time
		*/
		static const int type = camp::stringType;

		/**
		*  @brief
		*    Convert from qsf::Time to std::string
		*
		*  @param[in] source
		*    Source qsf::Time instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static std::string to(const qsf::Time& source)
		{
			// TODO(sw) should be replaced with std::to_string
			char buffer[128];
			#ifdef WIN32
				// TODO(mk) Display time text like this 1h20m40s
				// TODO(co) Please don't display time text like this 1h20m40s. This in here is not for "display", it's a generic "to string" were it must be possible to easily parse it. If you need a nicer visualization inside a GUI, add it somewhere in there.
				sprintf_s(buffer, 128, "%I64d", source.getMicroseconds());
			#else
				sprintf(buffer, "%lld", source.getMicroseconds());
			#endif
			return buffer;
		}

		/**
		*  @brief
		*    Convert from any type to qsf::Time
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a qsf::Time
		*
		*  @note
		*     - We simply reuse ValueMapper<std::string>.
		*/
		template <typename T>
		static qsf::Time from(const T& source)
		{
			std::string timeString = ValueMapper<std::string>::from(source);
			if (timeString.empty())
			{
				return qsf::Time::ZERO;
			}
			else if (timeString.back() == 's')
			{
				// Might be seconds or milliseconds check if the character before the s is an m
				if (timeString.length() >= 3 && timeString[timeString.length() - 2] == 'm')
				{
					// Notation in milliseconds (ms)
					timeString.erase(timeString.length() - 2, 2);
					return qsf::Time::fromMilliseconds(std::stoul(timeString));
				}
				else
				{
					// Notation in seconds
					timeString.erase(timeString.length() - 1, 1);
					return qsf::Time::fromSeconds(std::stof(timeString));
				}
			}
			else if (timeString.back() == 'm')
			{
				// Notation in minutes
				timeString.erase(timeString.length() - 1, 1);
				return qsf::Time::fromMinutes(std::stof(timeString));
			}
			else if (timeString.back() == 'h')
			{
				// Notation in hours
				timeString.erase(timeString.length() - 1, 1);
				return qsf::Time::fromHours(std::stof(timeString));
			}
			else
			{
				// Notation in microseconds (default)
				return qsf::Time::fromMicroseconds(boost::lexical_cast<uint64>(timeString));
			}
		}

	};


} // camp_ext
