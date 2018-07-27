// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

#include <glm/glm.hpp>

namespace qsf
{
	template <typename Vector>
	Normalized<Vector>::Normalized(const Vector& v) :
		mValue(glm::normalize(v))
	{
		// (vs) Unable to use the isZero function from Math here easily as Math includes the normalized vector which would lead to a circular include
		// This relies on the glm property that a default created vector is zero initialized instead.
		QSF_CHECK(v != Vector(), "Unable to normalize a zero vector",
			QSF_REACT_THROW);
	}

	template <typename Vector>
	Normalized<Vector>::operator const Vector &() const
	{
		return mValue;
	}

	template <typename Vector>
	const Vector& Normalized<Vector>::operator *() const
	{
		return mValue;
	}
}
