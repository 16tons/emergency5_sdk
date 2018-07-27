// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

// This file contains helper functions for writing, reading and checking the header of file contents
#include <qsf/forward/BoostPTree.h>
#include <qsf/serialization/binary/BinarySerializer.h>

namespace qsf
{
	namespace serialization
	{
		static const char* FORMAT_TAG = "Format";
		static const char* TYPE_TAG = "Type";
		static const char* VERSION_TAG = "Version";

		// Access to the default header data that is used among all QSF files.
		//@{
		void writeHeader(boost::property_tree::ptree& root, const std::string& type, unsigned int version);
		void readHeader(const boost::property_tree::ptree& root, std::string& type, unsigned int& version);

		/** This reads the header data and compares the contents to parameter passed.
		* An exception is thrown if either the header data is not found or does not comply to the requirements passed.
		* You can either pass an exact version or a range of supported versions [minVersion, maxVersion].
		* The exact version number found is returned which is only additional information when using the range variant but is done in both cases for consistency.
		*/
		template <typename Archive>
		unsigned int ensureFormat(Archive& root, const std::string& type, unsigned int version);
		template <typename Archive>
		unsigned int ensureFormat(Archive& root, const std::string& type, unsigned int minVersion, unsigned int maxVersion);
		//@}
	}
}

#include "qsf_ai/serialization/FileHeader-inl.h"
