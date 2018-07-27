// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/base/Percentage.h"

#include <glm/glm.hpp>

namespace qsf
{
	namespace ai
	{
		/**
		* Minor helper class defining a place to cross during pathfinding.
		* Unlike the classical portal definition as an edge, we define a circle around a home position.
		* This can be converted to a fitting classical edge portal given a perspective point dynamically.
		* It is therefore called a dynamic portal.
		*/
		class QSF_AI_API_EXPORT DynamicPortal
		{
		public:
			DynamicPortal(); // defines a dummy portal from the origin of a coordinate system and zero free space
			DynamicPortal(const glm::vec3& point, UnsignedFloat freeSpace);

			glm::vec3 mHomePosition;
			UnsignedFloat mFreeSpace;

			bool operator ==(const DynamicPortal& other) const;

			// Helper function to interpolate linearly between two portals and create a new portal there
			static DynamicPortal interpolateLinearly(const DynamicPortal& previous, const DynamicPortal& current, Percentage factor);
		};
	}
}

#include "qsf_ai/navigation/DynamicPortal-inl.h"
