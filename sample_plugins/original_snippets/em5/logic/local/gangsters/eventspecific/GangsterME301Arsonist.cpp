// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterME301Arsonist.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/door/DoorComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/health/InjuryAtStartupComponent.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
#include "em5/game/targetpoint/GetIntoBuildingTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagHelper.h>

#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used definitions                              ]
	//[-------------------------------------------------------]
	namespace
	{
		const qsf::StringHash MESSAGE_START_FIRE = "em5::GangsterME301Arsonist_start_fire";
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterME301Arsonist::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterME301Arsonist");

	const uint32 GangsterME301Arsonist::MESSAGE_FIRE_STARTED = qsf::StringHash("em5::GangsterME301Arsonist_FireStarted");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterME301Arsonist::GangsterME301Arsonist() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mTimeToStartFire(qsf::Time::fromSeconds(45.0f)),
		mFirefighterBuildingEvacuateTime(qsf::Time::fromSeconds(2.5f)),
		mMaxBurningObjectsToStartFire(9),
		mNextTargetIndex(0),
		mAllDone(false)
	{
		mSetGangsterFlagOnStartup = false;
	}

	void GangsterME301Arsonist::setTimeToStartFire(qsf::Time time)
	{
		mTimeToStartFire = time;
	}

	void GangsterME301Arsonist::setFirefighterBuildingEvacuateTime(qsf::Time time)
	{
		mFirefighterBuildingEvacuateTime = time;
	}

	void GangsterME301Arsonist::setMaxBurningObjectsToStartFire(uint32 value)
	{
		mMaxBurningObjectsToStartFire = value;
	}

	void GangsterME301Arsonist::initializeWithTargets(const std::vector<qsf::Entity*>& targetBuildings)
	{
		qsf::game::EventTagHelper eventTagHelper("GangsterME301Arsonist");

		for (qsf::Entity* entity : targetBuildings)
		{
			std::vector<BuildingInfo>& buildingsGroup = eventTagHelper.checkEntityHasEventTag(*entity, "GROUP_01") ? mBuildingsGroup01 : mBuildingsGroup02;
			buildingsGroup.emplace_back();
			buildingsGroup.back().mEntity = entity;
			buildingsGroup.back().mPosition = EntityHelper(entity).getPosition();
		}

		qsf::Entity* finalPoint = nullptr;
		if (eventTagHelper.tryAcquireTaggedEntity(finalPoint, "ARSONIST_END"))
		{
			mFinalPosition = EntityHelper(finalPoint).getPosition();
		}
		else
		{
			// Fallback: Use initial position as final position
			mFinalPosition = EntityHelper(getEntity()).getPosition();
		}
	}

	void GangsterME301Arsonist::startFireAtRandomTarget()
	{
		// Choose burning building at random
		//  -> Using only GROUP_01 buildings here (except there are none)
		if (!mBuildingsGroup01.empty())
		{
			const size_t targetIndex = qsf::Random::getRandomSize(0, mBuildingsGroup01.size() - 1);
			startFireInBuilding(mBuildingsGroup01[targetIndex], true);
		}
		else
		{
			const size_t targetIndex = qsf::Random::getRandomSize(0, mBuildingsGroup02.size() - 1);
			startFireInBuilding(mBuildingsGroup02[targetIndex], true);
		}

		// Next target please!
		chooseNextTarget();
	}

	void GangsterME301Arsonist::placeAtNextTarget()
	{
		// Move arsonist
		//  -> Using only GROUP_02 buildings here
		qsf::Entity& buildingEntity = mBuildingsGroup02[mNextTargetIndex].mEntity.getSafe();
		BuildingComponent& buildingComponent = buildingEntity.getComponentSafe<BuildingComponent>();

		qsf::Entity& doorEntity = QSF_MAKE_REF(getMap().getEntityById(buildingComponent.getMainDoorId()));
		DoorComponent& doorComponent = doorEntity.getComponentSafe<DoorComponent>();

		const glm::vec3 position = doorComponent.getDoorOffsetWorldSpacePosition();
		const glm::quat rotation = doorComponent.getDoorRotationWorldSpaceRotation();
		getEntitySafe().getOrCreateComponentSafe<qsf::TransformComponent>().setPositionAndRotation(position, rotation);

		// Tell him to do his thing right here
		addWaypoint(buildingEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterME301Arsonist::onStartup()
	{
		if (!GangsterBaseLogic::onStartup())
			return false;

		// Make sure he is immune to fire
		getEntitySafe().getOrCreateComponentSafe<FireReceiverComponent>().setActive(false);

		mStartFireMessageProxy.registerAt(qsf::MessageConfiguration(MESSAGE_START_FIRE, getEntityId()), boost::bind(&GangsterME301Arsonist::onStartFireMessage, this, _1));

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterME301Arsonist::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Nothing special to do in here, all gangster behavior is handled in "onIdle"
	}

	void GangsterME301Arsonist::onIdle()
	{
		if (!mAllDone)
		{
			// Bored? Get next waypoint!
			if (chooseNextTarget())
			{
				// Go to next building
				addWaypoint(*mBuildingsGroup02[mNextTargetIndex].mEntity, GetIntoBuildingTargetPointProvider::TARGET_POINT_ID);
			}
			else
			{
				// No targets remaining, run to final point
				mAllDone = true;
				addWaypoint(mFinalPosition);
			}
		}
	}

	void GangsterME301Arsonist::onWaypointReached(const Waypoint& waypoint)
	{
		// Set fire on building
		BuildingComponent* buildingComponent = waypoint.mEntity.valid() ? waypoint.mEntity->getComponent<BuildingComponent>() : nullptr;
		if (nullptr != buildingComponent)
		{
			qsf::ActionComponent& actionComponent = getEntitySafe().getOrCreateComponentSafe<qsf::ActionComponent>();

			if (!buildingComponent->isBurning())
			{
				// Push actions
				actionComponent.clearPlan();
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH).init(*waypoint.mEntity);
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_01_start", true, false, false, qsf::Time::fromSeconds(0.3f));
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_02_loop", true, true, false, qsf::Time::ZERO, mTimeToStartFire);
				actionComponent.pushAction<SignalAction>(action::COMMAND_HIGH).init(qsf::MessageConfiguration(MESSAGE_START_FIRE, getEntityId(), waypoint.mEntity->getId()), qsf::MessageParameters(), false);
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_02_end", true);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GangsterME301Arsonist::onStartFireMessage(const qsf::MessageParameters& parameters)
	{
		// Delay fire starting a bit more in case there are too many objects burning
		uint32 objectsBurning = 0;
		for (FireComponent* fireComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<FireComponent>())
		{
			if (fireComponent->isBurning() && nullptr == fireComponent->getComplexFireComponent())
				++objectsBurning;
		}
		for (ComplexFireComponent* fireComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<ComplexFireComponent>())
		{
			if (fireComponent->isBurning())
				++objectsBurning;
		}

		if (objectsBurning > mMaxBurningObjectsToStartFire)
		{
			// Play animation some more, then emit the same message once again
			//  -> Note the reverse order here
			qsf::ActionComponent& actionComponent = getEntitySafe().getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.pushAction<SignalAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(parameters.getConfiguration(), parameters, false);
			actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init("em5/skeleton/person/female_manipulate_object_kneeling_02_loop", true, true, false, qsf::Time::fromSeconds(0.4f), qsf::Time::fromSeconds(5.0f));
		}
		else
		{
			startFireInBuilding(mBuildingsGroup02[mNextTargetIndex], true);	// Always fullburn as requested by Stefan Bartsch
		}
	}

	void GangsterME301Arsonist::startFireInBuilding(BuildingInfo& building, bool fullFire)
	{
		building.mVisited = true;
		qsf::Entity& buildingEntity = building.mEntity.getSafe();
		BuildingComponent& buildingComponent = buildingEntity.getComponentSafe<BuildingComponent>();

		FreeplayEvent& freeplayEvent = QSF_MAKE_REF(EntityHelper(getEntity()).getFreeplayEvent());

		// Trap persons or place someone on the balcony
		qsf::game::EventTagHelper eventTagHelper("GangsterME301Arsonist");

		if (eventTagHelper.checkEntityHasEventTag(buildingEntity, "RESCUE_WINDOW"))
		{
			// Spawn person
			qsf::Entity& personEntity = QSF_MAKE_REF(SpawnPointComponent::spawnRandomAt(qsf::Transform(), getMap(), assets::SPAWNPOOL_DEFAULT_PERSON, false));
			EntityHelper(personEntity).enterContainer(buildingEntity, container::CONTAINERTYPE_PASSENGER);

			// Let person wave out of window
			buildingComponent.letPersonWaveOutOfWindow(personEntity, &freeplayEvent);
			personEntity.getOrCreateComponentSafe<InjuryAtStartupComponent>().setInjuryName("em20_me301_dlk");
		}

		if (eventTagHelper.checkEntityHasEventTag(buildingEntity, "RESCUE_EVACUATE"))
		{
			buildingComponent.setupTrappedInjuredPersons(0, 0, 1, 1, false, true);
			buildingComponent.setTrappedPersonsInjuryId(qsf::StringHash("em20_me301_01"));
			buildingComponent.createTrappedPerson(freeplayEvent);
			buildingComponent.setFirefighterRescueTime(mFirefighterBuildingEvacuateTime);

			// Note that the "PersonInBuildingLogic" is not really necessary, but it keeps more than one fireman from entering the building
			PersonInBuildingLogic& personInBuildingLogic = buildingEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>().getOrCreateGameLogicSafe<PersonInBuildingLogic>();
			personInBuildingLogic.addAllowedUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER);
			personInBuildingLogic.setNumberValidTeamUnitsNeeded(1);
		}

		// Time for the fire
		FireComponent* fireComponent = buildingEntity.getComponent<FireComponent>();
		if (nullptr != fireComponent && fireComponent->isActive())
		{
			// Start fire
			fireComponent->startFire(&freeplayEvent);
			if (fullFire)
			{
				fireComponent->setEnergyPercent(1.0f);
			}
		}
		else
		{
			ComplexFireComponent* complexFireComponent = buildingEntity.getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent && complexFireComponent->isActive())
			{
				// Start fire
				if (fullFire)
				{
					complexFireComponent->startFireOnAllFireComponents(&freeplayEvent);
					complexFireComponent->setEnergyPercent(1.0f);
				}
				else
				{
					complexFireComponent->startFire(&freeplayEvent);
				}
			}
		}

		// Emit message to let the event instance know
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(MESSAGE_FIRE_STARTED, buildingEntity.getId(), getEntityId()));
	}

	bool GangsterME301Arsonist::chooseNextTarget()
	{
		const glm::vec3& ownPosition = EntityHelper(getEntity()).getPosition();

		// Sort remaining targets by distance
		boost::container::flat_map<float, size_t> sorted;
		for (size_t index = 0; index < mBuildingsGroup02.size(); ++index)
		{
			// Ignore buildings already visited or already burning
			BuildingInfo& building = mBuildingsGroup02[index];
			if (!building.mVisited && !EntityHelper(building.mEntity).isBurning())
			{
				sorted.emplace(glm::distance(building.mPosition, ownPosition), index);
			}
		}

		if (sorted.empty())
			return false;

		// Choose any of the first three
		const size_t randomIndexInSorted = qsf::Random::getRandomSize(0, std::min<size_t>(sorted.size() - 1, 3));
		mNextTargetIndex = (sorted.begin() + randomIndexInSorted)->second;
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
