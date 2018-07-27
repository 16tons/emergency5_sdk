// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/physics/collision/CollisionHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 collision helper class
	*/
	class EM5_API_EXPORT CollisionHelper : public qsf::CollisionHelper
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit inline CollisionHelper(qsf::Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CollisionHelper();

		/**
		*  @brief
		*    "true" if entity is in water
		*/
		bool isInWater(qsf::Entity& entity, bool includePositionHeight = false) const;

		/**
		*  @brief
		*    "true" if position is in water
		*/
		bool isInWater(const glm::vec3& position, bool includePositionHeight = false, float waterHeight = 1.0f) const;

		/**
		*  @brief
		*    "true" if target is in blocking area for caller
		*/
		bool isInBlockedArea(const qsf::Entity& caller, qsf::Entity& target) const;
		bool isInBlockedArea(const qsf::Entity& caller, const glm::vec3& position) const;

		/**
		*  @brief
		*    Tries to find a free position where to place the given entity
		*    New version with EM3 router
		*
		*  @param[in] sourceEntity
		*    The entity to find a collision-free position for
		*  @param[in] transform
		*    The preferred transformation for the entity, around which a valid position should be found
		*  @param[in] radius
		*    Radius of the area to search for a free position
		*  @param[in] safety
		*    Additional space required around the entity at the target position, so that it will be accepted as valid free position
		*  @param[in] collisionFlags
		*    Collision bitmask
		*  @param[out] outPosition
		*    The resulting position; not touched if result is "false"
		*
		*  @return
		*    "true" if a position was found, "false" if not
		*/
		bool findFreePositionEM3(const qsf::Entity& sourceEntity, const qsf::Transform& transform, float radius, float safety, glm::vec3& outPosition) const;
		bool findFreePositionEM3(const qsf::Entity& sourceEntity, float searchRadius, float safety, glm::vec3& outPosition) const;
		bool findFreePositionEM3(const qsf::Entity& sourceEntity, const glm::vec3& position, float searchRadius, float safety, glm::vec3& outPosition) const;

		bool hasCollisionEM3(const qsf::Entity& sourceEntity, float searchRadius, float safety) const;
		bool hasCollisionEM3(const qsf::Entity& sourceEntity, const glm::vec3& position, float safety) const;
		bool hasCollisionEM3(const glm::vec3& boxExtends, const qsf::Transform& transform) const;
		bool hasCollisionEM3IgnoreEntity(const glm::vec3& boxExtends, const qsf::Transform& transform, qsf::Entity& ignoreEntity) const;
		bool hasCollisionEM3Helilanding(const qsf::Entity& sourceEntity, const glm::vec3& position, float safety) const;

		bool findFreePositionEM3Safety(const qsf::Entity& sourceEntity, const qsf::Transform& transform, float radius1, float radius2, float radius3, float safety, glm::vec3& outPosition) const;

		bool isEntityUnderBridge(qsf::Entity& entity) const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/map/CollisionHelper-inl.h"
