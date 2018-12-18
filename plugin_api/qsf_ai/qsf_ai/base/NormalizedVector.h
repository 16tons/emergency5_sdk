// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	// Template for normalized vector items.
	template <typename Vector>
	class Normalized
	{
	public:
		Normalized(const Vector& v);

		// Automatic and manual conversion back to core data type
		operator const Vector&() const;
		const Vector& operator*() const;

		// TODO(vs) Add operations that keep the normalized property like unary minus and rotate
	private:
		Vector mValue;
	};
}

#include "qsf_ai/base/NormalizedVector-inl.h"
