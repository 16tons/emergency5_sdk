// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/Path.h"
#include "qsf_ai/base/UnsignedFloat.h"
#include "qsf_ai/navigation/steering/NavigationComponentCollision.h"
#include "qsf_ai/navigation/NavigationComponent.h"
#include "qsf_ai/navigation/pathfinding/postprocessing/PathAdaptationResult.h"

#include <glm/glm.hpp>


namespace qsf
{
	class Entity;

	namespace ai
	{
		class TurningConstraint;

		// Helper class that holds all the data that is temporarily needed for the evasion calculation for one obstacle
		class TemporaryObstacleData
		{
		public:
			TemporaryObstacleData();

			glm::vec2 mVertices[4];
			glm::vec2 mMidpoint;
			float mDesiredDistance;
		};

		// Helper function to read the dimensions to move around and to evade into the parameter passed.
		// Returns whether the data was successfully read
		bool tryReadObstructingDimensions(const Entity& entity, TemporaryObstacleData& data);

		// Returns whether the obstacle is so close to the path that if someone with turningConstraint dimensions would use it, it would actually collide.
		// The current position is assumed to be at startPos and the entity starts following the path at its current node index
		bool doesObstacleBlockPath(const glm::vec3& startPos, const Path& path, const TurningConstraint& turningConstraint, const TemporaryObstacleData& obstacle);

		// Try to adapt a path to move around a detected collision.
		// Returns whether the path may be followed without colliding with the collision anymore.
		// The path needs to be set into adaptation planned mode before calling this function and construction finished again afterwards.
		path::AdaptionResult tryAdaptPathByMovingAroundTheOutlines(const glm::vec3& startPos, NavigationComponent& navi, const NavigationComponentCollision& collision);
	}
}
