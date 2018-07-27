// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/SpatialConfiguration.h"

#include <glm/glm.hpp>

#include <vector>


namespace qsf
{
	namespace ai
	{
		// Contains some helper functions to create closest points for traffic lane worlds for very specific use cases

		/**
		* Find combinations of position on ideal lane & direction of the lane that are closest to the given position and fitting to the user type.
		*/
		QSF_AI_API_EXPORT void findClosestConfigurationsOnIdealLane(const glm::vec3& pos, unsigned int mapId, unsigned int moverType, std::vector<SpatialConfiguration3D>& closestConfigs);

		/**
		* Find combinations of position on ideal & direction that are close and in opposite direction to the current facing.
		*/
		QSF_AI_API_EXPORT void findClosestConfigurationsOnOppositeDirectionIdealLanes(const SpatialConfiguration3D& currentConfig, unsigned int mapId, unsigned int moverType, std::vector<SpatialConfiguration3D>& bestConfigs);
	}
}
