// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/TrafficAccident01Event.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/ai/em4Router/actor/EObject.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/event/EventHelper.h"
#include "em5/event/standard/destruction/helper/CarAccidentHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/cleanup/LayerCleanup.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/hint/HintLowEnergyObserver.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf_ai/navigation/em4Router/em2012/RouteFinder/RouteFinder.h>
#include <qsf_ai/navigation/em4Router/wrapper/EM3Singletons.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Random.h>
#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string EVENT_TAG_CARCRASH_VICTIM("CARCRASH_VICTIM");
	const std::string EVENT_TAG_CARCRASH_BRAWL("CARCRASH_BRAWL");

	// Event hints
	const std::string CHEMICALTRANSPORT_HINT_01 = "EM5_EVENT_CHEMICALTRANSPORT_HINT_01";
	const std::string CHEMICALTRANSPORT_HINT_02 = "EM5_EVENT_CHEMICALTRANSPORT_HINT_02";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TrafficAccident01Event::FREEPLAY_EVENT_ID = qsf::StringHash("em5::TrafficAccident01Event");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool TrafficAccident01Event::canSpawnEntitiesCollisionFree(const qsf::Layer& layer, qsf::Map& map)
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
			if (targetEntityHelper.isCivilRoadVehicle())
			{
				// TODO(mk) Remove this workaround, this tones of workaround kills us everywere.
				// Those got edited without collision :-( As workaroud use the collision component of the undamaged vehicle.
				std::string asset = CarAccidentHelper::getUndamagedEntityName(*targetEntity);
				qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId(qsf::StringHash(asset));
				if (nullptr != prototype)
				{
					collisionComponent = prototype->getComponent<qsf::BulletCollisionComponent>();
				}
			}
			else if (targetEntityHelper.isCivilPerson())
			{
				// Check if it's an injured person outside cars
				if (isInjuredOutsideCars(*targetEntity))
				{
					collisionComponent = targetEntity->getComponent<qsf::BulletCollisionComponent>();
				}
			}

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
	TrafficAccident01Event::TrafficAccident01Event() :
		mAccidentLayerId(qsf::getUninitialized<uint32>()),
		mHasFire(false),
		mIsBrawling(false),
		mChanceFire(0.4f),
		mChanceHardInjury(0.4f),
		mHint01TimeSeconds(20.f),
		mHint03LifeEnergy(0.2f),
		mChemicalTransportHint01WaitTime(qsf::Time::fromSeconds(120.0f))
	{
		// Nothing here
	}

	TrafficAccident01Event::~TrafficAccident01Event()
	{
		// Nothing here
	}

	void TrafficAccident01Event::init(qsf::Layer* accidentLayer)
	{
		if (nullptr != accidentLayer)
			mAccidentLayerId = accidentLayer->getId();
	}

	float TrafficAccident01Event::getChanceFire() const
	{
		return mChanceFire;
	}

	void TrafficAccident01Event::setChanceFire(float chance)
	{
		mChanceFire = chance;
	}

	float TrafficAccident01Event::getChanceHardInjury() const
	{
		return mChanceHardInjury;
	}

	void TrafficAccident01Event::setChanceHardInjury(float chance)
	{
		mChanceHardInjury = chance;
	}

	float TrafficAccident01Event::getHint01WaitTime() const
	{
		return mHint01TimeSeconds;
	}

	void TrafficAccident01Event::setHint01WaitTime(float seconds)
	{
		mHint01TimeSeconds = seconds;
	}

	float TrafficAccident01Event::getHint03LifeEnergy() const
	{
		return mHint03LifeEnergy;
	}

	void TrafficAccident01Event::setHint03LifeEnergy(float energy)
	{
		mHint03LifeEnergy = energy;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool TrafficAccident01Event::onStartup()
	{
		if (qsf::isUninitialized(mAccidentLayerId))
			return false;

		qsf::Layer* layer = getMap().getLayerManager().getLayerById(mAccidentLayerId);
		if (nullptr == layer)
			return false;

		if (!canSpawnEntitiesCollisionFree(*layer, getMap()))
			return false;

		// Show the accident
		setupScene(*layer);

		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void TrafficAccident01Event::onRun()
	{
		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Set start hint observers after the event entity was added
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();
		hintHelper.createAndInitializeGeneralHintAtDamagedVehicle();

		startHintObservers();

		// Fail condition, no one may die.
		ObjectiveHelper(*this).addDeadCivilPersonsFailCondition(1);
		ObjectiveHelper(*this).addDeadUnitPersonsFailCondition(1);
	}

	void TrafficAccident01Event::onShutdown()
	{
		// Ensure everything from the event is cleaned up.
		qsf::Layer* layer = getMap().getLayerManager().getLayerById(mAccidentLayerId);
		if (nullptr != layer)
		{
			LayerCleanup::createLayerCleanup(*layer);
		}
	}

	void TrafficAccident01Event::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		// If the drivers are still brawling
		if (mIsBrawling == true && mLayerBrawler.size() >= 2)
		{
			qsf::Entity* firstBrawler = getMap().getEntityById(mLayerBrawler[0]);
			qsf::Entity* secondBrawler = getMap().getEntityById(mLayerBrawler[1]);
			if (nullptr == firstBrawler || nullptr == secondBrawler)
			{
				QSF_ERROR("TrafficAccident01Event: One or both brawler are unknown. ID  first: " << mLayerBrawler[0] << ", second: " << mLayerBrawler[1], QSF_REACT_NONE);

				// This should not happen but if, set the brawling flag to false so the event don't break
				mIsBrawling = false;
			}
			else
			{
				qsf::Entity* injuredPerson = nullptr;
				qsf::Entity* winnerPerson = nullptr;

				// If brawler are arrested
				if (EntityHelper(*firstBrawler).isArrested() || EntityHelper(*secondBrawler).isArrested())
				{
					// Stop brawling
					mIsBrawling = false;
				}

				// Check if the first brawler is injured
				if (EntityHelper(*firstBrawler).isPersonInjured())
				{
					injuredPerson = firstBrawler;
					winnerPerson = secondBrawler;
				}

				// Check if the second brawler is injured
				else if (EntityHelper(*secondBrawler).isPersonInjured())
				{
					injuredPerson = secondBrawler;
					winnerPerson = firstBrawler;
				}

				// If one brawler is injured apply cheering action to the winner
				if (nullptr != injuredPerson)
				{
					qsf::Time cheeringTime = qsf::Time::fromHours(1.0f);
					qsf::ActionComponent& actionComponent = winnerPerson->getOrCreateComponentSafe<qsf::ActionComponent>();
					AnimationHelper animationHelper(*winnerPerson);
					actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCheer(), true, true, false, qsf::Time::ZERO, cheeringTime);

					mIsBrawling = false;
				}
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool TrafficAccident01Event::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		ObserverHelper observerHelper(*this);
		const EntityHelper entityHelper(targetEntity);

		if (eventSpreadReason == eventspreadreason::Reason::INJURY)
		{
			// An injured has only 20% lifeenergy
			createObserver<HintLowEnergyObserver>(targetEntity.getId(), "EM5_EVENT_CARACCIDENT_01_HINT_03").initialize(mHint03LifeEnergy, HintLowEnergyObserver::HINTENERGYTYPE_LIFE);
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void TrafficAccident01Event::hintCallback(Observer& hintObserver)
	{
		const std::string& observerName = hintObserver.getName();

		//if (hintObserver.getName() == "EM5_EVENT_CARACCIDENT_01_HINT_02")	// TODO(mk) This code looks more familiar
		if (0 == observerName.compare("EM5_EVENT_CARACCIDENT_01_HINT_02"))
		{
			// Only show when there is still an event vehicle burning
			bool hasBurningVehicle = false;
			if (!mLayerVehicles.empty())
			{
				for (uint64 vehicleId : mLayerVehicles)
				{
					qsf::Entity* vehicleEntity = getMap().getEntityById(vehicleId);
					if (nullptr != vehicleEntity && EntityHelper(*vehicleEntity).isBurning())
					{
						hasBurningVehicle = true;
						break;
					}
				}
			}
			if (!hasBurningVehicle)
				return;
		}

		if (!wasHintShown(observerName))
		{
			showHint(observerName, true);
		}
	}

	const qsf::Entity* TrafficAccident01Event::getFocusEntity()
	{
		const qsf::Map& map = getMap();

		const ObjectiveHelper objectiveHelper(*this);

		{ // Are there vehicles to clean up?
			const Objective* cleanupVehiclesObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_CLEANUPVEHICLES);
			if (nullptr != cleanupVehiclesObjective)
			{
				if (cleanupVehiclesObjective->checkAccomplished() == false)
				{
					// Use the event location entity instead of the car, to avoid wrong position when its transported to the hq
					return getEventLocationEntity();
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

		{ // Is there an entity to transport?
			const Objective* transportPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TRANSPORTINJURED);
			if (nullptr != transportPersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(transportPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					// Use the event location entity instead of the person, to avoid wrong position when its transported to the hq
					return getEventLocationEntity();
				}
			}
		}

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

		// None found
		return getEventLocationEntity();
	}

	bool TrafficAccident01Event::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void TrafficAccident01Event::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);
		serializer.serialize(mAccidentLayerId);
		serializer.serialize(mHasFire);
		serializer.serialize(mLayerVehicles);
		serializer.serialize(mLayerContaminationVehicles);
		serializer.serialize(mLayerTrappedPersons);
		serializer.serialize(mLayerBrawler);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TrafficAccident01Event::setupScene(qsf::Layer& accidentLayer)
	{
		// Deactivate the layer first to be sure that the layer is loaded new
		MapHelper::activateLayer(accidentLayer, false);

		// Show the accident
		MapHelper::activateLayer(accidentLayer, true);

		// Injuries for persons inside cars
		std::vector<uint32> injuryPoolTrapped;
		injuryPoolTrapped.push_back(injury::HEAD_BODY_LACERATION.getHash());
		injuryPoolTrapped.push_back(injury::BROKEN_ARM.getHash());
		injuryPoolTrapped.push_back(injury::BROKEN_LEG.getHash());
		injuryPoolTrapped.push_back(injury::INNER_BLEEDING.getHash());
		injuryPoolTrapped.push_back(injury::BROKEN_SKULL.getHash());
		injuryPoolTrapped.push_back(injury::BRAIN_BLEEDING.getHash());
		injuryPoolTrapped.push_back(injury::WHIPLASH.getHash());
		injuryPoolTrapped.push_back(injury::SHOCK.getHash());

		// Injuries for injured persons outside cars (those which got hit by cars)
		std::vector<uint32> injuryPoolOnStreet;
		injuryPoolOnStreet.push_back(injury::HEAD_BODY_LACERATION);
		injuryPoolOnStreet.push_back(injury::BROKEN_ARM);
		injuryPoolOnStreet.push_back(injury::BROKEN_LEG);
		injuryPoolOnStreet.push_back(injury::INNER_BLEEDING);
		injuryPoolOnStreet.push_back(injury::BROKEN_SKULL);
		injuryPoolOnStreet.push_back(injury::BRAIN_BLEEDING);

		// Check all entities in the accident layer
		qsf::Layer::EntityIdSet entities = accidentLayer.getEntityIds();	// Careful, don't use a reference as layer is modified in the loop
		for (uint64 entityId : entities)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(entityId);
			if (nullptr != targetEntity)
			{
				EntityHelper targetEntityHelper(*targetEntity);
				if (targetEntityHelper.isCivilRoadVehicle())
				{
					const bool startFire = qsf::Random::getRandomChance(mChanceFire);
					qsf::Entity& replacementVehicleEntity = CarAccidentHelper::prepareAccidentCar(*targetEntity, accidentLayer, *this, startFire);
					mHasFire |= startFire;

					mLayerVehicles.push_back(replacementVehicleEntity.getId());

					// Use the first vehicle as event location position
					if (!isEventLocationEntitySet())
					{
						createEventLocationEntity(replacementVehicleEntity);
					}

					// Check if there is a contamination
					if (isContaminationVehicle(replacementVehicleEntity))
					{
						mLayerContaminationVehicles.push_back(replacementVehicleEntity.getId());
					}
				}

				else if (targetEntityHelper.isCivilPerson())
				{
					// Check if it's an injured person outside cars
					if (isInjuredOutsideCars(*targetEntity))
					{
						// Injure person
						EventIdComponent::addToEvent(*targetEntity, *this, eventspreadreason::EVENT_SPECIFIC);

						const uint32 injury(injuryPoolOnStreet[qsf::Random::getRandomSize(0, injuryPoolOnStreet.size() - 1)]);
						targetEntity->getComponentSafe<HealthComponent>().applyInjuryById(injury, this, false);
					}

					// Check if its an brawler
					else if (isBrawler(*targetEntity))
					{
						mLayerBrawler.push_back(targetEntity->getId());
					}

					// Else the person is trapped inside a vehicle
					else
					{
						mLayerTrappedPersons.push_back(targetEntity->getId());
					}
				}
			}
		}

		// Start brawl
		if (mLayerBrawler.size() >= 2)
		{
			qsf::Entity* firstBrawler = getMap().getEntityById(mLayerBrawler[0]);
			qsf::Entity* secondBrawler = getMap().getEntityById(mLayerBrawler[1]);

			if (nullptr != firstBrawler && nullptr != secondBrawler)
			{
				// Set the navigation map id to 1, to avoid that the brawler only walks on sidewalk
				firstBrawler->getComponentSafe<qsf::ai::NavigationComponent>().setActive(true);
				firstBrawler->getComponentSafe<qsf::ai::NavigationComponent>().setNativePrimaryNavigationMapId(1);

				attackTarget(*firstBrawler, *secondBrawler);
				mIsBrawling = true;
			}
		}

		// Start contamination
		if (mLayerContaminationVehicles.size() >= 1)
		{
			qsf::Entity* contaminationVehicle = getMap().getEntityById(mLayerContaminationVehicles[0]);
			QSF_ASSERT(nullptr != contaminationVehicle, "TrafficAccident01Event::setupScene can't find entity by ID: " << mLayerContaminationVehicles[0], return);

			ContaminationComponent* contaminationComponent = contaminationVehicle->getComponent<ContaminationComponent>();
			QSF_ASSERT(nullptr != contaminationComponent, "TrafficAccident01Event::setupScene contaminationVehicle has no ContaminationComponent", return);

			// No need to start contamination simulation manual here because its already started when fire simulation started
			EntityHelper(*contaminationVehicle).startFire(this);

			// Injure trapped persons
			for (uint64 entityId : mLayerTrappedPersons)
			{
				qsf::Entity* trappedPersons = getMap().getEntityById(entityId);

				// Heal all injuries first so that they can be contaminated
				trappedPersons->getOrCreateComponentSafe<HealthComponent>().healAllInjuries();

				// Apply injury
				// Hard injury
				if (qsf::Random::getRandomChance(mChanceHardInjury))
				{
					trappedPersons->getOrCreateComponentSafe<HealthComponent>().injurePersonByEventById(injury::CHEMICAL_CONTAMINATION_SHORTAGE_OF_BREATH, this);
				}

				// Not so hard injury
				else
				{
					trappedPersons->getOrCreateComponentSafe<HealthComponent>().injurePersonByEventById(injury::CHEMICAL_CONTAMINATION_BREATH_BURN, this);
				}

				// TODO(db) Maybe we should use "injurePersonByContamination" instead of "injurePersonByEventById"
			}
		}
	}

	void TrafficAccident01Event::attackTarget(qsf::Entity& attackerEntity, qsf::Entity& targetEntity)
	{
		// Attacker
		{
			qsf::ActionComponent& actionComponent = attackerEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			// Move to target
			qsf::ai::ReachObjectGoal* reachObjectGoal = new qsf::ai::ReachObjectGoal(attackerEntity, targetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID);
			actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(reachObjectGoal, MovementModes::MOVEMENT_MODE_RUN);

			// Start fight
			actionComponent.pushAction<ReachVictimAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(targetEntity);
			actionComponent.pushAction<CloseQuartersCombatAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(targetEntity, weapon::FIST, weapon::FIST, 0, 0.2f, 0.02f);
		}

		// Victim
		{
			// Let the victim wait to better reach it
			qsf::ActionComponent& victimActionComponent = targetEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
			victimActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(15.0f));
		}

		// Add gangster flags
		{
			// Add gangster flag for first brawler
			PersonComponent* personComponent = attackerEntity.getComponent<PersonComponent>();
			QSF_CHECK(nullptr != personComponent, "Entity does not have a person component and thus cannot be turned into a gangster!", return);
			personComponent->setGangsterFlag(true);

			// Add gangster tag for second brawler
			personComponent = targetEntity.getComponent<PersonComponent>();
			QSF_CHECK(nullptr != personComponent, "Entity does not have a person component and thus cannot be turned into a gangster!", return);
			personComponent->setGangsterFlag(true);
		}

		{
			// Set event IDs
			EventIdComponent::addToEvent(attackerEntity, *this, eventspreadreason::GANGSTER);
			EventIdComponent::addToEvent(targetEntity, *this, eventspreadreason::GANGSTER);
		}
	}


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	bool TrafficAccident01Event::isInjuredOutsideCars(const qsf::Entity& personEntity)
	{
		// Check if it's an injured person outside cars
		qsf::game::EventTagComponent* tagComponent = personEntity.getComponent<qsf::game::EventTagComponent>();
		if (tagComponent)
		{
			if (tagComponent->hasTag(EVENT_TAG_CARCRASH_VICTIM))
			{
				return true;
			}
		}

		// Check also MetadataComponent for tags
		{
			// HACK: Leveldesign did not use EventTagComponent but used the tag in the MetaDataComponent instead.
			//		 Only 2-3 persons checked here, so we check MetadataComponent instead of reworking all the layers in the editor.
			qsf::MetadataComponent& metadataComponent = personEntity.getComponentSafe<qsf::MetadataComponent>();
			if (metadataComponent.hasTag(EVENT_TAG_CARCRASH_VICTIM))
			{
				return true;
			}
		}

		return false;
	}

	bool TrafficAccident01Event::isBrawler(const qsf::Entity& personEntity)
	{
		// Check if it's an person that brawls
		qsf::game::EventTagComponent* tagComponent = personEntity.getComponent<qsf::game::EventTagComponent>();
		if (tagComponent)
		{
			if (tagComponent->hasTag(EVENT_TAG_CARCRASH_BRAWL))
			{
				return true;
			}
		}

		// Check also MetadataComponent for tags
		{
			// HACK: Leveldesign did not use EventTagComponent but used the tag in the MetaDataComponent instead.
			//		 Only 2-3 persons checked here, so we check MetadataComponent instead of reworking all the layers in the editor.
			qsf::MetadataComponent& metadataComponent = personEntity.getComponentSafe<qsf::MetadataComponent>();
			if (metadataComponent.hasTag(EVENT_TAG_CARCRASH_BRAWL))
			{
				return true;
			}
		}

		return false;
	}

	bool TrafficAccident01Event::isContaminationVehicle(const qsf::Entity& vehicleEntity)
	{
		// Check if vehicle has contamination component
		return (nullptr != vehicleEntity.getComponent<ContaminationComponent>());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TrafficAccident01Event::startHintObservers()
	{
		// This hints only work for one contaminated vehicle
		if (mLayerContaminationVehicles.size() >= 1)
		{
			// hint 01: contamination vehicle is still burning
			createGeneralObserver<HintMessageGeneralObserver>(CHEMICALTRANSPORT_HINT_01).initialize(
				qsf::MessageConfiguration(Messages::EM5_OBJECT_STOP_BURNING, mLayerContaminationVehicles[0]), mChemicalTransportHint01WaitTime, false);

			// hint 02: after vehicle is extinguished
			createGeneralObserver<HintMessageGeneralObserver>(CHEMICALTRANSPORT_HINT_02).initialize(
				qsf::MessageConfiguration(Messages::EM5_OBJECT_STOP_BURNING, mLayerContaminationVehicles[0]));
		}

		// Hints for accidents with fire
		if (mHasFire)
		{
			// Start a hint when a vehicle burns and player has not given a command to extinguish the fire after x seconds
			createGeneralObserver<HintMessageGeneralObserver>("EM5_EVENT_CARACCIDENT_01_HINT_01").initialize(
				qsf::MessageConfiguration(Messages::EM5_ACTION_EXTINGUISH_FIRE_STARTED),
				qsf::Time::fromSeconds(mHint01TimeSeconds),
				false);

			// Start a hint when the player has ordered a towcar and there is still a vehicle burning
			createGeneralObserver<HintMessageGeneralObserver>("EM5_EVENT_CARACCIDENT_01_HINT_02").initialize(
				qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_TECH_TOWCAR));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
