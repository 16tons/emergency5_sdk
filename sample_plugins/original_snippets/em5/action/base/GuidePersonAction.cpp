// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/game/targetpoint/LeadPersonTargetPointProvider.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Map.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier GuidePersonAction::ACTION_ID = "em5::GuidePersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GuidePersonAction::GuidePersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mGuideType(ARREST),
		mForcePosition(true)
	{
		// Nothing here
	}

	GuidePersonAction::~GuidePersonAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void GuidePersonAction::init(uint64 targetEntityId, GuideType guideType, bool forcePosition)
	{
		mTargetEntityId = targetEntityId;
		mGuideType = guideType;
		mForcePosition = forcePosition;
	}

	uint64 GuidePersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void GuidePersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mGuideType);
		serializer.serialize(mForcePosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool GuidePersonAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Squad already carrying someone/something
		if (EntityHelper(callerEntity).isCarrying())
			return false;

		// Check if the target is already linked to some parent
		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->hasParent())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getCarryPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result GuidePersonAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 guide person action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		qsf::MovableComponent* movableComponent = targetEntity->getComponent<qsf::MovableComponent>();
		if (movableComponent != nullptr)
		{
			movableComponent->setActive(false);
		}

		{ // Update the link component of the target entity
			qsf::LinkComponent& linkComponent = targetEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.linkToParent(getEntity());
			EntityHelper::setupEntityAsGhost(*targetEntity, getEntity());
		}

		if (mForcePosition)
		{
			// Enforce the correct position (teleporting)
			qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			const qsf::TransformComponent& callerTransformComponent = callerEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

			switch (mGuideType)
			{
				case ARREST:
				{
					// Turn gangster away from police (aka same rotation as police)
					const glm::vec3 offsetWorld = callerTransformComponent.getTransform().vec3DirectionLocalToWorld(ArrestGangsterAction::getArrestOffset());
					targetTransformComponent.setPositionAndRotation(callerTransformComponent.getPosition() - offsetWorld, callerTransformComponent.getRotation());
					break;
				}

				case LEAD:
				{
					// Hide current equipment (swap it back in inventory)
					EntityHelper(callerEntity).storeActiveEquipmentInInventory();

					const glm::vec3 offsetWorld = callerTransformComponent.getTransform().vec3DirectionLocalToWorld(LeadPersonTargetPointProvider::getOffset(*targetEntity));
					targetTransformComponent.setPositionAndRotation(callerTransformComponent.getPosition() - offsetWorld, callerTransformComponent.getRotation());
					break;
				}
			}
		}

		{ // Set the carried person to the carry person logic
			// Get the carry person logic
			CarryPersonLogic* carryPersonLogic = EntityHelper(callerEntity).getGameLogic<CarryPersonLogic>();
			if (nullptr == carryPersonLogic)
				return qsf::action::RESULT_DONE;

			// Set the carried person to the logic
			carryPersonLogic->setCarryPerson(mTargetEntityId);

			// Update the collision information of the caller and target
			qsf::BulletBoxCollisionComponent* bulletBoxCollisionComponent = callerEntity.getComponent<qsf::BulletBoxCollisionComponent>();
			if (nullptr != bulletBoxCollisionComponent)
			{
				glm::vec3 currentCollisionDimensions = bulletBoxCollisionComponent->getHalfExtents();
				bulletBoxCollisionComponent->setHalfExtents(currentCollisionDimensions * glm::vec3(2, 1, 2));

				// We need to update the router component as well
				qsf::ai::RouterComponent* routerComponent = callerEntity.getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
				{
					routerComponent->recreateCollisionBox();
				}

				qsf::BulletBoxCollisionComponent* targetbulletBoxCollisionComponent = targetEntity->getComponent<qsf::BulletBoxCollisionComponent>();
				if (nullptr != targetbulletBoxCollisionComponent)
				{
					targetbulletBoxCollisionComponent->setActive(false);
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void GuidePersonAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
