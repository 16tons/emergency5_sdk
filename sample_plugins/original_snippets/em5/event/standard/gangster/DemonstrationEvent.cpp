// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/DemonstrationEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/action/gangster/DemonstrateAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/observer/Observer.h"
#include "em5/game/Game.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/generic/MessageObjectiveGeneralObserver.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// EventTags
	const std::string TAG_SOUND_ENTITY = "EM2016_SOUND_DEMO";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DemonstrationEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::DemonstrationEvent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool DemonstrationEvent::canSpawnEntitiesCollisionFree(const qsf::Layer& layer, qsf::Map& map)
	{
		CollisionHelper collisionHelper(map);

		// Find all entities where collisions matter
		const qsf::Map& layerBufferMap = layer.getInternalBufferMap();
		auto entities = layerBufferMap.getEntities();
		for (qsf::Entity* entity : entities)
		{
			PersonSpawnPointComponent* personSpawnPointComponent = entity->getComponent<PersonSpawnPointComponent>();

			if (nullptr != personSpawnPointComponent)
			{
				if (!personSpawnPointComponent->canSpawnWithoutCollision(map))
					return false;
			}
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DemonstrationEvent::DemonstrationEvent() :
		mDemonstrationLayerId(qsf::getUninitialized<uint32>()),
		mDemonstrationRadius(15.0f),
		mProtesterNumber(0),
		mTimeoutDelay(qsf::Time::ZERO),
		mMaximalProtester(10000),
		mGangsterNumber(2),
		mTimerMessage(Messages::EM5_EVENT_TIMER_SIGNAL, getId(), getMap().getCoreEntity().getId())
	{
		// Nothing here
	}

	DemonstrationEvent::~DemonstrationEvent()
	{
		// Nothing here
	}

	void DemonstrationEvent::init(qsf::Layer* demonstrationLayer)
	{
		if (nullptr != demonstrationLayer)
		{
			mDemonstrationLayerId = demonstrationLayer->getId();

			mCenterEntityIds.clear();
			mCenterTurnedViolent.clear();

			qsf::ComponentMapQuery componentMapQuery(demonstrationLayer->getInternalBufferMap());
			for (const qsf::game::EventTagComponent*eventTagComponent : componentMapQuery.getAllInstances<qsf::game::EventTagComponent>())
			{
				if (eventTagComponent->hasTag(qsf::StringHash("DEMONSTRATION_CENTER"))
					|| eventTagComponent->hasTag(qsf::StringHash("DEMONSTRATION_HOTEL_CENTER")))
				{
					mCenterEntityIds.push_back(eventTagComponent->getEntityId());
					mCenterTurnedViolent.push_back(false);
				}
				if (eventTagComponent->hasTag(qsf::StringHash("DEMONSTRATION_CAM")))
				{
					createEventLocationEntity(eventTagComponent->getEntity());
				}
			}
		}
	}

	float DemonstrationEvent::getDemonstrationRadius() const
	{
		return mDemonstrationRadius;
	}

	void DemonstrationEvent::setDemonstrationRadius(float radius)
	{
		mDemonstrationRadius = radius;
	}

	uint32 DemonstrationEvent::getGangsterNumber() const
	{
		return mGangsterNumber;
	}

	void DemonstrationEvent::setGangsterNumber(uint32 gangsterNumber)
	{
		mGangsterNumber = gangsterNumber;
	}

	qsf::Time DemonstrationEvent::getTimeoutDelay() const
	{
		return mTimeoutDelay;
	}

	void DemonstrationEvent::setTimeoutDelay(qsf::Time timeoutDelay)
	{
		mTimeoutDelay = timeoutDelay;
	}

	uint32 DemonstrationEvent::getMaximalProtester() const
	{
		return mMaximalProtester;
	}

	void DemonstrationEvent::setMaximalProtester(uint32 maximalProtester)
	{
		mMaximalProtester = maximalProtester;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool DemonstrationEvent::onStartup()
	{
		if (qsf::isUninitialized(mDemonstrationLayerId))
			return false;

		qsf::Layer* layer = getMap().getLayerManager().getLayerById(mDemonstrationLayerId);
		if (nullptr == layer)
			return false;

		if (!canSpawnEntitiesCollisionFree(*layer, getMap()))
			return false;

		// Show the accident
		setupScene(*layer);

		if (mProtesterNumber < mGangsterNumber * mCenterEntityIds.size())	// Do not start a demonstration Event with less protester then gangster
			return false;

		// Show minimap icon
		for (uint64 centerEntityId : mCenterEntityIds)
		{
			EM5_GAME.getMiniMapIconRegistrationLogic().registerEventIcon(centerEntityId);
		}

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void DemonstrationEvent::onShutdown()
	{
		// Show minimap icon
		for (uint64 centerEntityId : mCenterEntityIds)
		{
			EM5_GAME.getMiniMapIconRegistrationLogic().unregisterIcon(centerEntityId);
		}

		// Ensure everything from the event is cleaned up
		qsf::Layer* layer = getMap().getLayerManager().getLayerById(mDemonstrationLayerId);
		if (nullptr != layer)
		{
			LayerCleanup::createLayerCleanup(*layer);
		}

		// All persons should go home, the party is over
		for (const PersonComponent* personComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<PersonComponent>())
		{
			qsf::Entity& personEntity = personComponent->getEntity();

			qsf::ActionComponent* actionComponent = personEntity.getComponent<qsf::ActionComponent>();
			if (nullptr == actionComponent)
				continue;

			DemonstrateAction* demonstrateAction = actionComponent->getAction<DemonstrateAction>();
			if (nullptr == demonstrateAction)
				continue;

			if (demonstrateAction->getEventId() != getId())
				continue;

			actionComponent->clearPlan();
		}

		mDelayHint01TimerMessageProxy.unregister();
	}

	void DemonstrationEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startHintObservers();

		ObjectiveHelper objectiveHelper(*this);

		// Add main objective
		Objective& chaseAwayProtester = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CHASEAWAYPROTESTER);
		chaseAwayProtester.setNeededNumber(mProtesterNumber);

		// Fail condition, only 2 civil or units are allowed to die
		objectiveHelper.addDeadCivilPersonsFailCondition(3);
		objectiveHelper.addDeadUnitPersonsFailCondition(3);

		ObserverHelper observerHelper(*this);

		// Fail condition time out
		if (mTimeoutDelay > qsf::Time::ZERO)
		{
			Objective& objectiveTimer = objectiveHelper.getOrCreateObjective(ObjectiveHelper::OBJECTIVE_FAIL_TIMER, Objective::OBJECTIVETYPE_FAILCONDITION, "$Time should not run out");
			objectiveTimer.setNeededNumber(1);

			// Create general observer for the timeout objective
			MessageObjectiveGeneralObserver& observerTimer = observerHelper.createGeneralObserver<MessageObjectiveGeneralObserver>();
			observerTimer.registerMessage(mTimerMessage, 2);
			observerTimer.connectToObjective(objectiveTimer, MessageObjectiveGeneralObserver::REACTION_INC_CURRENT_NUMBER);

			// Start the timeout timer
			EM5_GAMETIMERS.addTimer(mTimerMessage, mTimeoutDelay);
		}

		// Fail condition too many protester
		{
			Objective& objectiveTooManyProtester = objectiveHelper.getOrCreateObjective(ObjectiveHelper::OBJECTIVE_FAIL_TOOMANYPROTESTER, Objective::OBJECTIVETYPE_FAILCONDITION, "$Too many protester at a demonstration");
			objectiveTooManyProtester.setNeededNumber(mMaximalProtester + 1);
			objectiveTooManyProtester.setCurrentNumber(mProtesterNumber);
		}

		// Hint after 15 sec
		{
			qsf::MessageConfiguration message(qsf::StringHash("EM5X_DEMONSTRATION_HINT01_DELAY_SHOW"), getId());
			mDelayHint01TimerMessageProxy.registerAt(message, boost::bind(&DemonstrationEvent::onShowHint01, this, _1));
			EM5_GAMETIMERS.addTimer(message, qsf::Time::fromSeconds(15.0f));
		}
	}

	void DemonstrationEvent::onShowHint01(const qsf::MessageParameters& parameters)
	{
		showHint("EM5X_DEMONSTRATION_HINT01", false);
	}

	bool DemonstrationEvent::onFailure(EventResult& eventResult)
	{
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);

		if (mProtesterNumber > mMaximalProtester)
		{
			setFailHintsForMiniEvent(eventResult, "EM5X_MINI03_FAIL01", "EM5X_SUPERV_MINI03_FAIL01", "");
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_TIMER == failObjective)
		{
			setFailHintsForMiniEvent(eventResult, "EM5X_MINI03_FAIL02", "EM5X_SUPERV_MINI03_FAIL02", "");
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE08_FAIL01", "EM5_GE01_SUPERV_FAIL01", "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03");
		}
		else if (ObjectiveHelper::OBJECTIVE_FAIL_DEADSQUADPERSONS == failObjective)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE07_FAIL05", "EM5_GE03_SUPERV_FAIL04", "EM5_EVENT_TOO_MANY_PERSONS_DIED_HINT_03");
		}
		else
		{
			// Call base implementation
			FreeplayEvent::onFailure(eventResult);
		}

		if (isMiniEvent())
		{
			// Clean up the event by hand
			MapHelper mapHelper(getMap());
			for (uint64 entityId : getEntityIds())
			{
				qsf::Entity* entity = getMap().getEntityById(entityId);
				if (nullptr != entity && EntityHelper(*entity).isEntityPerson())
				{
					mapHelper.destroyEntity(*entity);
				}
			}
		}

		return true;
	}

	void DemonstrationEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		qsf::ComponentMapQuery componentMapQuery(getMap());

		mProtesterNumber = 0;

		float halfRadius = mDemonstrationRadius / 2.0f;

		// Just to be sure
		QSF_CHECK_ONCE(mCenterTurnedViolent.size() == mCenterEntityIds.size(), "Programming error: mCenterTurnedViolent needs to have the same size as mCenterEntityIds", return);

		for (uint32 i = 0; i < mCenterEntityIds.size(); ++i)
		{
			qsf::Entity* centerEntity = getMap().getEntityById(mCenterEntityIds[i]);
			if (nullptr != centerEntity)
			{
				const glm::vec3& centerPosition = centerEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

				uint32 numberOfPeopleToMakeAngry = 0;

				if (!mCenterTurnedViolent[i])
				{
					// Check if we have police in our demonstration, it will make some poeple angry
					std::vector<CommandableComponent*> commandableComponents;
					componentMapQuery.getInstancesInCircle<CommandableComponent>(centerPosition, mDemonstrationRadius, commandableComponents);

					if (!commandableComponents.empty())
					{
						// All units can reveal violent protester
						numberOfPeopleToMakeAngry = mGangsterNumber;
						mCenterTurnedViolent[i] = true;
					}
				}

				// Catch people from the streets
				std::vector<PersonComponent*> personComponents;
				componentMapQuery.getInstancesInCircle<PersonComponent>(centerPosition, mDemonstrationRadius, personComponents);

				for (const PersonComponent* personComponent : personComponents)
				{
					qsf::Entity& personEntity = personComponent->getEntity();

					if (qsf::game::DistanceHelper::getDistance(personEntity, *centerEntity) > mDemonstrationRadius)
						continue;

					qsf::ActionComponent& actionComponent = personEntity.getOrCreateComponentSafe<qsf::ActionComponent>();

					EntityHelper entityHelper(personEntity);
					if (entityHelper.isCivilPerson() && !entityHelper.isPersonInjured() && entityHelper.getContainerEntity() == nullptr)
					{
						// Try to make me angry, try it
						if (numberOfPeopleToMakeAngry > 0)
						{
							entityHelper.turnIntoGangster("em5::GangsterBaseLogic", false);

							GangsterBaseLogic* gangsterBaseLogic = personEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>().getGameLogic<GangsterBaseLogic>();
							if (gangsterBaseLogic != nullptr)
							{
								gangsterBaseLogic->setAsGangsterFlagged(true);
								gangsterBaseLogic->setEquippedWeaponTypes(GangsterBaseLogic::WeaponFlagSet(weapon::FIST));
								gangsterBaseLogic->setAutoAggression(true);
								gangsterBaseLogic->setAutoAggroRange(12.0f);
								gangsterBaseLogic->setAutoDeAggroRange(15.0f);
								gangsterBaseLogic->setCelebrateWinTime(qsf::Time::fromSeconds(30.0f));
								gangsterBaseLogic->addAutoTargetEventTag("Policeman", 3);
								gangsterBaseLogic->addAutoTargetEventTag("PoliceSekman", 3);

								EventIdComponent::addToEvent(personEntity, *this, eventspreadreason::GANGSTER);

								actionComponent.clearPlan();

								actionComponent.pushAction<DemonstrateAction>(action::DANGER_IMMINENT).init(getId(), centerPosition, mDemonstrationRadius, "");

								--numberOfPeopleToMakeAngry;

								ObjectiveHelper objectiveHelper(*this);
								Objective* chaseAwayProtester = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CHASEAWAYPROTESTER);
								if (nullptr != chaseAwayProtester)
								{
									chaseAwayProtester->setNeededNumber(chaseAwayProtester->getNeededNumber() - 1);
								}
							}
						}

						if (!entityHelper.isGangsterPerson() && !entityHelper.isArrested())
						{
							// Calculate the number of protester
							++mProtesterNumber;

							// Is this person new ? welcome him !
							if (actionComponent.getAction<DemonstrateAction>() == nullptr && actionComponent.getCurrentPriority() < action::DANGER_URGENT)
							{
								// Uniform distributed offset in a circle around center for our target
								const float radius = halfRadius * sqrt(qsf::Random::getRandomFloat(0.0f, 1.0f));
								const float angle = qsf::Random::getRandomFloat(-glm::pi<float>(), glm::pi<float>());
								const glm::vec3& targetPosition = centerPosition + glm::vec3(sin(angle) * radius, 0.0f, cos(angle) * radius);

								actionComponent.clearPlan();
								actionComponent.pushAction<MoveAction>(action::DANGER_IMMINENT, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(targetPosition, false, 0.0f, halfRadius)), MovementModes::MOVEMENT_MODE_FREE_WALK);
								actionComponent.pushAction<DemonstrateAction>(action::DANGER_IMMINENT).init(getId(), centerPosition, mDemonstrationRadius);
							}
						}
					}
				}
			}
		}

		// Check objectives with the number of protester
		ObjectiveHelper objectiveHelper(*this);
		Objective* chaseAwayProtesterObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CHASEAWAYPROTESTER);
		if (nullptr != chaseAwayProtesterObjective)
		{
			if (mProtesterNumber == 0)
			{
				chaseAwayProtesterObjective->setCurrentNumber(chaseAwayProtesterObjective->getNeededNumber());
				chaseAwayProtesterObjective->setAccomplished();
			}
			else
			{
				uint32 needed = chaseAwayProtesterObjective->getNeededNumber();
				uint32 chased = chaseAwayProtesterObjective->getCurrentNumber();

				if (needed - chased < mProtesterNumber)
				{
					needed = mProtesterNumber + chased;
				}
				else if (needed - chased > mProtesterNumber)
				{
					chased = needed - mProtesterNumber;
				}
				chaseAwayProtesterObjective->setNeededNumber(needed);
				chaseAwayProtesterObjective->setCurrentNumber(chased);

				if (mSoundEntity.valid() && (chased > (needed * 0.6f)))
				{
					// Destroy sound entity if 2/3 of the demonstration persons are gone
					MapHelper(getMap()).destroyEntity(mSoundEntity.getSafe());
				}
			}
		}

		Objective* objectiveFailTooManyProtester = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_TOOMANYPROTESTER);
		if (nullptr != objectiveFailTooManyProtester)
		{
			objectiveFailTooManyProtester->setCurrentNumber(mProtesterNumber);
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	void DemonstrationEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* DemonstrationEvent::getFocusEntity()
	{
		if (isEventLocationEntitySet())
			return getEventLocationEntity();

		if (mCenterEntityIds.empty())
			return nullptr;

		return getMap().getEntityById(mCenterEntityIds.front());
	}

	bool DemonstrationEvent::checkEventConfiguration()
	{
		// Empty at the moment
		return true;
	}

	void DemonstrationEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mDemonstrationLayerId);
		serializer.serialize(mCenterEntityIds);
		serializer.serialize(mDemonstrationRadius);
		serializer.serialize(mGangsterNumber);
		serializer.serialize(mCenterTurnedViolent);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DemonstrationEvent::setupScene(qsf::Layer& demonstrationLayer)
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(demonstrationLayer, false);

		// Show the accident
		MapHelper::activateLayer(demonstrationLayer, true);

		mProtesterNumber = 0;

		for (uint64 demoEntityId : demonstrationLayer.getEntityIds())
		{
			qsf::Entity* demoEntity = getMap().getEntityById(demoEntityId);
			if (demoEntity != nullptr)
			{
				PersonSpawnPointComponent* personSpawnPointComponent = demoEntity->getComponent<PersonSpawnPointComponent>();
				if (personSpawnPointComponent != nullptr)
				{
					qsf::Entity* personEntity = personSpawnPointComponent->spawnRandom();

					if (nullptr != personEntity)
					{
						qsf::ActionComponent& actionComponent = personEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
						actionComponent.clearPlan();

						// No need to search for the real center for this persons, they don't need the check anyway
						const glm::vec3& position = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
						actionComponent.pushAction<DemonstrateAction>(action::DANGER_IMMINENT).init(getId(), position, mDemonstrationRadius);

						++mProtesterNumber;

						// Demonstrants cannot use a umbrella -> disable it
						PersonComponent* personComponent = personEntity->getComponent<PersonComponent>();
						if (nullptr != personComponent)
						{
							personComponent->setCanUseUmbrella(false);
						}
					}
				}
				else if (EntityHelper(demoEntity).checkEntityHasEventTag(TAG_SOUND_ENTITY))
				{
					mSoundEntity = demoEntity;
				}
			}
		}
	}

	void DemonstrationEvent::startHintObservers()
	{
		// Empty at the moment
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
