// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Transform.h"
#include "qsf/physics/PhysicsWorldComponent.h"

#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Map;
	class BulletCollisionComponent;
}
class btCollisionShape;
class btCollisionObject;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF collision helper class
	*/
	class QSF_API_EXPORT CollisionHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit inline CollisionHelper(Map& map);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~CollisionHelper();

		/**
		*  @brief
		*    Returns whether the source entity is colliding with the target or any of the list of target objects.
		*
		*    Uses the bullet collision information to determine this relation.
		*/
		bool doesEntityCollideWithAnyOf(const Entity& source, const std::vector<Entity*>& targets) const;
		bool doesEntityCollideWith(const Entity& source, const Entity& target) const;

		/**
		* @brief
		*    Checks whether the entity can be added at its current position without creating a collision.
		*
		* @note
		*    Make sure that the relevant information for the bullet collision component like transform and collision mask & filter flags are correct before calling this function.
		*/
		bool canBeAddedWithoutCollision(const Entity& source) const;

		/**
		*  @brief
		*    Checks whether an entity with the given transform and collision setup can be added without creating a collision.
		*
		*  @note
		*    Make sure that the relevant information for the bullet collision component like transform and collision mask & filter flags are correct before calling this function.
		*    Variant of the above function which allows for would be test without changing the current data.
		*/
		bool canBeAddedWithoutCollision(const Transform& transform, BulletCollisionComponent& collisionComponent) const;

		/**
		*  @brief
		*    Tries to find a free position where to place the given entity
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
		*
		*  @todo
		*    - TODO(co) Review those both methods, are those the same?
		*/
		bool findFreePosition(const Entity& sourceEntity, const Transform& transform, float radius, float safety, uint16 collisionFlags, glm::vec3& outPosition) const;
		bool findPhysicsFreePosition(const qsf::Entity& sourceEntity, const qsf::Transform& transform, float radius, uint16 collisionFlags, glm::vec3& outPosition) const;

		/**
		*  @brief
		*    Check for colliding components when inserting a collision shape
		*
		*  @param[in] transform
		*    The transformation where to test for collisions
		*  @param[in] collisionShape
		*    The shape to test
		*  @param[out] outCollisions
		*    The resulting list of colliding components
		*
		*  @return
		*    "true" if at least one collision was found, "false" if not
		*/
		bool getCollisionsAt(const Transform& transform, btCollisionShape& collisionShape, std::vector<BulletCollisionComponent*>& outCollisions) const;

		/**
		*  @brief
		*    Check for colliding components for an existing collision object
		*
		*  @param[in] collisionObject
		*    The collision object to check the collisions for
		*  @param[out] outCollisions
		*    The resulting list of colliding entities
		*  @param[out] filterGroup
		*    The filter group to use
		*  @param[out] filterMask
		*    The filter mask to use
		*
		*  @return
		*    "true" if at least one collision was found, "false" if not
		*/
		bool getCollisionsAt(btCollisionObject& collisionObject, std::vector<Entity*>& outCollisions, uint16 filterGroup = 0xffff, uint16 filterMask = 0xffff) const;
		bool getCollisionsAt(btCollisionObject& collisionObject, std::vector<PhysicsWorldComponent::HitResult>& outCollisions, uint16 filterGroup = 0xffff, uint16 filterMask = 0xffff) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Map& mMap;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/CollisionHelper-inl.h"
