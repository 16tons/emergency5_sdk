// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/DynamicPortal.h"

#include <glm/glm.hpp>

namespace qsf
{
	namespace ai
	{
		/**
		* Minor helper class defining an edge to cross during pathfinding.
		*/
		class QSF_AI_API_EXPORT EdgePortal
		{
		public:
			EdgePortal(); // defines a dummy portal from the origin of a coordinate system
			EdgePortal(const glm::vec3& point); // defines a zero width portal from one point
			EdgePortal(const glm::vec3& left, const glm::vec3& right);

			// Helper function to convert to the basic dynamic portal format used inside a path
			DynamicPortal toCirclePortal() const;

			glm::vec3 mLeft;
			glm::vec3 mRight;
		};
	}
}

#include "qsf_ai/navigation/pathfinding/smoothing/EdgePortal-inl.h"
