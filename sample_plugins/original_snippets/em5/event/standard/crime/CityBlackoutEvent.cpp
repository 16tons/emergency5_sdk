// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/CityBlackoutEvent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/spawnpoint/civilist/CivilistSpawnPointComponent.h"
#include "em5/environment/lighting/LightControlComponent.h"
#include "em5/event/helper/WarblerTweetHelper.h"
#include "em5/event/standard/destruction/helper/CarAccidentHelper.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/freeplay/cleanup/CleanupManager.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/health/InjuryAtStartupComponent.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterBlackoutFP.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterBlackoutMini.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagHelper.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>
#include <qsf_game/environment/time/TimeOfDayComponent.h>
#include <qsf_game/environment/time/TimeOfDayHelper.h>
#include <qsf_game/environment/weather/WeatherComponent.h>
#include <qsf_game/environment/weather/WeatherSystem.h>
#include <qsf_game/timer/GameTimerManager.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	namespace
	{
		// Event tags for mini-event variant
		const std::string TAG_INJUREDS_P1			= "EM2017_MINI05_INJURED";
		const std::string TAG_FIRES_P1				= "EM2017_MINI05_FIRE";
		const std::string TAG_FIRES_P2				= "EM2017_MINI05_FIRE_P2";
		const std::string TAG_AREA_SCHEME_MINI		= "EM2017_MINI05_STROM_0";
		const std::string TAG_TERRORIST_SPAWN_MINI	= "MINI05_TERRORIST_SPAWN";
		const std::string TAG_TERRORCAR				= "EM2017_MINI05_TERRORCAR";
		const std::string TAG_TERRORCAR_POINT		= "MINI05_TERRORCAR_POINT";
		const std::string TAG_BOMB_POINT			= "MINI05_BOMB_POINT";
		const std::string TAG_TRIGGER_POINT			= "MINI05_TERRORIST_POINT";
		const std::string TAG_DESTROY_BUILDING		= "MINI05_DESTROY_BUILDING";

		// Event tags for freeplay event variant
		const std::string TAG_AREA_SCHEME_FP		= "EM2017_FP04_AREA_0";
		const std::string TAG_EBOX_SCHEME_FP		= "EM2017_FP04_EBOX_0";
		const std::string TAG_TERRORIST_SPAWN_FP	= "EM2017_FP04_TERRORIST_SPAWN";

		// Local messages
		qsf::StringHash MESSAGE_ID_TIMER("EM2017_CITYBLACKOUT_TIMER");
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CityBlackoutEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::CityBlackoutEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CityBlackoutEvent::CityBlackoutEvent() :
		mDeadPersonsToFail(2),
		mBurningObjectsToFail(13),
		mEventLayer(nullptr),
		mAddLayer(nullptr),
		mEraseLayer(nullptr),
		mAddLayerP2(nullptr),
		mEraseLayerP2(nullptr),
		mTweet01(*new WarblerTweetHelper())
	{
		// Nothing here
	}

	CityBlackoutEvent::~CityBlackoutEvent()
	{
		delete &mTweet01;
	}

	void CityBlackoutEvent::init(qsf::Layer& eventLayer)
	{
		mEventLayer = &eventLayer;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool CityBlackoutEvent::onStartup()
	{
		if (nullptr == mEventLayer)
			return false;

		// Setup scene for event
		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void CityBlackoutEvent::onShutdown()
	{
		// Cleanup eboxes and areas
		for (size_t i = 0; i < mEboxInfos.size(); ++i)
		{
			setBlackoutAreaActive(static_cast<uint32>(i), false);

			if (mEboxInfos[i].mEbox.valid())
			{
				EboxComponent* eboxComponent = mEboxInfos[i].mEbox->getComponent<EboxComponent>();
				if (nullptr != eboxComponent)
					eboxComponent->setEboxIntact();

				mEboxInfos[i].mEbox->destroyComponent<UsableByEngineerComponent>();
			}
		}

		// Cleanup layers again
		for (const auto& pair : mLayersToCleanup)
		{
			LayerCleanup::createLayerCleanup(*pair.first, pair.second);
		}

		// Unregister message proxies
		mGameTimerMessageProxy.unregister();
		mBombExplosionMessageProxy.unregister();
		mDamageEboxMessageProxy.unregister();
		mRepairEboxMessageProxy.unregister();
	}

	void CityBlackoutEvent::onRun()
	{
		// Register for game timer messages
		mGameTimerMessageProxy.registerAt(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId()), boost::bind(&CityBlackoutEvent::onGameTimerMessage, this, _1));

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();
	}

	bool CityBlackoutEvent::onFailure(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
		/*
			// Check the event fail reason
			const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
			if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE)
			{
				setFailHintsForMiniEvent(eventResult, "EM2017_MINI01_FAIL04", "EM2017_MINI04_SUPERV_FAIL01");
			}
			else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_DEADSQUADPERSONS)
			{
				setFailHintsForMiniEvent(eventResult, "EM5_GE14_FAIL02", "EM5_GE01_SUPERV_FAIL02");
			}
			else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS)
			{
				setFailHintsForMiniEvent(eventResult, "EM5_GE08_FAIL01", "EM5_GE01_SUPERV_FAIL01");
			}
			else
			{
				// Call base implementation
				return FreeplayEvent::onFailure(eventResult);
			}
		*/
		}
		else
		{
			// Call base implementation
			return FreeplayEvent::onFailure(eventResult);
		}

		return true;
	}

	bool CityBlackoutEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI05_SUPERV_END01");
		}

		return true;
	}

	void CityBlackoutEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool CityBlackoutEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Is it the bomb?
		if (eventSpreadReason == eventspreadreason::GANGSTER)
		{
			if (nullptr != targetEntity.getComponentById("em5::BombComponent"))
			{
				mBomb = targetEntity;

				// Tweeter gets active
				mTweet01.tweetNow();
			}
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* CityBlackoutEvent::getFocusEntity()
	{
		// Focus on the gangster
		if (mGangster.valid() && mGangster->getComponentSafe<PersonComponent>().getGangsterFlag())
		{
			return mGangster.get();
		}

		// Or anything else
		return getRandomEventEntity();
	}

	bool CityBlackoutEvent::checkEventConfiguration()
	{
		if (nullptr == mEventLayer)
			return false;

		return true;
	}

	void CityBlackoutEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);
	}

	void CityBlackoutEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(fw): implement function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CityBlackoutEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		// Objectives to win the event are added elsewhere (e.g. when an ebox gets damaged)

		// Fail conditions
		{
			if (mDeadPersonsToFail > 0)
			{
				objectiveHelper.addDeadCivilPersonsFailCondition(mDeadPersonsToFail);
				objectiveHelper.addDeadUnitPersonsFailCondition(mDeadPersonsToFail);
			}

			objectiveHelper.addEscapedGangstersFailCondition(1);

			if (isMiniEvent() && mBurningObjectsToFail > 0)
			{
				objectiveHelper.addBurningObjectsFailCondition(mBurningObjectsToFail);
			}
		}
	}

	void CityBlackoutEvent::setupScene()
	{
#ifdef ENDUSER
		// Correct time of day only for mini-event, or during development
		if (isMiniEvent())
#endif
		{
			// Make sure that it's night time
			qsf::game::TimeOfDayComponent* timeOfDayComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::TimeOfDayComponent>();
			if (nullptr != timeOfDayComponent)
			{
				qsf::Time currentTime = timeOfDayComponent->getCurrentTimeOfDay();
				if (!qsf::game::TimeOfDayHelper::isInBetweenDayTime(currentTime, mMinTriggerTimeOfDay, mMaxTriggerTimeOfDay))
				{
					timeOfDayComponent->setTime(mCorrectionTimeOfDay);
				}

				// Update light state of all dynamic lighting before applying blackout
				LightControlComponent::updateAllLightStates(timeOfDayComponent->getCurrentTimeOfDay(), true);
			}
		}

		if (isMiniEvent())
		{
			setupSceneMiniEvent();
		}
		else
		{
			setupSceneFreeplay();
		}
	}

	void CityBlackoutEvent::setupSceneMiniEvent()
	{
		// Enforce storm weather
		qsf::game::WeatherComponent* weatherComponent = getMap().getCoreEntity().getComponent<qsf::game::WeatherComponent>();
		if (nullptr != weatherComponent)
		{
			qsf::game::WeatherSystem::WeatherStateManager& weatherStateManager = QSFGAME_WEATHER.getWeatherStateManager();
			qsf::game::WeatherState* weatherState = weatherStateManager.findElement(qsf::StringHash("Storm"));
			if (nullptr != weatherState)
			{
				weatherComponent->fadeToWeatherState(*weatherState, qsf::Time::fromSeconds(5.0f));
			}
		}

		// We want free streets for this event
		EM5_FREEPLAY.getCleanupManager().removeAllCivilians();

		if (nullptr != mEventLayer)
		{
			// Find relevant sub-layers
			const std::vector<qsf::Layer*>& subLayers = mEventLayer->getLayers();
			for (qsf::Layer* subLayer : subLayers)
			{
				if (boost::ends_with(subLayer->getName(), "_add"))
				{
					if (boost::ends_with(subLayer->getName(), "_P2_add"))
						mAddLayerP2 = subLayer;
					else
						mAddLayer = subLayer;
				}
				else if (boost::ends_with(subLayer->getName(), "_erase"))
				{
					if (boost::ends_with(subLayer->getName(), "_P2_erase"))
						mEraseLayerP2 = subLayer;
					else
						mEraseLayer = subLayer;
				}
			}

			QSF_CHECK(nullptr != mAddLayer, "Add layer for first phase not found", QSF_REACT_THROW);
			QSF_CHECK(nullptr != mEraseLayer, "Erase layer for first phase not found", QSF_REACT_THROW);
			QSF_CHECK(nullptr != mAddLayerP2, "Add layer for second phase not found", QSF_REACT_THROW);
			QSF_CHECK(nullptr != mEraseLayerP2, "Erase layer for second phase not found", QSF_REACT_THROW);

			// Set layers for phase 1
			MapHelper::activateLayer(*mAddLayer, true);
			MapHelper::activateLayer(*mEraseLayer, false);
			mLayersToCleanup.emplace_back(mAddLayer, mEraseLayer);
		}

		qsf::game::EventTagHelper eventTagHelper("CityBlackout");

		// Gather areas
		for (uint32 number = 1; number <= 2; ++number)
		{
			const std::string areaName = TAG_AREA_SCHEME_MINI + std::to_string(number);

			// In mini-event, there are no eboxes; we use the same data structure nevertheless
			mEboxInfos.emplace_back();
			EboxInfo& info = mEboxInfos.back();
			info.mEbox.clear();
			info.mState = EBOX_INTACT;
			eventTagHelper.acquireTaggedEntitiesInLayer(info.mAreaBoxes, areaName, *mEventLayer);
		}

		// Get building to destroy later on
		eventTagHelper.acquireTaggedEntity(mBuildingToDestroy, TAG_DESTROY_BUILDING);

		// Cause havok: Injure persons and start fires
		for (qsf::Component* component : qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_INJUREDS_P1), QSF_MAINMAP))
		{
			HealthHelper(component->getEntity()).injurePersonByInjuryAtStartup(this);
		}
		for (qsf::Component* component : qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_FIRES_P1), QSF_MAINMAP))
		{
			EntityHelper(component->getEntity()).startFire(this);
		}

		// Get terrorist spawn point
		qsf::Entity& spawnPointEntity = eventTagHelper.acquireTaggedEntityInLayer(TAG_TERRORIST_SPAWN_MINI, *mEventLayer);
		mTerroristSpawnPoint = spawnPointEntity.getComponent<SpawnPointComponent>();
		QSF_CHECK(mTerroristSpawnPoint.valid(), "Entity with tag '" << TAG_TERRORIST_SPAWN_MINI << "' is no spawn point", QSF_REACT_THROW);

		// Spawn gangster
		{
			mGangster = mTerroristSpawnPoint->spawnRandom(false);
			GangsterBlackoutMini& gangsterLogic = dynamic_cast<GangsterBlackoutMini&>(EntityHelper(mGangster).turnIntoGangsterByType("CityBlackoutTerroristMini"));

			// Initialize with different target points
			gangsterLogic.setCar(eventTagHelper.acquireTaggedEntityInLayer(TAG_TERRORCAR, *mEventLayer));
			gangsterLogic.setTargetPointForDriving(eventTagHelper.acquireTaggedEntityInLayer(TAG_TERRORCAR_POINT, *mEventLayer));
			gangsterLogic.setBombPoint(eventTagHelper.acquireTaggedEntityInLayer(TAG_BOMB_POINT, *mEventLayer));
			gangsterLogic.setTriggerPoint(eventTagHelper.acquireTaggedEntityInLayer(TAG_TRIGGER_POINT, *mEventLayer));
			gangsterLogic.setTimeToDefuseBomb(mTimeToDefuseBomb);
			gangsterLogic.setTimeToWaitForExplosion(mTimeToWaitForExplosion);
			gangsterLogic.setEscapeTargetTag("EM2017_MINI05_DESPAWN");

			EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::GANGSTER);
		}

		// Setup tweet
		mTweet01.initialize("EM2017_MINI05_TWEET_01", *this, "EM2017_MINI05_TWEET_01", "EM2017_MINI05_TWEET_01_GAPER");

		// Create message listener
		mBombExplosionMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_EXPLODED), boost::bind(&CityBlackoutEvent::onBombExplosion, this, _1));

		// Lights out in area 01
		setBlackoutAreaActive(0, true);
	}

	void CityBlackoutEvent::setupSceneFreeplay()
	{
		qsf::game::EventTagHelper eventTagHelper("CityBlackout");

		// Gather eboxes
		for (uint32 number = 1; number <= 4; ++number)
		{
			const std::string eventTag = TAG_EBOX_SCHEME_FP + std::to_string(number);
			qsf::Entity& eboxEntity = eventTagHelper.acquireTaggedEntityInLayer(eventTag, *mEventLayer);

			const std::string areaName = TAG_AREA_SCHEME_FP + std::to_string(number);

			mEboxInfos.emplace_back();
			EboxInfo& info = mEboxInfos.back();
			info.mEbox = eboxEntity;
			info.mState = EBOX_INTACT;
			eventTagHelper.acquireTaggedEntitiesInLayer(info.mAreaBoxes, areaName, *mEventLayer);
		}

		// Get terrorist spawn point
		qsf::Entity& spawnPointEntity = eventTagHelper.acquireTaggedEntityInLayer(TAG_TERRORIST_SPAWN_FP, *mEventLayer);
		mTerroristSpawnPoint = spawnPointEntity.getComponent<SpawnPointComponent>();
		QSF_CHECK(mTerroristSpawnPoint.valid(), "Entity with tag '" << TAG_TERRORIST_SPAWN_FP << "' is no spawn point", QSF_REACT_THROW);

		// HACK(fw)
		static_cast<CivilistSpawnPointComponent&>(mTerroristSpawnPoint.getSafe()).setSpawnPoolsAsString("gangster_terrorist");

		// Make ebox 01 defective, also switching out the lights there
		makeEboxDefective(0);

		// Create message listener for damaging and repairing
		mDamageEboxMessageProxy.registerAt(qsf::MessageConfiguration(GangsterBlackoutFP::MESSAGE_ON_EBOX_DAMAGE), boost::bind(&CityBlackoutEvent::onEboxDamageByGangster, this, _1));
		mRepairEboxMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED), boost::bind(&CityBlackoutEvent::onEboxRepaired, this, _1));

		// Start timer
		EM5_GAMETIMERS.addTimer(qsf::MessageConfiguration(MESSAGE_ID_TIMER, getId(), 0), mWaitTimeForArea02);
	}

	void CityBlackoutEvent::setBlackoutAreaActive(uint32 index, bool blackout)
	{
		QSF_CHECK(index < mEboxInfos.size(), "Invalid ebox index " << index, QSF_REACT_THROW);
		MapHelper(QSF_MAINMAP).setEnergySectionEnabled(mEboxInfos[index].mAreaBoxes, !blackout);
	}

	void CityBlackoutEvent::makeEboxDefective(uint32 index)
	{
		QSF_CHECK(index < mEboxInfos.size(), "Invalid ebox index " << index, QSF_REACT_THROW);
		QSF_CHECK(mEboxInfos[index].mState == EBOX_INTACT, "EBox " << index << " to make defective was not intact, but " << mEboxInfos[index].mState, QSF_REACT_NONE);

		mEboxInfos[index].mState = EBOX_DEFECTIVE;
		qsf::Entity& ebox = mEboxInfos[index].mEbox.getSafe();

		EboxComponent& eboxComponent = ebox.getOrCreateComponentSafe<EboxComponent>();
		eboxComponent.setEboxDefective();

		// Add component for using with engineer
		UsableByEngineerComponent& usableByEngineerComponent = ebox.getOrCreateComponentSafe<UsableByEngineerComponent>();
		usableByEngineerComponent.initWithUseTime(mTimeToRepairEbox);

		// Add to event
		EventIdComponent::addToEvent(ebox, *this, eventspreadreason::EVENT_SPECIFIC);

		// Add to objective
		Objective& objectiveRepairEboxes = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);
		objectiveRepairEboxes.increaseNeededNumber(&ebox);

		// Let the lights go out in area
		setBlackoutAreaActive(index, true);

		// Event-specific reaction
		if (!isMiniEvent())
		{
			switch (index)
			{
				case 0:
				{
					// Start a traffic accident
					startTrafficAccident("FP04_AREA_01_ACCIDENTTRAFFIC");
					break;
				}

				case 1:
				{
					// Let the gangster go
					spawnFreeplayGangster();

					// Person gets injured
					injurePerson("EM2017_FP04_AREA_02_VICTIM");

					showHint("EM2017_FP04_OFFENDER_HINT01", false);
					break;
				}

				case 2:
				{
					// Start a traffic accident
					startTrafficAccident("FP04_AREA_03_ACCIDENTTRAFFIC");

					showHint("EM2017_FP04_ACCIDENT_HINT01", false);
					break;
				}

				case 3:
				{
					// Person gets injured
					injurePerson("EM2017_FP04_AREA_04_VICTIM");

					showHint("EM2017_FP04_INJURY_HINT01", false);
					break;
				}
			}
		}
	}

	void CityBlackoutEvent::spawnFreeplayGangster()
	{
		mGangster = mTerroristSpawnPoint->spawnRandom(false);
		GangsterBlackoutFP& gangsterLogic = dynamic_cast<GangsterBlackoutFP&>(EntityHelper(mGangster).turnIntoGangsterByType("CityBlackoutTerroristFP"));

		// Only two eboxes left as target to actually run to
		std::vector<qsf::Entity*> nextEboxes;
		nextEboxes.push_back(mEboxInfos[2].mEbox.get());
		nextEboxes.push_back(mEboxInfos[3].mEbox.get());
		gangsterLogic.setTargetEboxes(nextEboxes);
		gangsterLogic.setTimeToDamageEbox(mTimeToDamageEbox);

		EventIdComponent::addToEvent(*mGangster, *this, eventspreadreason::GANGSTER);

		qsf::ActionComponent& actionComponent = mGangster->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<PlayAnimationAction>().init(AnimationHelper(*mGangster).getAnimationEngineerRepairStandingLoop(), true, true, false, qsf::Time::ZERO, qsf::Time::fromSeconds(5.0f));

		// After this action, the gangster logic will do the rest
	}

	void CityBlackoutEvent::startTrafficAccident(const std::string& accidentLayerName)
	{
		qsf::Layer* accidentLayer = nullptr;
		for (qsf::Layer* layer : mEventLayer->getLayers())
		{
			// Check layer name
			if (layer->getName() == accidentLayerName)
			{
				accidentLayer = layer;
				break;
			}
		}
		QSF_CHECK(nullptr != accidentLayer, "Accident layer '" << accidentLayerName << "' not found", return);

		// Activate the layer
		MapHelper(getMap()).activateLayer(*accidentLayer, true);
		mLayersToCleanup.emplace_back(accidentLayer, nullptr);

		// Check all entities in the accident layer
		qsf::Layer::EntityIdSet entities = accidentLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				if (EntityHelper(*targetEntity).isCivilRoadVehicle())
				{
					CarAccidentHelper::prepareAccidentCar(*targetEntity, *accidentLayer, *this, false);
				}
				else
				{
					// Person to injure?
					InjuryAtStartupComponent* injuryAtStartupComponent = targetEntity->getComponent<InjuryAtStartupComponent>();
					if (nullptr != injuryAtStartupComponent)
					{
						HealthHelper(*targetEntity).injurePersonByEventById(injuryAtStartupComponent->getInjuryId(), this);
					}
				}
			}
		}
	}

	void CityBlackoutEvent::injurePerson(const std::string& eventTag)
	{
		// Get a random spawn point
		qsf::EntityVectorScratchBuffer spawnpoints;
		qsf::game::EventTagHelper("CityBlackout").acquireTaggedEntitiesInLayer(spawnpoints, eventTag, *mEventLayer);
		QSF_CHECK(!spawnpoints.empty(), "No spawn point found with tag '" << eventTag << "'", return);

		qsf::Entity* spawnpoint = qsf::Random::getAnyOf(*spawnpoints);
		SpawnPointComponent* spawnPointComponent = spawnpoint->getComponent<SpawnPointComponent>();
		QSF_CHECK(nullptr != spawnPointComponent, "Spawn point entity has no spawn point component", return);

		qsf::Entity* person = spawnPointComponent->spawnRandom(false);
		QSF_CHECK(nullptr != person, "No person spawned", return);

		// Spawn point could have an injury at startup component assigned
		InjuryAtStartupComponent* injuryAtStartupComponent = spawnpoint->getComponent<InjuryAtStartupComponent>();
		if (nullptr != injuryAtStartupComponent)
		{
			HealthHelper(*person).injurePersonByEventById(injuryAtStartupComponent->getInjuryId(), this);
		}
		else
		{
			HealthHelper(*person).injurePerson(injury::BROKEN_ARM, this);
		}
	}

	void CityBlackoutEvent::onGameTimerMessage(const qsf::MessageParameters& parameters)
	{
		const uint32 timerId = parameters.getFilter<uint32>(2);
		switch (timerId)
		{
			case 0:
			{
				// When the event is running for X seconds, the second area is switched out (if the player was not faster)
				if (mEboxInfos[1].mState == EBOX_INTACT)
				{
					makeEboxDefective(1);
				}
			}
		}
	}

	void CityBlackoutEvent::onBombExplosion(const qsf::MessageParameters& parameters)
	{
		// Gangster just triggered the bomb explosion, now we have to care for the effects

		// Destroy the building
		BuildingComponent& buildingComponent = mBuildingToDestroy->getComponentSafe<BuildingComponent>();
		buildingComponent.setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
		buildingComponent.setCollapseRubbleVisible(true);

		// Set layers for phase 2
		MapHelper::activateLayer(*mAddLayerP2, true);
		MapHelper::activateLayer(*mEraseLayerP2, false);
		mLayersToCleanup.emplace_back(mAddLayerP2, mEraseLayerP2);

		// Lights out in area 02
		setBlackoutAreaActive(1, true);

		// Start more fires
		for (qsf::Component* component : qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_FIRES_P2), QSF_MAINMAP))
		{
			EntityHelper(component->getEntity()).startFire(this);
		}
	}

	void CityBlackoutEvent::onEboxDamageByGangster(const qsf::MessageParameters& parameters)
	{
		// Gangster just frickled around a bit, now let the lights go out there
		const uint64 eboxId = parameters.getFilter(1);
		uint32 index = 0;
		for (auto& info : mEboxInfos)
		{
			if (info.mEbox->getId() == eboxId)
				break;
			++index;
		}

		QSF_CHECK(index < 4, "Invalid index", return);
		makeEboxDefective(index);
	}

	void CityBlackoutEvent::onEboxRepaired(const qsf::MessageParameters& parameters)
	{
		// Find out which ebox was repaired
		uint32 index = 0;
		{
			for (const auto& eboxInfo : mEboxInfos)
			{
				if (eboxInfo.mEbox.valid() && eboxInfo.mEbox->getId() == parameters.getFilter(1))
				{
					break;
				}
				++index;
			}

			if (index >= mEboxInfos.size())
			{
				// Not found
				return;
			}
		}

		// Switch light on in area again
		setBlackoutAreaActive(index, false);

		// Mark as repaired
		mEboxInfos[index].mState = EBOX_REPAIRED;
		mEboxInfos[index].mEbox->destroyComponent<UsableByEngineerComponent>();

		// Update objective
		Objective& objectiveRepairEboxes = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIREBOXES);
		objectiveRepairEboxes.increaseCurrentNumber(mEboxInfos[index].mEbox.get());

		// Event progress
		if (index == 0)
		{
			// Next one, except it got defective already
			if (mEboxInfos[1].mState == EBOX_INTACT)
			{
				makeEboxDefective(1);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
