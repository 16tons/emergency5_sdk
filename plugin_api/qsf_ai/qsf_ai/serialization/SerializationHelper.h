// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/forward/BoostPTree.h>
#include <qsf/forward/BoostSerializationAccess.h>
#include <qsf/serialization/binary/BinarySerializer.h>

#include <vector>


namespace qsf
{
	class BinarySerializer;

	namespace serialization
	{
		// Reference texts for representing boolean values in plain text formats.
		static const char* BOOLEAN_TRUE_TEXT = "True";
		static const char* BOOLEAN_FALSE_TEXT = "False";

		// Tries to read a boolean from string input by comparing against the reference values defined above.
		// Throws an exception if a boolean could not be read.
		// These functions are case sensitive and require exact input.
		// TODO(vs) Should we be case insensitive here?
		//@{
		bool readBoolean(const std::string& text);
		bool readBoolean(const boost::property_tree::ptree& node);
		bool readBooleanFromChild(const boost::property_tree::ptree& node, const std::string& key);
		//@}

		// Convert boolean to a standard plain text value.
		//@{
		const char* getBooleanAsText(bool value);
		void writeBoolean(boost::property_tree::ptree& node, bool value);
		void addBooleanChild(boost::property_tree::ptree& root, const std::string& key, bool value);
		//@}

		// Convert different kinds of glm vectors to and from plain text. The delimiter specifies how the individual vector component values are separated.
		//@{
		template <typename Vector>
		Vector readVector(const std::string& text, const std::string& delimiter = std::string(" "));
		template <typename Vector>
		std::string getVectorAsText(const Vector& v, const std::string& delimiter = std::string(" "));
		//@}

		// Vector<bool> is special because it is no complete vector to optimize storing bools as bits
		// The generic serialization fails and we need a special function here.
		void QSF_AI_API_EXPORT serializeVectorOfBools(BinarySerializer& serializer, std::vector<bool>& data);
	}
}

#include "qsf_ai/serialization/SerializationHelper-inl.h"
