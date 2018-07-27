// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

namespace qsf
{
	namespace serialization
	{
		template <typename Vector>
		Vector readVector(const std::string& text, const std::string& delimiter)
		{
			Vector v;

			typedef boost::char_separator<char> Seperator;
			Seperator separator(delimiter.c_str());
			typedef boost::tokenizer<Seperator> Tokenized;
			Tokenized tokens(text, separator);
			Tokenized::iterator token = tokens.begin();
			for (glm::length_t i = 0; i < v.length(); ++i, ++token)
			{
				QSF_CHECK(token != tokens.end(), "Unable to parse " << typeid(Vector).name() << " from " << text << " using delimiter " << delimiter << ", to few tokens found",
					QSF_REACT_THROW);

				v[i] = boost::lexical_cast<typename Vector::value_type>(*token);
			}

			return v;
		}

		template <typename Vector>
		std::string getVectorAsText(const Vector& v, const std::string& delimiter)
		{
			QSF_WARN_IF(delimiter.empty(), "An empty delimiter does not allow to recreate the serialized vector data later on",
				QSF_REACT_NONE);

			std::stringstream buffer;
			for (glm::length_t i = 0; i < v.length(); ++i)
			{
				buffer << v[i];
				if (i != v.length() - 1)
					buffer << delimiter;
			}

			return buffer.str();
		}
	}
}
