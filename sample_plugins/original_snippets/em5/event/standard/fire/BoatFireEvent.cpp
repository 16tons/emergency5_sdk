// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/BoatFireEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/ai/em4Router/actor/EObject.h"
#include "em5/base/RandomEnumerator.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Random.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Global variables                                      ]
	//[-------------------------------------------------------]
	namespace detail
	{
		// Hints
		const std::string BURNINGSHIP_HINT_01 = "EM5_EVENT_BURNINGSHIP_HINT_01";	// Hint Ship has only X% life energy
		const std::string BURNINGSHIP_HINT_02 = "EM5_EVENT_BURNINGSHIP_HINT_02";
		const std::string BURNINGSHIP_HINT_03 = "EM5_EVENT_BURNINGSHIP_HINT_03";
		const float BURNINGSHIP_HINT_01_LIFENERGY(0.2f);
		const float BURNINGSHIP_HINT_03_LIFENERGY(0.1f);
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BoatFireEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::BoatFireEvent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool BoatFireEvent::canSpawnEntitiesCollisionFree(const qsf::Layer& layer, qsf::Map& map)
	{
		CollisionHelper collisionHelper(map);

		// Find all entities where collisions matter.
		// That's vehicles and persons outside cars.
		// The rest is just debris.
		const qsf::Map& layerBufferMap = layer.getInternalBufferMap();
		const auto& entities = layerBufferMap.getEntities();
		for (qsf::Entity* targetEntity : entities)
		{
			qsf::BulletCollisionComponent* collisionComponent = nullptr;
			const qsf::TransformComponent& transformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			EntityHelper targetEntityHelper(*targetEntity);
			if (targetEntity->getComponent<BoatComponent>() != nullptr)
			{
				collisionComponent = targetEntity->getComponent<qsf::BulletCollisionComponent>();
			}
			// TODO(mk) how can we check if the linked spawnpoints would create persons without collision

			if (nullptr != collisionComponent)
			{
				// Get box extends from the bullet collision
				glm::vec3 anchorPoint, extensions;
				glm::quat rotation;
				collisionComponent->getAsOrientedBoundingBox(anchorPoint, rotation, extensions);

				// Get all colliding objects
				qsf::ai::CollisionList collisionRange;
				qsf::ai::EM3::Router->FindCollisions(extensions, -1, qsf::ai::ERouterObjectState(transformComponent.getPosition(), glm::mat3_cast(transformComponent.getRotation())), collisionRange, qsf::ai::EOTC_ANYTHING); // call with values for right now

				for (qsf::ai::ECollisionObject* collision : collisionRange)
				{
					// Check type of the collision object
					qsf::ai::EActor* actor = collision->GetObject();
					if (nullptr != actor)
					{
						// Collision with persons and vehicles are not allowed
						if (actor->GetType() == qsf::ai::EAT_VEHICLE || actor->GetType() == qsf::ai::EAT_PERSON)
						{
							return false;
						}
					}
				}
			}
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BoatFireEvent::BoatFireEvent() :
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mPersonsInWater(4),
		mPersonsOnDeck(2),
		mEventLayerId(qsf::getUninitialized<uint32>()),
		mBurnShipAtStartup(true)
	{
		// Nothing here
	}

	BoatFireEvent::~BoatFireEvent()
	{
		// Nothing here
	}

	void BoatFireEvent::initWithLayer(qsf::Layer* eventLayer)
	{
		// If event layer is set, the layer is activated in the startup function
		if (eventLayer != nullptr)
			mEventLayerId = eventLayer->getId();
	}

	uint64 BoatFireEvent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	void BoatFireEvent::setTargetEntity(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();
	}

	void BoatFireEvent::setBurnShipAtStartup(bool burnShipAtStartup)
	{
		mBurnShipAtStartup = burnShipAtStartup;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool BoatFireEvent::onStartup()
	{
		// In case the layer is set, we activate it and get the target entity from it
		qsf::Layer* eventLayer = getMap().getLayerManager().getLayerById(mEventLayerId);
		if (eventLayer != nullptr)
		{
			if (!canSpawnEntitiesCollisionFree(*eventLayer, getMap()))
				return false;

			if (!startupFromLayer(*eventLayer))
			{
				return false;
			}
		}

		// search for all spawnpoints and sort them in the right variable
		findSpawnPoints();

		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "Could not find entity with ID: " << mTargetEntityId, return false);
		QSF_CHECK(mSpawnPointsInWater.size() + mSpawnPointsOnDeck.size(), "Could not find any spawnpoints linked to target.", return false);

		// Mark the entity so it wont get effected by other events
		EventIdComponent& eventIdComponent = targetEntity->getOrCreateComponentSafe<EventIdComponent>();
		eventIdComponent.setEvent(*this, eventspreadreason::NO_REASON);

		spawnPersonsForRescue();

		setRunning();

		// Register Message proxies
		qsf::MessageConfiguration messagePersonRescueByHeli(Messages::EM5_ACTION_PICKUP_PERSON_WITH_RESCUEHELICOPTER);
		mPersonRescueByHeliMessageProxy.registerAt(messagePersonRescueByHeli, boost::bind(&BoatFireEvent::onPersonRescued, this, _1));

		qsf::MessageConfiguration messagePersonRescueByDiver(Messages::EM5_ACTION_PLACE_PERSON_FROM_DIVER_ACTION);
		mPersonRescueByDiverMessageProxy.registerAt(messagePersonRescueByDiver, boost::bind(&BoatFireEvent::onPersonRescued, this, _1));

		qsf::MessageConfiguration messageBoatDestroyed(Messages::EM5_BOAT_DESTROYED, mTargetEntityId);
		mBoatDestroyedMessageProxy.registerAt(messageBoatDestroyed, boost::bind(&BoatFireEvent::onBoatDestroyed, this, _1));

		// Done
		return true;
	}

	void BoatFireEvent::onShutdown()
	{
		// Ensure everything from the event is cleaned up.
		qsf::Layer* layer = getMap().getLayerManager().getLayerById(mEventLayerId);
		if (nullptr != layer)
		{
			LayerCleanup::createLayerCleanup(*layer);
		}

		mPersonRescueByHeliMessageProxy.unregister();
		mPersonRescueByDiverMessageProxy.unregister();
		mBoatDestroyedMessageProxy.unregister();
	}

	void BoatFireEvent::onRun()
	{
		// No null pointer check in here. The factory already checked these pointers.
		// Get the vehicle to burn
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return;

		// Remove event ID component
		targetEntity->destroyComponent<EventIdComponent>();

		if (mBurnShipAtStartup)
		{
			// Let the vehicle burn
			EntityHelper(*targetEntity).startFire(this);
		}

		// Set all observers and objectives for the first target
		startObjectives(*targetEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(*targetEntity);

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		createEventLocationEntity(*targetEntity);
	}

	bool BoatFireEvent::onFailure(EventResult& eventResult)
	{
		const uint32 failObjective = getFailReasonObjectiveTypeId(eventResult.mObjectives);

		if (ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS == failObjective)
		{
			// Person died
			showHint("EM5_EVENT_BURNINGSHIP_FAIL_HINT_01", false);
		}

		else if (nullptr == getMap().getEntityById(mTargetEntityId))
		{
			// Ship is sunk
			showHint("EM5_EVENT_BURNINGSHIP_FAIL_HINT_02", false);
		}

		// Call base implementation
		return FreeplayEvent::onFailure(eventResult);
	}

	void BoatFireEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		if (nullptr == getMap().getEntityById(mTargetEntityId))
		{
			// Event failed...
			setFinished(false);
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool BoatFireEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& observerDied = ObserverHelper(*this).createObserver<PersonDiedObserver>(targetEntity.getId());
			observerDied.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);

			// Done
			return true;
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	const qsf::Entity* BoatFireEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();

		{ // Is the ship still burning?
			qsf::Entity* candidateEntity = map.getEntityById(mTargetEntityId);
			if (nullptr != candidateEntity && EntityHelper(*candidateEntity).isBurning())
			{
				return candidateEntity;
			}
		}

		const ObjectiveHelper objectiveHelper(*this);

		{ // Is there any other fire left?
			const Objective* extingushFireObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
			if (nullptr != extingushFireObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(extingushFireObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Are there any persons injured?
			const Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			if (nullptr != treatPersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(treatPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		return getEventLocationEntity();
	}

	bool BoatFireEvent::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void BoatFireEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base Implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mTargetEntityId);
	}

	void BoatFireEvent::hintCallback(Observer& hintObserver)
	{
		if (detail::BURNINGSHIP_HINT_02 == hintObserver.getName())
		{
			if (CollisionHelper(getMap()).isInWater(*hintObserver.getEntity(), true))
			{
				showHint(hintObserver.getName(), true);
			}
		}
		else
		{
			showHint(hintObserver.getName(), true);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BoatFireEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		// Create the fail condition
		Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
		failConditionPersonDiedObjective.setNeededNumber(1);
	}

	void BoatFireEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		// Hint 01    Target ship has 20% lifeenergy
		createObserver<HintLowEnergyObserver>(targetEntity.getId(), detail::BURNINGSHIP_HINT_01).initialize(detail::BURNINGSHIP_HINT_01_LIFENERGY, HintLowEnergyObserver::HINTENERGYTYPE_DAMAGE);
	}

	void BoatFireEvent::onPersonRescued(const qsf::MessageParameters& parameters)
	{
		uint64 entityId = parameters.getFilter(0);

		// Hint 03    Target person has 10% lifeenergy
		createObserver<HintLowEnergyObserver>(entityId, detail::BURNINGSHIP_HINT_03).initialize(detail::BURNINGSHIP_HINT_03_LIFENERGY, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
	}

	void BoatFireEvent::onBoatDestroyed(const qsf::MessageParameters& parameters)
	{
		Objective& rescuePeople = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_RESCUEPERSONWITHHELI);
		for (uint64 entityID : rescuePeople.getCurrentEntityIdSet())
		{
			qsf::Entity* entity = getMap().getEntityById(entityID);
			if (nullptr != entity)
			{
				qsf::ActionComponent& actionComponent = entity->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.clearPlan();
				actionComponent.pushAction<DisappearAction>();
			}
		}
	}

	bool BoatFireEvent::startupFromLayer(qsf::Layer& eventLayer)
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(eventLayer, false);

		// Show the layer
		MapHelper::activateLayer(eventLayer, true);

		qsf::Layer::EntityIdSet entities = eventLayer.getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			// Here a simple check to get the boat, maybe this is not enough
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity && targetEntity->getComponent<BoatComponent>())
			{
				mTargetEntityId = targetEntity->getId();
				break;
			}
		}

		return true;
	}

	void BoatFireEvent::findSpawnPoints()
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return;

		const BoatComponent* boatComponent = targetEntity->getComponent<BoatComponent>();
		if (nullptr == boatComponent)
			return;

		for (uint64 entityId : boatComponent->getSpawnPointIds())
		{
			// Get the linked entity
			qsf::Entity* linkedEntity = QSF_MAINMAP.getEntityById(entityId);
			if (nullptr == linkedEntity)
				continue;

			// Check if entity has a spawn component
			if (nullptr == linkedEntity->getComponent<PersonSpawnPointComponent>())
				continue;

			// Check for "on deck" tag
			const qsf::game::EventTagComponent* eventTagComponent = linkedEntity->getComponent<qsf::game::EventTagComponent>();
			if (nullptr != eventTagComponent && eventTagComponent->hasTag(qsf::StringHash("ONDECK")))
			{
				mSpawnPointsOnDeck.push_back(linkedEntity);
			}
			else
			{
				mSpawnPointsInWater.push_back(linkedEntity);
			}
		}
	}

	void BoatFireEvent::spawnPersonsForRescue()
	{
		// Spawn the persons in water
		RandomEnumerator<qsf::Entity> openSpawnPointsInWater(constructionpolicy::COPY, mSpawnPointsInWater);
		for (uint32 i = 0; i < mPersonsInWater; ++i)
		{
			if (openSpawnPointsInWater.empty())
			{
				QSF_ERROR("Boat fire event: Unable to spawn enough persons in water", QSF_REACT_NONE);
				break;
			}

			qsf::Entity* spawnPointEntity = openSpawnPointsInWater.getNext();
			if (nullptr == spawnPointEntity)
				continue;

			qsf::Entity* personEntity = EventHelper::spawnEntity(*spawnPointEntity);
			if (nullptr == personEntity)
				continue;

			EventHelper::copyPosition(*spawnPointEntity, *personEntity);

			// Drown person
			EventHelper(*this).makeDrownedPerson(*personEntity, true);

			// We rotate the person randomly
			qsf::TransformComponent& transformComponent = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::quat randomRotation = transformComponent.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), qsf::Random::getRandomFloat(-glm::pi<float>(), glm::pi<float>()));
			transformComponent.setRotation(randomRotation);

			// Hint 02    Target person has 25% lifeenergy
			createObserver<HintLowEnergyObserver>(personEntity->getId(), detail::BURNINGSHIP_HINT_02).initialize(detail::BURNINGSHIP_HINT_01_LIFENERGY, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
		}

		// Spawn the persons on deck
		RandomEnumerator<qsf::Entity> openSpawnPointsOnDeck(constructionpolicy::COPY, mSpawnPointsOnDeck);
		for (uint32 i = 0; i < mPersonsOnDeck; ++i)
		{
			if (openSpawnPointsOnDeck.empty())
			{
				QSF_ERROR("Boat fire event: Unable to spawn enough persons on deck", QSF_REACT_NONE);
				break;
			}

			qsf::Entity* spawnPointEntity = openSpawnPointsOnDeck.getNext();
			if (nullptr == spawnPointEntity)
				continue;

			qsf::Entity* personEntity = EventHelper::spawnEntity(*spawnPointEntity);
			if (nullptr == personEntity)
				continue;

			EventHelper::copyPosition(*spawnPointEntity, *personEntity);

			// Remove person placement component
			personEntity->destroyComponentById(qsf::StringHash("em5::PersonPlacementComponent"));

			EventHelper(*this).makeRescuablePerson(*personEntity);
			// TODO(db): Injure person instead of start fire
			EntityHelper(*personEntity).startFire(this);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
