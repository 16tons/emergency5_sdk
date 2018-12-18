// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/TrainAccidentEvent.h"
#include "em5/action/base/FallDownHangingPersonAction.h"
#include "em5/action/tech/PickupPersonByFlyingHeliAction.h"
#include "em5/action/fire/DLK/DLKSavePersonAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/effect/HighlightComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/logic/observer/generic/MessageObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/UnhideBuriedObserver.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/cleanup/PersonCleanup.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/gui/hud/WarblerFeedWindow.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/Random.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/query/ComponentMapQuery.h>
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
	const std::string TAG_INJURED_PERSON = "EM2017_FP03_INJURED";
	const std::string TAG_HANGING_PERSON = "EM2017_FP03_HANGING";
	const std::string TAG_CUT_TRAIN = "EM2017_FP03_TRAIN";
	const std::string TAG_DEBRIS = "EM2017_FP03_DEBRIS";
	const std::string TAG_EBOX = "EM2017_FP03_EBOX";
	const std::string TAG_EBOX_PERSON = "EM2017_FP03_ELECTRICITY_INJURED";
	const std::string TAG_EBOX_EFFECT = "EM2017_FP03_ELECTRICITY";
	const std::string TAG_TWEET_CAMERA = "EM2017_MINI06_TWEET_01";
	const std::string TAG_TWEET_PERSON = "EM2017_MINI06_TWEET_01_GAPER";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TrainAccidentEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::TrainAccidentEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TrainAccidentEvent::TrainAccidentEvent() :
		mAccidentLayerId(qsf::getUninitialized<uint32>()),
		mEventLayer(nullptr),
		mEraseLayer(nullptr),
		mPersonHangingTime(qsf::Time::fromSeconds(180.f)),
		mDeadPersonsToFail(1),
		mFailLimitBurningObjects(20),
		mHangingPersonRescued(false),
		mEboxRepaired(false),
		mCanPlayPickupPersonTweet(true)
	{
		// Nothing here
	}

	TrainAccidentEvent::~TrainAccidentEvent()
	{
		// Nothing here
	}

	void TrainAccidentEvent::init(qsf::Layer* accidentLayer)
	{
		if (nullptr != accidentLayer)
		{
			mAccidentLayerId = accidentLayer->getId();
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool TrainAccidentEvent::onStartup()
	{
		if (qsf::isUninitialized(mAccidentLayerId))
			return false;

		qsf::Layer* mainLayer = getMap().getLayerManager().getLayerById(mAccidentLayerId);
		if (nullptr == mainLayer)
			return false;

		if (!getSubLayerFromMainLayer(*mainLayer))
			return false;

		// Start Objectives before the scene is setup, so we catch all burning objects for our fail conditions
		startObjectives();

		setupScene(*mainLayer);

		// Event is now running
		setRunning();

		return true;
	}

	void TrainAccidentEvent::onShutdown()
	{
		// Create a layer cleanup to clean up the rest
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer, mEraseLayer);
		}

		mEboxRepairedProxy.unregister();
		mBuriedFoundMessageProxy.unregister();
		mPersonDropedByHelicopterProxy.unregister();
		mHangingPersonSaveProxy.unregister();

		// Ensure everything from the event is cleaned up.
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer);
		}

		if (mTweetPersonEntity.valid())
		{
			mTweetPersonEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
		}

		// Ensure the environment layer is cleaned up
		MapHelper(getMap()).activateLayerByName("trains");
	}

	void TrainAccidentEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Register rescuing the hanging person
		if (mHangingPerson.valid())
		{
			mHangingPersonSaveProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_PERSON_ON_DLK, mHangingPerson->getId()), boost::bind(&TrainAccidentEvent::onHangingPersonSaved, this));
		}

		// Register repair action
		if (mEBoxEntity.valid())
		{
			mEboxRepairedProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_REPAIRED_OR_USED, mEBoxEntity->getId()), boost::bind(&TrainAccidentEvent::onEboxRepaired, this));
		}

		// Register find buried message proxy
		mBuriedFoundMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_RESCUE_DOG_FOUND_PERSON), boost::bind(&TrainAccidentEvent::onBuriedFound, this, _1));
	}

	bool TrainAccidentEvent::onFailure(EventResult& eventResult)
	{
		// Incase hanging person was not saved. fall down now
		if (!mHangingPersonRescued && mPersonHangingTime > qsf::Time::ZERO)
		{
			startFallingHangingPerson();
		}

		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (!mHangingPersonRescued && mPersonHangingTime <= qsf::Time::ZERO)
		{
			setFailHintsForMiniEvent(eventResult, "EM2017_MINI06_FAIL01", "EM2017_MINI06_SUPERV_FAIL01");
		}
		else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_BURNING_OBJECTANDBUILDING)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE02_FAIL06", "EM5_GE05_SUPERV_FAIL04");
		}
		else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_INJUREDSQUAD)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE07_FAIL05", "EM5_GE01_SUPERV_FAIL01");
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

	bool TrainAccidentEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI06_SUPERV_END01");
		}

		return true;
	}

	void TrainAccidentEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Update hanging person timer
		if (!mHangingPersonRescued)
		{
			mPersonHangingTime -= timePassed;
			if (mPersonHangingTime <= qsf::Time::ZERO)
			{
				if (checkForDLKActionInProgress())
				{
					// Action is in progress, so this fail condition can't be hit anymore
					mHangingPersonRescued = true;
				}
				else
				{
					QSF_LOG_PRINTS(DEBUG, "Train accident event failed: Hanging person died");
					// Person gets killed and so the event is finished with some seconds delay
					startFallingHangingPerson();

					// End the event
					setFinished(false);
				}
			}
		}

		checkForSelectedTechHeli();
		checkForTrackPersonTweet();

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool TrainAccidentEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* TrainAccidentEvent::getFocusEntity()
	{
		return getRandomEventEntity();
	}

	bool TrainAccidentEvent::checkEventConfiguration()
	{
		qsf::Layer* mainLayer = getMap().getLayerManager().getLayerById(mAccidentLayerId);
		if (nullptr == mainLayer)
			return false;

		if (!getSubLayerFromMainLayer(*mainLayer))
			return false;

		// Don't start the same event location twice
		if (!mEraseLayer->isActive())
			return false;

		if (mEventLayer->isActive())
			return false;

		return true;
	}

	void TrainAccidentEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);
	}

	void TrainAccidentEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(mk) implement function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TrainAccidentEvent::startObjectives()
	{
		// Set start hint observers after the event entity was added
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();

		// Create the need condition
		Objective& needConditionRepairEBox = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIRTRACKELECTRIC);
		needConditionRepairEBox.setNeededNumber(1); // Hard scripted value

		// Fail condition, no one may die.
		ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(mDeadPersonsToFail);
		ObjectiveHelper(*this).addBurningObjectsAndBuildingsFailCondition(mFailLimitBurningObjects);
		ObjectiveHelper(*this).addInjuredUnitPersonsFailCondition(1);
	}

	bool TrainAccidentEvent::getSubLayerFromMainLayer(qsf::Layer& mainLayer)
	{
		const std::vector<qsf::Layer*>& subLayers = mainLayer.getLayers();
		for (size_t i = 0; i < subLayers.size(); ++i)
		{
			if (subLayers[i]->getName().find("_erase") != std::string::npos)
			{
				mEraseLayer = subLayers[i];
			}
			else if (subLayers[i]->getName().find("_add") != std::string::npos)
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

	void TrainAccidentEvent::setupScene(qsf::Layer& mainLayer)
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(*mEventLayer, false);

		// Show the accident
		MapHelper::activateLayer(*mEventLayer, true);

		// Hide the non important environment
		MapHelper::activateLayer(*mEraseLayer, false);

		// Hide the "trains" layer to avoid clashes with environment trains
		MapHelper(getMap()).deactivateLayerByName("trains", false);

		EventHelper eventHelper(*this);
		ObjectiveHelper objectiveHelper(*this);

		// Setup injured persons
		qsf::Layer::EntityIdSet entities = mEventLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.isRoadVehicle())
				{
					if (targetEntityHelper.checkEntityHasEventTag(TAG_CUT_TRAIN) && !targetEntityHelper.isContainerEmpty())
					{
						// For the right objective texts, create the objective and change the text before we update the gui
						Objective* getPersonsOutOfCar = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_GETPERSONOUTOFCAR);
						if (nullptr != getPersonsOutOfCar)
						{
							getPersonsOutOfCar->setText("EM2017_MINI06_OBJ09");
						}

						RoadVehicleComponent& roadVehicleComponent = targetEntity->getComponentSafe<RoadVehicleComponent>();
						roadVehicleComponent.trapPersons(this);
						roadVehicleComponent.setCanBeCut(true, this);
						roadVehicleComponent.setCanSpawnDriver(false);
					}
				}
				else if (targetEntityHelper.isCivilPerson())
				{
					if (targetEntityHelper.checkEntityHasEventTag(TAG_INJURED_PERSON))
					{
						HealthHelper(*targetEntity).injurePersonByInjuryAtStartup(this);
					}
					else if (targetEntityHelper.checkEntityHasEventTag(TAG_HANGING_PERSON))
					{
						mHangingPerson = targetEntity;

						EventIdComponent::addToEvent(*targetEntity, *this, eventspreadreason::EVENT_SPECIFIC);

						// Add minimap icon by hand
						MinimapHelper::registerIcon(*targetEntity, MiniMapIconType::VICTIM_RESCUE_DLK);
						objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONHANGONTRAINTRACK).increaseNeededNumber(targetEntity);
					}
					else if (targetEntityHelper.checkEntityHasEventTag(TAG_EBOX_PERSON))
					{
						mEBoxPersons.push_back(targetEntity);

						// This persons should be carried by helicopter after the Ebox is repaired. Give them the CallForRescueComponent here but turn of the ability to take off with heli
						CallForRescueComponent& callForRescueComponent = targetEntity->getOrCreateComponentSafe<CallForRescueComponent>();
						callForRescueComponent.setCanRescuedByHelicopter(false);

						glm::vec3 positionOnTrainTrack = targetEntityHelper.getPosition();
						HealthHelper(*targetEntity).injurePersonByInjuryAtStartup(this, injury::ELECTRIC_SHOCK);
						// Because the persons glitch through the train track while they get injured, remove them manually back to the old position
						targetEntity->destroyComponent<PersonPlacementComponent>();
						targetEntityHelper.setPosition(positionOnTrainTrack);

						objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEPERSONSFROMTRAINTRACK).increaseNeededNumber(targetEntity);
					}
					else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_PERSON))
					{
						mTweetPersonEntity = targetEntity;
						eventHelper.makeTweetPerson(*targetEntity, true);
					}
				}
				else if (nullptr != targetEntity->getComponent<BuryComponent>())
				{
					// For the right objective texts, create the objective and change the text before we update the gui
					Objective& buriedPersonObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON);
					buriedPersonObjective.setText("EM2017_ME202_OBJ03");

					// Setup persons under debris
					BuryComponent& buryComponent = targetEntity->getComponentSafe<BuryComponent>();
					for (auto buryEntityId : buryComponent.BuriedEntitiesArray.getVariable())
					{
						qsf::Entity* buriedEntity = getMap().getEntityById(buryEntityId);
						if (buriedEntity != nullptr)
						{
							buryComponent.buryEntity(*buriedEntity);

							static const uint32 injuryPool[3] = { injury::BROKEN_SKULL, injury::INNER_BLEEDING, injury::HEAD_BODY_LACERATION };
							const qsf::StringHash& randomInjury = injuryPool[qsf::Random::getRandomUint(0, 2)];
							eventHelper.makeMissingPerson(*buriedEntity, randomInjury, false);
						}
					}

					// Show the debris
					targetEntity->setActive(true);

					// If it's a broken tree, register it for the event -- this adds an objective and observers
					if (buryComponent.getCutableBySaw())
					{
						EventIdComponent::addToEvent(*targetEntity, *this, eventspreadreason::BROKEN_TREE);
					}
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_EBOX))
				{
					mEBoxEntity = targetEntity;

					// TODO(mk) This is code is copied from DefectEboxEvent::onStartup()
					// Initialize defect ebox
					{
						EboxComponent& eboxComponent = mEBoxEntity->getOrCreateComponentSafe<EboxComponent>();
						eboxComponent.setEboxDefective();

						// Add component for using with engineer
						UsableByEngineerComponent& usableByEngineerComponent = mEBoxEntity->getOrCreateComponentSafe<UsableByEngineerComponent>();
						usableByEngineerComponent.initWithUseTime(qsf::Time::fromSeconds(10.0f));

						// Add to event
						EventIdComponent::addToEvent(mEBoxEntity.getSafe(), *this, eventspreadreason::EVENT_SPECIFIC);
					}
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_EBOX_EFFECT))
				{
					// Only top-level entities, as the children have the same tag in some event locations edited as well
					if (nullptr == targetEntityHelper.getParentEntity())
					{
						QSF_CHECK(!mEBoxEffect.valid(), "Multiple ebox effect entities found", QSF_REACT_NONE);
						mEBoxEffect = targetEntity;
					}
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_CAMERA))
				{
					mTweetCameraEntity = targetEntity;
				}
			}
		}

		// Not all important objects are in our event layer (eg. houses in baselayer, they are used in multiple events and can't move in specific event folder)
		const std::string TAG_FIRE = mainLayer.getName() + "_FIRE";
		const std::vector<qsf::game::EventTagComponent*>& startFireComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_FIRE), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* startFireComponent : startFireComponents)
		{
			EntityHelper(startFireComponent->getEntity()).startFire(this);
		}

		const std::string TAG_DESTROY = mainLayer.getName() + "_DESTROY";
		const std::vector<qsf::game::EventTagComponent*>& destroyedComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_DESTROY), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* destroyedComponent : destroyedComponents)
		{
			// Only support buildings
			BuildingComponent* buildingComponent = destroyedComponent->getEntity().getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				buildingComponent->setDestroyedWithoutAnimation(this);
			}
		}
	}

	bool TrainAccidentEvent::checkForDLKActionInProgress()
	{
		if (!mHangingPerson.valid())
			return false;	// Error, person is not valid

		for (CommandableComponent* commandableComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<CommandableComponent>())
		{
			if (commandableComponent->isFireFighterDLK())
			{
				qsf::ActionComponent& actionComponent = commandableComponent->getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				DLKSavePersonAction* dlkSavePersonAction = actionComponent.getAction<DLKSavePersonAction>();
				if (nullptr != dlkSavePersonAction && dlkSavePersonAction->getTargetEntityId() == mHangingPerson->getId())
				{
					return true;
				}
			}
		}
		return false;
	}

	void TrainAccidentEvent::checkForSelectedTechHeli()
	{
		// If the tech heli is selected and we select injured persons on the train track (eboxPersons), we play a hint to inform the player for invalid unit orders
		if (mEboxRepaired)
			return;

		bool noHeliInSelection = true;

		const qsf::Map& map = getMap();
		for (uint64 selectedEntityId : EM5_GAME.getSelectionManager().getSelectedIdSet())
		{
			qsf::Entity* selectedEntity = map.getEntityById(selectedEntityId);
			if (nullptr == selectedEntity)
				continue;

			CommandableComponent* commandableComponent = selectedEntity->getComponent<CommandableComponent>();

			if (nullptr != commandableComponent && commandableComponent->isTechHeli())
			{
				noHeliInSelection = false;
				break;
			}
		}

		if (noHeliInSelection)
			return;

		for (qsf::WeakPtr<qsf::Entity>& eboxPerson : mEBoxPersons)
		{
			if (!eboxPerson.valid())
				continue;

			HighlightComponent* highlightComponent = eboxPerson->getComponent<HighlightComponent>();
			if (nullptr != highlightComponent && highlightComponent->getHighlightFlags(HighlightComponent::HighlightFlags::FLAG_SELECTABLE))
			{
				HintHelper::showHint("EM2017_FP03_EBOX_HINT01", getPlayerIndex(), qsf::Time::fromSeconds(15.0f));
			}
		}
	}

	void TrainAccidentEvent::checkForTrackPersonTweet()
	{
		if (mCanPlayPickupPersonTweet && mTweetCameraEntity.valid() && isMiniEvent())
		{
			for (CommandableComponent* commandableComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<CommandableComponent>())
			{
				if (commandableComponent->isTechHeli())
				{
					qsf::ActionComponent& actionComponent = commandableComponent->getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
					PickupPersonByFlyingHeliAction* pickupPersonByFlyingHeliAction = actionComponent.getAction<PickupPersonByFlyingHeliAction>();
					if (nullptr != pickupPersonByFlyingHeliAction && pickupPersonByFlyingHeliAction->isStrecherWaitForLoading())
					{
						// Check if a person from traintrack is touched
						uint64 entityId = pickupPersonByFlyingHeliAction->getTargetId();
						for (qsf::WeakPtr<qsf::Entity>& eboxPerson : mEBoxPersons)
						{
							if (!eboxPerson.valid())
								continue;

							if (eboxPerson->getId() == entityId)
							{
								EM5_GUI.getIngameHud().getOrCreateWarblerFeedWindow().addWarblerTweet("EM2017_MINI06_TWEET_01", mTweetCameraEntity->getId());
								mCanPlayPickupPersonTweet = false;
								return;
							}
						}

					}
				}
			}
		}
	}

	void TrainAccidentEvent::onHangingPersonSaved()
	{
		mHangingPersonRescued = true;
		mHangingPersonSaveProxy.unregister();

		showSupervisorMessage("EM2017_MINI06_OBJECTIVE05");

		if (mHangingPerson.valid())
		{
			// Delete minimap icon
			MinimapHelper::unregisterIcon(*mHangingPerson);

			// Currently the person is on the dlk, some seconds later we drop the person on the ground
			mHangingPersonSaveProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_TOOKOUTOFWINDOW_PERSON, mHangingPerson->getId()), boost::bind(&TrainAccidentEvent::onHangingPersonSavedPlacedFromDLK, this));
		}
	}

	void TrainAccidentEvent::onHangingPersonSavedPlacedFromDLK()
	{
		mHangingPersonSaveProxy.unregister();

		if (mHangingPerson.valid())
		{
			HealthHelper(*mHangingPerson).injurePersonByInjuryAtStartup(this, injury::WEAKNESS, false);

			// Finish objective (only support one hanging person)
			ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONHANGONTRAINTRACK).increaseCurrentNumber(mHangingPerson.get());
		}
	}

	void TrainAccidentEvent::onEboxRepaired()
	{
		mEboxRepaired = true;
		mEboxRepairedProxy.unregister();

		Objective& rescuePersonFromTrainTrackObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEPERSONSFROMTRAINTRACK);
		for (const qsf::WeakPtr<qsf::Entity>& eboxPerson : mEBoxPersons)
		{
			if (eboxPerson.valid())
			{
				// Make persons on track rescuable by helicopter
				CallForRescueComponent& callForRescueComponent = eboxPerson->getOrCreateComponentSafe<CallForRescueComponent>();
				callForRescueComponent.setCanRescuedByHelicopter(true);

				// Register the person to a objective
				rescuePersonFromTrainTrackObjective.increaseNeededNumber(eboxPerson.get());

				// Handle minimap icon
				MinimapHelper::unregisterIcon(*eboxPerson);
				MinimapHelper::registerIcon(*eboxPerson, MiniMapIconType::VICTIM_RESCUE_HELI);
			}
		}

		showSupervisorMessage("EM2017_MINI06_OBJECTIVE08");

		// Turn off the effect
		showSparkleEffect(false);

		// Complete objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_REPAIRTRACKELECTRIC).setAccomplished();

		// To fix minimap icons for injured persons
		mPersonDropedByHelicopterProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PLACE_PERSON_FROM_FLYING_HELI_ACTION), boost::bind(&TrainAccidentEvent::onDropedPersonsFromHeli, this, _1));
	}

	void TrainAccidentEvent::onBuriedFound(const qsf::MessageParameters& parameters)
	{
		uint64 foundEntityId = parameters.getFilter(1);
		qsf::Entity* personEntity = getMap().getEntityById(foundEntityId);
		if (nullptr == personEntity)
			return;

		qsf::LinkComponent* linkComponent = personEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && nullptr != linkComponent->getComponentFromEntityOrLinkedParent<BuryComponent>())
		{
			// Injure the person
			// Add injury objectives
			EventSpreadHelper::setEventIdComponent(this, *personEntity, eventspreadreason::INJURY);

			// Add objectives
			Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2);
			rescuePeople.increaseNeededNumber(personEntity);

			UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(personEntity->getId());
			unhideBuriedObserver.connectToObjective(rescuePeople);
		}
	}

	void TrainAccidentEvent::onDropedPersonsFromHeli(const qsf::MessageParameters& parameters)
	{
		uint64 dropedEntityId = parameters.getFilter(1);
		qsf::Entity* personEntity = getMap().getEntityById(dropedEntityId);
		if (nullptr == personEntity)
			return;

		for (const qsf::WeakPtr<qsf::Entity>& eboxPerson : mEBoxPersons)
		{
			if (!eboxPerson.valid())
				continue;

			if (dropedEntityId == eboxPerson->getId())
			{
				HealthComponent* healthComponent = personEntity->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					// Reregister the injured icon
					healthComponent->updateInjuredIcon();
				}

				ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEPERSONSFROMTRAINTRACK).increaseCurrentNumber(personEntity);
			}
		}
	}

	void TrainAccidentEvent::showSparkleEffect(bool active)
	{
		if (!mEBoxEffect.valid())
			return;

		qsf::LinkComponent* linkComponent = mEBoxEffect->getComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			for (qsf::LinkComponent* linkedEntityComponent : linkComponent->getChildLinks())
			{
				linkedEntityComponent->getEntity().setActive(active);
			}
		}
	}

	void TrainAccidentEvent::startFallingHangingPerson()
	{
		if (mHangingPerson.valid())
		{
			// Nobody came to rescue you, harhar
			mHangingPerson->destroyComponent<CallForRescueComponent>();

			mHangingPerson->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<FallDownHangingPersonAction>().init(true);

			// Trick, to avoid cleanup this person by the layercleanup in case the event fails, push this entity in an other layer
			mHangingPerson->getOrCreateComponentSafe<qsf::MetadataComponent>().setLayerId(1);

			// Because we changed the cleanup logic manually, we have to add an extra solution
			mHangingPerson->destroyComponent<EventIdComponent>();
			mHangingPerson->getOrCreateComponentSafe<qsf::GameLogicComponent>().getOrCreateGameLogic<PersonCleanup>();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
