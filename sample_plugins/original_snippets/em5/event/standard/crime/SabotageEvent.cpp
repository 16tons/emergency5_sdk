// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/SabotageEvent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MoverType.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/component/person/GaperComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/observer/UnhideBuriedObserver.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/Game.h"
#include "em5/gui/hud/WarblerFeedWindow.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_logic/targetpoint/TargetPoint.h>

#include <qsf_ai/decision/reactions/ReactionComponent.h>
#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/perception/SingleSensorComponent.h>

#include <qsf_game/component/event/EventTagHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
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
	const std::string TAG_INJURED = "EM2017_FP06_INJURED";
	const std::string TAG_GANGSTER_FLEE = "EM2017_GANGSTER_FLEE";
	const std::string TAG_DEBRIS = "EM2017_FP06_DEBRIS";
	const std::string TAG_CIVIL_ESCAPE	= "EM2017_FP06_ESCAPE";
	const std::string TAG_BURNING		= "EM2017_FP06_FIRE";
	const std::string TAG_TWEET_CAMERA	= "EM2017_MINI07_TWEET_01";
	const std::string TAG_TWEET_PERSON	= "EM2017_MINI07_TWEET_01_GAPER";
	const std::string TAG_TWEET_DEBRIS	= "EM2017_FP06_DEBRIS_TWEET";
	const std::string TAG_RUNNING_START	= "EM2017_FP06_ESCAPE";
	const std::string TAG_RUNNING_END	= "EM2017_FP06_ESCAPE_END";
	const std::string TAG_STAGE			= "EM2017_FP06_STAGE";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SabotageEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::SabotageEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SabotageEvent::SabotageEvent() :
		mEventLayerId(qsf::getUninitialized<uint32>()),
		mEventLayer(nullptr),
		mEraseLayer(nullptr),
		mGangsterLogicName("em5::GangsterCivilUnarmedLogic"),
		mIsGangsterHidden(false),
		mCanTweet(false),
		mHasTweeted(false),
		mTimeTillTweet(qsf::Time::fromMilliseconds(100)),
		mNumberPersonsTillTweet(0)
	{
		// Nothing here
	}

	SabotageEvent::~SabotageEvent()
	{
		// Nothing here
	}

	void SabotageEvent::init(qsf::Layer& eventLayer)
	{
		mEventLayerId = eventLayer.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool SabotageEvent::onStartup()
	{
		if (qsf::isUninitialized(mEventLayerId))
			return false;

		qsf::Layer* mainLayer = getMap().getLayerManager().getLayerById(mEventLayerId);
		if (nullptr == mainLayer)
			return false;

		if (!getSubLayerFromMainLayer(*mainLayer))
			return false;

		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void SabotageEvent::onShutdown()
	{
		mGangsterFoundMessageProxy.unregister();
		mBuriedFoundMessageProxy.unregister();

		// Ensure everything from the event is cleaned up
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer);
		}

		if (mTweetPersonEntity.valid())
		{
			mTweetPersonEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
		}

		if (nullptr != mEraseLayer)
		{
			MapHelper::activateLayer(*mEraseLayer, true);
		}

		// Reactivate sensor
		for (qsf::ai::SingleSensorComponent* singleSensorComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<qsf::ai::SingleSensorComponent>())
		{
			singleSensorComponent->setActive(true);
			singleSensorComponent->setAutomaticUpdate(true);
		}
		for (qsf::ai::ReactionComponent* reactionComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<qsf::ai::ReactionComponent>())
		{
			reactionComponent->setActive(true);
		}
	}

	void SabotageEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();

		// Register find buried message proxy
		mBuriedFoundMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_RESCUE_DOG_FOUND_PERSON), boost::bind(&SabotageEvent::onBuriedFound, this, _1));
	}

	bool SabotageEvent::onFailure(EventResult& eventResult)
	{
		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_BURNING_OBJECTS)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE02_FAIL06", "EM5_GE05_SUPERV_FAIL04");
		}
		else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE08_FAIL01", "EM5_GE01_SUPERV_FAIL01");
		}
		else if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_DEADSQUADPERSONS)
		{
			setFailHintsForMiniEvent(eventResult, "EM5_GE14_FAIL02", "EM5_GE01_SUPERV_FAIL02");
		}
		else
		{
			// Call base implementation
			return FreeplayEvent::onFailure(eventResult);
		}

		return true;
	}

	bool SabotageEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI07_SUPERV_END01");
		}

		return true;
	}

	void SabotageEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// Check objectives for success or failure
		checkObjectivesState();

		if (mCanTweet && !mHasTweeted && isMiniEvent())
		{
			// TODO(mk) If the Warblertweet window gets improved and is able to take valid screenshots without extra updating, remove this code
			mTimeTillTweet -= timePassed;
			if (mTimeTillTweet <= qsf::Time::ZERO)
			{
				mHasTweeted = true;

				// Make picture after we unhide the target
				EM5_GUI.getIngameHud().getOrCreateWarblerFeedWindow().addWarblerTweet("EM2017_MINI07_TWEET_01", mTweetCameraEntity->getId());
			}
		}

		for (size_t i = 0; i < mStartEscaping.size(); ++i)
		{
			StartEscaping& startEscaping = mStartEscaping[i];
			if (!startEscaping.mStarted)
			{
				startEscaping.mTimeTillEscape -= timePassed;
				if (startEscaping.mTimeTillEscape <= qsf::Time::ZERO)
				{
					startEscaping.mStarted = true;
					if (startEscaping.mEscapeEntity.valid())
					{
						pushMoveToEscapePoint(*startEscaping.mEscapeEntity);
					}
				}
			}
			else if (!startEscaping.mRemovedInvincible)
			{
				if (startEscaping.mEscapeEntity.valid() && nullptr == startEscaping.mEscapeEntity->getOrCreateComponentSafe<qsf::ActionComponent>().getAction<MoveAction>())
				{
					startEscaping.mRemovedInvincible = true;

					HealthHelper(*startEscaping.mEscapeEntity).setInvincible(false);
				}
			}
		}
	}

	bool SabotageEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* SabotageEvent::getFocusEntity()
	{
		return getRandomEventEntity();
	}

	bool SabotageEvent::checkEventConfiguration()
	{
		qsf::Layer* mainLayer = getMap().getLayerManager().getLayerById(mEventLayerId);
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

	void SabotageEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);
	}

	void SabotageEvent::hintCallback(Observer& hintObserver)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SabotageEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		objectiveHelper.addDeadUnitPersonsFailCondition(2);
		objectiveHelper.addDeadCivilPersonsFailCondition(2);
		objectiveHelper.addBurningObjectsFailCondition(20);
	}

	bool SabotageEvent::getSubLayerFromMainLayer(qsf::Layer& mainLayer)
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

	void SabotageEvent::setupScene()
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(*mEventLayer, false);

		// Show the accident
		MapHelper::activateLayer(*mEventLayer, true);

		// Hide the non important environment
		MapHelper::activateLayer(*mEraseLayer, false);

		EventHelper eventHelper(*this);
		ObjectiveHelper objectiveHelper(*this);
		qsf::game::EventTagHelper eventTagHelper("Sabotage");

		qsf::Layer::EntityIdSet entities = mEventLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.checkEntityHasEventTag(TAG_INJURED))
				{
					if (targetEntityHelper.isEntityPerson())
					{
						HealthHelper(*targetEntity).injurePersonByFire(this, false);
					}
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_GANGSTER_FLEE))
				{
					if (isMiniEvent())
					{
						// No gangster for the minievent variant, delete him him to avoid interrupting the event somehow
						getMap().destroyEntityById(entityId);
						continue;
					}

					if (targetEntityHelper.isEntityPerson())
					{
						// Hide the gangster
						qsf::game::HiddenComponent::setHidden(*targetEntity, true);
						mIsGangsterHidden = true;

						targetEntityHelper.turnIntoGangster(qsf::StringHash(mGangsterLogicName), false);

						// Add new objectives
						Objective& needFindKillerObjective = objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE);
						needFindKillerObjective.increaseNeededNumber(targetEntity);

						// Add the person to the event
						EventSpreadHelper::setEventIdComponent(this, *targetEntity, eventspreadreason::GANGSTER);

						mGangsterFoundMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE), boost::bind(&SabotageEvent::onDroneFoundGangster, this, _1));
					}
				}
				else if (nullptr != targetEntity->getComponent<BuryComponent>())
				{
					// Setup persons under debris
					BuryComponent& buryComponent = targetEntity->getComponentSafe<BuryComponent>();
					for (auto buryEntityId : buryComponent.BuriedEntitiesArray.getVariable())
					{
						qsf::Entity* buriedEntity = getMap().getEntityById(buryEntityId);
						if (buriedEntity != nullptr)
						{
							if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_DEBRIS))
							{
								// The tweet debris has extra logic, no injured person here till we unhide it by hand. Avoid finding him by rescue dog
								qsf::game::HiddenComponent::setHidden(*buriedEntity, true, qsf::game::HiddenComponent::HIDDENTYPE_DEFAULT);
								mTweetDebrisEntity = targetEntity;
								buryComponent.setBlocked(true);
								continue;
							}
							mNumberPersonsTillTweet++;

							buryComponent.buryEntity(*buriedEntity);

							static const uint32 injuryPool[4] = { injury::HEAD_BODY_LACERATION, injury::INNER_BLEEDING, injury::BROKEN_SKULL, injury::BRAIN_BLEEDING };
							const qsf::StringHash& randomInjury = injuryPool[qsf::Random::getRandomUint(0, 3)];
							eventHelper.makeMissingPerson(*buriedEntity, randomInjury, false);
						}
					}

					// Show the debris
					targetEntity->setActive(true);
				}
				// Check this event tag before TAG_CIVIL_ESCAPE because it contains "TAG_RUNNING_END"
				else if (eventTagHelper.startsWithEventTag(*targetEntity, TAG_RUNNING_END))
				{
					mCivilEscapePoints.push_back(targetEntity);
				}
				else if (eventTagHelper.startsWithEventTag(*targetEntity, TAG_CIVIL_ESCAPE))
				{
					mCivilEscapePersons.push_back(targetEntity);

					qsf::ai::NavigationComponent* navigationComponent = targetEntity->getOrCreateComponent<qsf::ai::NavigationComponent>();
					navigationComponent->setMoverType(MoverType::GANGSTER_PERSON);
					navigationComponent->setNativePrimaryNavigationMapId(NavigationMap::FREE_GROUND_AREA);

					qsf::ActionComponent& actionComponent = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

					actionComponent.clearPlan();
					AnimationHelper animationHelper(*targetEntity);
					// For all actions use a action priority higher than default, to avoid having interruption from escape / flee from fire logic
					// To avoid an armada of synchronous borgs, add small delay before starting
					actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_LOW).init( qsf::Time::fromMilliseconds(qsf::Random::getRandomInt(0, 500)));
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_LOW).init(animationHelper.getAnimationGangsterCowerFearfullyStart(), true);
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_LOW).init(animationHelper.getAnimationGangsterCowerFearfullyLoop(), true, true);

					// To avoid the fleefromFireLogic and the corresponding icons, temporary make invincible
					HealthHelper(*targetEntity).setInvincible(true);

					mStartEscaping.push_back(StartEscaping(*targetEntity, qsf::Time::fromSeconds(qsf::Random::getRandomFloat(2.0f, 5.0f))));
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_CAMERA))
				{
					mTweetCameraEntity = targetEntity;
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_PERSON))
				{
					mTweetPersonEntity = targetEntity;
					eventHelper.makeTweetPerson(*targetEntity, true);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_STAGE))
				{
					mStageEntity = targetEntity;
				}
			}
		}

		// Not all important objects are in our event layer (eg. houses in baselayer, they are used in multiple events and can't move in specific event folder)
		const std::vector<qsf::game::EventTagComponent*>& housesComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_BURNING), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* houseComponent : housesComponents)
		{
			EntityHelper(houseComponent->getEntity()).startFire(this);
		}

		// Turn off all sensor components, to avoid have persons stop at the near attraction instead running away in fear
		for (qsf::ai::SingleSensorComponent* singleSensorComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<qsf::ai::SingleSensorComponent>())
		{
			singleSensorComponent->setActive(false);
			singleSensorComponent->setAutomaticUpdate(false);
		}
		for (qsf::ai::ReactionComponent* reactionComponent : qsf::ComponentMapQuery(getMap()).getAllInstances<qsf::ai::ReactionComponent>())
		{
			reactionComponent->setActive(false);
		}
	}

	void SabotageEvent::onDroneFoundGangster(const qsf::MessageParameters& parameters)
	{
		const uint64 gangsterEntityId = parameters.getFilter(1);
		qsf::Entity* gangsterEntity = getMap().getEntityById(gangsterEntityId);

		if (nullptr == gangsterEntity)
			return;

		EntityHelper entityHelper(*gangsterEntity);
		if (!entityHelper.checkEntityHasEventTag(TAG_GANGSTER_FLEE))
		{
			// Gangster is not part of this event
			return;
		}

		mIsGangsterHidden = false;

		// Increase prisoner seek objective
		ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDKILLERWITHSEKDRONE).increaseCurrentNumber(gangsterEntity);

		// Start fleeing
		GangsterBaseLogic* gangsterLogic = entityHelper.getGameLogic<GangsterBaseLogic>();
		if (gangsterLogic != nullptr && !entityHelper.isGangsterFleeing())
		{
			gangsterLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
		}
	}

	void SabotageEvent::onBuriedFound(const qsf::MessageParameters& parameters)
	{
		uint64 foundEntityId = parameters.getFilter(1);

		for (uint64 allReadyFoundEntityIds : mAllReadyFoundPersonsIds)
		{
			if (foundEntityId == allReadyFoundEntityIds)
			{
				return;
			}
		}
		mAllReadyFoundPersonsIds.push_back(foundEntityId);

		qsf::Entity* personEntity = getMap().getEntityById(foundEntityId);
		if (nullptr == personEntity)
			return;

		// Injure the person
		// Add injury objectives
		EventSpreadHelper::setEventIdComponent(this, *personEntity, eventspreadreason::INJURY);

		// Add objectives
		Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_PERSONSBURIEDHEAVYDEBRIS2);
		rescuePeople.increaseNeededNumber(personEntity);

		UnhideBuriedObserver& unhideBuriedObserver = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(personEntity->getId());
		unhideBuriedObserver.connectToObjective(rescuePeople);

		mNumberPersonsTillTweet--;

		if (mTweetCameraEntity.valid() && !mCanTweet && isMiniEvent() && !mHasTweeted && mNumberPersonsTillTweet <= 0)
		{
			mCanTweet = true;

			if (mTweetDebrisEntity.valid())
			{
				// Setup persons under debris
				BuryComponent& buryComponent = mTweetDebrisEntity->getComponentSafe<BuryComponent>();
				buryComponent.setBlocked(false);
				for (auto buryEntityId : buryComponent.BuriedEntitiesArray.getVariable())
				{
					qsf::Entity* buriedEntity = getMap().getEntityById(buryEntityId);
					if (buriedEntity == nullptr)
						continue;

					static const uint32 injuryPool[4] = { injury::HEAD_BODY_LACERATION, injury::INNER_BLEEDING, injury::BROKEN_SKULL, injury::BRAIN_BLEEDING };
					const qsf::StringHash& randomInjury = injuryPool[qsf::Random::getRandomUint(0, 3)];
					EventHelper(*this).makeMissingPerson(*buriedEntity, randomInjury, false);
					EventSpreadHelper::setEventIdComponent(this, *buriedEntity, eventspreadreason::INJURY);

					// Unhide the target by hand (story: the tweeterguy finds the person for the player)
					qsf::game::HiddenComponent::setHidden(*buriedEntity, false);

					// Don`t count the last person to the objective, close it manually
					Objective& findPersons = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON);
					findPersons.increaseCurrentNumber(buriedEntity);

					// Increase the rescue by crane objective
					rescuePeople.increaseNeededNumber(buriedEntity);

					UnhideBuriedObserver& unhideBuriedObserver2 = ObserverHelper(*this).createObserver<UnhideBuriedObserver>(buriedEntity->getId());
					unhideBuriedObserver2.connectToObjective(rescuePeople);
				}
			}
		}
	}

	void SabotageEvent::pushMoveToEscapePoint(qsf::Entity& escapePerson)
	{
		qsf::game::EventTagComponent* eventTagComponent = escapePerson.getComponent<qsf::game::EventTagComponent>();
		if (nullptr != eventTagComponent)
		{
			std::string eventTag = eventTagComponent->getTagStartsWithTag(TAG_RUNNING_START);
			if (!eventTag.empty())
			{
				qsf::ActionComponent& actionComponent = escapePerson.getOrCreateComponentSafe<qsf::ActionComponent>();
				// Delete the animation action on the action plan
				actionComponent.clearPlan();

				std::string prefix = eventTag.substr(TAG_RUNNING_START.size() +1, (eventTag.size() - 1) - TAG_RUNNING_START.size() );
				qsf::Entity* targetEntityPoint = getEscapePointById(atoi(prefix.c_str()));
				if (nullptr != targetEntityPoint)
				{
					const qsf::logic::TargetPoint targetPoint(EntityHelper(*targetEntityPoint).getPosition(), false, 0.0f, 0.1f);
					actionComponent.pushAction<MoveAction>(action::COMMAND_LOW).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_RUN);
				}

				if (mStageEntity.valid())
				{
					actionComponent.pushAction<TurnToAction>(action::COMMAND_LOW).init(mStageEntity->getId());
				}

				escapePerson.getOrCreateComponentSafe<GaperComponent>().setUseMobilephone(true);
			}
		}
	}

	qsf::Entity* SabotageEvent::getEscapePointById(int escapePointIndex)
	{
		for (const qsf::WeakPtr<qsf::Entity>& escapePoints : mCivilEscapePoints)
		{
			if (!escapePoints.valid())
				continue;

			if (EntityHelper(*escapePoints).checkEntityHasEventTag(TAG_RUNNING_END + "_" + std::to_string(escapePointIndex)))
			{
				return escapePoints.get();
			}
		}
		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
