// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	inline Line2D Line2D::fromTwoPoints(const glm::vec2& pointA, const glm::vec2& pointB)
	{
		return Line2D(pointA, pointB - pointA);
	}

	inline Line2D Line2D::fromOriginAndDirection(const glm::vec2& origin, const glm::vec2& direction)
	{
		return Line2D(origin, direction);
	}

	inline Line2D::Line2D(const glm::vec2& origin, const glm::vec2& direction) :
		mOrigin(origin),
		mDirection(direction)
	{
		// Can't use isZero from math here because it would lead to a cyclic include
		QSF_CHECK(direction != glm::vec2(), "Trying to create a degenerated line with zero direction",
			QSF_REACT_THROW);
	}

	inline const glm::vec2& Line2D::getOrigin() const
	{
		return mOrigin;
	}

	inline const glm::vec2& Line2D::getDirection() const
	{
		return mDirection;
	}
}
