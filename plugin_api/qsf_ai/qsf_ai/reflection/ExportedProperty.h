// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/StringHash.h>

namespace qsf
{
	namespace ai
	{
		// Encapsulate the relation between the name and the id of a variable exported to CAMP.
		template <typename Type>
		class ExportedProperty
		{
		public:
			ExportedProperty(const char* name, const Type& defaultValue);

			const char* mName;
			const unsigned int mId;
			const Type mDefault;
		};


		// inline implementation
		template <typename Type>
		ExportedProperty<Type>::ExportedProperty(const char* name, const Type& defaultValue) :
			mName(name),
			mId(StringHash(name)),
			mDefault(defaultValue)
		{}
	}
}
