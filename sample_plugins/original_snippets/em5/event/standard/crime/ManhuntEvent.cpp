// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/ManhuntEvent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/event/helper/WarblerTweetHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/math/Random.h>
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
	const std::string TAG_ATTESTOR = "EM2017_FP05_ATTESTOR";
	const std::string TAG_HOUSE	= "EM2017_FP05_HOUSE";
	const std::string TAG_MINIEVENT_TARGET = "EM2017_MINI03_HOUSE";
	const std::string TAG_TWEET_CAMERA = "EM2017_MINI03_TWEET_01";

	// Texts
	const std::string TAG_OBJECTIVE_TEXT= "EM2017_ME204_OBJ03";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ManhuntEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::ManhuntEvent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	const std::string ManhuntEvent::getHouseEventagFromEventLayerName(const std::string& layerName)
	{
		return TAG_HOUSE + layerName.substr(layerName.find_last_of("_"), layerName.size());
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ManhuntEvent::ManhuntEvent() :
		mEventLayer(nullptr),
		mEventSceneryLayer(nullptr),
		mEventEraseLayer(nullptr),
		mCanSetupGangsterHouses(true),
		mFoundRightHouse(false),
		mNumberAttackedFalseHouses(0),
		mTimeTillEventLose(qsf::Time::fromSeconds(180.f)),
		mWarblerTweetHelper(*new WarblerTweetHelper()),
		mCanTriggerHint02(true)
	{
		// Nothing here
	}

	ManhuntEvent::~ManhuntEvent()
	{
		delete &mWarblerTweetHelper;
	}

	void ManhuntEvent::init(qsf::Layer& eventLayer)
	{
		mEventLayer = &eventLayer;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool ManhuntEvent::onStartup()
	{
		if (nullptr == mEventLayer)
			return false;

		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void ManhuntEvent::onShutdown()
	{
		// Create a layer cleanup to clean up the rest
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer);
		}

		if (nullptr != mEventSceneryLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventSceneryLayer, mEventEraseLayer);
		}

		// Remove all fake buildings
		while (!mFalseHouses.empty())
		{
			removeRandomFakeBuilding();
		}

		removeFakeBuildingIcons(*mGangsterHouse);

		// Remove all talking marker
		for (const qsf::WeakPtr<qsf::Entity>& attestor : mAttestors)
		{
			if (!attestor.valid())
				continue;

			attestor->destroyComponent<SpeakToPersonComponent>();
		}

		mSpeakToPersonMessageProxy.unregister();
		mAttackBuildingMessageProxy.unregister();
		mAttackBuildingFakeMessageProxy.unregister();
	}

	void ManhuntEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();

		mSpeakToPersonMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_SPEAK_TO_PERSON), boost::bind(&ManhuntEvent::onSpeakToPerson, this, _1));
		mAttackBuildingMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_BUILDING_FINISH), boost::bind(&ManhuntEvent::onAttackBuilding, this, _1));
		mAttackBuildingFakeMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_FAKEBUILDING_FINISH), boost::bind(&ManhuntEvent::onAttackBuilding, this, _1));
	}

	bool ManhuntEvent::onFailure(EventResult& eventResult)
	{
		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (mNumberAttackedFalseHouses > 1)
		{
			setFailHintsForMiniEvent(eventResult, "EM2017_MINI03_FAIL01", "EM2017_MINI03_SUPERV_FAIL01");
		}
		else if (mTimeTillEventLose <= qsf::Time::ZERO)
		{
			setFailHintsForMiniEvent(eventResult, "EM2017_MINI03_FAIL02", "EM2017_MINI03_SUPERV_FAIL02");
		}
		else
		{
			// Call base implementation
			return FreeplayEvent::onFailure(eventResult);
		}

		return true;
	}

	bool ManhuntEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI03_SUPERV_END01");
		}

		return true;
	}

	void ManhuntEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();

		if (mNumberAttackedFalseHouses > 1)
		{
			// Abort event
			QSF_LOG_PRINTS(DEBUG, "Manhunt event failed: Attacked to many wrong houses");
			setFinished(false);
		}

		if (!mFoundRightHouse)
		{
			// Check if building is attacked
			BuildingComponent* buildingComponent = mGangsterHouse->getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				for (auto iterator : buildingComponent->getLinks())
				{
					qsf::Entity& linkedEntity = (*iterator.second).getTargetEntity();
					if (UnitHelper(linkedEntity).isPoliceSEKPerson())
					{
						// All policemen inside the building has the same state, checking only one is enough
						mFoundRightHouse = true;
						break;
					}
				}
			}

			mTimeTillEventLose -= timePassed;
			if (mTimeTillEventLose <= qsf::Time::ZERO && !mFoundRightHouse)
			{
				// Abort event
				QSF_LOG_PRINTS(DEBUG, "Manhunt event failed: Event time was over");
				setFinished(false);
			}
		}
	}

	bool ManhuntEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* ManhuntEvent::getFocusEntity()
	{
		return getRandomEventEntity();
	}

	bool ManhuntEvent::checkEventConfiguration()
	{
		if (nullptr == mEventLayer)
			return false;

		// Don't start the same event location twice
		if (mEventLayer->isActive())
			return false;

		return true;
	}

	void ManhuntEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);
	}

	void ManhuntEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(mk) implement function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ManhuntEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(1);
		ObjectiveHelper(*this).addBurningObjectsFailCondition(20);
	}

	void ManhuntEvent::setupScene()
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(*mEventLayer, false);

		// Show the accident
		MapHelper::activateLayer(*mEventLayer, true);

		ObjectiveHelper objectiveHelper(*this);

		qsf::Layer::EntityIdSet entities = mEventLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.checkEntityHasEventTag(TAG_ATTESTOR))
				{
					mAttestors.push_back(targetEntity);

					SpeakToPersonComponent& speakToPersonComponent = targetEntity->getOrCreateComponentSafe<SpeakToPersonComponent>();
					speakToPersonComponent.updateIcons();

					objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TALK_TO_PERSON).increaseNeededNumber(targetEntity);
					EventIdComponent::addToEvent(*targetEntity, *this, eventspreadreason::EVENT_SPECIFIC);
				}
			}
		}

		// Not all important objects are in our event layer (eg. houses in baselayer, they are used in multiple events and can't move in specific event folder)
		const std::string TAG_HOUSE_REAL = getHouseEventagFromEventLayerName(mEventLayer->getName());
		const std::vector<qsf::game::EventTagComponent*>& housesComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_HOUSE_REAL), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* houseComponent : housesComponents)
		{
			qsf::Entity& houseEntity = houseComponent->getEntity();
			if (houseEntity.getComponent<BuildingComponent>())
			{
				mFalseHouses.push_back(houseEntity);
			}
		}

		if (isMiniEvent())
		{
			// For minievent choose one pre edited target
			std::vector<qsf::WeakPtr<qsf::Entity>> newFalseHouses;
			for (qsf::WeakPtr<qsf::Entity>& falseHouse : mFalseHouses)
			{
				if (EntityHelper(falseHouse.getSafe()).checkEntityHasEventTag(TAG_MINIEVENT_TARGET))
				{
					mGangsterHouse = falseHouse;
				}
				else
				{
					newFalseHouses.push_back(falseHouse);
				}
			}
			mFalseHouses = newFalseHouses;

			mEventSceneryLayer = mEventLayer->getLayerManager().getLayerByName("MINI03_scenery");
			if (nullptr != mEventSceneryLayer)
			{
				MapHelper::activateLayer(*mEventSceneryLayer, false);
				MapHelper::activateLayer(*mEventSceneryLayer, true);
			}

			// We have to deactivate the "worldevent_erase" layer (the skateboard park)
			mEventEraseLayer = mEventLayer->getLayerManager().getLayerByName("worldevent_erase");
			if (nullptr != mEventEraseLayer)
			{
				MapHelper::activateLayer(*mEventEraseLayer, false);
			}
		}

		if (!mGangsterHouse.valid())
		{
			// One random house is the gangster house
			mGangsterHouse = qsf::Random::getPickOutAnyOf(mFalseHouses);
		}
	}

	void ManhuntEvent::setupGangsterHouses()
	{
		if (mCanSetupGangsterHouses)
		{
			mCanSetupGangsterHouses = false;

			// Setup houses after we have defined a gangster house
			for (const qsf::WeakPtr<qsf::Entity>& houseEntity : mFalseHouses)
			{
				setupGangsterHouse(*houseEntity, true);
			}

			setupGangsterHouse(*mGangsterHouse, false);
		}
	}

	void ManhuntEvent::setupGangsterHouse(qsf::Entity& houseEntity, bool isFakeObject)
	{
		qsf::GameLogicComponent& gameLogicComponent = houseEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
		GangsterInBuildingLogic& gangsterInBuildingLogic = gameLogicComponent.getOrCreateGameLogicSafe<GangsterInBuildingLogic>();
		gangsterInBuildingLogic.setAllowNormalPolice(false);
		gangsterInBuildingLogic.setCanSpeakWithHostageTaker(false);
		gangsterInBuildingLogic.setFakeObject(isFakeObject);

		EventIdComponent::addToEvent(houseEntity, *this, eventspreadreason::EVENT_SPECIFIC);

		if (!isFakeObject)
		{
			BuildingComponent* buildingComponent = houseEntity.getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				buildingComponent->spawnGangsterInside(3, 0, 0);

				// Remove status / minimap icons from gangster house (we want to display a search icon instead)
				MinimapHelper::unregisterIcon(houseEntity);
				StatusOverlayComponent::hideIcon(houseEntity, StatusOverlayComponent::STATUS_ICON_GANGSTER);
			}
		}

		StatusOverlayComponent::showIcon(houseEntity, StatusOverlayComponent::STATUS_ICON_SEARCH_VARIOUS);
	}

	void ManhuntEvent::removeRandomFakeBuilding()
	{
		if (mFalseHouses.empty())
			return;

		// Remove the marked buildings from list
		qsf::WeakPtr<qsf::Entity> nonTargetHouse = qsf::Random::getPickOutAnyOf(mFalseHouses);
		if (nonTargetHouse.valid())
		{
			removeFakeBuildingIcons(*nonTargetHouse);
		}

		checkFalseHouseEmpty();
	}

	void ManhuntEvent::removeFakeBuildingIcons(qsf::Entity& houseEntity)
	{
		// To avoid having the old target as result of the getFocusEntity()
		houseEntity.destroyComponent<EventIdComponent>();

		qsf::GameLogicComponent& gameLogicComponent = houseEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
		GangsterInBuildingLogic& gangsterInBuildingLogic = gameLogicComponent.getOrCreateGameLogicSafe<GangsterInBuildingLogic>();
		gameLogicComponent.deleteGameLogic(gangsterInBuildingLogic);

		MinimapHelper::unregisterIcon(houseEntity);
		StatusOverlayComponent::hideIcon(houseEntity, StatusOverlayComponent::STATUS_ICON_SEARCH_VARIOUS);
		StatusOverlayComponent::hideIcon(houseEntity, StatusOverlayComponent::STATUS_ICON_GANGSTER);
	}

	void ManhuntEvent::checkFalseHouseEmpty()
	{
		if (!mFalseHouses.empty())
			return;

		if (getState() != FreeplayEvent::STATE_RUNNING)
			return;	// Don`t call this function in event shutdown

		showHint("EM2017_MINI03_HINT01", false);

		MinimapHelper::unregisterIcon(mGangsterHouse.getSafe());
		StatusOverlayComponent::hideIcon(mGangsterHouse.getSafe(), StatusOverlayComponent::STATUS_ICON_SEARCH_VARIOUS);

		// Give gangster icon back, manually
		EM5_GAME.getMiniMapIconRegistrationLogic().registerGangsterIcon(mGangsterHouse->getId());
		StatusOverlayComponent::showIcon(mGangsterHouse.getSafe(), StatusOverlayComponent::STATUS_ICON_GANGSTER);

		// Send minimap ping
		MinimapHelper::registerPingByEntity(*mGangsterHouse);
	}

	void ManhuntEvent::onSpeakToPerson(const qsf::MessageParameters& parameters)
	{
		uint64 personId = qsf::getUninitialized<uint64>();
		parameters.getParameter("PersonId", personId);

		// Make sure the person belongs to this event
		bool personFromOtherEvent = true;
		for (const qsf::WeakPtr<qsf::Entity>& attestor : mAttestors)
		{
			if (!attestor.valid())
				continue;

			if (attestor->getId() == personId)
			{
				personFromOtherEvent = false;
				break;
			}
		}

		if (personFromOtherEvent)
			return;


		if (mCanSetupGangsterHouses)
		{
			showHint("EM2017_MINI03_HINT03", false);

			setupGangsterHouses();
		}
		else
		{
			removeRandomFakeBuilding();
		}

		qsf::Entity* personEntity = getMap().getEntityById(personId);
		if (personEntity != nullptr)
		{
			// To avoid having the old target as result of the getFocusEntity()
			personEntity->destroyComponent<EventIdComponent>();

			ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TALK_TO_PERSON).increaseCurrentNumber(personEntity);
		}
	}

	void ManhuntEvent::onAttackBuilding(const qsf::MessageParameters& parameters)
	{
		uint64 buildingId = qsf::getUninitialized<uint64>();
		parameters.getParameter("BuildingId", buildingId);
		qsf::Entity* buildingEntity = getMap().getEntityById(buildingId);
		if (buildingEntity == nullptr)
			return;

		// We receive the message for every policeman leaving the building (normally 3 times per building). Count every house only once.
		for (uint64 attackedBuildingId : mAttackedBuildings)
		{
			if (attackedBuildingId == buildingId)
			{
				// We already attacked this house, don't increase counter
				return;
			}
		}
		mAttackedBuildings.push_back(buildingId);

		if (buildingId == mGangsterHouse->getId())
		{
			mFoundRightHouse = true;

			// Call this function manually to delete all event details
			ManhuntEvent::onShutdown();

			if (isMiniEvent())
			{
				mWarblerTweetHelper.initialize("EM2017_MINI03_TWEET_01", *this, TAG_TWEET_CAMERA, "");
				mWarblerTweetHelper.tweetDelayed(qsf::Time::fromSeconds(2.0f));
			}

			// Show hint for accomplished objective
			showSupervisorMessage("EM2017_MINI03_SUPERV_OBJ02");

			// Finish the talking objective
			ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TALK_TO_PERSON).setAccomplished();

			// Switch the objective text
			ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS).setText(TAG_OBJECTIVE_TEXT);
		}
		else
		{
			if (mCanTriggerHint02)
			{
				mCanTriggerHint02 = false;

				// Show hint for wrong house
				showHint("EM2017_MINI03_HINT02", false);
			}

			// Removed this concrete house from the fake houses to avoid "chossing" this house again by random remove
			for (auto it = mFalseHouses.begin(); it != mFalseHouses.end(); it++)
			{
				if (!it->valid())
					continue;

				if ((*it)->getId() != buildingId)
					continue;

				// Removed this h
				mFalseHouses.erase(it);
				break;
			}

			removeFakeBuildingIcons(*buildingEntity);
			checkFalseHouseEmpty();

			// Disable a random attestor
			qsf::WeakPtr<qsf::Entity> attestorEntity = qsf::Random::getAnyOf(mAttestors);
			if (attestorEntity.valid())
			{
				attestorEntity->destroyComponent<SpeakToPersonComponent>();
				ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TALK_TO_PERSON).increaseCurrentNumber(attestorEntity.get());
			}

			// Increase the fail counter
			++mNumberAttackedFalseHouses;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
