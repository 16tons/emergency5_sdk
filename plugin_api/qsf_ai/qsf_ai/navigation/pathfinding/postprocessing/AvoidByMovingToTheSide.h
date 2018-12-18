// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/navigation/TurningConstraint.h"
#include "qsf_ai/navigation/pathfinding/postprocessing/PathAdaptationResult.h"
#include "qsf_ai/navigation/steering/NavigationComponentCollision.h"
#include "qsf_ai/navigation/NavigationComponent.h"


namespace qsf
{
	namespace ai
	{
		// Move all the path positions inside the indicated range to the side.
		// The idealLaneUsage defines where on the space we want to end up, 0.f means completely left, 1.f means completely right (see NavigationComponent idealLaneFactor).
		// The bool skipEvasion points is needed because these are already extruded
		// Helper function for the above algorithm
		void extrudePath(Path& path, const TurningConstraint& turningConstraint, unsigned int minIndexToChange, unsigned int maxIndexToChange, Percentage idealLaneUsage, bool skipEvasionPoints);

		// Algorithm to adapt an existing path to avoid a collision by moving to the side
		path::AdaptionResult tryAdaptPathByMovingToTheSides(const glm::vec3& startPos, NavigationComponent& navi, const NavigationComponentCollision& collision);
	}
}
