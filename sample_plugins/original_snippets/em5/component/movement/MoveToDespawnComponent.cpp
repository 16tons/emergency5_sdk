// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/spawnpoint/civilist/VehicleSpawnPointComponent.h"
#include "em5/game/spawn/component/SpawnGroupComponent.h"
#include "em5/action/move/ArrivedAtDespawnAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/plugin/Jobs.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/goal/ReachConsecutivePointGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/goal/ReachMultiPointGoal.h>
#include <qsf_ai/navigation/MoveAction.h>

#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MoveToDespawnComponent::COMPONENT_ID = qsf::StringHash("em5::MoveToDespawnComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                    ]
	//[-------------------------------------------------------]
	MoveToDespawnComponent::MoveToDespawnComponent(qsf::Prototype* prototype):
		qsf::Component(prototype),
		mCountDespawnTries(0),
		mPriority(20),
		mMovementMode(qsf::getUninitialized<uint32>()),
		mStartedMovement(false)
	{
		// Nothing here
	}

	MoveToDespawnComponent::~MoveToDespawnComponent()
	{
		// Nothing here
	}

	uint32 MoveToDespawnComponent::getActionPriority() const
	{
		return mPriority;
	}

	void MoveToDespawnComponent::setActionPriority(uint32 priority)
	{
		mPriority = priority;
	}

	uint32 MoveToDespawnComponent::getMovementMode() const
	{
		return mMovementMode;
	}

	void MoveToDespawnComponent::setMovementMode(uint32 movementMode)
	{
		mMovementMode = movementMode;
	}

	void MoveToDespawnComponent::getExitAndTransitSpawnPoints(std::vector<CivilistSpawnPointComponent*>& exitSpawnPointComponentVector, std::vector<CivilistSpawnPointComponent*>& transitSpawnPointComponentVector)
	{
		uint32 spawnPointComponentId = 0;

		EntityHelper entityHelper(getEntity());
		if (entityHelper.isCivilPerson())
		{
			spawnPointComponentId = PersonSpawnPointComponent::COMPONENT_ID;
		}
		else if (entityHelper.isCivilRoadVehicle())
		{
			spawnPointComponentId = VehicleSpawnPointComponent::COMPONENT_ID;
		}
		else
		{
			return;
		}

		SpawnGroupComponent* spawnGroupComponent = getEntity().getComponent<SpawnGroupComponent>();
		uint32 requiredSpawnTag = (nullptr != spawnGroupComponent) ? spawnGroupComponent->getSpawnTag().getHash() : qsf::getUninitialized<uint32>();

		const qsf::ComponentCollection::ComponentList<Component>& components = qsf::ComponentMapQuery(getEntity().getMap()).getAllInstancesById(spawnPointComponentId);
		const qsf::ComponentCollection::ComponentList<CivilistSpawnPointComponent>& spawnPoints = reinterpret_cast<const qsf::ComponentCollection::ComponentList<CivilistSpawnPointComponent>&>(components);

		// Get all active exits
		for (CivilistSpawnPointComponent* spawnPointComponent : spawnPoints)
		{
			// Test if we can despawn at this point
			if (spawnPointComponent->isActive())
			{
				// If there is a spawn tag set, the exit spawn point must have the same tag
				if (qsf::isUninitialized(requiredSpawnTag) || spawnPointComponent->getTagHash() == requiredSpawnTag)
				{
					if (spawnPointComponent->isExit())
					{
						exitSpawnPointComponentVector.push_back(spawnPointComponent);
					}
					else if (spawnPointComponent->isTransit())
					{
						transitSpawnPointComponentVector.push_back(spawnPointComponent);
					}
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void MoveToDespawnComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isActive())
		{
			onSetActive(true);
		}
	}

	void MoveToDespawnComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (isSimulating() && active)
			{
				mActionComponent = getEntity().getOrCreateComponent<qsf::ActionComponent>();
				setUpdateFrequency(qsf::Time::fromSeconds(2.0f));
			}
			else
			{
				mUpdateJobProxy.unregister();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MoveToDespawnComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		// Movement update
		if (mActionComponent.valid() && mActionComponent->getCurrentPriority() < mPriority)	// Respect priority when pushing actions
		{
			// No movement for hidden entities
			if (nullptr != getEntity().getComponent<qsf::game::HiddenComponent>())
				return;

			// Make sure there is no pre-defined animation (this is an editing error, but we resolve this here to be sure)
			EntityHelper entityHelper(getEntity());
			if (entityHelper.hasEnabledMeshAnimationTestComponent())
				return;

			if (mStartedMovement)
			{
				// No movement?
				if (glm::distance2(entityHelper.getPosition(), mStartPosition) < 0.1f * 0.1f)
				{
					MapHelper(getEntity().getMap()).destroyEntity(getEntity());
					return;
				}
			}
			else
			{
				if (entityHelper.isPersonInjured())
				{
					// Don't allow persons to move in case they are injured
					return;
				}
				mStartedMovement = true;
				mStartPosition = entityHelper.getPosition();
			}

			startMoveToDespawnPoint();
		}
	}

	void MoveToDespawnComponent::setUpdateFrequency(qsf::Time updateInterval)
	{
		qsf::jobs::JobConfiguration jobConfig;
		jobConfig.mTimeBetweenCalls = updateInterval;
		mUpdateJobProxy.registerAt(Jobs::SIMULATION_SPAWN_PERSONS, boost::bind(&MoveToDespawnComponent::updateJob, this, _1), jobConfig);
	}

	void MoveToDespawnComponent::startMoveToDespawnPoint()
	{
		std::vector<CivilistSpawnPointComponent*> exitSpawnPoints;
		std::vector<CivilistSpawnPointComponent*> transitSpawnPoints;

		getExitAndTransitSpawnPoints(exitSpawnPoints, transitSpawnPoints);

		// Despawn the entity on random despawn point
		if (!exitSpawnPoints.empty())
		{
			qsf::ActionComponent& actionComponent = *mActionComponent;	// Must be valid here

			qsf::logic::TargetPoint moveTargetPoint;
			moveTargetPoint.mTolerance = EntityHelper(getEntity()).isCivilPerson() ? 1.0f : 0.5f;	// Don't set the tolerance too low, the target point may be e.g. a bit in the ground

			// And test if we get a random despawn point
			if (mCountDespawnTries < sNumberOfRandomTries)
			{
				const CivilistSpawnPointComponent* exitSpawnPointComponent = qsf::Random::getAnyOf(exitSpawnPoints);	// Cannot be nullptr, see above

				moveTargetPoint.mPosition = exitSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>().getPosition();

				// Add one transit point
				if (transitSpawnPoints.empty())
				{
					actionComponent.pushAction<MoveAction>(action::DAILY_ROUTINE).init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), mMovementMode);
				}
				else
				{
					std::vector<qsf::logic::TargetPoint> consecutiveTargetPointsVector;
					consecutiveTargetPointsVector.push_back(moveTargetPoint);

					// Get one transit point and add it to the vector of ceonscutive Goals
					const CivilistSpawnPointComponent* transitSpawnPointComponent = qsf::Random::getAnyOf(transitSpawnPoints);	// Cannot be nullptr, see above
					moveTargetPoint.mPosition = transitSpawnPointComponent->getEntity().getComponentSafe<qsf::TransformComponent>().getPosition();
					consecutiveTargetPointsVector.push_back(moveTargetPoint);

					actionComponent.pushAction<MoveAction>(action::DAILY_ROUTINE).init(new qsf::ai::ReachConsecutivePointGoal(consecutiveTargetPointsVector), mMovementMode);
				}

				actionComponent.pushAction<ArrivedAtDespawnAction>(action::BLOCKING).init(moveTargetPoint);
			}
			else
			{
				// If not we have to test all despawn points
				std::vector<qsf::logic::TargetPoint> moveTargetPointsVector;
				for (auto despawnPoint : exitSpawnPoints)
				{
					const qsf::TransformComponent& despawnPointTransformComponent = despawnPoint->getEntity().getComponentSafe<qsf::TransformComponent>();
					moveTargetPoint.mPosition = despawnPointTransformComponent.getPosition();
					// moveTargetPoint.mTolerance is set before, so we don't need to set it here again
					moveTargetPointsVector.push_back(moveTargetPoint);
				}
				actionComponent.pushAction<MoveAction>(action::DAILY_ROUTINE).init(new qsf::ai::ReachMultiPointGoal(moveTargetPointsVector), mMovementMode);
				actionComponent.pushAction<ArrivedAtDespawnAction>(action::BLOCKING).init(moveTargetPointsVector);

				// Normally this code should not be executed again.
				// Typically the entity is completely stuck because it didn't find a way although we provided all possible exits.
				// Planning again is most likely just a waste of resources.
				// There is the unlikely case that the entity was only temporarily blocked.
				// Therefore we still plan again, but lower the frequency greatly to avoid mostly useless searches
				setUpdateFrequency(qsf::Time::fromSeconds(5.0f));
			}

			// Increment the counter
			++mCountDespawnTries;
		}
	}


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 MoveToDespawnComponent::sNumberOfRandomTries = 3;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
