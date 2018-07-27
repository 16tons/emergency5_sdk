// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qpoint.h>

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
	*    Value mapper for QPoint
	*
	*  @note
	*    - This class tells CAMP how to handle QPoint as a string type
	*/
	template <>
	struct ValueMapper<QPoint>
	{

		/**
		*  @brief
		*    CAMP type associated to QPoint
		*/
		static const int type = camp::stringType;

		/**
		*  @brief
		*    Convert from QPoint to std::string
		*
		*  @param[in] source
		*    Source QPoint instance to convert
		*
		*  @return
		*    \a source converted to a std::string
		*/
		static std::string to(const QPoint& source)
		{
			char buffer[128];
			#ifdef WIN32
				sprintf_s(buffer, 128, "%d %d", source.x(), source.y());
			#else
				sprintf(buffer, "%d %d", source.x(), source.y());
			#endif
			return buffer;
		}

		/**
		*  @brief
		*    Convert from any type to QPoint
		*
		*  @param[in] source
		*    Source value to convert
		*
		*  @return
		*    \a source converted to a QPoint
		*
		*  @note
		*     - We simply reuse ValueMapper<std::string>.
		*/
		template <typename T>
		static QPoint from(const T& source)
		{
			std::vector<std::string> components;

			// We have to use a named instance, because you cannot bind a temporary to a non const reference.
			// The second parameter of boost::split is a non const reference
			// Gcc compiler erro: error: invalid initialization of non-const reference of type 'std::basic_string<char>&'' from an rvalue of type 'std::string {aka std::basic_string<char>}'
			std::string sourceString = ValueMapper<std::string>::from(source);
			boost::split(components, sourceString, boost::is_any_of(" "));
			if (components.size() > 1)
			{
				return QPoint(std::stoi(components[0]), std::stoi(components[1]));
			}
			else if (!components.empty())
			{
				return QPoint(std::stoi(components[0]), 0);
			}
			else
			{
				return QPoint(0, 0);
			}
		}

	};


} // camp_ext
