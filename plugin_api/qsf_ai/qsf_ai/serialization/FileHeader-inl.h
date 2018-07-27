// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/base/GetUninitialized.h>

namespace qsf
{
	namespace serialization
	{
		template <typename Archive>
		unsigned int ensureFormat(Archive& root, const std::string& type, unsigned int version)
		{
			std::string containedType;
			unsigned int containedVersion = getUninitialized<unsigned int>();
			readHeader(root, containedType, containedVersion);
			QSF_CHECK(containedType == type, "Unexpected type " << containedType << " found, expected type " << type,
				QSF_REACT_THROW);
			QSF_CHECK(containedVersion == version, "Unsupported version " << containedVersion << " found, expected version " << version,
				QSF_REACT_THROW);

			return containedVersion;
		}

		template <typename Archive>
		unsigned int ensureFormat(Archive& root, const std::string& type, unsigned int minVersion, unsigned int maxVersion)
		{
			std::string containedType;
			unsigned int containedVersion = getUninitialized<unsigned int>();
			readHeader(root, containedType, containedVersion);
			QSF_CHECK(containedType == type, "Unexpected type " << containedType << " found, expected type " << type,
				QSF_REACT_THROW);
			QSF_CHECK(containedVersion >= minVersion, "Unsupported version " << containedVersion << " found, minimum version is" << minVersion,
				QSF_REACT_THROW);
			QSF_CHECK(containedVersion <= maxVersion, "Unsupported version " << containedVersion << " found, maximum version is" << maxVersion,
				QSF_REACT_THROW);

			return containedVersion;
		}
	}
}
