// Copyright (C) 2012-2017 Promotion Software GmbH

#include <qsf/base/StringHash.h>

namespace qsf
{
	namespace ai
	{
		inline SystemComponent::SystemComponent(Prototype* prototype, const char* name)  :
			Component(prototype),
			mName(name),
			mHashedId(StringHash(name))
		{}

		inline const char* SystemComponent::getName() const
		{
			return mName;
		}
	}
}
