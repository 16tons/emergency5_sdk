// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

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
	*    Value mapper for uint64
	*
	*  @note
	*    - This class tells CAMP how to handle uint64 as a string type
	*/
	template <>
	struct ValueMapper<uint64>
	{

		/**
		*  @brief
		*    CAMP type associated to uint64
		*/
		static const int type = camp::stringType;

		/**
		*  @brief
		*    Convert from uint64 to std::string
		*
		*  @param[in] source
		*    Source uint64 instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static std::string to(const uint64& source)
		{
			char buffer[128];
			#ifdef WIN32
				sprintf_s(buffer, 128, "%I64u", source);
			#else
				sprintf(buffer, "%llu", source);
			#endif
			return buffer;
		}

		/**
		*  @brief
		*    Convert from any type to uint64
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a uint64
		*
		*  @note
		*     - We simply reuse ValueMapper<std::string>.
		*/
		template <typename T>
		static uint64 from(const T& source)
		{
			return boost::lexical_cast<uint64>(ValueMapper<std::string>::from(source));
		}

	};


} // camp_ext
