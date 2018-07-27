// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/EvacuateTrainStationEvent.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MoverType.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/observer/BombExplosionObserver.h"
#include "em5/logic/observer/DefuseBombObserver.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/Game.h"
#include "em5/gui/hud/WarblerFeedWindow.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf_ai/perception/AspectComponent.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>
#include <qsf/math/Color4.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// EventTags
	const std::string TAG_TRAIN_STATION = "EM2017_MINI04_STATION";
	const std::string TAG_TRAIN_STATION_2 = "EM2017_MINI04_STATION_2";
	const std::string TAG_EVACUATE_TARGET = "EM2017_MINI04_POINT";
	const std::string TAG_FLEEING_CIVL = "EM2017_MINI04_PASSENGER";
	const std::string TAG_FLEEING_INJURED = "EM2017_MINI04_SCHOCK";
	const std::string TAG_BOMB_FAKE = "EM2017_MINI04_CASE";
	const std::string TAG_BOMB_CASE = "EM2017_MINI04_BOMB_ERASE";	// One fake bomb also has this tag. This is the "visible" version of the bomb
	const std::string TAG_BOMB = "EM2017_MINI04_BOMB";				// Real bomb entity (with bomb component)
	const std::string TAG_GAPER = "EM2017_MINI04_GAPER";
	const std::string TAG_TWEET_CAMERA = "EM2017_MINI04_TWEET_01";
	const std::string TAG_EVACUATE_AREA = "EM2017_MINI04_EVACUATE_AREA";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 EvacuateTrainStationEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::EvacuateTrainStationEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EvacuateTrainStationEvent::EvacuateTrainStationEvent() :
		mMainLayer(nullptr),
		mEventLayer(nullptr),
		mEraseLayer(nullptr),
		mTimeTillBombExplode(qsf::Time::fromSeconds(45.0f)),
		mTimeToBombDefuse(qsf::Time::fromSeconds(10.0f)),
		mTimeDelayLeavingStation(qsf::Time::fromSeconds(0.8f)),
		mTrainStationIsEvacuated(false),
		mPersonsShocked(false),
		mBombDefused(false),
		mNumberPersonsEvacuated(0),
		mCanTweet(true),
		mPersonsLeaveTillTweet(1)
	{
		// Nothing here
	}

	EvacuateTrainStationEvent::~EvacuateTrainStationEvent()
	{
		// Nothing here
	}

	void EvacuateTrainStationEvent::init(qsf::Layer& eventLayer)
	{
		mMainLayer = &eventLayer;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool EvacuateTrainStationEvent::onStartup()
	{
		if (nullptr == mMainLayer)
			return false;

		if (!getSubLayerFromMainLayer())
			return false;

		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void EvacuateTrainStationEvent::onShutdown()
	{
		if (mEvacuateArea.valid())
		{
			MinimapHelper::getMiniMapIconRegistrationLogic().unregisterPolygon(mEvacuateArea.getSafe().getId());
		}

		// Create a layer cleanup to clean up the rest
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer, mEraseLayer);
		}

		mEvacuatePersonsMessageProxy.unregister();
		mBombExplodeMessageProxy.unregister();
		mSearchBombMessageProxy.unregister();
	}

	void EvacuateTrainStationEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();

		mEvacuatePersonsMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_FREE_PERSON_FROM_BUILDING), boost::bind(&EvacuateTrainStationEvent::onEvacuatePerson, this, _1));
		if (mBombEntity.valid())
		{
			mBombExplodeMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_DEFUSED, mBombEntity->getId()), boost::bind(&EvacuateTrainStationEvent::onBombDefused, this, _1));
		}
		mSearchBombMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_POLICE_DOG_FOUND_OBJECT), boost::bind(&EvacuateTrainStationEvent::onFoundBomb, this, _1));

		// Show minimap icon
		if (mEvacuateArea.valid())
		{
			const qsf::Transform& transform = mEvacuateArea->getComponentSafe<qsf::TransformComponent>().getTransform();

			std::vector<glm::vec3> vertices;
			vertices.emplace_back(transform.vec3PositionLocalToWorld(glm::vec3(-0.5f, 0.0f, +0.5f)));
			vertices.emplace_back(transform.vec3PositionLocalToWorld(glm::vec3(-0.5f, 0.0f, -0.5f)));
			vertices.emplace_back(transform.vec3PositionLocalToWorld(glm::vec3(+0.5f, 0.0f, -0.5f)));
			vertices.emplace_back(transform.vec3PositionLocalToWorld(glm::vec3(+0.5f, 0.0f, +0.5f)));
			MinimapHelper::getMiniMapIconRegistrationLogic().registerPolygon(mEvacuateArea->getId(), qsf::Color4(qsf::Color3::ORANGE, 0.5f), vertices);
		}
	}

	bool EvacuateTrainStationEvent::onFailure(EventResult& eventResult)
	{
		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_EXPLODED_BOMBS)
		{
			// Bomb timer
			setFailHintsForMiniEvent(eventResult, "EM2017_MINI04_FAIL01", "EM2017_MINI04_SUPERV_FAIL01");
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

		return true;
	}

	bool EvacuateTrainStationEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI04_SUPERV_END01");
		}

		return true;
	}

	void EvacuateTrainStationEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Update objective "Chase away gapers"
		if (mEvacuateArea.valid())
		{
			const qsf::TransformComponent& transformComponent = mEvacuateArea->getComponentSafe<qsf::TransformComponent>();

			// Count gapers inside box
			uint32 numCivilistsInBox = 0;
			{
				for (qsf::WeakPtr<qsf::Entity>& areaEntity : mPotentialPersonsInArea)
				{
					// Some persons could have left the map already
					if (!areaEntity.valid())
						continue;

					PersonComponent* personComponent = areaEntity->getComponent<PersonComponent>();
					if (nullptr != personComponent && personComponent->getPersonFlags(PersonComponent::PersonFlags::FLAG_IGNORE_MEGAPHONE))
						continue;

					const bool insideBox = EntityHelper(areaEntity).isInsideBox(transformComponent, true);
					if (insideBox)
					{
						// If this is a new civilist, add it to the set of known civilists to chase away
						mCivilistsToChaseAway.insert(areaEntity->getId());
						++numCivilistsInBox;
					}

					StatusOverlayComponent::showIcon(areaEntity.getSafe(), StatusOverlayComponent::STATUS_ICON_GAPER, insideBox);
				}
			}

			const uint32 numCivilistsToChaseAway = static_cast<uint32>(mCivilistsToChaseAway.size());

			Objective& objective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CHASEAWAYPROTESTER);
			objective.setCurrentNumber(numCivilistsToChaseAway - numCivilistsInBox);
			objective.setNeededNumber(numCivilistsToChaseAway);

			if (objective.checkAccomplished() && !mTrainStationIsEvacuated && numCivilistsToChaseAway == static_cast<uint32>(mPotentialPersonsInArea.size()))
			{
				mTrainStationIsEvacuated = true;

				showSupervisorMessage("EM2017_MINI05_OBJECTIVE02");

				activateBombCases();
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool EvacuateTrainStationEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* EvacuateTrainStationEvent::getFocusEntity()
	{
		return getRandomEventEntity();
	}

	bool EvacuateTrainStationEvent::checkEventConfiguration()
	{
		if (nullptr == mMainLayer)
			return false;

		if (!getSubLayerFromMainLayer())
			return false;

		// Don't start the same event location twice
		if (!mEraseLayer->isActive())
			return false;

		if (mEventLayer->isActive())
			return false;

		return true;
	}

	void EvacuateTrainStationEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);
	}

	void EvacuateTrainStationEvent::hintCallback(Observer& hintObserver)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EvacuateTrainStationEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		// Start objective "Chase away gapers"
		Objective& chaseAwayGapers = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CHASEAWAYPROTESTER);
		chaseAwayGapers.setText("EM2017_MINI04_OBJ03");
		chaseAwayGapers.setNeededNumber(1);		// Gets corrected soon, but this is needed so that the objective is not immediately accomplished

		objectiveHelper.addDeadUnitPersonsFailCondition(1);
		objectiveHelper.addDeadCivilPersonsFailCondition(1);
		objectiveHelper.addBurningObjectsFailCondition(20);
	}

	bool EvacuateTrainStationEvent::getSubLayerFromMainLayer()
	{
		const std::vector<qsf::Layer*>& subLayers = mMainLayer->getLayers();
		for (size_t i = 0; i < subLayers.size(); ++i)
		{
			if (subLayers[i]->getName().find("_erase") != std::string::npos)
			{
				mEraseLayer = subLayers[i];
			}
			else if (subLayers[i]->getName().find("_event") != std::string::npos)
			{
				mEventLayer = subLayers[i];
			}

			if (mEraseLayer != nullptr && mEventLayer != nullptr)
			{
				break;
			}
		}

		return (mEraseLayer != nullptr && mEventLayer != nullptr);
	}

	void EvacuateTrainStationEvent::setupScene()
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(*mEventLayer, false);

		// Show the accident
		MapHelper::activateLayer(*mEventLayer, true);

		// Hide the non important environment
		MapHelper::activateLayer(*mEraseLayer, false);

		EventHelper eventHelper(*this);
		ObjectiveHelper objectiveHelper(*this);

		qsf::Layer::EntityIdSet entities = mEventLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.checkEntityHasEventTag(TAG_EVACUATE_TARGET))
				{
					mEvacuationEntity = targetEntity;
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_FLEEING_CIVL))
				{
					mFleeingCivil.push_back(targetEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_GAPER))
				{
					mGapingCivil.push_back(targetEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_BOMB))
				{
					mBombEntity = targetEntity;

					qsf::game::HiddenComponent::setHidden(*mBombEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_BOMB_FAKE))
				{
					mFakeBombEntities.push_back(targetEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TRAIN_STATION_2))
				{
					mStationEntity2 = targetEntity;
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_CAMERA))
				{
					mTweetCameraEntity = targetEntity;
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_EVACUATE_AREA))
				{
					mEvacuateArea = targetEntity;
				}
			}
		}

		mReleasedFromMegaphoneMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_MAGNET_STOP_FOLLOW), boost::bind(&EvacuateTrainStationEvent::onReleasedFromMegaphone, this, _1));

		mPotentialPersonsInArea.insert(mPotentialPersonsInArea.begin(), mGapingCivil.begin(), mGapingCivil.end());
		mPotentialPersonsInArea.insert(mPotentialPersonsInArea.begin(), mFleeingCivil.begin(), mFleeingCivil.end());

		// Not all important objects are in our event layer (eg. houses in baselayer, they are used in multiple events and can't move in specific event folder)
		qsf::game::EventTagManagerComponent& eventTagManager = QSF_MAINMAP.getCoreEntity().getOrCreateComponentSafe<qsf::game::EventTagManagerComponent>();
		mStationEntity = eventTagManager.getEntityByTag(qsf::StringHash(TAG_TRAIN_STATION));
		// We checked the event for this station entity already, empty station means internal error
		QSF_ASSERT(mStationEntity.valid(), "EvacuateTrainStationEvent::setupScene() can't find entity with tag " + TAG_TRAIN_STATION + ", configuration mismatch", QSF_REACT_THROW);

		//Put all civilians in the main station building
		for (qsf::WeakPtr<qsf::Entity>& fleeingPerson : mFleeingCivil)
		{
			setupFleeingPerson(*fleeingPerson);
		}

		BuildingComponent* buildingComponent = mStationEntity->getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			buildingComponent->createTrappedPerson(*this);
			buildingComponent->updateIcons();

			PersonInBuildingLogic& personInBuildingLogic = mStationEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>().getOrCreateGameLogicSafe<PersonInBuildingLogic>();
			personInBuildingLogic.addAllowedUnit(CommandableComponent::UNITTAG_POLICE_POLICE);
			personInBuildingLogic.addAllowedUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER);
			personInBuildingLogic.setNumberValidTeamUnitsNeeded(6);
		}
		EventSpreadHelper::setEventIdComponent(this, *mStationEntity, eventspreadreason::EVENT_SPECIFIC);
	}

	void EvacuateTrainStationEvent::setupFleeingPerson(qsf::Entity& fleeingPerson)
	{
		EntityHelper entityHelper(fleeingPerson);
		PersonsPosition personPosition;
		personPosition.entityId = fleeingPerson.getId();
		personPosition.finalTransform = entityHelper.getTransform();
		mPersonsPositions.push_back(personPosition);

		entityHelper.enterContainer(*mStationEntity, container::CONTAINERTYPE_TRAPPED);
		EventSpreadHelper::setEventIdComponent(this, fleeingPerson, eventspreadreason::EVENT_SPECIFIC);

		Objective& objectiveTakeOutsideAll = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FREEPERSONSFROMBUILDING);
		objectiveTakeOutsideAll.setText("EM2017_MINI04_OBJ01");
		objectiveTakeOutsideAll.increaseNeededNumber(&fleeingPerson);

		// Some entities have wrong editing, delete the wrong components
		entityHelper.disableMeshAnimationTestComponent();
	}

	void EvacuateTrainStationEvent::activateBombCases()
	{
		// Create objective
		Objective& takeBombOutsideObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDBOMBINCASE);
		for (qsf::WeakPtr<qsf::Entity>& bombEntity : mFakeBombEntities)
		{
			// Create bomb target
			bombEntity->getOrCreateComponentSafe<PoliceDogTargetComponent>();
			takeBombOutsideObjective.increaseNeededNumber(bombEntity.get());
		}
	}

	void EvacuateTrainStationEvent::onEvacuatePerson(const qsf::MessageParameters& parameters)
	{
		uint64 personId = parameters.getFilter(1);
		qsf::Entity* fleeingPerson = getMap().getEntityById(personId);
		if (nullptr != fleeingPerson && mPersonsPositions.size() > 0)
		{
			// Override the default behavior for evacuated persons, let them move to predefined positions
			// Respond to the message to avoid creating a "TrappedPersonLogic"
			parameters.respond(true);
			// Because we send a respond, the observer is not evaluated -> set objective by hand
			Objective& objectiveTakeOutsideAll = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FREEPERSONSFROMBUILDING);
			objectiveTakeOutsideAll.increaseCurrentNumber(fleeingPerson);

			// Free them manually from container
			EntityHelper entityHelper(*fleeingPerson);
			entityHelper.leaveContainer();
			// For better look, we use two possible starting positions
			if (mNumberPersonsEvacuated % 2 == 0 || !mStationEntity2.valid())
			{
				// Persons with even numbers to the train station door
				entityHelper.placeEntityAtBuildingDoor(mStationEntity.getSafe(), false);
			}
			else
			{
				// Persons with odd numbers to the second door
				entityHelper.setPosition(EntityHelper(mStationEntity2.getSafe()).getPosition());
			}
			entityHelper.fadeIn();

			// To avoid having the old target as result of the getFocusEntity()
			fleeingPerson->destroyComponent<EventIdComponent>();

			fleeingPerson->destroyComponent<MoveToDespawnComponent>();

			fleeingPerson->destroyComponent<qsf::ai::AspectComponent>();

			// Give them a pre edited position for movement
			qsf::Transform goalTransform = (mEvacuationEntity.valid() ? EntityHelper(*mEvacuationEntity).getTransform() : mPersonsPositions[0].finalTransform);
			for (PersonsPosition personPosition : mPersonsPositions)
			{
				if (personPosition.entityId == fleeingPerson->getId())
				{
					goalTransform = personPosition.finalTransform;
					break;
				}
			}

			qsf::ActionComponent& actionComponent = fleeingPerson->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			qsf::logic::TargetPoint moveTargetConfig;
			moveTargetConfig.mPosition = goalTransform.getPosition();
			moveTargetConfig.mOrientation = goalTransform.getRotation();

			actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(
				new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_FREE_RUN);
		}

		++mNumberPersonsEvacuated;

		if (mNumberPersonsEvacuated >= static_cast<int>(mPersonsPositions.size()))
		{
			// Remove the event status icon
			BuildingComponent* buildingComponent = mStationEntity->getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				buildingComponent->updateIcons();
			}
		}

		if (mCanTweet && mNumberPersonsEvacuated >= mPersonsLeaveTillTweet && isMiniEvent())
		{
			if (mTweetCameraEntity.valid())
			{
				EM5_GUI.getIngameHud().getOrCreateWarblerFeedWindow().addWarblerTweet("EM2017_MINI04_TWEET_01", mTweetCameraEntity->getId());
				mCanTweet = false;
			}
		}
	}

	void EvacuateTrainStationEvent::onBombDefused(const qsf::MessageParameters& parameters)
	{
		mBombDefused = true;
		// The objective for defusing bomb is handled over an bomb defuse observer
	}

	void EvacuateTrainStationEvent::onFoundBomb(const qsf::MessageParameters& parameters)
	{
		// Check if the bomb is related to our bomb
		const uint64 bombEntityId = parameters.getFilter(1);
		qsf::Entity* bombEntity = nullptr;
		for (qsf::WeakPtr<qsf::Entity>& fakeBombEntity : mFakeBombEntities)
		{
			if (fakeBombEntity.valid() && fakeBombEntity->getId() == bombEntityId)
			{
				bombEntity = fakeBombEntity.get();
				break;
			}
		}
		if (nullptr == bombEntity)
		{
			// Bomb is not related to our event
			return;
		}

		// Update the objective
		Objective& findBombCaseObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDBOMBINCASE);

		if (EntityHelper(*bombEntity).checkEntityHasEventTag(TAG_BOMB_CASE))
		{
			// Found real bomb
			for (qsf::WeakPtr<qsf::Entity>& fakeBombEntity : mFakeBombEntities)
			{
				if (!fakeBombEntity.valid())
					continue;

				// Set the objective accomplished, don`t matter if other spots are active, we found the bomb.
				findBombCaseObjective.increaseCurrentNumber(fakeBombEntity.get());

				fakeBombEntity->destroyComponent<PoliceDogTargetComponent>();
			}

			qsf::game::HiddenComponent::setHidden(*bombEntity, true);
			activateRealBomb();
		}
		else
		{
			findBombCaseObjective.increaseCurrentNumber(bombEntity);

			// Police dog should run back at once
			parameters.respond(qsf::Time::fromSeconds(0.0f));
		}
	}

	void EvacuateTrainStationEvent::activateRealBomb()
	{
		if (!mBombEntity.valid())
		{
			QSF_ERROR("EvacuateTrainStationEvent::activateRealBomb() Bomb entity is not valid", return);
		}

		showSupervisorMessage("EM2017_MINI05_OBJECTIVE03");

		// Make bomb visible
		qsf::game::HiddenComponent::setHidden(*mBombEntity, false);

		BombComponent& bombComponent = mBombEntity->getOrCreateComponentSafe<BombComponent>();
		bombComponent.setTimeRunning(mTimeTillBombExplode);
		bombComponent.setTimeToDefuse(mTimeToBombDefuse);
		bombComponent.startBombTimer(*this, true);

		EventSpreadHelper::setEventIdComponent(this, *mBombEntity, eventspreadreason::GANGSTER);

		// Create disarm bomb objective
		Objective& disarmObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_DISPOSEBOMB);
		disarmObjective.increaseNeededNumber(mBombEntity.get());

		// Create new objective observer and connect it to the objective
		ObserverHelper observerHelper(*this);
		DefuseBombObserver& observerDisarmBomb = observerHelper.createObserver<DefuseBombObserver>(mBombEntity->getId());
		observerDisarmBomb.connectToObjective(disarmObjective);

		// Add the bomb to the fail condition that no more than one bomb shall explode
		Objective& failConditionBombExplosions = addFailCondition(ObjectiveHelper::OBJECTIVE_FAIL_EXPLODED_BOMBS, 1, "");

		// Create and connect observers
		BombExplosionObserver& observerBombExplosion = observerHelper.createObserver<BombExplosionObserver>(mBombEntity->getId());
		observerBombExplosion.connectToObjective(failConditionBombExplosions);
	}

	void EvacuateTrainStationEvent::onReleasedFromMegaphone(const qsf::MessageParameters& parameters)
	{
		const uint64 personEntityId = parameters.getFilter(1);
		auto findIteratorCivil = std::find_if(mGapingCivil.begin(), mGapingCivil.end(), [personEntityId](const qsf::WeakPtr<qsf::Entity>& entity) {
			return entity.valid() && entity->getId() == personEntityId;
		});

		auto findIteratorFleeing = std::find_if(mFleeingCivil.begin(), mFleeingCivil.end(), [personEntityId](const qsf::WeakPtr<qsf::Entity>& entity) {
			return entity.valid() && entity->getId() == personEntityId;
		});

		qsf::Entity* targetEntity = nullptr;

		if (findIteratorCivil != mGapingCivil.end())
		{
			if (findIteratorCivil->valid())
			{
				targetEntity = findIteratorCivil->get();
			}
		}

		if (findIteratorFleeing != mFleeingCivil.end())
		{
			if (findIteratorFleeing->valid())
			{
				targetEntity = findIteratorFleeing->get();
			}
		}

		if (nullptr != targetEntity)
		{
			// Is some of our civilians the move to despawn component
			targetEntity->destroyComponent<MoveToDespawnComponent>();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
