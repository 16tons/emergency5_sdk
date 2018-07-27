// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/glm.hpp>

namespace qsf
{
	/**
	* Extremely simple wrapper around a line in 2D just to keep the data together.
	* TODO(vs) To be migrated to QSF.
	*/
	class Line2D
	{
	public:
		// named c'tors to avoid semantic confusion
		static Line2D fromTwoPoints(const glm::vec2& pointA, const glm::vec2& pointB);
		static Line2D fromOriginAndDirection(const glm::vec2& origin, const glm::vec2& direction);

		const glm::vec2& getOrigin() const;
		const glm::vec2& getDirection() const;

	private:
		Line2D(const glm::vec2& origin, const glm::vec2& direction);

		glm::vec2 mOrigin;
		glm::vec2 mDirection;
	};
}

#include "qsf_ai/base/Line2D-inl.h"
