// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/EnterBuildingAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/gangster/EscapeHidingFootAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/game/Game.h"
#include "em5/game/component/GameComponent.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier EscapeHidingFootAction::ACTION_ID = "em5::EscapeHidingFootAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EscapeHidingFootAction::EscapeHidingFootAction() :
		EscapeAction(ACTION_ID),
		mIsCivilPerson(false),
		mStuckTime(qsf::Time::ZERO),
		mTargetId(qsf::getUninitialized<uint64>()),
		mHidingInBuilding(false)
	{
		// Nothing here
	}

	EscapeHidingFootAction::~EscapeHidingFootAction()
	{
		// Nothing here
	}

	void EscapeHidingFootAction::init(const std::string& escapeTargetTag)
	{
		// TODO(fw): Evaluate escape target tag!

		mEscapeTargetTag = escapeTargetTag;

		BuildingComponent* building = chooseBuilding();
		qsf::Entity* tree = chooseVegetation();
		int i = qsf::Random::getRandomUint(0, 1);

		if ((i == 0 && nullptr != building) || (i == 1 && nullptr == tree))
		{
			QSF_CHECK(nullptr != building, "No possible hiding spot!", QSF_REACT_THROW);
			qsf::logic::TargetPoint moveTargetPoint;
			moveTargetPoint.mPosition = building->getDoorPosition();
			moveTargetPoint.mTolerance = 2.f;
			mHidingInBuilding = true;
			// Call base class implementation
			qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
			return;
		}

		QSF_CHECK(nullptr != tree, "No possible hiding spot!", QSF_REACT_THROW);
		const qsf::TransformComponent& treeTransformComponent = tree->getComponentSafe<qsf::TransformComponent>();
		qsf::logic::TargetPoint moveTargetPoint;
		moveTargetPoint.mPosition = treeTransformComponent.getPosition();
		moveTargetPoint.mTolerance = 2.f;
		qsf::ai::MoveAction::init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_RUN, nullptr);
		mHidingInBuilding = false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void EscapeHidingFootAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		qsf::ai::MoveAction::serialize(serializer);

		serializer.serialize(mIsCivilPerson);
		serializer.serialize(mStuckTime);
		serializer.serialize(mTargetId);
		serializer.serialize(mOldPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result EscapeHidingFootAction::updateAction(const qsf::Clock& clock)
	{
		if (!checkAllowUpdate())
		{
			return qsf::action::RESULT_CONTINUE;
		}

		// Call base implementation
		qsf::action::Result result = qsf::ai::MoveAction::updateAction(clock);

		// In EM5, a failed AI movement should not lead to automatic clearing of the action plan
		if (qsf::action::RESULT_CLEARLIST == result)
			result = qsf::action::RESULT_DONE;

		if (result == qsf::action::RESULT_DONE)
		{
			if (mHidingInBuilding)
			{
				qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				qsf::Entity* building = getMap().getEntityById(mTargetId);
				actionComponent.pushAction<EnterBuildingAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*building);
				HintHelper::showSupervisorMessage("ID_HINT_GANGSTER_HIDE_HOUSE_01");
				qsf::Entity& iconEntity = createIconEntity(getEntity());
				EM5_GAME.getMiniMapIconRegistrationLogic().registerEventIcon(iconEntity.getId());
			}
			else
			{
				qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				HintHelper::showSupervisorMessage("ID_HINT_GANGSTER_HIDE_TREE_01");
				qsf::Entity& iconEntity = createIconEntity(getEntity());
				EM5_GAME.getMiniMapIconRegistrationLogic().registerEventIcon(iconEntity.getId());
				actionComponent.pushAction<DisappearAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.0f), false);
			}
		}

		return result;
	}

	bool EscapeHidingFootAction::onStartup()
	{
		// Call base class implementation
		if (!qsf::ai::MoveAction::onStartup())
			return false;

		const EntityHelper entityHelper(getEntity());
		mIsCivilPerson = entityHelper.isCivilPerson();

		// TODO(mk) I assume to reach the goal of perfect persons, we keep this line here
		if (mIsCivilPerson)
		{
			getEntity().getOrCreateComponentById("em5::PersonPlacementComponent");
		}

		return true;
	}

	void EscapeHidingFootAction::onShutdown()
	{
		// Call base implementation
		qsf::ai::MoveAction::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	uint32 EscapeHidingFootAction::getMovementMode(uint32 movementModeId)
	{
		// If argument is invalid, check for possible movement mode
		if (qsf::isUninitialized(movementModeId))
		{
			EntityHelper entityHelper(getEntity());
			if (entityHelper.isSquadPerson())
			{
				// TODO(mk) I build this with paramedic in head, for other carry persons, maybe fire fighters or police/gangster, we need probability other checks
				if (entityHelper.isCarryingAnotherPerson())
				{
					return MovementModes::MOVEMENT_MODE_CARRY_PERSON;
				}
			}
		}
		return movementModeId;
	}

	bool EscapeHidingFootAction::checkAllowUpdate()
	{
		// TODO(mk) To optimize this function we can keep the gameComponent as member
		if (mIsCivilPerson)
		{
			return getMap().getCoreEntity().getOrCreateComponent<GameComponent>()->getCivilPersonMovement();
		}
		return true;
	}

	bool EscapeHidingFootAction::checkPositionStuck()
	{
		glm::vec3 newPosition = EntityHelper(getEntity()).getPosition();
		bool positionStuck = (mOldPosition == newPosition ? true : false);
		mOldPosition = newPosition;

		return positionStuck;
	}

	bool EscapeHidingFootAction::checkBuildingCandidate(qsf::Entity& building) const
	{

		const EntityHelper entityHelper(building);

		// Test if the building fits our requirements


		// Check if building is part of another event
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInMapBoundaries())
			return false;

		// Check if entity is a industrial plant
		if (entityHelper.isEntityIndustrialPlant())
			return false;

		// Check if entity is a normal building
		if (!entityHelper.isEntityBuilding())
			return false;

		// Check if this building is damaged
		if (entityHelper.isBuildingDamaged())
			return false;

		// Check if building can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		// Check if this building is empty
		if (!entityHelper.isContainerEmpty())
			return false;


		// Accepted
		return true;
	}

	BuildingComponent* EscapeHidingFootAction::chooseBuilding() const
	{
		std::vector<BuildingComponent*> buildingComponents;

		// Pick those buildings that fit our needs
		for (BuildingComponent* buildingComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<BuildingComponent>())
		{
			if (checkBuildingCandidate(buildingComponent->getEntity()))
			{
				buildingComponents.push_back(buildingComponent);	// We found a suitable candidate!
			}
		}

		// No building fits our needs
		if (buildingComponents.empty())
			return nullptr;

		// Find the building that is closest to the gangster
		const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
		glm::vec3 selfPosition = selfTransformComponent.getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		BuildingComponent* closestBuilding = nullptr;

		for (BuildingComponent* buildingComponent : buildingComponents)
		{
			// TODO(co) Why is this commented?
			//const qsf::TransformComponent& buildingTransformComponent = buildingComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

			const glm::vec3& pointPosition = buildingComponent->getDoorPosition();
			const float squaredDistance = glm::distance2(pointPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				// New closest building found
				shortestSquaredDistance = squaredDistance;
				closestBuilding = buildingComponent;
			}
		}

		return closestBuilding;
	}

	qsf::Entity* EscapeHidingFootAction::chooseVegetation() const
	{
		std::vector<qsf::Entity*> entities;

		for (const TreeComponent* component : qsf::ComponentMapQuery(getMap()).getAllInstances<TreeComponent>())
		{
			entities.push_back(&component->getEntity());
		}

		for (const HedgeComponent* component : qsf::ComponentMapQuery(getMap()).getAllInstances<HedgeComponent>())
		{
			entities.push_back(&component->getEntity());
		}

		// No entity fits our needs
		if (entities.empty())
			return nullptr;

		// Find the entity that is closest to the gangster
		const qsf::TransformComponent& selfTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
		glm::vec3 selfPosition = selfTransformComponent.getPosition();
		float shortestSquaredDistance = std::numeric_limits<float>::max();
		qsf::Entity* closest = nullptr;

		for (qsf::Entity* entity : entities)
		{
			// TODO(co) Why is this commented?
			//const qsf::TransformComponent& buildingTransformComponent = buildingComponent->getEntity().getComponentSafe<qsf::TransformComponent>();

			const glm::vec3& pointPosition = entity->getComponentSafe<qsf::TransformComponent>().getPosition();
			const float squaredDistance = glm::distance2(pointPosition, selfPosition);
			if (squaredDistance < shortestSquaredDistance)
			{
				// New closest building found
				shortestSquaredDistance = squaredDistance;
				closest = entity;
			}
		}

		return closest;
	}

	qsf::Entity& EscapeHidingFootAction::createIconEntity(qsf::Entity& gangsterEntity)
	{
		glm::vec3 entityPosition = gangsterEntity.getComponentSafe<qsf::TransformComponent>().getPosition();

		const float randomOffsetX = qsf::Random::getRandomFloat(0, 100);
		const float randomOffsetY = qsf::Random::getRandomFloat(0, 100);

		entityPosition.x += randomOffsetX;
		entityPosition.y += randomOffsetY;

		// Create icon entity
		qsf::Entity* iconEntity = getMap().createEntity();
		QSF_CHECK(nullptr != iconEntity, "There is no entity created; MissingPersonEvent method: createIconEntity", QSF_REACT_THROW);

		iconEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(entityPosition);

		{
			qsf::LinkComponent& linkComponent = iconEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.setParentId(gangsterEntity.getId());
			linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);
		}

		return *iconEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
