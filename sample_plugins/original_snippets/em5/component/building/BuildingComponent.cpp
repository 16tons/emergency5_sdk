// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/building/BuildingClippingManager.h"
#include "em5/component/building/BuildingLightTimeControlComponent.h"
#include "em5/component/door/DoorComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/damage/SimpleDamageData.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/ai/MovementModes.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TrappedPersonFleeAction.h"
#include "em5/application/Application.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/GameplayAssets.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/spawn/SpawnPoolManager.h"
#include "em5/game/GameSimulation.h"
#include "em5/game/Game.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/logic/AtmosphericRadioMessageHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/GameCounters.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/AvoidThreatsGoal.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/Math.h>
#include <qsf/math/Random.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/component/CameraComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/renderer/window/RenderWindow.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/job/JobArguments.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/settings/RendererSettingsGroup.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSubMesh.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementations                                ]
	//[-------------------------------------------------------]
	namespace detail
	{
		static qsf::Entity* createPersonEntity()
		{
			const std::string& randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(assets::SPAWNPOOL_DEFAULT_PERSON);
			return randomName.empty() ? nullptr : MapHelper(QSF_MAINMAP).createObjectByLocalPrefabAssetId(qsf::StringHash(randomName));
		}

		static uint32 setTimer(uint32 timerId, const qsf::MessageConfiguration& message, const qsf::Time& timerTime)
		{
			qsf::game::GameTimerManager& gameTimerManager = EM5_GAMETIMERS;
			if (qsf::isInitialized(timerId))
			{
				qsf::game::GameTimer* gameTimer = gameTimerManager.findTimerById(timerId);
				if (nullptr != gameTimer)
				{
					gameTimer->resetTimer(message, timerTime);
					return timerId;
				}
			}

			timerId = gameTimerManager.addTimer(message, timerTime);

			return timerId;
		}

		static void pauseTimer(uint32 timerId)
		{
			if (qsf::isInitialized(timerId))
			{
				qsf::game::GameTimer* gameTimer = EM5_GAMETIMERS.findTimerById(timerId);
				if (nullptr != gameTimer)
				{
					gameTimer->pause();
				}
			}
		}

		static void resumeTimer(uint32 timerId)
		{
			if (qsf::isInitialized(timerId))
			{
				qsf::game::GameTimer* gameTimer = EM5_GAMETIMERS.findTimerById(timerId);
				if (nullptr != gameTimer)
				{
					gameTimer->resume();
				}
			}
		}

		static bool cancelTimer(uint32 timerId)
		{
			return qsf::isInitialized(timerId) ? EM5_GAMETIMERS.removeTimerById(timerId) : false;
		}

		static const qsf::Time& getRemainingTime(uint32 timerId)
		{
			if (qsf::isInitialized(timerId))
			{
				qsf::game::GameTimer* gameTimer = EM5_GAMETIMERS.findTimerById(timerId);
				if (nullptr != gameTimer)
				{
					return gameTimer->getRemainingTime();
				}
			}

			return qsf::Time::ZERO;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuildingComponent::COMPONENT_ID		= qsf::StringHash("em5::BuildingComponent");
	const uint32 BuildingComponent::CLIPPING_ENABLED	= qsf::StringHash("em5::ClippingEnabled");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void BuildingComponent::onCaughtFire()
	{
		// TODO(sw) The following line "getting the freeplay event" works only correctly, when the freeplay event is set by other components (e.g. FireComponent) before this method call
		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();

		// Prepare all eboxes
		prepareEboxes();

		bool personsRemainInside = false;
		if (mCanCreateTrappedPersons && nullptr != freeplayEvent)
		{
			personsRemainInside = true;

			createTrappedPerson(*freeplayEvent);
			setTrappedPersonTimer(mTrappedPersonsRescueTime);
		}
		else
		{
			// Cycle through all entities in the container
			// -> Do this in a separate step before actually handling the entities, so we don't run into problems because of manipulation of the iterated list
			qsf::EntityVectorScratchBuffer entitiesInBuilding;
			for (auto iterator : getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				qsf::Entity& targetEntity = containerLink.getTargetEntity();

				// Add the entity to the exiting entities if its not waving
				if (targetEntity.getId() != mPersonInWindowEntityId)
				{
					entitiesInBuilding.push_back(&containerLink.getTargetEntity());
				}
			}

			// Exit the entities now
			for (qsf::Entity* containedEntity : entitiesInBuilding)
			{
				EntityHelper containedEntityHelper(*containedEntity);

				// Check if the entity is a person
				// -> Do nothing if the person is already injured!
				if (containedEntityHelper.isEntityPerson() && !containedEntityHelper.isPersonInjured() && !containedEntityHelper.checkForInitializedEventIdComponent())
				{
					// Set a person into the window with a fixed chance of 50%
					if (!personsRemainInside && letPersonWaveOutOfWindow(*containedEntity, freeplayEvent, 0.5f))
					{
						personsRemainInside = true;
					}
					else
					{
						// This one can flee so, let it run for its life
						qsf::ActionComponent* actionComponent = containedEntity->getOrCreateComponent<qsf::ActionComponent>();
						QSF_CHECK(nullptr != actionComponent, "This person has no action component and it could not be created", QSF_REACT_THROW);
						actionComponent->clearPlan();
						actionComponent->pushAction<ExitBuildingAction>(action::DANGER_URGENT);

						// TODO(jm): Set better goal than this
						actionComponent->pushAction<MoveAction>(action::DANGER_IMMINENT).init(new qsf::ai::AvoidThreatsGoal(20.0f, 1), MovementModes::MOVEMENT_MODE_RUN_PANIC);
					}
				}
			}
		}

		// Trap person in neighbor building
		if (qsf::isInitialized<uint64>(mEvacuateBuildingId) && nullptr != freeplayEvent)
		{
			qsf::Entity* evacuateBuilding = getEntity().getMap().getEntityById(mEvacuateBuildingId);
			if (evacuateBuilding != nullptr)
			{
				BuildingComponent* buildingComponent = evacuateBuilding->getComponent<BuildingComponent>();
				if (buildingComponent != nullptr)
				{
					// Take only valid buildings
					if (!buildingComponent->isBurning() && !buildingComponent->isDestroyed() && EntityHelper(*evacuateBuilding).getFreeplayEvent() == nullptr)
					{
						buildingComponent->setupTrappedPersons(1, 3, true);
						buildingComponent->createTrappedPerson(*freeplayEvent);

						// Door is not blocked, police can enter the building
						buildingComponent->setIsDoorBlocked(false);
					}
				}
			}
		}

		{
			const uint32 freeplayEventId = (nullptr != freeplayEvent ? freeplayEvent->getId() : qsf::getUninitialized<uint32>());

			// We have finished the reaction, so send the message
			qsf::MessageParameters parameters;
			parameters.setParameter("WavingPerson", isPersonWavingOutOfWindow());
			parameters.setParameter("PersonsInside", personsRemainInside);	// There will be persons that won't leave the building
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_BUILDING_STARTED_BURNING, freeplayEventId), parameters);
		}
	}

	void BuildingComponent::onStoppedFire(bool destroyed)
	{
		// Stop the particle effect on the main door
		enableParticle(mDoorParticleEffectId, false);

		// Reset all eboxes
		resetEboxes();

		if (destroyed && qsf::isInitialized(mTrappedPersonsDieTimerId))
		{
			// The building was destroyed -> the persons are now dead too, when timer still running
			killTrappedPersons(qsf::MessageParameters());
		}

		// Stop timer if it is still running
		detail::cancelTimer(mTrappedPersonsDieTimerId);
		mTrappedPersonsDieTimerId = qsf::getUninitialized<uint32>();

		// Emit message that the fire stopped
		{
			FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();
			uint32 freeplayEventId = (nullptr != freeplayEvent ? freeplayEvent->getId() : qsf::getUninitialized<uint32>());

			qsf::MessageParameters parameters;
			parameters.setParameter("WavingPerson", isPersonWavingOutOfWindow());
			parameters.setParameter("PersonsInside", isContainingTrappedPersons());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_BUILDING_STOPPED_BURNING, freeplayEventId, getEntityId()));
		}

		// Do not stop waving person, even if the fire is extinguished

		updateIcons();
	}

	bool BuildingComponent::letPersonWaveOutOfWindow(qsf::Entity& personEntity, FreeplayEvent* freeplayEvent, float chance)
	{
		// Check if the person can wave out of the window
		if (qsf::isInitialized(mPersonInWindowEntityId) || qsf::isUninitialized(mEventWindowId))
			return false;

		// Check random chance
		if (chance < 1.0f && !qsf::Random::getRandomChance(chance))
			return false;

		// Get event window entity
		const qsf::Entity* eventWindowEntity = getEntity().getMap().getEntityById(mEventWindowId);
		QSF_CHECK(nullptr != eventWindowEntity, "There is no event window ID with this ID", return false);

		// Get person component of contained entity
		PersonComponent& personComponent = personEntity.getComponentSafe<PersonComponent>();
		personComponent.setWavesOutOfBurningHouse(true);

		if (nullptr != freeplayEvent)
		{
			// Add person to event
			EventIdComponent::addToEvent(personEntity, *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);
		}

		// Add gamelogic
		qsf::GameLogicComponent* logicComponent = personEntity.getOrCreateComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != logicComponent, "There was no game logic created", return false);

		logicComponent->createGameLogicByTypeId(qsf::StringHash("em5::PersonJumpOutOfWindowLogic"));

		// Put that person in position TODO(jm): this might not be the right position so change it a little
		mPersonInWindowEntityId = personEntity.getId();
		qsf::TransformComponent& personTransformComponent = personEntity.getComponentSafe<qsf::TransformComponent>();
		const qsf::TransformComponent& windowTransformComponent = eventWindowEntity->getComponentSafe<qsf::TransformComponent>();
		personTransformComponent.setPositionAndRotation(windowTransformComponent.getPosition(), windowTransformComponent.getRotation());

		// Show the person
		qsf::game::HiddenComponent::setHidden(personEntity, false);

		// Get health component
		HealthComponent* healthComponent = personEntity.getOrCreateComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponent, "There was no health component created", return false);

		// Deactivates regeneration
		healthComponent->setRegenerationDisabled(true);

		// Deactivate movement logic
		MoveToDespawnComponent* moveToDespawnComponent = personEntity.getComponent<MoveToDespawnComponent>();
		if (nullptr != moveToDespawnComponent)
		{
			moveToDespawnComponent->setActive(false);
		}

		// Deactivate AI navigation
		qsf::ai::NavigationComponent* navigationComponent = personEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}

		// Let that persons wave out of windows
		AnimationHelper personAnimationHelper(personEntity);
		personAnimationHelper.clearAnimation();
		personAnimationHelper.playAnimation(personAnimationHelper.getAnimationWaveHelpFromWindow(), true, false);

		// Show person trapped icon
		StatusOverlayComponent::showIcon(personEntity, StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, true);

		return true;
	}

	void BuildingComponent::stopPersonWaveOutOfWindow()
	{
		if (qsf::isInitialized(mPersonInWindowEntityId))
		{
			qsf::Entity* personEntity = getEntity().getMap().getEntityById(mPersonInWindowEntityId);
			if (nullptr != personEntity)
			{
				// Stop the waving animation
				PersonComponent& personComponent = personEntity->getComponentSafe<PersonComponent>();
				personComponent.setWavesOutOfBurningHouse(false);

				// Remove the logic which kicks the person out of the window
				//personEntity->destroyComponent<qsf::GameLogicComponent>();	// TODO(co) Why is this commented?
				qsf::GameLogicComponent* logicComponent = personEntity->getOrCreateComponent<qsf::GameLogicComponent>();
				QSF_CHECK(nullptr != logicComponent, "There was no game logic created", return);

				qsf::GameLogic* jumpGameLogic = logicComponent->getGameLogicByTypeId(qsf::StringHash("em5::PersonJumpOutOfWindowLogic"));
				if (nullptr != jumpGameLogic)
				{
					logicComponent->deleteGameLogic(*jumpGameLogic);
				}

				// Activate movement logic
				MoveToDespawnComponent& moveToDespawnComponent = personEntity->getComponentSafe<MoveToDespawnComponent>();
				moveToDespawnComponent.setActive(true);

				// TODO(sw) When the person should be made invisible again, please don't interact directly with the render component here
				//          Because this makes problems in multiplay

				// Get health component
				HealthComponent* healthComponent = personEntity->getOrCreateComponent<HealthComponent>();
				QSF_CHECK(nullptr != healthComponent, "There was no health component created", return);

				// Reactivates regeneration
				healthComponent->setRegenerationDisabled(false);

				// Hide person trapped icon
				StatusOverlayComponent::showIcon(*personEntity, StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, false);
			}

			// Person isn't standing in the window anymore
			mPersonInWindowEntityId = qsf::getUninitialized<uint64>();
		}
	}

	qsf::Entity* BuildingComponent::getPersonWavingOutOfWindow() const
	{
		return getEntity().getMap().getEntityById(mPersonInWindowEntityId);
	}

	bool BuildingComponent::isContainingTrappedPersons() const
	{
		return !getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED).empty();
	}

	void BuildingComponent::setupTrappedPersons(uint16 minPersonCount, uint16 maxPersonCount, bool createPersons, const qsf::Time& rescueTime)
	{
		QSF_CHECK(minPersonCount > 0 && minPersonCount <= maxPersonCount, "Invalid values for trapped persons", return;);

		mSetupNumberOfPersonsTrapped = static_cast<uint16>(qsf::Random::getRandomUint(minPersonCount, maxPersonCount));
		mSetupNumberOfInjuredPersonsTrapped = 0;
		mCanCreateTrappedPersons = createPersons;
		mTrappedPersonsRescueTime = rescueTime;

		mIsDoorBlocked = true;

		updateIcons();
	}

	void BuildingComponent::setupTrappedInjuredPersons(uint16 minHealthyPersonCount, uint16 maxHealthyPersonCount, uint16 minInjuredPersonCount, uint16 maxInjuredPersonCount, bool blockedDoor, bool createPersons, const qsf::Time& rescueTime)
	{
		QSF_CHECK((minHealthyPersonCount > 0 || minInjuredPersonCount > 0) && minHealthyPersonCount <= maxHealthyPersonCount && minInjuredPersonCount <= maxInjuredPersonCount, "Invalid values for trapped persons", return;);

		mSetupNumberOfInjuredPersonsTrapped = static_cast<uint16>(qsf::Random::getRandomUint(minInjuredPersonCount, maxInjuredPersonCount));
		uint16 unInjuredPersons = static_cast<uint16>(qsf::Random::getRandomUint(minHealthyPersonCount, maxHealthyPersonCount));
		mSetupNumberOfPersonsTrapped = mSetupNumberOfInjuredPersonsTrapped + unInjuredPersons;
		mCanCreateTrappedPersons = createPersons;
		mTrappedPersonsRescueTime = rescueTime;

		mIsDoorBlocked = blockedDoor;

		// Show person trapped icon
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_AXE, true);
	}

	void BuildingComponent::createTrappedPerson(FreeplayEvent& freeplayEvent)
	{
		if (mCanCreateTrappedPersons)
		{
			// Create trapped persons
			for (uint16 i = 0; i < mSetupNumberOfPersonsTrapped; ++i)
			{
				qsf::Entity* personEntity = detail::createPersonEntity();
				if (personEntity)
				{
					EntityHelper(*personEntity).enterContainer(getEntity(), container::CONTAINERTYPE_TRAPPED);

					if (i < mSetupNumberOfInjuredPersonsTrapped || qsf::isInitialized(mTrappedPersonsInjuryId))	// TODO(fw): The second check looks bogus, but seems to be needed for plague events
					{
						const uint32 injuryId = qsf::isInitialized(mTrappedPersonsInjuryId) ? mTrappedPersonsInjuryId : injury::HEAD_BODY_LACERATION.getHash();
						HealthHelper(*personEntity).injurePerson(injuryId, &freeplayEvent, false);
					}

					// Add the person to the event
					// TODO(sw) correct event spread reason?
					EventSpreadHelper::setEventIdComponent(&freeplayEvent, *personEntity, eventspreadreason::EVENT_SPECIFIC);
				}
			}

			if (mSetupNumberOfPersonsTrapped > 0)
			{
				updateIcons();
				registerDebugDraw();

				// Add the building to the event
				EventSpreadHelper::setEventIdComponent(&freeplayEvent, getEntity(), eventspreadreason::EVENT_SPECIFIC);
			}

			// Create the persons only once
			mCanCreateTrappedPersons = false;
		}
	}

	bool BuildingComponent::isContainingGangsters() const
	{
		return !getAllLinksByContainerType(container::CONTAINERTYPE_GANGSTER).empty();
	}

	bool BuildingComponent::isContainingHostages() const
	{
		return !getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE).empty();
	}

	qsf::Time BuildingComponent::getRemainingTimeForTrappedPersons() const
	{
		return detail::getRemainingTime(mTrappedPersonsDieTimerId);
	}

	void BuildingComponent::setTrappedPersonTimer(qsf::Time time)
	{
		// No Timer when we have no rescue time
		if (mTrappedPersonsRescueTime > qsf::Time::ZERO)
		{
			// Setup wait "timer" until the trapped persons dies
			// TODO(sw) show a "progress bar" when this timer is active
			qsf::MessageConfiguration message(Messages::EM5_ENTITY_TIMER_SIGNAL, getEntityId());
			mMessageProxyKillTrappedPersons.registerAt(message, boost::bind(&BuildingComponent::killTrappedPersons, this, _1));
			mTrappedPersonsDieTimerId = detail::setTimer(mTrappedPersonsDieTimerId, message, mTrappedPersonsRescueTime);
		}
	}

	void BuildingComponent::untrapPersons()
	{
		untrapPersonsInternal(false);
	}

	void BuildingComponent::untrapPerson_SingleInjured()
	{
		untrapPersonsInternal(true);
	}

	void BuildingComponent::untrapPersonsInternal(bool singleInjuredPerson)
	{
		// The door was opened hopefully in time
		detail::cancelTimer(mTrappedPersonsDieTimerId);
		mTrappedPersonsDieTimerId = qsf::getUninitialized<uint32>();

		// Let the persons flee
		qsf::Time fleeWaitTime(qsf::Time::fromMilliseconds(250));
		std::vector<qsf::Entity*> trappedPersonEntities = getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED);

		bool containsNonInjured = false;
		for (qsf::Entity* personEntity : trappedPersonEntities)
		{
			HealthComponent* healthComponent = personEntity->getComponent<HealthComponent>();
			if (healthComponent != nullptr && !healthComponent->isInjured())
			{
				containsNonInjured = true;
				break;
			}
		}

		// We ask explicit for injured persons, so also look at the hostages
		if (singleInjuredPerson && containsNonInjured == false)
		{
			std::vector<qsf::Entity*> hostagePersonEntities = getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE);
			for (qsf::Entity* personEntity : hostagePersonEntities)
			{
				HealthComponent* healthComponent = personEntity->getComponent<HealthComponent>();
				if (healthComponent != nullptr && healthComponent->isInjured())
				{
					untrapPersonInternal(*personEntity, fleeWaitTime);
					break;
				}
			}
		}

		for (qsf::Entity* personEntity : trappedPersonEntities)
		{
			qsf::ActionComponent& actionComponent = personEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

			HealthComponent* healthComponent = personEntity->getComponent<HealthComponent>();
			if (healthComponent != nullptr && !healthComponent->isInjured())
			{
				// Injured persons need to keep the injured action
				actionComponent.clearPlan();
			}
			else if (healthComponent != nullptr && healthComponent->isInjured() && !singleInjuredPerson && containsNonInjured)
			{
				// Don't handle injured Persons in case we have noninjuredPersons for injured
				continue;
			}

			if (untrapPersonInternal(*personEntity, fleeWaitTime))
			{
				fleeWaitTime += qsf::Time::fromSeconds(1.0f);
			}

			if (singleInjuredPerson)
				break; // One take one injured Person
		}

		updateIcons();
	}

	bool BuildingComponent::untrapPersonInternal(qsf::Entity& personToUntrap, qsf::Time fleeWaitTime)
	{
		personToUntrap.getOrCreateComponent<qsf::TransformComponent>()->setPositionAndRotation(getDoorPosition(), getDoorRotation());

		// Make sure that they can receive fire damage
		personToUntrap.getOrCreateComponent<FireReceiverComponent>();

		qsf::ActionComponent& actionComponent = personToUntrap.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(fleeWaitTime);
		// Let the person exit the building with an fade in effect
		actionComponent.pushAction<ExitBuildingAction>(action::DANGER_URGENT, qsf::action::INSERT_AT_FRONT).init(true);

		// Emit message that the person is rescued
		if (EntityHelper(personToUntrap).isInContainer(getEntity(), container::CONTAINERTYPE_HOSTAGE))
		{
			// For hostages
			qsf::MessageParameters messageParameters;
			messageParameters.setParameter("buildingId", getEntityId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FREE_HOSTAGE_FROM_BUILDING, personToUntrap.getId()), messageParameters);

			return true;
		}
		else
		{
			qsf::MessageParameters messageParameters;
			messageParameters.setParameter("buildingId", getEntityId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FREE_PERSON_FROM_BUILDING, personToUntrap.getId()), messageParameters);

			// Don't add the logic when someone says so
			if (!messageParameters.hasResponse())
			{
				actionComponent.pushAction<TrappedPersonFleeAction>(action::DANGER_URGENT).init(qsf::Time::fromSeconds(10.0f), *this);
			}
		}
		return false;
	}

	void BuildingComponent::updateIcons()
	{
		if (isSimulating())
		{
			// Clear state at first
			EM5_GAME.getMiniMapIconRegistrationLogic().unregisterIcon(getEntityId());

			// We support multiple status overlay icons, but we chained the status icon with minimap icon. To support the right minimap icon only show one status icon (deleting a status icon will also delete minimap icons, so be careful with this)
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_AXE, false);
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS, false);
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_GANGSTER, false);

			if (!getAllLinksByContainerType(container::CONTAINERTYPE_GANGSTER).empty())
			{
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_GANGSTER, true);
			}
			else if (!getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE).empty() || !getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED).empty())
			{
				if (mIsDoorBlocked)
				{
					StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_AXE, true);
				}
				else
				{
					EM5_GAME.getMiniMapIconRegistrationLogic().registerEventIcon(getEntityId());
					StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS, true);
				}
			}
			else if (isBurning())
			{
				EM5_GAME.getMiniMapIconRegistrationLogic().registerIcon(getEntityId(), MiniMapIconType::BUILDING_ON_FIRE);
			}
		}
	}

	glm::vec3 BuildingComponent::getDoorPosition() const
	{
		qsf::Entity* doorEntity = nullptr;
		const DoorComponent* doorComponent = getMainDoorComponent(&doorEntity);
		if (nullptr == doorComponent)
		{
			return EntityHelper(*doorEntity).getPosition();
		}
		else
		{
			return doorComponent->getDoorOffsetWorldSpacePosition();
		}
	}

	glm::quat BuildingComponent::getDoorRotation() const
	{
		qsf::Entity* doorEntity = nullptr;
		DoorComponent* doorComponent = getMainDoorComponent(&doorEntity);
		if (nullptr == doorComponent)
		{
			return EntityHelper(*doorEntity).getTransform().getRotation();
		}
		else
		{
			return doorComponent->getDoorRotationWorldSpaceRotation();
		}
	}

	qsf::Transform BuildingComponent::getDoorTransform() const
	{
		qsf::Entity* doorEntity = nullptr;
		DoorComponent* doorComponent = getMainDoorComponent(&doorEntity);
		if (nullptr == doorComponent)
		{
			return EntityHelper(*doorEntity).getTransform();
		}
		else
		{
			qsf::Transform transform;
			doorComponent->getDoorWorldSpaceTransform(transform);
			return transform;
		}
	}

	glm::vec3 BuildingComponent::getExplosionCenterWorldSpacePosition() const
	{
		qsf::Transform originalLocalToWorld;

		const qsf::TransformComponent* transformComponent = getPrototype().getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			originalLocalToWorld = transformComponent->getTransform();
		}

		return originalLocalToWorld.vec3PositionLocalToWorld(mExplosionCenter);
	}

	void BuildingComponent::setExplosionCenterWorldSpacePosition(const glm::vec3& worldSpacePosition)
	{
		qsf::Transform originalLocalToWorld;

		const qsf::TransformComponent* transformComponent = getPrototype().getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			originalLocalToWorld = transformComponent->getTransform();
		}

		setExplosionCenter(originalLocalToWorld.vec3PositionWorldToLocal(worldSpacePosition));

		// TODO(db) The "ExplosionCenter" is the name of the property, used for camp export.
		setPropertyOverrideState("ExplosionCenter", Object::PROPERTY_OVERRIDE);
	}

	bool BuildingComponent::getWorldPositionInFrontOfDoor(float distance, glm::vec3& position, glm::quat& rotation) const
	{
		const qsf::Entity* doorEntity = getEntity().getMap().getEntityById(mDoors.mMainDoorId);
		if (nullptr != doorEntity)
		{
			const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
			if (nullptr != doorComponent)
			{
				doorComponent->getWorldPositionInFrontOfDoor(distance, position, rotation);
				return true;
			}
		}

		return false;
	}

	bool BuildingComponent::hasDoor() const
	{
		return (nullptr != getEntity().getMap().getEntityById(mDoors.mMainDoorId));
	}

	void BuildingComponent::setMainDoorDamaged(bool isDamaged)
	{
		if (!isSimulating())
			return;

		if (mMainDoorIsDamaged == isDamaged)
			return;

		mMainDoorIsDamaged = isDamaged;

		if (isDamaged)
		{

			// Pause the trapped person die timer because someone is knocking at the main door
			detail::pauseTimer(mTrappedPersonsDieTimerId);

			// Create smoke effect particle when not already done
			if (qsf::isInitialized(mDoorParticleEffectId))
			{
				// Enable only particle when burning
				enableParticle(mDoorParticleEffectId, isBurning());
			}
			else
			{
				qsf::Map& map = getEntity().getMap();
				qsf::Entity* particleEntity = map.createObjectByLocalPrefabAssetId(assets::PREFAB_BUILDING_SMOKE);
				QSF_ASSERT(nullptr != particleEntity, "BuildingComponent::setMainDoorDamaged() Could not create main door smoke particle entity from prefab", return);
				if (nullptr != particleEntity)
				{
					qsf::TransformComponent* particleTransformComponent = particleEntity->getOrCreateComponent<qsf::TransformComponent>();

					glm::vec3 doorEntityPosition = qsf::Math::GLM_VEC3_ZERO;
					const qsf::Entity* doorEntity = map.getEntityById(mDoors.mMainDoorId);
					qsf::TransformComponent* doorEntityTransformComponent = (doorEntity != nullptr) ? doorEntity->getComponent<qsf::TransformComponent>() : nullptr;
					if (nullptr != doorEntityTransformComponent)
					{
						doorEntityPosition = doorEntityTransformComponent->getPosition();
					}

					particleTransformComponent->setPosition(doorEntityPosition);

					mDoorParticleEffectId = particleEntity->getId();

					// Enable particle when burning
					enableParticle(particleEntity, isBurning());
				}
			}

			// Don't set mIsDoorBlocked here, it would lead to gameplay issues
		}
		else
		{
			// Resume trapped person die timer because knocking at the main door as stopped
			detail::resumeTimer(mTrappedPersonsDieTimerId);

			// Disable the smoke particle
			enableParticle(mDoorParticleEffectId, false);
		}
	}

	void BuildingComponent::hideParticleChildrenAtMapSave()
	{
		mDamageData.hideParticleChildrenAtMapSave(getPrototype().getPrototypeManager());
	}

	void BuildingComponent::setDamagedLevel1Material(const qsf::AssetProxy& assetProxy)
	{
		if (assetProxy != mDamagedLevel1Material && assetProxy.isValid())
		{
			mDamagedLevel1Material = assetProxy;
		}
	}

	void BuildingComponent::setIntactMaterial(const qsf::AssetProxy& assetProxy)
	{
		if (assetProxy != mIntactMaterial && assetProxy.isValid())
		{
			mIntactMaterial = assetProxy;
		}
	}

	void BuildingComponent::setCollapseRubbleVisible(bool show)
	{
		// Show/hide all collapse rubble slot elements
		const qsf::Map& map = getEntity().getMap();
		for (uint64 rubbleId : mCollapseRubbleEntityIds)
		{
			qsf::Entity* rubbleEntity = map.getEntityById(rubbleId);
			if (nullptr != rubbleEntity)
			{
				qsf::game::HiddenComponent::setHidden(*rubbleEntity, !show);
			}
		}
	}

	void BuildingComponent::spawnGangsterInside(int numberOfGangsters, int numberOfHostages, int numberOfInjuredHostages, GangsterPool gangsterPoolType)
	{
		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();

		qsf::Map& map = getEntity().getMap();
		for (int i = 0; i < numberOfGangsters; ++i)
		{
			// Get random gangster prefab for every gangster
			std::string gangsterSpawnPoolName = MapHelper(map).getSpawnpoolNameFromGangsterPool(gangsterPoolType);
			qsf::Entity* gangster = SpawnPointComponent::spawnRandomAt(qsf::Transform(), map, gangsterSpawnPoolName, false);
			if (nullptr != gangster)
			{
				EntityHelper entityHelper(*gangster);
				entityHelper.turnIntoGangster(qsf::StringHash("em5::GangsterCivilUnarmedLogic"), false);
				entityHelper.enterContainer(getEntity(), container::CONTAINERTYPE_GANGSTER);

				qsf::ActionComponent* actionComponent = gangster->getComponent<qsf::ActionComponent>();
				if (nullptr != actionComponent)
				{
					// Clear remaining move and despawn actions
					actionComponent->clearPlan();
				}

				// Add the person to the event
				//  -> In case the building is part of the event, an objective for gangsters inside building (or hostage taker, in case there are hostages) is created
				EventSpreadHelper::setEventIdComponent(freeplayEvent, *gangster, eventspreadreason::GANGSTER);
			}
		}

		int numberInjuredPersonsLeft = numberOfInjuredHostages;
		for (int i = 0; i < (numberOfHostages + numberOfInjuredHostages); ++i)
		{
			const std::string& randomName = EM5_GAME.getSimulation().getSpawnPoolManager().getRandomNameFromSpawnPool(assets::SPAWNPOOL_DEFAULT_PERSON);
			qsf::Entity* hostage = MapHelper(QSF_MAINMAP).createObjectByLocalPrefabAssetId(qsf::StringHash(randomName));
			if (nullptr != hostage)
			{
				EntityHelper entityHelper(*hostage);
				entityHelper.enterContainer(getEntity(), container::CONTAINERTYPE_HOSTAGE);
				qsf::ActionComponent* actionComponent = hostage->getComponent<qsf::ActionComponent>();
				if (nullptr != actionComponent)
				{
					// Clear remaining move and despawn actions
					actionComponent->clearPlan();
				}

				// Add the person to the event
				EventSpreadHelper::setEventIdComponent(freeplayEvent, *hostage, eventspreadreason::HOSTAGE);

				// Handle injured hostages, injure them after the event spread is happen
				if (numberInjuredPersonsLeft > 0)
				{
					// Injure hostages with a special injury
					HealthHelper(*hostage).injurePersonByHostageTaking(freeplayEvent);
					--numberInjuredPersonsLeft;
				}
			}
		}

		if (numberOfGangsters > 0 || numberOfHostages > 0)
		{
			updateIcons();
			// Debug
			registerDebugDraw();
		}
	}

	void BuildingComponent::setDestroyedWithoutAnimation(FreeplayEvent* freeplayEvent)
	{
		// set the collapse value true to avoid playing an collapse animation
		mCollapseComplete = true;

		if (nullptr != freeplayEvent)
		{
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::EVENT_SPECIFIC);
		}

		setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
	}

	void BuildingComponent::resetEboxes()
	{
		mMessageProxyEboxRepaired.clear();

		for (uint64 entityId : mEboxEntityIds)
		{
			onEboxRepaired(qsf::MessageParameters(), entityId);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	bool BuildingComponent::isBurning() const
	{
		return EntityHelper(getEntity()).isBurning();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void BuildingComponent::onSetDamageState(DamageState damageState)
	{
		// Calculate mCollapseAnimationYDistance now or we won't have the mesh anymore
		if (damageState == DAMAGESTATE_DESTROYED)
		{
			glm::vec3 min;
			glm::vec3 max;
			getEntity().getOrCreateComponentSafe<qsf::MeshComponent>().getWorldAxisAlignedBoundingBox(min, max);

			// Get the necessary distance to determine when the collapse animation entity can be released
			mCollapseAnimationYDistance = glm::abs(min.y - max.y);
		}

		applyCurrentMesh();
		mDirtyMaterialAabb = true;

		// Show/hide particle effects, lights, children
		setLightControllerActive(damageState == DAMAGESTATE_INTACT);

		// Enable/disable spawns
		setSpawnsActive(damageState == DAMAGESTATE_INTACT);

		const MapHelper mapHelper(getEntity().getMap());
		mapHelper.showEntityParticleIds(mDamageData.DamagedLevel1Particles.getVariable(), damageState == DAMAGESTATE_DAMAGE_LEVEL1, false, mUseCameraClipping, mCurrentFadeOffset);
		mapHelper.showEntityParticleIds(mDamageData.DamagedLevel2Particles.getVariable(), damageState == DAMAGESTATE_DAMAGE_LEVEL2, false, mUseCameraClipping, mCurrentFadeOffset);
		mapHelper.showEntityParticleIds(mDamageData.DestroyedParticles.getVariable(),     damageState == DAMAGESTATE_DESTROYED,     false, mUseCameraClipping, mCurrentFadeOffset);

		// Destroyed entities are only visible when the building is intact, the name was chosen wrong and used for intact entities from level design instead
		mapHelper.showEntityIds(mDestroyedEntityIds,     damageState != DAMAGESTATE_DESTROYED,     mUseCameraClipping, mCurrentFadeOffset);
		mapHelper.showEntityIds(mDamagedLevel1EntityIds, damageState == DAMAGESTATE_DAMAGE_LEVEL1, mUseCameraClipping, mCurrentFadeOffset);
		mapHelper.showEntityIds(mDamagedLevel2EntityIds, damageState == DAMAGESTATE_DAMAGE_LEVEL2, mUseCameraClipping, mCurrentFadeOffset);

		// Update building clipping listener
		// TODO(co) Review whether or not this is really required
		if (isSimulating())
		{
			unregisterMovableObjectRenderingListener();
			registerMovableObjectRenderingListener();
		}
	}

	void BuildingComponent::onSetIntact()
	{
		// Turn off fire
		setFireStateIntact();

		applyDamageLevel1MaterialToIntactMesh(false);
		setDamageOverlaysActive(false, false);
		updateTintableMesh();

		// Reset emissive map values
		mCollapseComplete = false;

		qsf::Map& map = getEntity().getMap();
		if (qsf::isInitialized(mCollapseAnimationEntityId))
		{
			map.destroyEntityById(mCollapseAnimationEntityId);
			mCollapseAnimationEntityId = qsf::getUninitialized<uint64>();
		}

		{ // Set the state of smut & emissive burn overlays
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
			materialVariationManager.setEntityMaterialPropertyValue(getEntity(), "FireGlowIntensity", qsf::MaterialPropertyValue::fromFloat(0.0f));
		}

		// Interrupt collapse animation
		if (qsf::isInitialized(mCollapseAnimationEntityId))
		{
			mCollapseAnimationYDistance = FLT_MIN;
		}

		// Cleanup particles
		const MapHelper mapHelper(getEntity().getMap());
		mapHelper.disableEntityParticleIds(mDamageData.DamagedLevel1Particles.getVariable());
		mapHelper.disableEntityParticleIds(mDamageData.DamagedLevel2Particles.getVariable());
		mapHelper.disableEntityParticleIds(mDamageData.DestroyedParticles.getVariable());

		{ // Cleanup trapped persons
			detail::cancelTimer(mTrappedPersonsDieTimerId);
			mTrappedPersonsDieTimerId = qsf::getUninitialized<uint32>();
			setMainDoorDamaged(false);
			enableParticle(mDoorParticleEffectId, false);

			auto copyOfBuildingLinks = getLinks();
			for (auto iterator : copyOfBuildingLinks)
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				qsf::Entity& containedEntity = containerLink.getTargetEntity();

				if (containerLink.mContainerCategory == container::CONTAINERTYPE_PROGRESS ||
					containerLink.mContainerCategory == container::CONTAINERTYPE_SQUAD)
				{
					// Squad entities inside buildings have always actions like "RescuePersonFromBuildingAction". make sure gamelogic is in valid state (and possible overlay components are destoryed)
					containedEntity.getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();

					// Let them exit the building
					EntityHelper entityHelper(containedEntity);
					entityHelper.leaveContainer();
					entityHelper.placeEntityAtBuildingDoor(getEntity(), false);
				}
				else
				{
					// Delete the entities in building
					removeFromContainer(containedEntity);
					containedEntity.getMap().destroyEntityById(containedEntity.getId());
				}
			}
		}

		// Hide all rubble
		setCollapseRubbleVisible(false);

		// Disable all icons
		if (isSimulating())
		{
			updateIcons();
		}
	}

	void BuildingComponent::onSetDamagedLevel1()
	{
		sendBuildingGetsDamagedMessage();

		mCollapseAnimationYDistance = 0.0f;
		mCollapseElapsedTime = 0.0f;
		mCollapseDelayTimeout = 0.0f;
		mCollapseComplete = false;
		applyDamageLevel1MaterialToIntactMesh(true);
		setDamageOverlaysActive(true, true);
		updateTintableMesh();

		// Interrupt collapse animation
		if (qsf::isInitialized(mCollapseAnimationEntityId))
		{
			mCollapseAnimationYDistance = FLT_MIN;
		}

		// Update all icons
		if (isSimulating())
		{
			updateIcons();
		}
	}

	void BuildingComponent::onSetDamagedLevel2()
	{
		sendBuildingGetsDamagedMessage();

		mCollapseAnimationYDistance = 0.0f;
		mCollapseElapsedTime = 0.0f;
		mCollapseDelayTimeout = 0.0f;
		mCollapseComplete = false;
		applyDamageLevel1MaterialToIntactMesh(false);
		setDamageOverlaysActive(true, true);
		updateTintableMesh();

		// Interrupt collapse animation
		if (qsf::isInitialized(mCollapseAnimationEntityId))
		{
			mCollapseAnimationYDistance = FLT_MIN;
		}

		// Update all icons
		if (isSimulating())
		{
			updateIcons();
		}
	}

	void BuildingComponent::onSetDestroyed()
	{
		// Turn off fire
		setFireStateDestroyed();

		applyDamageLevel1MaterialToIntactMesh(false);
		setDamageOverlaysActive(true, true);
		updateTintableMesh();

		// Play collapsing sound
		qsf::MetadataComponent& metadataComponent = getEntity().getOrCreateComponentSafe<qsf::MetadataComponent>();
		if (metadataComponent.hasTag("Small"))
		{
			AudioHelper::playBuildingBreakdownSmallSound(mAudioProxy, getEntity(), qsf::Time::fromSeconds(mCollapseDelay));
			mAudioProxy.synchronizeWithMultiplay();
		}
		else if (metadataComponent.hasTag("Big"))
		{
			AudioHelper::playBuildingBreakdownBigSound(mAudioProxy, getEntity(), qsf::Time::fromSeconds(mCollapseDelay));
			mAudioProxy.synchronizeWithMultiplay();
		}
		else //if (metadataComponent.hasTag("Medium"))
		{
			AudioHelper::playBuildingBreakdownMediumSound(mAudioProxy, getEntity(), qsf::Time::fromSeconds(mCollapseDelay));
			mAudioProxy.synchronizeWithMultiplay();
		}

		// Hide overlay icons
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_AXE, false);

		// Create a new entity with a copy of our current damaged mesh to make it look like it's collapsing
		if (qsf::isUninitialized(mCollapseAnimationEntityId) && !mCollapseComplete)
		{
			qsf::Map& map = getEntity().getMap();
			qsf::Entity& thisEntity = getEntity();
			qsf::Entity& collapseAnimationEntity = QSF_MAKE_REF(map.createEntity());

			// Flag entity as temporary
			collapseAnimationEntity.setSerializable(false);

			// Move into the right layer
			EntityHelper(thisEntity).moveEntityInOwnerLayer(collapseAnimationEntity);

			mCollapseAnimationEntityId = collapseAnimationEntity.getId();

			// Copy transformation
			qsf::TransformComponent* thisTransformComponent = thisEntity.getComponent<qsf::TransformComponent>();
			qsf::TransformComponent* transformComponent = collapseAnimationEntity.createComponent<qsf::TransformComponent>();
			if (nullptr != thisTransformComponent && nullptr != transformComponent)
			{
				transformComponent->setTransform(thisTransformComponent->getTransform());
			}

			// Copy mesh
			qsf::compositing::TintableMeshComponent* tintableMeshComponent = collapseAnimationEntity.createComponent<qsf::compositing::TintableMeshComponent>();
			if (nullptr != tintableMeshComponent)
			{
				tintableMeshComponent->setMesh(getDamagedMesh());

				qsf::compositing::TintableMeshComponent* thisTintableMeshComponent = thisEntity.getComponent<qsf::compositing::TintableMeshComponent>();
				if (nullptr != thisTintableMeshComponent)
				{
					tintableMeshComponent->setTintPalette(thisTintableMeshComponent->getTintPalette());
					tintableMeshComponent->setTintPaletteIndex(thisTintableMeshComponent->getTintPaletteIndex());
					tintableMeshComponent->updateMaterial(true);
				}

				// Collapsed ruins are fully shown later
				setHiddenSelfOnly(true);

				// Set the state of smut & emissive burn overlays
				static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
				materialVariationManager.setEntityMaterialPropertyValue(collapseAnimationEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(true));		// Enable smut map
				materialVariationManager.setEntityMaterialPropertyValue(collapseAnimationEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(true));	// Enable emissive map
				materialVariationManager.setEntityMaterialPropertyValue(collapseAnimationEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(true));	// Enable Perlin noise on emissive map

				setCustomParametersForFireGlowAndSmutOverlay(*tintableMeshComponent);
			}

			if (!mCollapseComplete)
			{
				mCollapseDelayTimeout = 0.0f;
				qsf::jobs::JobConfiguration jobConfiguration;
				jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(33);
				mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_MESH, boost::bind(&BuildingComponent::updateCollapseAnimation, this, _1), jobConfiguration);
			}
		}

		// Update all icons
		if (isSimulating())
		{
			updateIcons();
		}

		// TODO(mk) Restart cleanup here
	}

	void BuildingComponent::onSetFireLifePercentage(float fireLifePercentage)
	{
		if (fireLifePercentage == 0.0f)
		{
			// Destroy building
			setDamageState(DAMAGESTATE_DESTROYED);
		}
		else if (fireLifePercentage < 0.7f && getDamageState() < DAMAGESTATE_DAMAGE_LEVEL2)
		{
			// Severely damage building
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL2);
		}
		else if (fireLifePercentage < 1.0f && getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			// Slightly damage building
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}

		updateFireExtinguishBlocked();
	}

	void BuildingComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// Burning buildings are always damaged
		if (getDamageState() < DAMAGESTATE_DAMAGE_LEVEL1)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL1);
		}

		setupFireFadeAnimation();
		updateFireExtinguishBlocked();
		GameCounters::incrementGameCounter(GameCounters::BUILDINGS_BURNING);
	}

	void BuildingComponent::onFireStopped()
	{
		GameCounters::decrementGameCounter(GameCounters::BUILDINGS_BURNING);
	}

	void BuildingComponent::onFireDamageReceived(float amount, qsf::Component* sender, bool damageJustStarted)
	{
		if (damageJustStarted)
		{
			// TODO(fw): If needed, add parameters to the message
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_BUILDING_RECEIVED_FIRE_DAMAGE, getEntityId()));
		}
	}

	void BuildingComponent::onExplosion(FireComponent* fireComponent)
	{
		if (fireComponent != nullptr)
		{
			fireComponent->showOnExplosionFireParticles();
		}
		else
		{
			ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (complexFireComponent != nullptr)
			{
				std::vector<FireComponent*> fireComponents;
				complexFireComponent->getAllFireComponents(fireComponents);
				for (FireComponent* fireComp : fireComponents)
				{
					fireComp->showOnExplosionFireParticles();
				}
			}
		}

		// Play explosion audio
		qsf::MetadataComponent& metadataComponent = getEntity().getOrCreateComponentSafe<qsf::MetadataComponent>();
		if (metadataComponent.hasTag("Small"))
		{
			AudioHelper::playExplosionSmallSound(mAudioProxy, getEntity());
			mAudioProxy.synchronizeWithMultiplay();
		}
		else if (metadataComponent.hasTag("Big"))
		{
			AudioHelper::playExplosionVeryBigSound(mAudioProxy, getEntity());
			mAudioProxy.synchronizeWithMultiplay();
		}
		else //if (metadataComponent.hasTag("Medium"))
		{
			AudioHelper::playExplosionBigSound(mAudioProxy, getEntity());
			mAudioProxy.synchronizeWithMultiplay();
		}

		// Play atmo hint
		AtmosphericRadioMessageHelper::triggerAtmoHintExplosion(getEntity());
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool BuildingComponent::onStartup()
	{
		bool result = false;

		if (SimpleDamageComponent::onStartup())
		{
			// In case a collapse is in progress, try to continue the animation
			if (qsf::isInitialized(mCollapseAnimationEntityId) && !mCollapseComplete)
			{
				qsf::jobs::JobConfiguration jobConfiguration;
				jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(33);
				mAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_MESH, boost::bind(&BuildingComponent::updateCollapseAnimation, this, _1));
			}

			mDirtyMaterialAabb = true;
			setCollapseRubbleVisible(false);
			result = true;
		}

		return result;
	}

	void BuildingComponent::onSetSimulating(bool simulating)
	{
		// Call base implementation
		SimpleDamageComponent::onSetSimulating(simulating);

		// Inform container
		PrototypeContainer::enableSimulatingMode(simulating);

		mDirtyMaterialAabb = true;

		// We going out of simulation business check if we need to decrement a game counter
		if (simulating)
		{
			// Building clipping
			mBuildingClippingManager = &EM5_GUI.getIngameHud().getBuildingClippingManager();
			registerMovableObjectRenderingListener();
		}
		else
		{
			if (isBurning())
			{
				GameCounters::decrementGameCounter(GameCounters::BUILDINGS_BURNING);
			}

			// Cancel any running game timer
			if (qsf::isInitialized(mTrappedPersonsDieTimerId))
			{
				detail::cancelTimer(mTrappedPersonsDieTimerId);
				qsf::setUninitialized(mTrappedPersonsDieTimerId);
			}

			// Building clipping
			mBuildingClippingManager = nullptr;
			unregisterMovableObjectRenderingListener();
		}
	}

	void BuildingComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Serialize container
		PrototypeContainer::serialize(serializer);

		// Serialize base class
		SimpleDamageComponent::serialize(serializer);

		mDamageData.serialize(serializer);

		serializer.serialize(mDoors.mMainDoorId);
		serializer.serialize(mLights.mLightIds);
		serializer.serialize(mLights.mLightsActive);
		serializer.serialize(mDamagedLevel1EntityIds);
		serializer.serialize(mDamagedLevel2EntityIds);
		serializer.serialize(mDestroyedEntityIds);
		serializer.serialize(mCollapseAnimationEntityId);
		serializer.serialize(mCollapseAnimationYDistance);
		serializer.serialize(mCollapseElapsedTime);
		serializer.serialize(mCollapseDampeningFactor);
		serializer.serialize(mCollapseDelay);
		serializer.serialize(mCollapseDelayTimeout);
		serializer.serialize(mCollapseComplete);
		serializer.serialize(mDamageLevel1MaterialActive);
		serializer.serialize(mUseCameraClipping);

		// Store the flags here
		bool explosionStartFlag = mInternalFlags.isSet(SHOW_EXPLOSION);
		bool meshActiveFlag = mInternalFlags.isSet(MESH_ACTIVE);
		serializer.serialize(explosionStartFlag);
		serializer.serialize(meshActiveFlag);

		if (serializer.isReading())
		{
			mInternalFlags.set(SHOW_EXPLOSION, explosionStartFlag);
			mInternalFlags.set(MESH_ACTIVE, meshActiveFlag);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void BuildingComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		SimpleDamageComponent::onObjectRenderingStartup();

		// Now we can update the building because we have all childs visible
		if (getDamageState() == DAMAGESTATE_DAMAGE_LEVEL1)
		{
			applyDamageLevel1MaterialToIntactMesh(true);
		}
	}

	void BuildingComponent::onObjectRendering(const qsf::Component& component, const Ogre::Camera& ogreCamera)
	{
		// Call the JobProxyMovableObjectRenderingListener base implementation (not SimpleDamageComponent by intent?)
		JobProxyMovableObjectRenderingListener::onObjectRendering(component, ogreCamera);

		QSF_ASSERT(nullptr != mBuildingClippingManager, "EM5 building clipping manager pointer is a null pointer", QSF_REACT_NONE);
		mBuildingClippingManager->onBuildingComponentRendered(static_cast<const qsf::MeshComponent&>(component), static_cast<BuildingComponent&>(*this), ogreCamera);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BuildingComponent::updateCollapseAnimation(const qsf::JobArguments& jobArguments)
	{
		mCollapseDelayTimeout += jobArguments.getSecondsPassed();

		qsf::Map& map = getEntity().getMap();
		qsf::Entity* collapseAnimationEntity = map.getEntityById(mCollapseAnimationEntityId);

		if (mCollapseDelayTimeout > mCollapseDelay)
		{
			setHiddenSelfOnly(mCollapseDelayTimeout < mCollapseDelay + mCollapseShowRuinsDelay);

			// Move the mesh along the Y-axis until it has disappeared underneath the terrain
			if (nullptr != collapseAnimationEntity)
			{
				qsf::TransformComponent* transformComponent = collapseAnimationEntity->getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					mCollapseElapsedTime += jobArguments.getSecondsPassed();
					const float difference = jobArguments.getSecondsPassed() * (9.81f * mCollapseElapsedTime) * mCollapseDampeningFactor; // Current time step * gravity * time

					glm::vec3 position = transformComponent->getPosition();
					position.y -= difference;
					transformComponent->setPosition(position);

					mCollapseAnimationYDistance -= difference;
				}
			}

			// Clean up (destroy entity & unregister job proxy)
			if (0.0f >= mCollapseAnimationYDistance || nullptr == collapseAnimationEntity)
			{
				setHiddenSelfOnly(false);
				mCollapseAnimationYDistance = 0.0f;
				mCollapseElapsedTime = 0.0f;
				mCollapseDelayTimeout = 0.0f;
				mCollapseComplete = true;
				map.destroyEntityById(mCollapseAnimationEntityId);
				mCollapseAnimationEntityId = qsf::getUninitialized<uint64>();
				mAnimationJobProxy.unregister();
			}
		}
		else
		{
			setHiddenSelfOnly(true);

			if (nullptr != collapseAnimationEntity)
			{
				// TODO(tl): Hack to ensure that material is set right // TODO(mk) Whats is the "hack" here? what would be the right way or timing? Looks good for me.
				static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
				materialVariationManager.setEntityMaterialPropertyValue(*collapseAnimationEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(true));		// Enable smut map
				materialVariationManager.setEntityMaterialPropertyValue(*collapseAnimationEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(true));	// Enable emissive map
				materialVariationManager.setEntityMaterialPropertyValue(*collapseAnimationEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(true));	// Enable Perlin noise on emissive map

				setCustomParametersForFireGlowAndSmutOverlay(collapseAnimationEntity->getOrCreateComponentSafe<qsf::MeshComponent>());
			}
		}
	}

	void BuildingComponent::updateClippingAnimation(const qsf::JobArguments& jobArguments)
	{
		// TODO(pv) Potential performance hog - The eternal quest: Optimize this
		const qsf::RenderWindow* renderWindow = EM5_APPLICATION.getRenderWindow();
		if (nullptr != renderWindow)
		{
			const qsf::CameraComponent* cameraComponent = renderWindow->getCameraComponent();
			if (nullptr != cameraComponent)
			{
				const qsf::Entity& cameraEntity = cameraComponent->getEntity();
				const qsf::TransformComponent* cameraTransformComponent = cameraEntity.getComponent<qsf::TransformComponent>();
				if (nullptr != cameraTransformComponent)
				{
					const glm::vec3& cameraPosition = cameraTransformComponent->getPosition();

					const qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
					if (nullptr != meshComponent)
					{
						const Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
						if (nullptr != ogreEntity)
						{
							const Ogre::Aabb ogreWorldAabb = ogreEntity->getWorldAabb();
							const float minY = ogreWorldAabb.getMinimum().y;
							const float maxY = ogreWorldAabb.getMaximum().y;
							const float height = maxY - minY;
							mCurrentFadeOffset = mCurrentCameraHeight - ((height > 35.0f) ? 11.0f : glm::mix(11.0f, 0.0f, (mCurrentCameraHeight - minY - 5.0f) / height));

							// Check that all fires are hidden
							hideFire(true);

							if (mCurrentCameraHeight != cameraPosition.y)
							{
								mCurrentCameraHeight = cameraPosition.y;

								const MapHelper mapHelper(getEntity().getMap());

								switch (getDamageState())
								{
									case DAMAGESTATE_INTACT:
										mapHelper.showEntityIds(mDestroyedEntityIds, true, mUseCameraClipping, mCurrentFadeOffset);
										break;

									case DAMAGESTATE_DAMAGE_LEVEL1:
										mapHelper.showEntityParticleIds(mDamageData.DamagedLevel1Particles.getVariable(), true, false, true, mCurrentFadeOffset);
										mapHelper.showEntityIds(mDamagedLevel1EntityIds, true, true, mCurrentFadeOffset);
										mapHelper.showEntityIds(mDestroyedEntityIds, true, mUseCameraClipping, mCurrentFadeOffset);
										break;

									case DAMAGESTATE_DAMAGE_LEVEL2:
										mapHelper.showEntityParticleIds(mDamageData.DamagedLevel2Particles.getVariable(), true, false, mUseCameraClipping, mCurrentFadeOffset);
										mapHelper.showEntityIds(mDamagedLevel2EntityIds, true, mUseCameraClipping, mCurrentFadeOffset);
										mapHelper.showEntityIds(mDestroyedEntityIds, true, mUseCameraClipping, mCurrentFadeOffset);
										break;

									case DAMAGESTATE_DESTROYED:
										mapHelper.showEntityParticleIds(mDamageData.DestroyedParticles.getVariable(), true, false, mUseCameraClipping, mCurrentFadeOffset);
										break;

									case DAMAGESTATE_INVALID:
										// Nothing to do in this case
										break;
								}
							}
						}
					}
				}
			}
		}
	}

	void BuildingComponent::setLightsActive(bool active, bool force)
	{
		// Anything to do?
		if (!force && mLights.mLightsActive == active)
			return;

		// Change state
		mLights.mLightsActive = active;

		// Cycle through all lights
		const qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mLights.mLightIds)
		{
			// Find light entity
			qsf::Entity* lightEntity = map.getEntityById(entityId);
			if (nullptr != lightEntity)
			{
				EntityHelper(*lightEntity).setLightActive(active);
			}
		}
	}

	void BuildingComponent::setLightControllerActive(bool active)
	{
		// Enable/Disable building light time control component depending of the damage state (only intact buildings have a working time based light control)
		BuildingLightTimeControlComponent* buildingLightTimeControlComponent = getEntity().getComponent<BuildingLightTimeControlComponent>();
		if (nullptr != buildingLightTimeControlComponent)
		{
			buildingLightTimeControlComponent->setActive(active);
		}
	}

	void BuildingComponent::setSpawnsActive(bool active)
	{
		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			SpawnPointComponent* spawnPointComponent = linkComponent->getComponentFromEntityOrLinkedChild<SpawnPointComponent>();
			if (nullptr != spawnPointComponent)
			{
				spawnPointComponent->setActive(active);
			}
		}
	}

	DoorComponent* BuildingComponent::getMainDoorComponent(qsf::Entity** outBestEntityFound) const
	{
		// Get the door entity
		qsf::Entity* doorEntity = getEntity().getMap().getEntityById(mDoors.mMainDoorId);
		if (nullptr == doorEntity)
		{
			// Set building itself as best entity found as door
			if (nullptr != outBestEntityFound)
				*outBestEntityFound = &getEntity();

			// Door component not found
			return nullptr;
		}
		else
		{
			if (nullptr != outBestEntityFound)
				*outBestEntityFound = doorEntity;

			// Get the door component
			return doorEntity->getComponent<DoorComponent>();
		}
	}

	void BuildingComponent::showChildMeshes(EntityIdArray& entityIdArray, bool show)
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : entityIdArray)
		{
			// Get the entity
			const qsf::Entity* entity = map.getEntityById(entityId);
			if (nullptr != entity)
			{
				// Update the mesh
				qsf::MeshComponent* meshComponent = entity->getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					meshComponent->setActive(show);
				}
			}
		}
	}

	void BuildingComponent::setDamageOverlaysActive(bool smutActive, bool emissiveActive)
	{
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			// Set the state of smut & emissive burn overlays
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// Please, don't apply fire glow, damage etc. to persons (e.g. on a balcony)
				if (nullptr == linkedEntity->getComponent<qsf::game::PersonComponent>())
				{
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(smutActive));			// Enable smut map
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable emissive map
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplyFireGlow", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable Perlin noise on emissive map
				}
			}

			qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent)
			{
				setCustomParametersForFireGlowAndSmutOverlay(*meshComponent);
			}
		}
	}

	void BuildingComponent::killTrappedPersons(const qsf::MessageParameters& parameters)
	{
		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();

		// When this method gets called the timer is gone/canceled -> no need to save the ID to it anymore
		mTrappedPersonsDieTimerId = qsf::getUninitialized<uint32>();

		// Send a message that the time is up for the trapped persons
		qsf::MessageParameters trappedPersonsKilledMessageParameters;
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_BUILDING_TRAPPED_PERSONS_DIED, getEntityId()), trappedPersonsKilledMessageParameters);

		// A Message listener can deny that the persons get killed when setting a response
		const bool killPersons = !trappedPersonsKilledMessageParameters.hasResponse();

		// Time is over now the trapped persons must die
		std::vector<qsf::Entity*> trappedPersonEntities = getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED);
		for (qsf::Entity* trappedPerson : trappedPersonEntities)
		{
			if (killPersons)
			{
				HealthComponent* healthComponent = trappedPerson->getComponent<HealthComponent>();
				if (healthComponent)
				{
					healthComponent->killPerson(freeplayEvent);
				}
			}

			removeFromContainer(*trappedPerson);
		}

		// Now no persons are trapped anymore only their corpses burned to ashes
	}

	void BuildingComponent::enableParticle(uint64 particleEntityId, bool enable)
	{
		enableParticle(getEntity().getMap().getEntityById(particleEntityId), enable);
	}

	void BuildingComponent::enableParticle(qsf::Entity* particleEntity, bool enable)
	{
		if (nullptr != particleEntity)
		{
			qsf::ParticlesComponent* particleComponent = particleEntity->getComponent<qsf::ParticlesComponent>();
			if (particleComponent)
			{
				particleComponent->setEmitterEnabled(enable);
			}
		}
	}

	void BuildingComponent::applyDamageLevel1MaterialToIntactMesh(bool materialActive)
	{
		if (mDamageLevel1MaterialActive != materialActive)
		{
			mDamageLevel1MaterialActive = materialActive;

			// Exchange the current materials with damage or intact materials
			exchangeDamageMaterials(getEntity(), materialActive);

			{ // Nasty asset fix hack: Sadly, the low quality meshes usually don't have "blackrooms" edited, resulting in transparent windows were one can look into nothing
			  // -> Disable alpha map usage for damage materials and low mesh quality usage
			  // -> Of course, this also disables the alpha map usage for e.g. signs inside the same texture, but this looks less horrible as the "window into void" assets
			  // -> We can't just apply this to the entity a such, but need to do so just for a specific material asset to not make e.g. decals on the buildings ugly. Locate
			  //    and do this only for the primary material asset.
				// Locate the primary material asset
				const qsf::MeshComponent* meshComponent = getEntity().getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					const Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
					if (nullptr != ogreEntity)
					{
						const Ogre::v1::MeshPtr& ogreMeshPtr = ogreEntity->getMesh();
						if (!ogreMeshPtr.isNull())
						{
							qsf::GlobalAssetId globalAssetId = qsf::getUninitialized<qsf::GlobalAssetId>();
							{
								const short numSubMeshes = ogreMeshPtr->getNumSubMeshes();
								for (short i = 0; i < numSubMeshes; ++i)
								{
									const Ogre::v1::SubMesh* ogreSubMesh = ogreMeshPtr->getSubMesh(i);
									QSF_ASSERT(nullptr != ogreSubMesh, "em5::BuildingComponent::applyDamageLevel1MaterialToIntactMesh(): OGRE sub-mesh is invalid", QSF_REACT_NONE);
									const qsf::GlobalAssetId currentGlobalAssetId = boost::lexical_cast<qsf::GlobalAssetId>(ogreSubMesh->getMaterialName());
									if (qsf::AssetProxy(currentGlobalAssetId).getLocalAssetName().find("window") != std::string::npos)
									{
										globalAssetId = currentGlobalAssetId;

										// Get us out of the loop
										i = numSubMeshes;
									}
								}
							}

							// Set entity material property value
							if (qsf::isInitialized(globalAssetId))
							{
								qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();
								const qsf::MaterialPropertyValue materialPropertyValue = qsf::MaterialPropertyValue::fromBoolean(!(materialActive && qsf::RendererSettingsGroup::getInstanceSafe().getMeshQuality() == qsf::RendererSettingsGroup::MESH_QUALITY_LOW));
								materialVariationManager.setEntityMaterialPropertyValue(getEntity(), "UseAlphaMap", materialPropertyValue, getDamageMaterialName(globalAssetId));
							}
						}
					}
				}
			}

			mDirtyMaterialAabb = true;
		}
	}

	void BuildingComponent::sendBuildingGetsDamagedMessage()
	{
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_BUILDING, getEntityId()), parameters);
	}

	void BuildingComponent::startClipping()
	{
		if (!mClippingAnimationJobProxy.isValid())
		{
			QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(getEntity(), "ApplyClipping", qsf::MaterialPropertyValue::fromBoolean(true));
			mClippingAnimationJobProxy.registerAt(qsf::QsfJobs::ANIMATION_MESH, boost::bind(&BuildingComponent::updateClippingAnimation, this, _1));
			mUseCameraClipping = mDirtyMaterialAabb = true;
			continueClipping();
		}
	}

	void BuildingComponent::continueClipping()
	{
		if (mDirtyMaterialAabb)
		{
			mDirtyMaterialAabb = false;
			const qsf::Entity& entity = getEntity();
			const qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
			const qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent && nullptr != transformComponent)
			{
				const Ogre::v1::Entity* ogreEntity = meshComponent->getOgreEntity();
				if (nullptr != ogreEntity)
				{
					const Ogre::Aabb ogreWorldAabb = ogreEntity->getWorldAabb();

					glm::vec3 position = transformComponent->getPosition();
					qsf::GroundMapQuery(entity.getMap()).getTerrainHeightAtXZPosition(position.x, position.z, position.y);

					static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

					// xyz = object world space bounding box minimum, w = ground height
					const Ogre::Vector3& minimum = ogreWorldAabb.getMinimum();
					materialVariationManager.setEntityMaterialPropertyValue(entity, "WorldSpaceAabbMinimumHeight", qsf::MaterialPropertyValue::fromFloat4(minimum.x, minimum.y, minimum.z, position.y));
				}
			}
		}
	}

	void BuildingComponent::endClipping()
	{
		if (mClippingAnimationJobProxy.isValid())
		{
			hideFire(false);
			mClippingAnimationJobProxy.unregister();
			QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(getEntity(), "ApplyClipping", qsf::MaterialPropertyValue::fromBoolean(false));
			mUseCameraClipping = false;
		}
	}

	void BuildingComponent::hideFire(bool hide)
	{
		// Fire particles are disabled
		if (getDamageState() == DAMAGESTATE_DAMAGE_LEVEL1 || getDamageState() == DAMAGESTATE_DAMAGE_LEVEL2)
		{
			const ComplexFireComponent* complexFireComponent = getEntity().getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				for (uint64 entityId : complexFireComponent->getAllFireParticleIds())
				{
					qsf::Entity* entity = getEntity().getMap().getEntityById(entityId);
					if (nullptr != entity)
					{
						qsf::ParticlesComponent* particlesComponent = entity->getComponent<qsf::ParticlesComponent>();
						if (nullptr != particlesComponent)
						{
							particlesComponent->setHidden(hide);
						}
					}
				}
			}
		}
	}

	void BuildingComponent::updateFireExtinguishBlocked()
	{
		bool blocked = false;

		const qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mEboxEntityIds)
		{
			qsf::Entity* entity = map.getEntityById(entityId);
			if (nullptr != entity)
			{
				const EboxComponent* eboxComponent = entity->getComponent<EboxComponent>();
				if (eboxComponent != nullptr && eboxComponent->isDefect())
				{
					blocked = true;

					// We now have what we wanted to know, so get us out of the loop at once
					break;
				}
			}
		}

		DamageComponent* damageComponent = getEntity().getComponent<DamageComponent>();
		if (damageComponent != nullptr)
		{
			damageComponent->setFireExtinguishBlocked(blocked);
		}
	}

	void BuildingComponent::prepareEboxes()
	{
		mMessageProxyEboxRepaired.reserve(mEboxEntityIds.size());

		const qsf::Map& map = getEntity().getMap();
		for (uint64 entityId : mEboxEntityIds)
		{
			qsf::Entity* eboxEntity = map.getEntityById(entityId);
			if (nullptr != eboxEntity)
			{
				EboxComponent* eboxComponent = eboxEntity->getComponent<EboxComponent>();
				if (eboxComponent != nullptr)
				{
					eboxComponent->setEboxDefective();

					// Add component for using with engineer
					UsableByEngineerComponent& usableByEngineerComponent = eboxEntity->getOrCreateComponentSafe<UsableByEngineerComponent>();
					usableByEngineerComponent.initWithUseTime(mEboxRepairTime);

					// Add to event
					FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();
					if (nullptr != freeplayEvent)
					{
						EventIdComponent::addToEvent(*eboxEntity, *freeplayEvent, eventspreadreason::EVENT_SPECIFIC);

						ObjectiveHelper(*freeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES).increaseNeededNumber(eboxEntity);
					}

					// If we did repair the ebox we should react
					const uint64 eboxId = eboxEntity->getId();
					mMessageProxyEboxRepaired.push_back(qsf::MessageProxy());
					mMessageProxyEboxRepaired.back().registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, eboxId), boost::bind(&BuildingComponent::onEboxRepaired, this, _1, eboxId));
				}
			}
		}
	}

	void BuildingComponent::onEboxRepaired(const qsf::MessageParameters& parameters, uint64 eboxId)
	{
		qsf::Entity* eboxEntity = getEntity().getMap().getEntityById(eboxId);
		if (nullptr == eboxEntity)
			return;

		// Switch back to normal ebox
		EboxComponent* eboxComponent = eboxEntity->getComponent<EboxComponent>();
		if (nullptr != eboxComponent)
		{
			eboxComponent->setEboxIntact();
		}

		// Complete objective
		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();
		if (nullptr != freeplayEvent)
		{
			ObjectiveHelper(*freeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES).increaseCurrentNumber(eboxEntity);
		}

		// Remove repairable component
		eboxEntity->destroyComponent<UsableByEngineerComponent>();
	}

	void BuildingComponent::showDebug()
	{
		std::stringstream builder;
		builder.precision(0);
		float secondsTrappedPersonDie = detail::getRemainingTime(mTrappedPersonsDieTimerId).getSeconds();
		if (secondsTrappedPersonDie > 0.0f)
		{
			builder << "TimeTrappedPersonDie: " << secondsTrappedPersonDie << " Seconds\n";
		}

		size_t nrProgressPerson = getAllLinksByContainerType(container::CONTAINERTYPE_PROGRESS).size();
		if (nrProgressPerson > 0)
		{
			builder << "SquadsInside: " << nrProgressPerson << "\n";
		}
		size_t nrTrappedPerson = getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED).size();
		if (nrTrappedPerson > 0)
		{
			builder << "TrappedPersons: " << nrTrappedPerson << "\n";
		}
		size_t nrContainedGangster = getAllLinksByContainerType(container::CONTAINERTYPE_GANGSTER).size();
		if (nrContainedGangster > 0)
		{
			builder << "Gangster: " << nrContainedGangster << "\n";
		}
		size_t nrHostage = getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE).size();
		if (nrHostage > 0)
		{
			builder << "Hostage: " << nrHostage;
		}

		if (builder.str().empty())
		{
			mDebugDrawProxy.unregister();
		}
		else
		{
			glm::vec3 drawPosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
			drawPosition.y += 20;	// For better look

			mDebugDrawProxy.registerForComponent(*this);
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(builder.str(), drawPosition));
		}
	}

	void BuildingComponent::updateDebugDraw()
	{
		// Initialize the debug draw
		bool showDebugDisplay = true;

		if (showDebugDisplay)
		{
			// Check if the entity is visible (inside the players camera view port) Note: This check works only correct with the game camera and not with the free camera
			showDebugDisplay = (showDebugDisplay && EntityHelper(getEntity()).isEntityVisibleOnScreen());
		}

		if (showDebugDisplay)
		{
			showDebug();
		}
		else
		{
			mDebugDrawProxy.unregister();
		}
	}

	void BuildingComponent::registerDebugDraw()
	{
#ifndef ENDUSER
		// Debug
		if (!mDebugJobProxy.isValid())
		{
			qsf::jobs::JobConfiguration jobConfiguration;

			// We control only lights depending of the current game time -> call us only each realtime second
			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
			mDebugJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&BuildingComponent::updateDebugDraw, this), jobConfiguration);

			// One shot update after getting activated
			updateDebugDraw();
		}
#endif
	}

	void BuildingComponent::setHiddenSelfOnly(bool hidden)
	{
		// Just modify the OGRE scene node visibility to not change any logic relevant states
		Ogre::SceneNode* ogreSceneNode = getEntity().getOrCreateComponentSafe<qsf::MeshComponent>().getOgreSceneNode();
		if (nullptr != ogreSceneNode)
		{
			ogreSceneNode->setVisible(!hidden);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
