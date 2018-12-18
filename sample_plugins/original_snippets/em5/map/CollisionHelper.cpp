// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/ai/em4Router/actor/EObject.h"

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/polygon/BlockerPolygonComponent.h>
#include <qsf/component/polygon/LiquidPolygonComponent.h>
#include <qsf/math/Math.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	bool CollisionHelper::isInWater(qsf::Entity& target, bool includePositionHeight) const
	{
#if 0
		// Take all liquid entities
		qsf::ComponentMapQuery mainMapQuery(mMap);
		std::vector<qsf::Entity*> liquidEntitys;
		for (qsf::LiquidPolygonComponent* liquidPolygonComponent : mainMapQuery.getAllInstances<qsf::LiquidPolygonComponent>())
		{
			liquidEntitys.push_back(&liquidPolygonComponent->getEntity());
		}

		// Check if they collide with the target
		return doesEntityCollideWithAnyOf(target, liquidEntitys);
#else
		// Alternative: Use ground maps
		return isInWater(EntityHelper(target).getPosition(), includePositionHeight);
#endif
	}

	bool CollisionHelper::isInWater(const glm::vec3& position, bool includePositionHeight, float waterHeight) const
	{
		float height1;
		float height2;
		qsf::GroundMapQuery(mMap, GroundMaps::FILTER_NO_LIQUIDS).getHeightAt(position, height1);	// Default filter, because we can have persons, standing on houses, and the houses pivot is under water (eg Hamburg main event 08, flooding)
		const bool hasWater = qsf::GroundMapQuery(mMap, GroundMaps::FILTER_LIQUIDS_ONLY).getHeightAt(position, height2, false);	// Don't dare to use the expensive ray query callback

		const float waterHeightAboveGround = height2 - height1;
		float positionHeightAboveWater = 0.0f;
		if (includePositionHeight)
		{
			positionHeightAboveWater = position.y - height2;
		}

		// The position is in water when
		// - A the position (x,y) is any water
		// - The water height above ground is greater as 1 meter, and
		// - The position height above the water is lower or equal 0.8 meter
		return (hasWater && (waterHeightAboveGround > waterHeight) && (positionHeightAboveWater <= 0.8f));	// Water depth of 0.8 meter (changed from 1m to 0.8m because of better tolerance for persons on small boats)
	}

	bool CollisionHelper::isInBlockedArea(const qsf::Entity& caller, qsf::Entity& target) const
	{
		glm::vec3 position = EntityHelper(target).getPosition();
		if (!isInBlockedArea(caller, position))
			return false;

		// To prevent problems with injured lying at the edge of an obstacle, check positions around
		position.x += 1.f;
		if (!isInBlockedArea(caller, position))
			return false;
		position.x -= 2.f;
		if (!isInBlockedArea(caller, position))
			return false;
		position.x += 1.f;
		position.z += 1.f;
		if (!isInBlockedArea(caller, position))
			return false;
		position.z -= 2.f;
		if (!isInBlockedArea(caller, position))
			return false;

		return true;
	}

	bool CollisionHelper::isInBlockedArea(const qsf::Entity& caller, const glm::vec3& position) const
	{
		qsf::ai::RouterComponent* routerComponent = caller.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != routerComponent)
		{
			qsf::ai::EGenericObject* eGenericObject = routerComponent->getEGenericObject();
			if (qsf::ai::ERouteFinder::Instance()->GetSpeedFactor(eGenericObject, position) <= 0)
				return true;
		}

		return false;
/* old implementation
		// TODO(mk) Check the time the function needs, over 1 milli sec is much
		qsf::ComponentMapQuery mainMapQuery(mMap);
		for (qsf::BlockerPolygonComponent* blockerPolygonComponent : mainMapQuery.getAllInstances<qsf::BlockerPolygonComponent>())
		{
			// Exclude buildings, cars, etc.
			//  -> We sometimes have injured persons inside building's bounding boxes, they are not reachable otherwise
			if (nullptr == blockerPolygonComponent->getEntity().getComponentById("em5::DamageComponent"))
			{
				if (blockerPolygonComponent->isPositionInside(worldSpacePosition, true))
				{
					// QSF_LOG_PRINTS(INFO, "BLOCKER: " << blockerPolygonComponent->getEntityId()); // TODO(co) Do we really need this log message? (say no to log spam)

					// The entity is inside a blocker polygon
					return true;
				}
			}
		}

		// No blocker polygon has collision with the entity
		return false;
*/
	}

	bool CollisionHelper::findFreePositionEM3(const qsf::Entity& sourceEntity, const qsf::Transform& transform, float radius, float safety, glm::vec3& outPosition) const
	{
		qsf::ai::EGenericObject* genericObject = const_cast<qsf::Entity&>(sourceEntity).getOrCreateComponentSafe<qsf::ai::RouterComponent>().getEGenericObject();
		outPosition = transform.getPosition();
		return qsf::ai::EM3::Router->FindFreePosition(genericObject, outPosition, radius, safety);
	}

	bool CollisionHelper::findFreePositionEM3(const qsf::Entity& sourceEntity, float searchRadius, float safety, glm::vec3& outPosition) const
	{
		qsf::TransformComponent* transformComponent = sourceEntity.getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			return findFreePositionEM3(sourceEntity, transformComponent->getPosition(), searchRadius, safety, outPosition);
		}

		return false;
	}

	bool CollisionHelper::findFreePositionEM3(const qsf::Entity& sourceEntity, const glm::vec3& position, float searchRadius, float safety, glm::vec3& outPosition) const
	{
		outPosition = position;

		qsf::ai::RouterComponent* entityRouterComponent = sourceEntity.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != entityRouterComponent)
		{
			qsf::ai::EGenericObject* gObject = entityRouterComponent->getEGenericObject();
			if (nullptr != gObject)
			{
				return qsf::ai::EM3::Router->FindFreePosition(gObject, outPosition, searchRadius, safety);
			}
		}

		return false;
	}

	bool CollisionHelper::hasCollisionEM3(const qsf::Entity& sourceEntity, float searchRadius, float safety) const
	{
		const qsf::TransformComponent* transformComponent = sourceEntity.getComponent<qsf::TransformComponent>();
		return (nullptr != transformComponent) ? hasCollisionEM3(sourceEntity, transformComponent->getPosition(), safety) : false;
	}

	bool CollisionHelper::hasCollisionEM3(const qsf::Entity& sourceEntity, const glm::vec3& position, float safety) const
	{
		qsf::ai::RouterComponent* entityRouterComponent = sourceEntity.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != entityRouterComponent)
		{
			qsf::ai::EGenericObject* gObject = entityRouterComponent->getEGenericObject();
			if (nullptr != gObject)
			{
				qsf::TransformComponent* transformComponent = sourceEntity.getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					return qsf::ai::EM3::Router->HasCollision(gObject, qsf::ai::ERouterObjectState(position, glm::mat3_cast(transformComponent->getRotation())), qsf::ai::ERF_NONE, safety);
				}
			}
		}

		return false;
	}

	bool CollisionHelper::hasCollisionEM3(const glm::vec3& boxExtends, const qsf::Transform& transform) const
	{
		return qsf::ai::EM3::Router->HasCollision(boxExtends, -1, qsf::ai::ERouterObjectState(transform.getPosition(), glm::mat3_cast(transform.getRotation())), qsf::ai::EOTC_ANYTHING, qsf::ai::ERF_NONE);
	}

	bool CollisionHelper::hasCollisionEM3IgnoreEntity(const glm::vec3& boxExtends, const qsf::Transform& transform, qsf::Entity& ignoreEntity) const
	{
		// Find actorId from routerComponent and put it to ignore list
		qsf::ai::RouterComponent* entityRouterComponent = ignoreEntity.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != entityRouterComponent)
		{
			qsf::ai::EActor* eActor = entityRouterComponent->getActor();
			if (nullptr != eActor)
			{
				qsf::ai::ERouterObjectState routerObjectState(transform.getPosition(), glm::mat3_cast(transform.getRotation()));
				qsf::ai::ECollisionBox box(boxExtends, transform.getPosition(), routerObjectState.GetRotation(), qsf::Math::GLM_VEC3_ZERO, qsf::Math::GLM_MAT3X3_IDENTITY, nullptr, false);

				box.SetObjectState(routerObjectState, false);
				return qsf::ai::EM3::Router->HasCollision(eActor, box, -1, false, qsf::ai::EOTC_INVALID);
			}
		}

		// No router or actor found, return false
		return false;
	}

	bool CollisionHelper::hasCollisionEM3Helilanding(const qsf::Entity& sourceEntity, const glm::vec3& position, float safety) const
	{
		qsf::ai::RouterComponent* entityRouterComponent = sourceEntity.getComponent<qsf::ai::RouterComponent>();
		if (nullptr != entityRouterComponent)
		{
			qsf::ai::EGenericObject* gObject = entityRouterComponent->getEGenericObject();
			if (nullptr != gObject)
			{
				qsf::TransformComponent* transformComponent = sourceEntity.getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					// Check only the position, not much around
					//  -> Checking the whole helicopter box leads to the helicopters too often not being able to land
					static qsf::ai::ECollisionBox box;
					box.SetExtents(glm::vec3(0.1f, 0.1f, 0.1f), false);
					box.SetObjectState(position, false);

					return qsf::ai::EM3::Router->HasCollision(gObject, box, qsf::ai::ERF_NONE, true, qsf::ai::EOTC_HELILANDING);
				}
			}
		}

		return false;
	}

	bool CollisionHelper::findFreePositionEM3Safety(const qsf::Entity& sourceEntity, const qsf::Transform& transform, float radius1, float radius2, float radius3, float safety, glm::vec3& outPosition) const
	{
		if (!findFreePositionEM3(sourceEntity, transform, radius1, safety, outPosition))
		{
			// Search with bigger radius
			if (!findFreePositionEM3(sourceEntity, transform, radius2, safety, outPosition))
			{
				if (!findFreePositionEM3(sourceEntity, transform, radius3, safety, outPosition))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool CollisionHelper::isEntityUnderBridge(qsf::Entity& entity) const
	{
		qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return false;

		// Raise the height of the position and make a query from top
		glm::vec3 checkPosition = transformComponent->getPosition();
		checkPosition.y += 100;
		float groundHeight;
		qsf::GroundMapQuery(mMap, GroundMaps::FILTER_DEFAULT).getHeightAt(checkPosition, groundHeight);

		return fabs(transformComponent->getPosition().y - groundHeight) > 1;	// Allow only minimal tolerance (assume the placement has to be updated)
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
