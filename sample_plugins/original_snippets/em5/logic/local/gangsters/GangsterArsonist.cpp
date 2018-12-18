// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterArsonist.h"
#include "em5/ai/MovementModes.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>

#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	namespace
	{
		qsf::logic::TargetPoint cheeringTargetPoint(qsf::Entity& callerEntity, qsf::Entity* targetEntity)
		{
			float radius = 0.0f;

			if (nullptr != targetEntity)
			{
				const ComplexFireComponent* complexFireComponent = targetEntity->getComponent<ComplexFireComponent>();
				if (nullptr != complexFireComponent)
				{
					std::vector<FireComponent*> allFireComponents;
					complexFireComponent->getAllFireComponents(allFireComponents);
					for (FireComponent* fireComponent : allFireComponents)
					{
						// Find the radius in which fire damage is taken
						float dangerRadius = glm::max(fireComponent->getSoftRadius(), fireComponent->getHardRadius());
						radius = glm::max(radius, dangerRadius);
					}
				}
				else
				{
					// Only relevant when no complex fire component exists
					const FireComponent* fireComponent = targetEntity->getComponent<FireComponent>();
					if (nullptr != fireComponent)
					{
						radius = glm::max(radius, fireComponent->getSoftRadius());
					}
				}
			}
			else
			{
				QSF_WARN("Target for gangster arsonist logic is not valid anymore", QSF_REACT_NONE);
			}

			// Fallback solution in case no reasonable soft/hard-radius exists
			radius = glm::min(radius, 5.0f);

			qsf::TransformComponent* gangsterTransformComponent = callerEntity.getOrCreateComponent<qsf::TransformComponent>();
			const glm::quat rot = gangsterTransformComponent->getRotation();
			const glm::quat rotationXZ = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rot);

			glm::vec3 cheerVector = radius * (rotationXZ * qsf::CoordinateSystem::getIn());

			// Step back from target to cheer
			glm::vec3 cheerPos = gangsterTransformComponent->getPosition() - cheerVector;
			return qsf::logic::TargetPoint(cheerPos, false, 0.0f, 0.0f, rotationXZ);
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterArsonist::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterArsonist");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterArsonist::GangsterArsonist() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>()),
		mCheeringTime(qsf::Time::fromSeconds(20.0f)),
		mNextAction(GO_TO_NEXT_BUILDING)
	{
		mEquippedWeaponTypes.set(weapon::FIST);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterArsonist::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if gangster still got work to do
		if (!actionComponent.getPlan().isEmpty())
		{
			// Gangster still busy, so we do nothing
			return;
		}

		switch (mNextAction)
		{
			case GO_TO_NEXT_BUILDING:
			{
				// Find a valid building
				BuildingComponent* nextBuildingToSetOnFire = findBuildingCandidate();
				if (nullptr == nextBuildingToSetOnFire)
				{
					mCurrentTargetId = qsf::getUninitialized<uint64>();
					break;
				}

				qsf::Entity& currentTargetEntity = nextBuildingToSetOnFire->getEntity();
				QSF_CHECK(qsf::isInitialized(currentTargetEntity.getId()), "ArsonistEvent: Invalid target to set on fire!", QSF_REACT_NONE);
				mCurrentTargetId = currentTargetEntity.getId();

				// Push a new movement action to make the person/gangster walk/run to the next target
				MoveAction* moveAction = &actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
				// TODO(mk) I think here is an wrong target point used
				qsf::ai::ReachObjectGoal* goal = new qsf::ai::ReachObjectGoal(*getEntity(), currentTargetEntity, PullPersonTargetPointProvider::TARGET_POINT_ID);

				moveAction->init(goal,  MovementModes::MOVEMENT_MODE_DEFAULT);
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Play gangster animation (manipulate object while kneeling down), simulating setting a building on fire
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(AnimationHelper(*getEntity()).getAnimationManipulateObjectKneeling(), true);

				// Now that the gangster reached the building and turned to it, she/he will set it on fire
				mNextAction = SET_BUILDING_ON_FIRE;
				break;
			}

			case SET_BUILDING_ON_FIRE:
			{
				// This should not happen at all!
				QSF_CHECK(qsf::isInitialized(mCurrentTargetId), "ArsonistEvent: Invalid target to set on fire!", mNextAction = GO_TO_NEXT_BUILDING; break;);

				FireReceiverComponent* fireReceiverComponent = getEntity()->getComponent<FireReceiverComponent>();
				QSF_CHECK(fireReceiverComponent, "No FireReceiverComponent attached to gangster entity!", QSF_REACT_NONE);

				if (fireReceiverComponent)
				{
					// Make arsonist invulnerable for the time of cheering etc.
					fireReceiverComponent->setActive(false);
				}

				qsf::Entity* currentBuildingEntity = getMap().getEntityById(mCurrentTargetId);

				// Set building on fire (ID is valid at this point, so the pointer has to as well)
				EntityHelper(*currentBuildingEntity).startFire(EntityHelper(*getEntity()).getFreeplayEvent());

				// The building started burning, so the next step for the gangster is to step away and watch the work / cheer
				mNextAction = WATCH_AND_CHEER;
				break;
			}

			case WATCH_AND_CHEER:
			{
				qsf::logic::TargetPoint cheerPosition = cheeringTargetPoint(*getEntity(), getMap().getEntityById(mCurrentTargetId));

				// Move away from building
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(cheerPosition), MovementModes::MOVEMENT_MODE_RUN);

				// Turn to building
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Cheer
				AnimationHelper animationHelper(*getEntity());
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCheer(), false, true, false, qsf::Time::ZERO, mCheeringTime);
				actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCheeringTime);

				// The gangster is done with that building, reset the target
				mCurrentTargetId = qsf::getUninitialized<uint64>();

				// One building is not enough; the gangster shall repeat the whole sequence, i.e. search for another burnable building
				mNextAction = GO_TO_NEXT_BUILDING;
				break;
			}

			case DO_NOTHING:
			{
				break;
			}

			default:
			{
				// Should not happen, but please prove me wrong if so
				QSF_CHECK(false, "ArsonistEvent: Gangster has invalid next action!", QSF_REACT_NONE);
			}
		}
	}

	void GangsterArsonist::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	void GangsterArsonist::onNoUnitsSpotted()
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	BuildingComponent* GangsterArsonist::findBuildingCandidate() const
	{
		// Get a listing of all building components (cannot be empty since findCandidate already checked)
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

		// Find the building that is closest to the arsonist
		const qsf::TransformComponent& selfTransformComponent = getEntity()->getComponentSafe<qsf::TransformComponent>();
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(selfTransformComponent.getPosition());
		std::sort(buildingComponents.begin(), buildingComponents.end(), compareObject);

		return buildingComponents[qsf::Random::getRandomInt(0, std::min((int)buildingComponents.size(), 3))];
	}

	bool GangsterArsonist::checkBuildingCandidate(qsf::Entity& building) const
	{
		const EntityHelper entityHelper(building);

		// Test if the building fits our requirements

		// Check if building is part of another event
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isInEventBoundaries())
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

		// Check is building is forbidden by difficulty tag
		qsf::game::EventTagComponent* eventTagComponent = building.getComponent<qsf::game::EventTagComponent>();

		if (nullptr != eventTagComponent && eventTagComponent->hasTag(qsf::StringHash("EVENT_BURNING_HARD")))
			return false;

		// Check if this building is empty
		if (!entityHelper.isContainerEmpty())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
