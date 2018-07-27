// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

// This file contains the necessary forward declarations to make the core serialization function a friend of objects that get serialized
namespace qsf
{
	class BinarySerializer;

	namespace serialization
	{
		// The core serialization template declaration
		template <typename T>
		struct serializer;
	}
}
