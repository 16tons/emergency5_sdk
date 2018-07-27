// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/WeaponDealEvent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/event/EventHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/gui/hud/WarblerFeedWindow.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/link/LinkComponentHelper.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/light/LightComponent.h>
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
	const std::string TAG_GANGSTER_ARMED = "ME201_TERRORIST_01";
	const std::string TAG_GANGSTER_UNARMED = "MINI07_OFFENDER_02";
	const std::string TAG_GANGSTER_CAR = "EM2017_FP02_VAN";
	const std::string TAG_WEAPONOBJECT = "EM2017_FP02_VAN_OBJECT";
	const std::string TAG_TWEET_CAMERA = "EM2017_MINI02_TWEET_01";
	const std::string TAG_TWEET_PERSON = "EM2017_MINI02_TWEET_01_GAPER";
	const std::string TAG_TWEET_KICKLIGHT = "EM2017_MINI02_TWEET_01_LIGHT";

	// Objectives
	const qsf::NamedIdentifier FOUND_WEAPON_INCAR = "Weapondeal_WeaponInCar";
	const std::string FOUND_WEAPON_INCAR_TEXT = "EM2017_MINI02_OBJ02";

	const qsf::NamedIdentifier CLEANUP_GANGSTER_CAR = "Cleanup_Gangster_Car";
	const std::string CLEANUP_GANGSTER_CAR_TEXT = "EM2017_MINI02_OBJ03";


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 WeaponDealEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::WeaponDealEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WeaponDealEvent::WeaponDealEvent() :
		mEventLayer(nullptr),
		mGangsterTypeArmed("WeapondealArmed"),
		mGangsterTypeUnarmed("WeapondealUnarmed"),
		mPoliceArrived(false),
		mDetectPoliceRadius(50),
		mDeadPersonsToFail(1),
		mWeaponfound(false),
		mHint01Played(false),
		mTweetDelay(qsf::Time::ZERO),
		mCanMakeTweet(false),
		mCanTurnOffLight(false)
	{
		// Nothing here
	}

	WeaponDealEvent::~WeaponDealEvent()
	{
		// Nothing here
	}

	void WeaponDealEvent::init(qsf::Layer& eventLayer)
	{
		mEventLayer = &eventLayer;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool WeaponDealEvent::onStartup()
	{
		if (nullptr == mEventLayer)
			return false;

		setupScene();

		// Event is now running
		setRunning();

		return true;
	}

	void WeaponDealEvent::onShutdown()
	{
		// Create a layer cleanup to clean up the rest
		if (nullptr != mEventLayer)
		{
			LayerCleanup::createLayerCleanup(*mEventLayer);
		}

		mFoundWeaponMessageProxy.unregister();

		if (mTweetPersonEntity.valid())
		{
			mTweetPersonEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
		}
	}

	void WeaponDealEvent::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		startObjectives();

		// Register find the weapon object
		mFoundWeaponMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_POLICE_DOG_FOUND_OBJECT, mGangsterCar->getId()), boost::bind(&WeaponDealEvent::onWeaponFound, this, _1));
	}

	bool WeaponDealEvent::onFailure(EventResult& eventResult)
	{
		// Check the event fail reason
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);
		if (failObjective == ObjectiveHelper::OBJECTIVE_FAIL_PERSONSESCAPE)
		{
			setFailHintsForMiniEvent(eventResult, "EM2017_MINI01_FAIL01", "EM2017_MINI01_SUPERV_FAIL01");
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

		return true;
	}

	bool WeaponDealEvent::onSuccess(EventResult& eventResult)
	{
		if (isMiniEvent())
		{
			showSupervisorMessage("EM2017_MINI02_SUPERV_END01");
		}

		return true;
	}

	void WeaponDealEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		if (!mPoliceArrived)
		{
			// Check if police arrived
			std::vector<CommandableComponent*> commandableComponents;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(EntityHelper(mGangsterCar.getSafe()).getPosition(), mDetectPoliceRadius, commandableComponents);

			for (CommandableComponent* commandableComponent : commandableComponents)
			{
				if (commandableComponent->isPoliceSTW() || commandableComponent->isPoliceMTW() || commandableComponent->isPoliceHeli() || commandableComponent->isPoliceSEKHeli()
					|| commandableComponent->isPoliceSEKPerson() || commandableComponent->isPolicePerson() || commandableComponent->isPoliceDogLeader() || commandableComponent->isPoliceDogCar())
				{
					startPoliceArriving();
					break;
				}
			}
		}

		if (!mHint01Played)
		{
			Objective* arrestGangsterObjective = ObjectiveHelper(*this).getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
			if (nullptr != arrestGangsterObjective && arrestGangsterObjective->checkAccomplished())
			{
				if (!mWeaponfound)
				{
					showSupervisorMessage("EM2017_MINI02_SUPERV_OBJ01");
				}
				mHint01Played = true;
			}
		}

		if (mCanMakeTweet || mCanTurnOffLight)
		{
			mTweetDelay += timePassed;
			if (mCanMakeTweet && mTweetDelay >= qsf::Time::fromSeconds(0.1f))
			{
				mCanMakeTweet = false;
				EM5_GUI.getIngameHud().getOrCreateWarblerFeedWindow().addWarblerTweet("EM2017_MINI02_TWEET_01", mTweetCameraEntity->getId());
			}

			if (mCanTurnOffLight && mTweetDelay >= qsf::Time::fromSeconds(0.2f))
			{
				mCanTurnOffLight = false;
				showKicklight(false);
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool WeaponDealEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* WeaponDealEvent::getFocusEntity()
	{
		if (!mPoliceArrived)
		{
			return mGangsterCar.get();
		}

		return getRandomEventEntity();
	}

	bool WeaponDealEvent::checkEventConfiguration()
	{
		if (nullptr == mEventLayer)
			return false;

		// Don't start the same event location twice
		if (mEventLayer->isActive())
			return false;

		return true;
	}

	void WeaponDealEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);
	}

	void WeaponDealEvent::hintCallback(Observer& hintObserver)
	{
		// TODO(mk) implement function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void WeaponDealEvent::startObjectives()
	{
		ObjectiveHelper objectiveHelper(*this);

		if (mWeaponObjectEntity.valid())
		{
			Objective& foundWeaponInCarObjective = objectiveHelper.getOrCreateObjective(FOUND_WEAPON_INCAR, Objective::OBJECTIVETYPE_REQUIRED);
			foundWeaponInCarObjective.setText(FOUND_WEAPON_INCAR_TEXT);
			foundWeaponInCarObjective.increaseNeededNumber(mWeaponObjectEntity.get());
		}

		objectiveHelper.addDeadCivilPersonsFailCondition(mDeadPersonsToFail);
		objectiveHelper.addDeadUnitPersonsFailCondition(1);
		objectiveHelper.addBurningObjectsFailCondition(20);

		// Add all gangster to objective to show the number to the player
		for (qsf::WeakPtr<qsf::Entity>& gangster : mUnarmedGangsters)
		{
			EventIdComponent::addToEvent(gangster.getSafe(), *this, eventspreadreason::GANGSTER);
			objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS).increaseNeededNumber(gangster.get());
		}

		for (qsf::WeakPtr<qsf::Entity>& gangster : mArmedGangsters)
		{
			EventIdComponent::addToEvent(gangster.getSafe(), *this, eventspreadreason::GANGSTER);
			objectiveHelper.getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS).increaseNeededNumber(gangster.get());
		}
	}

	void WeaponDealEvent::setupScene()
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(*mEventLayer, false);

		// Show the event layer
		MapHelper::activateLayer(*mEventLayer, true);

		qsf::Layer::EntityIdSet entities = mEventLayer->getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.isRoadVehicle())
				{
					if (targetEntityHelper.checkEntityHasEventTag(TAG_GANGSTER_CAR))
					{
						mGangsterCar = targetEntity;

						// Mark as police dog target
						targetEntity->getOrCreateComponentSafe<PoliceDogTargetComponent>();
						EventIdComponent::addToEvent(*targetEntity, *this, eventspreadreason::EVENT_SPECIFIC);
					}
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_GANGSTER_ARMED))
				{
					mArmedGangsters.push_back(targetEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_GANGSTER_UNARMED))
				{
					mUnarmedGangsters.push_back(targetEntity);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_WEAPONOBJECT))
				{
					mWeaponObjectEntity = targetEntity;

					// The weapon object is visible through the door of the car, disable it till the police dog found it
					showEntityMesh(*mWeaponObjectEntity, false);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_CAMERA))
				{
					mTweetCameraEntity = targetEntity;
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_PERSON))
				{
					mTweetPersonEntity = targetEntity;
					EventHelper(*this).makeTweetPerson(*targetEntity, true);
				}
				else if (targetEntityHelper.checkEntityHasEventTag(TAG_TWEET_KICKLIGHT))
				{
					mTweetKickLightEntity = targetEntity;
					showKicklight(false);
				}
			}
		}
	}

	void WeaponDealEvent::startPoliceArriving()
	{
		if (!mPoliceArrived)
		{
			mPoliceArrived = true;

			// Armed gangster attack police
			for (const qsf::WeakPtr<qsf::Entity>& gangster : mArmedGangsters)
			{
				if (!gangster.valid())
					continue;

				setupGangster(*gangster, mGangsterTypeArmed);
			}

			// Unarmed gangsters flee from police
			for (const qsf::WeakPtr<qsf::Entity>& gangster : mUnarmedGangsters)
			{
				if (!gangster.valid())
					continue;

				setupGangster(*gangster, mGangsterTypeUnarmed);

				// Flee from Police
				qsf::GameLogicComponent* gameLogicComponent = gangster->getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
					if (gangsterLogic != nullptr)
					{
						gangsterLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
					}
				}
			}

			if (mTweetCameraEntity.valid() && isMiniEvent())
			{
				showKicklight(true);

				mCanMakeTweet = true;
				mCanTurnOffLight = true;
			}
		}
	}

	void WeaponDealEvent::setupGangster(qsf::Entity& gangsterEntity, std::string gangsterLogicName)
	{
		// Some gangsters have looped animations or cigarettes in hand. Remove this to have a solid base for other gangster animations.
		EntityHelper gangsterEntityHelper(gangsterEntity);
		gangsterEntityHelper.deleteAllLinkedChild();
		gangsterEntityHelper.turnIntoGangsterByType(qsf::StringHash(gangsterLogicName));
	}

	void WeaponDealEvent::onWeaponFound(const qsf::MessageParameters& parameters)
	{
		if (mWeaponfound)
			return;

		if (mWeaponObjectEntity.valid())
		{
			showEntityMesh(*mWeaponObjectEntity, true);
		}

		// Finish found weapon objective
		ObjectiveHelper objectiveHelper(*this);
		objectiveHelper.getOrCreateObjective(FOUND_WEAPON_INCAR, Objective::OBJECTIVETYPE_REQUIRED).setAccomplished();

		if (mGangsterCar.valid())
		{
			// Activate "cleanup car" objective
			Objective& foundWeaponInCarObjective = objectiveHelper.getOrCreateObjective(CLEANUP_GANGSTER_CAR, Objective::OBJECTIVETYPE_REQUIRED);
			foundWeaponInCarObjective.setText(CLEANUP_GANGSTER_CAR_TEXT);
			foundWeaponInCarObjective.increaseNeededNumber(mGangsterCar.get());

			const ObserverHelper observerHelper(*this);
			TransportedToHqObserver& observerTransported = observerHelper.createObserver<TransportedToHqObserver>(mGangsterCar->getId());
			observerTransported.connectToObjective(foundWeaponInCarObjective);

			RoadVehicleComponent* roadVehicleComponent = mGangsterCar->getComponent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent)
			{
				roadVehicleComponent->setCanBeTowed(true, this);
				roadVehicleComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, true);
				roadVehicleComponent->setTooBigToTow(true);
			}

			StatusOverlayComponent::showIcon(mGangsterCar.getSafe(), StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, true);
		}

		// Show hint
		showSupervisorMessage("EM2017_MINI02_SUPERV_OBJ02");

		mWeaponfound = true;
	}

	void WeaponDealEvent::showEntityMesh(qsf::Entity& entity, bool active)
	{
		qsf::MeshComponent* weaponObjectMeshComponent = entity.getComponent<qsf::MeshComponent>();
		if (weaponObjectMeshComponent != nullptr)
		{
			weaponObjectMeshComponent->setActive(active);
		}
	}

	void WeaponDealEvent::showKicklight(bool active)
	{
		if (!mTweetKickLightEntity.valid())
			return;

		std::vector<qsf::LightComponent*> lightComponents;
		qsf::LinkComponentHelper(*mTweetKickLightEntity).getAllComponentsFromPrototypeOrLinkedChild<qsf::LightComponent>(lightComponents);
		for (qsf::LightComponent* lightComponent : lightComponents)
		{
			lightComponent->setActive(active);
			if (active)
			{
				// Looks like an very expensive function here, only do this for active mode
				lightComponent->updateOgreSceneNodeVisibility();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
