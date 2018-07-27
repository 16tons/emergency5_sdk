// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EnterBuildingAction::ACTION_ID = "em5::EnterBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnterBuildingAction::EnterBuildingAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	EnterBuildingAction::~EnterBuildingAction()
	{
		// Nothing here
	}

	void EnterBuildingAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void EnterBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool EnterBuildingAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		const BuildingComponent* buildingComponent = targetEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;	// target is no valid building

		glm::vec3 targetPosition = targetEntity->getComponentSafe<qsf::TransformComponent>().getPosition();
		if (buildingComponent->hasDoor())
			targetPosition = buildingComponent->getDoorPosition();

		// Make the distance check
		const  glm::vec3& callerPosition = callerEntity.getComponentSafe<qsf::TransformComponent>().getPosition();
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterBuildingDistance();
		const float currentSquaredDistance = glm::distance2(targetPosition, callerPosition);
		if (currentSquaredDistance > maxDistance * maxDistance)
			return false;

		return true;
	}

	qsf::action::Result EnterBuildingAction::updateAction(const qsf::Clock&)
	{
		// Get the most important objects first
		qsf::Entity* buildingEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == buildingEntity)
			return qsf::action::RESULT_DONE;

		const BuildingComponent* buildingComponent = buildingEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Wait for a short time
				// TODO(fw): Replace this with animation + fade out
				getComponent().pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f));

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_ENTER;
			}

			case STATE_ENTER:
			{
				// Enter the building now
				if (nullptr == getEntity().getComponent<qsf::MovableComponent>())
					return qsf::action::RESULT_DONE;

				const container::ContainerTypes containerType = (EntityHelper(getEntity()).isSquadPerson() ? container::CONTAINERTYPE_SQUAD : container::CONTAINERTYPE_PASSENGER);
				EntityHelper(getEntity()).enterContainer(*buildingEntity, containerType);

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
				// Done
				getComponent().pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f));
				return qsf::action::RESULT_DONE;
			}
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
