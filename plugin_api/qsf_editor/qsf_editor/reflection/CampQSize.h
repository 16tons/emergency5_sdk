// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qsize.h>

#include <camp/valuemapper.hpp>

#include <boost/algorithm/string.hpp>

#include <vector>
#include <stdio.h>	// For "sprintf_s()"/"sprintf()"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
namespace camp_ext
{


	/**
	*  @brief
	*    Value mapper for QSize
	*
	*  @note
	*    - This class tells CAMP how to handle QSize as a string type
	*/
	template <>
	struct ValueMapper<QSize>
	{

		/**
		*  @brief
		*    CAMP type associated to QSize
		*/
		static const int type = camp::stringType;

		/**
		*  @brief
		*    Convert from QSize to std::string
		*
		*  @param[in] source
		*    Source QSize instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static std::string to(const QSize& source)
		{
			char buffer[128];
			#ifdef WIN32
				sprintf_s(buffer, 128, "%d %d", source.width(), source.height());
			#else
				sprintf(buffer, "%d %d", source.width(), source.height());
			#endif
			return buffer;
		}

		/**
		*  @brief
		*    Convert from any type to QSize
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a QSize
		*
		*  @note
		*     - We simply reuse ValueMapper<std::string>.
		*/
		template <typename T>
		static QSize from(const T& source)
		{
			std::vector<std::string> components;

			// We have to use a named instance, because you cannot bind a temporary to a non const reference.
			// The second parameter of boost::split is a non const reference
			// Gcc compiler erro: error: invalid initialization of non-const reference of type 'std::basic_string<char>&'' from an rvalue of type 'std::string {aka std::basic_string<char>}'
			std::string sourceString = ValueMapper<std::string>::from(source);
			boost::split(components, sourceString, boost::is_any_of(" "));
			if (components.size() > 1)
			{
				return QSize(std::stoi(components[0]), std::stoi(components[1]));
			}
			else if (!components.empty())
			{
				return QSize(std::stoi(components[0]), 0);
			}
			else
			{
				return QSize(0, 0);
			}
		}

	};


} // camp_ext
