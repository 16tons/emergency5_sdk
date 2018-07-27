// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/police/ClearBarrierAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf_game/component/base/SelectableComponent.h>
#include <qsf_game/component/highlight/GroundMarkerComponent.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>
#include <qsf_ai/worldModel/WorldModelManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/renderer/mesh/MeshComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BarrierTapeComponent::COMPONENT_ID = qsf::StringHash("em5::BarrierTapeComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BarrierTapeComponent::BarrierTapeComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mPoleTransparency{0.0f, 0.0f},
		mTapeConnection(POLE_TO_POLE),
		mTapeConnectionPoliceEntityId(qsf::getUninitialized<uint64>()),
		mTapeTransparency(0.0f),
		mFinishedBuilding(false),
		mIsBarrier(false)
	{
		// Nothing to do in here
	}

	BarrierTapeComponent::~BarrierTapeComponent()
	{
		clearBarrier();
	}

	bool BarrierTapeComponent::createPole(uint32 poleIndex)
	{
		if (poleIndex > 1)
			return false;

		if (mPoleEntities[poleIndex].valid())
			return false;

		// Create pole entity
		const std::string polePrefabName = "em5/prefab/equipment/" + em5::equipment::POLICE_BARRIER_TAPE_POLE;
		qsf::Entity* poleEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(qsf::StringHash(polePrefabName));

		QSF_CHECK(nullptr != poleEntity, "Could not create entity by prefab '" << polePrefabName << "'", QSF_REACT_THROW);

		// Pole entity does not get a position here already
		QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(*poleEntity, "DiffuseColor", qsf::MaterialPropertyValue::fromFloat4(1.0f, 1.0f, 1.0f, 1.0f));

		// Remove collisions component to be sure
		qsf::BulletCollisionComponent* bulletCollisionComponent = poleEntity->getComponent<qsf::BulletCollisionComponent>();
		if (nullptr != bulletCollisionComponent)
		{
			poleEntity->destroyComponent(*bulletCollisionComponent);
		}

		mPoleEntities[poleIndex] = poleEntity;

		return true;
	}

	void BarrierTapeComponent::movePole(uint32 poleIndex, const glm::vec3& targetPosition)
	{
		glm::vec3 correctedTargetPosition = targetPosition;
		dropToGround(correctedTargetPosition);

		if (poleIndex > 1)
			return;

		// No preview pole to move there already?
		if (!mPoleEntities[poleIndex].valid())
		{
			createPole(poleIndex);
		}
		qsf::Entity& poleEntity = mPoleEntities[poleIndex].getSafe();

		// Set the position to where we want our pole
		qsf::TransformComponent& transformComponent = poleEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		transformComponent.setPosition(correctedTargetPosition);

		if (poleIndex > 0)
		{
			if (mPoleEntities[poleIndex - 1].valid())
			{
				// Rotate this pole and the previous pole to each other
				qsf::Entity& lastPoleEntity = mPoleEntities[poleIndex - 1].getSafe();
				qsf::TransformComponent& firstPoleTransformComponent = lastPoleEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 firstPolePosition = firstPoleTransformComponent.getPosition();

				firstPolePosition.y = correctedTargetPosition.y;	// Simple trick to only rotate around y axis
				if (firstPolePosition != correctedTargetPosition)
				{
					glm::quat lookAtPole = qsf::Math::getLookAtQuaternion(correctedTargetPosition, firstPolePosition, qsf::CoordinateSystem::getUp());

					// Set rotations of both poles
					transformComponent.setRotation(lookAtPole);
					firstPoleTransformComponent.setRotation(lookAtPole);
				}
			}
		}

		// Update pole marker
		setPoleMarker(poleIndex, isValidPolePos(poleIndex, transformComponent.getTransform()) ? qsf::Color4::GREEN : qsf::Color4::RED);
	}

	const glm::vec3& BarrierTapeComponent::getPolePosition(uint32 poleIndex) const
	{
		if (poleIndex < 2)
		{
			const qsf::Entity* poleEntity = mPoleEntities[poleIndex].get();
			if (nullptr != poleEntity)
			{
				qsf::TransformComponent* poleEntityTransformComponent = poleEntity->getComponent<qsf::TransformComponent>();
				if (nullptr != poleEntityTransformComponent)
				{
					return poleEntityTransformComponent->getPosition();
				}
			}
		}

		return qsf::Math::GLM_VEC3_ZERO;
	}

	bool BarrierTapeComponent::isPolePositionValid(uint32 poleIndex) const
	{
		if (poleIndex > 1)
			return false;

		if (!mPoleEntities[poleIndex].valid())
			return false;

		// Can we place it here?
		qsf::Entity& poleEntity = mPoleEntities[poleIndex].getSafe();
		return isValidPolePos(poleIndex, poleEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getTransform());
	}

	void BarrierTapeComponent::createBarrier()
	{
		QSF_CHECK(mConnectedBarrierTape.valid(), "createBarrier() was called on a barrier tape component with no valid barrier tape entity", return);
		qsf::Entity& tapeEntity = mConnectedBarrierTape->getEntity();

		// Setup bullet collision box
		qsf::BulletBoxCollisionComponent& bulletBoxCollisionComponent = tapeEntity.getOrCreateComponentSafe<qsf::BulletBoxCollisionComponent>();

		// We have to give only the half of the barrier length because we set here the half extends, which is a value from the center of the box to the edge
		bulletBoxCollisionComponent.setHalfExtents(glm::vec3(getBarrierLength() / 2.0f, 0.75f, 0.25f));	// Height is just a guess (doesn't matter much)
		bulletBoxCollisionComponent.setCollisionFilterGroup(collision::DEFAULTFILTER | collision::KINEMATICFILTER);
		bulletBoxCollisionComponent.setCollisionMask(collision::KINEMATICFILTER | collision::SENSORTRIGGER);

		{
			qsf::ai::RouterComponent* routerComponent = tapeEntity.getComponent<qsf::ai::RouterComponent>();
			if (nullptr != routerComponent)
			{
				routerComponent->recreateCollisionBox();
			}
		}

		// Create a dummy entity for a better click field
		mClickEntity = getEntity().getMap().createEntity();
		QSF_CHECK(mClickEntity.valid(), "Failed to create click support entity", QSF_REACT_THROW);

		{
			qsf::MeshComponent& meshComponent = mClickEntity->getOrCreateComponentSafe<qsf::MeshComponent>();
			meshComponent.setMesh(qsf::AssetProxy("em5/mesh/default/missing"));
			meshComponent.setVisibilityFlags(0);
		}

		const glm::vec3 barrierCenter = getBarrierCenter();
		const glm::quat barrierRotation = getBarrierRotation();

		{ // Set the position and rotation of the barrier, offset to get the pivot point in a height where the tape is
			qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setPositionAndRotation(barrierCenter + glm::vec3(0.0f, 0.8f, 0.0f), barrierRotation);
		}

		{ // Set the position and rotation of the tape, which also affects the position/rotation of the collision box
			qsf::TransformComponent& tapeTransformComponent = mConnectedBarrierTape->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			tapeTransformComponent.setPositionAndRotation(barrierCenter, barrierRotation);
		}

		{
			qsf::TransformComponent& clickTransformComponent = mClickEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			clickTransformComponent.setPositionAndRotation(barrierCenter, barrierRotation);
			clickTransformComponent.setScale(bulletBoxCollisionComponent.getHalfExtents());
		}

		// Link tape, poles and click entity to the barrier
		uint64 entityId = getEntityId();
		// TODO(co) This resolves EM_2017 bug 619 "Grafik | Glitch | Polizei Absperrbänder glitchen vor fertigstellung kurz". But why does this problem exist in here?
	//	mPoleEntities[0]->getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(entityId);
	//	mPoleEntities[1]->getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(entityId);
		tapeEntity.getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(entityId);
		mClickEntity->getOrCreateComponentSafe<qsf::LinkComponent>().setParentId(entityId);

		mIsBarrier = true;
	}

	bool BarrierTapeComponent::isBarrier() const
	{
		return mIsBarrier;
	}

	qsf::Entity* BarrierTapeComponent::getPoleEntity(uint32 poleIndex) const
	{
		return (poleIndex > 1) ? nullptr : mPoleEntities[poleIndex].get();
	}

	uint32 BarrierTapeComponent::getNumPoles() const
	{
		uint32 result = 0;

		if (mPoleEntities[0].valid())
			++result;

		if (mPoleEntities[1].valid())
			++result;

		return result;
	}

	glm::quat BarrierTapeComponent::getBarrierRotation() const
	{
		if (getNumPoles() < 2)
			return qsf::Math::GLM_QUAT_IDENTITY;

		const qsf::TransformComponent& firstPoleTransformComponent = mPoleEntities[0].getSafe().getOrCreateComponentSafe<qsf::TransformComponent>();
		const qsf::TransformComponent& secondPoleTransformComponent = mPoleEntities[1].getSafe().getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3 direction = glm::normalize(secondPoleTransformComponent.getPosition() - firstPoleTransformComponent.getPosition());

		const glm::vec3 barrierCenter = getBarrierCenter();
		const glm::vec3 guardPosition = barrierCenter + glm::vec3(direction.z, 0.0f, -direction.x);
		return qsf::Math::getLookAtQuaternion(barrierCenter, guardPosition, qsf::CoordinateSystem::getUp());
	}

	glm::vec3 BarrierTapeComponent::getBarrierCenter() const
	{
		if (getNumPoles() < 2)
			return qsf::Math::GLM_VEC3_ZERO;

		const qsf::TransformComponent& firstPoleTransformComponent = mPoleEntities[0].getSafe().getOrCreateComponentSafe<qsf::TransformComponent>();
		const qsf::TransformComponent& secondPoleTransformComponent = mPoleEntities[1].getSafe().getOrCreateComponentSafe<qsf::TransformComponent>();

		return (firstPoleTransformComponent.getPosition() + secondPoleTransformComponent.getPosition()) * 0.5f;
	}

	float BarrierTapeComponent::getBarrierLength() const
	{
		if (getNumPoles() < 2)
			return 0.0f;

		return qsf::game::DistanceHelper::get2dDistance(mPoleEntities[0].getSafe(), mPoleEntities[1].getSafe());
	}

	void BarrierTapeComponent::setPoleTransparency(uint32 poleIndex, float transparency)
	{
		if (poleIndex > 1)
			return;

		const qsf::Entity* poleEntity = mPoleEntities[poleIndex].get();
		if (nullptr != poleEntity && mPoleTransparency[poleIndex] != transparency)
		{
			mPoleTransparency[poleIndex] = transparency;

			// TODO(ca) This looks odd - there should be a defined default transparency for which the variation should then get destroyed to save resources
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
			materialVariationManager.setEntityMaterialPropertyValue(*poleEntity, "SrcBlend", qsf::MaterialPropertyValue::fromBlend((transparency != 0.0f) ? qsf::Renderer::Blend::SRC_ALPHA : qsf::Renderer::Blend::ONE));
			materialVariationManager.setEntityMaterialPropertyValue(*poleEntity, "DestBlend", qsf::MaterialPropertyValue::fromBlend((transparency != 0.0f) ? qsf::Renderer::Blend::INV_SRC_ALPHA : qsf::Renderer::Blend::ZERO));
			materialVariationManager.setEntityMaterialPropertyValue(*poleEntity, "IsDecal", qsf::MaterialPropertyValue::fromBoolean(transparency != 0.0f));
			materialVariationManager.setEntityMaterialPropertyValue(*poleEntity, "DiffuseColor", qsf::MaterialPropertyValue::fromFloat4(1.0f, 1.0f, 1.0f, 1.0f - transparency));
		}
	}

	float BarrierTapeComponent::getPoleTransparency(uint32 poleIndex) const
	{
		return (poleIndex > 1) ? 0.0f : mPoleTransparency[poleIndex];
	}

	void BarrierTapeComponent::setTapeTransparency(float transparency)
	{
		if (mConnectedBarrierTape.valid() && mTapeTransparency != transparency)
		{
			mTapeTransparency = transparency;
			mConnectedBarrierTape->setTransparency(transparency);
		}
	}

	float BarrierTapeComponent::getTapeTransparency() const
	{
		return mTapeTransparency;
	}

	void BarrierTapeComponent::setPoleMarker(uint32 poleIndex, const qsf::Color4& color)
	{
		qsf::Entity* poleEntity = getPoleEntity(poleIndex);
		if (nullptr != poleEntity)
		{
			qsf::game::GroundMarkerComponent* groundMarkerComponent = poleEntity->getOrCreateComponent<qsf::game::GroundMarkerComponent>();
			if (nullptr != groundMarkerComponent)
			{
				groundMarkerComponent->setGroundMarkerColor(color);
				groundMarkerComponent->setScale(2.0f);
				groundMarkerComponent->setActive(true);
			}
		}
	}

	void BarrierTapeComponent::deletePoleMarker(uint32 poleIndex)
	{
		qsf::Entity* poleEntity = getPoleEntity(poleIndex);
		if (nullptr != poleEntity)
		{
			qsf::game::GroundMarkerComponent* groundMarkerComponent = poleEntity->getComponent<qsf::game::GroundMarkerComponent>();
			if (nullptr != groundMarkerComponent)
			{
				poleEntity->destroyComponent<qsf::game::GroundMarkerComponent>();
			}
		}
	}

	void BarrierTapeComponent::createTape(TapeConnection tapeConnection, qsf::Entity* policeEntity)
	{
		QSF_CHECK(policeEntity != nullptr || tapeConnection == POLE_TO_POLE, "We cannot connect the tape to a police without an entity", return);

		if (mConnectedBarrierTape.valid())
		{
			destroyTape();
		}

		mTapeConnection = tapeConnection;

		switch (mTapeConnection)
		{
			case POLE_TO_POLE:
				mConnectedBarrierTape = SimpleBarrierTapeComponent::createBarrierTapeBetweenEntities(*mPoleEntities[0], "tape_bone", *mPoleEntities[1], "tape_bone");
				break;

			case POLE_TO_SQUAD:
				mConnectedBarrierTape = SimpleBarrierTapeComponent::createBarrierTapeBetweenEntities(*mPoleEntities[0], "tape_bone", *policeEntity, "Bip01_L_Hand");
				mTapeConnectionPoliceEntityId = policeEntity->getId();
				break;
		}

		mConnectedBarrierTape->setActive(true);
		mConnectedBarrierTape->setTransparency(0.0f);
	}

	bool BarrierTapeComponent::hasTape() const
	{
		return mConnectedBarrierTape.valid();
	}

	BarrierTapeComponent::TapeConnection BarrierTapeComponent::getTapeConnectionType() const
	{
		return mTapeConnection;
	}

	uint64 BarrierTapeComponent::getTapeConnectionPoliceEntityId() const
	{
		return mTapeConnectionPoliceEntityId;
	}

	void BarrierTapeComponent::destroyTape()
	{
		// Remove the barrier tape entity (not just the component)
		getEntity().getMap().destroyEntityById(mConnectedBarrierTape->getEntityId());
		mConnectedBarrierTape.clear();
		mTapeTransparency = 0.0f;
		mIsBarrier = false;
	}

	bool BarrierTapeComponent::isFinishedBuilding() const
	{
		return mFinishedBuilding;
	}

	void BarrierTapeComponent::setFinishedBuilding(bool finished)
	{
		mFinishedBuilding = finished;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void BarrierTapeComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Not implemented
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void BarrierTapeComponent::onShutdown()
	{
		clearBarrier();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool BarrierTapeComponent::isValidPolePos(uint32 poleIndex, const qsf::Transform& targetTransform) const
	{
		if (poleIndex > 1)
			return false;

		qsf::TransformComponent* firstPoleTransformComponent = nullptr;

		// Ensure second pole is within certain distances to the first pole
		if (poleIndex == 1)
		{
			const float MIN_POLE_GAP = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierMinLength();
			const float MAX_POLE_GAP = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierMaxLength();
			firstPoleTransformComponent = mPoleEntities[0]->getOrCreateComponent<qsf::TransformComponent>();
			if (nullptr != firstPoleTransformComponent)
			{
				const float distance = qsf::game::DistanceHelper::get2dDistance(targetTransform.getPosition(), firstPoleTransformComponent->getPosition());
				if (distance < MIN_POLE_GAP || distance > MAX_POLE_GAP)
					return false;
			}
		}

		// Check if the place is collision free
		if (mPoleEntities[poleIndex].valid() && !isPoleCollisionValid(*mPoleEntities[poleIndex], targetTransform))
			return false;

		if (!isPolePositionValid(targetTransform))
			return false;

		if (poleIndex == 1)
		{
			if (nullptr != firstPoleTransformComponent && !isTapePositionValid(firstPoleTransformComponent->getPosition(), targetTransform.getPosition()))
				return false;
		}

		return true;
	}

	bool BarrierTapeComponent::dropToGround(glm::vec3& position) const
	{
		// Try to find the ground at the given position
		float height = position.y;

		qsf::GroundMapQuery groundMapQuery(getEntity().getMap(), GroundMaps::FILTER_DEFAULT);
		if (!groundMapQuery.getHeightAtXZPosition(position.x, position.z, height))
		{
			// Ground was not found, so no change at all
			return false;
		}

		// Change the position y coordinate
		position.y = height;
		return true;
	}

	void BarrierTapeComponent::clearBarrier()
	{
		qsf::Map& map = getEntity().getMap();
		for (uint32 i = 0; i < 2; ++i)
		{
			if (nullptr != mPoleEntities[i].get())
			{
				map.destroyEntityById(mPoleEntities[i]->getId());
				mPoleEntities[i].clear();
			}
		}

		if (mConnectedBarrierTape.valid())
		{
			destroyTape();
		}

		if (mClickEntity.valid())
		{
			map.destroyEntityById(mClickEntity->getId());
			mClickEntity.clear();
		}

		mIsBarrier = false;
	}

	bool BarrierTapeComponent::isPoleCollisionValid(qsf::Entity& poleEntity, const qsf::Transform& targetTransform) const
	{
		qsf::BulletCollisionComponent* bulletCollisionComponent = poleEntity.getComponent<qsf::BulletCollisionComponent>();
		if (nullptr != bulletCollisionComponent)
		{
			qsf::CollisionHelper collisionHelperQsf(getEntity().getMap());
			if (!collisionHelperQsf.canBeAddedWithoutCollision(targetTransform, *bulletCollisionComponent))
				return false;

			CollisionHelper collisionHelperQsfEm5(getEntity().getMap());
			if (collisionHelperQsfEm5.isInWater(targetTransform.getPosition()))
				return false;
		}

		return true;
	}

	bool BarrierTapeComponent::isPolePositionValid(const qsf::Transform& targetTransform) const
	{
		// Have to check if it's not on a street
		qsf::ai::WorldModelManager& worldModelManager = qsf::ai::WorldModelManager::getInstance();
		const std::unique_ptr<qsf::ai::ManagedNavigationMapReadAccess> readAccess = worldModelManager.tryAcquireReadAccess(em5::NavigationMap::FREE_GROUND_AREA);
		if (nullptr == readAccess.get())
			return false;

		const qsf::ai::WorldModel& requestedMap = readAccess.get()->get();

		// Point where we want to place it
		const qsf::logic::TargetPoint requestedPoint(targetTransform.getPosition(), false, 0.0f, 0.0f);

		// Request legal points around there
		std::vector<qsf::ai::MapContactPoint> contactPoints;
		const boost::optional<unsigned int> moverType = boost::optional<unsigned int>();
		requestedMap.writeClosestLegalPoints(requestedPoint, moverType, contactPoints);

		// TODO(mz): Not sure yet if this will work in all cases (needs more tests)
		const float EPSILON = 0.1f;
		enum EAreaTypes
		{
			STREET_ALL = 0,
			STREET_NO_CIVIL = 1,
			SIDEWALK_ALL = 2,
			SIDEWALK_NO_CIVIL = 3,
			TRAIN_TRACKS = 4,
			FREE_AREA = 5,
			WATER = 10,
			AIR = 15,
			NOTHING = 20
		};

		bool result = false;
		for (const qsf::ai::MapContactPoint& mcp : contactPoints)
		{
			// Ignore points too far away
			if (glm::distance2(mcp.mClosestPosition, targetTransform.getPosition()) < 0.25f * 0.25f)
			{
				auto areaType = requestedMap.tryGetAreaType(mcp.mWorldElementId);
				if (areaType.is_initialized())
				{
					unsigned int areaTypeValue = areaType.get();
					float distance = mcp.mDistanceToLegalSpace;

					// Fine on sidewalk and street as well
					if (areaTypeValue == FREE_AREA)
					{
						result = true;
						break;
					}
					else if (distance <= EPSILON)	// TODO(fw): I wonder what this is for...?
					{
						return false;
					}
				}
			}
		}

		return result;
	}

	bool BarrierTapeComponent::isTapePositionValid(const glm::vec3& groundPos1, const glm::vec3& groundPos2) const
	{
		// Check if there are any collisions between the two poles
		qsf::PhysicsWorldComponent* physicsWorldComponent = getEntity().getMap().getCoreEntity().getComponent<qsf::PhysicsWorldComponent>();
		QSF_ASSERT(nullptr != physicsWorldComponent, "BarrierTapeComponent::isTapePositionValid needs the PhysicWorldComponent", QSF_REACT_NONE);
		if (nullptr != physicsWorldComponent)
		{
			// Make line from start to end but at the height of the pole-top
			const float HEIGHT_POLE = 1.f;	// Should be approximately correct height
			glm::vec3 rayStart(groundPos1);
			glm::vec3 rayEnd(groundPos2);
			rayStart.y += HEIGHT_POLE;
			rayEnd.y += HEIGHT_POLE;
			glm::vec3 dirTape(glm::normalize(rayEnd-rayStart));

			// Move start and end a little bit together to avoid hitting the poles
			const float POLE_RADIUS = 0.5f;
			rayStart += dirTape * POLE_RADIUS;
			rayEnd -= dirTape * POLE_RADIUS;

			// Do we hit something?
			glm::vec3 hitPositionBullet;
			qsf::Entity* firstHitEntity = physicsWorldComponent->rayTestFetchFirstHitEntity(rayStart, rayEnd, &hitPositionBullet);
			if (nullptr != firstHitEntity)
				return false;
		}

		// Check if point in the middle is not on a street
		// Could check more points in certain step sizes if necessary, but combined with the maximal length of the tape this should be sufficient already
		qsf::Transform checkTransform;
		checkTransform.setPosition((groundPos1+groundPos2)*0.5f);
		if (!isPolePositionValid(checkTransform))
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
