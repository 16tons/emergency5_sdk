// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    Value mapper for int64
	*
	*  @note
	*    - This class tells CAMP how to handle int64 as a string type
	*/
	template <>
	struct ValueMapper<int64>
	{

		/**
		*  @brief
		*    CAMP type associated to int64
		*/
		static const int type = camp::stringType;

		/**
		*  @brief
		*    Convert from int64 to std::string
		*
		*  @param[in] source
		*    Source int64 instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static std::string to(const int64& source)
		{
			char buffer[128];
			#ifdef WIN32
				sprintf_s(buffer, 128, "%I64d", source);
			#else
				sprintf(buffer, "%lld", source);
			#endif
			return buffer;
		}

		/**
		*  @brief
		*    Convert from any type to int64
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a int64
		*
		*  @note
		*     - We simply reuse ValueMapper<std::string>.
		*/
		template <typename T>
		static int64 from(const T& source)
		{
			return boost::lexical_cast<int64>(ValueMapper<std::string>::from(source));
		}

	};


} // camp_ext
