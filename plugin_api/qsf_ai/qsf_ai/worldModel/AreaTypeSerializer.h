// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/AreaType.h"

#include <qsf/serialization/Serializer.h>

#include <vector>

namespace qsf
{
	namespace ai
	{
		/** Reads AI area type defintions from a file.
		* Currently only plain text JSON is supported as format as the files are small and should be editable by hand.
		*/
		class AreaTypeSerializer : public Serializer
		{
		public:
			// Returns a linear list of contained area types. This list is not ordered in any way and may contain duplicates.
			static std::vector<AreaType> deserializeFromFile(const std::string& filename);
		};
	}
}
