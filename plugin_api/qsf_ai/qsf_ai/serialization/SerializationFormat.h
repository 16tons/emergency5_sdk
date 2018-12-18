// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	namespace serialization
	{
		// An enum used to define the format that serialized content is in.
		enum Format
		{
			JSON, // plain text using the JSON syntax
			BINARY, // proprietary binary format with the details specified within the specific loader
		};
	}
}
