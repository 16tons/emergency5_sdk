// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"

#include <camp/valuemapper.hpp>


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for qsf::StringHash
	*
	*  @note
	*    - This class tells CAMP how to handle qsf::StringHash as an integer type
	*/
	template <>
	struct ValueMapper<qsf::StringHash>
	{

		/**
		*  @brief
		*    CAMP type associated to qsf::StringHash
		*/
		static const int type = camp::intType;

		/**
		*  @brief
		*    Convert from qsf::StringHash to std::string
		*
		*  @param[in] source
		*    Source qsf::StringHash instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static long to(const qsf::StringHash& source)
		{
			return static_cast<long>(source);
		}

		static long to(qsf::StringHash source)
		{
			return static_cast<long>(source);
		}

		/**
		*  @brief
		*    Convert from any type to qsf::StringHash
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a qsf::StringHash
		*/
		static qsf::StringHash from(bool source)
		{
			return static_cast<uint32>(source);
		}

		static qsf::StringHash from(long source)
		{
			return static_cast<uint32>(source);
		}

		static qsf::StringHash from(double source)
		{
			return static_cast<uint32>(source);
		}

		static qsf::StringHash from(const std::string& source)
		{
			return boost::lexical_cast<uint32>(source);
		}

		static qsf::StringHash from(const camp::EnumObject& source)
		{
			return static_cast<uint32>(source.value());
		}

		static qsf::StringHash from(const camp::UserObject&)
		{
			CAMP_ERROR(camp::BadType(camp::userType, camp::intType));
		}

	};


} // camp_ext
