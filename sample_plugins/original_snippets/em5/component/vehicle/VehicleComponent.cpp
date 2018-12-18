// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/action/base/UnloadPersonFromVehicleAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/base/GameplayAssets.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/component/vehicle/parts/WheelComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/health/action/InjuredAction.h"
#include "em5/logic/AtmosphericRadioMessageHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/player/Player.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/GameCounters.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/AvoidThreatsGoal.h>
#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/audio/AudioManager.h>
#include <qsf/base/StringParser.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/prototype/PrototypeManager.h>
#include <qsf/prototype/helper/PrototypeManagerHelper.h>
#include <qsf/prototype/helper/SlotHelper.h>
#include <qsf/plugin/QsfJobs.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/asset/Asset.h>
#include <qsf/math/Random.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreEntity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Detail implementation                                 ]
	//[-------------------------------------------------------]
	namespace detail
	{
		const float MATERIALCHECK_UPDATE_FREQUENCY = 0.1f;		// Time in seconds between material checks
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 VehicleComponent::COMPONENT_ID = qsf::StringHash("em5::VehicleComponent");

	const uint32 VehicleComponent::CATEGORY_PASSENGER = 0;
	const uint32 VehicleComponent::CATEGORY_PERSONNEL = 1;
	const uint32 VehicleComponent::CATEGORY_VEHICLE = 2;


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	std::vector<uint64> VehicleComponent::EMPTY_ENTITY_VECTOR;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void VehicleComponent::setSpecialEntity(uint64 specialEntityId)
	{
		mSpecialEntityId = specialEntityId;
	}

	void VehicleComponent::setMainDoorType(DoorComponent::DoorType mainDoorType)
	{
		mMainDoorType = mainDoorType;
	}

	void VehicleComponent::openDoor(DoorComponent::DoorType doorType, bool open)
	{
		if (mDoorTypeToEntityIdsMap.empty())
		{
			precalculateDoorsToEntitiesMap();
		}

		// Search for all doors of the given type
		auto iterator = mDoorTypeToEntityIdsMap.find(doorType);
		if (iterator != mDoorTypeToEntityIdsMap.end())
		{
			// Update doors
			const qsf::Map& map = getEntity().getMap();
			for (uint64 entityId : iterator->second)
			{
				const qsf::Entity* doorEntity = map.getEntityById(entityId);
				if (nullptr != doorEntity)
				{
					DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
					if (nullptr != doorComponent)
					{
						// Open/ Close the door
						if (open)
						{
							doorComponent->openDoor();
						}
						else
						{
							doorComponent->closeDoor();
						}
					}
				}
			}
		}
	}

	bool VehicleComponent::areAllDoorsOfTypeInState(DoorComponent::DoorType doorType, DoorComponent::DoorState doorState, float percent) const
	{
		if (mDoorTypeToEntityIdsMap.empty())
		{
			precalculateDoorsToEntitiesMap();
		}

		// Search for all doors of the given type
		auto iterator = mDoorTypeToEntityIdsMap.find(doorType);
		if (iterator != mDoorTypeToEntityIdsMap.end())
		{
			// Check all doors
			const qsf::Map& map = getEntity().getMap();
			for (uint64 entityId : iterator->second)
			{
				const qsf::Entity* doorEntity = map.getEntityById(entityId);
				if (nullptr != doorEntity)
				{
					const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
					if (nullptr != doorComponent && doorComponent->getDoorState() != doorState)
					{
						if (percent != 1.0f)
						{
							// Detect if door is "nearly" in his final state
							// Detect opening door
							if (doorState == DoorComponent::DoorState::DOOR_OPEN
								&& doorComponent->getDoorState() == DoorComponent::DoorState::DOOR_OPENING && doorComponent->getDoorStateInPercent() >= percent)
							{
								continue;
							}
							// Detect closing door
							if (doorState == DoorComponent::DoorState::DOOR_CLOSED
								&& doorComponent->getDoorState() == DoorComponent::DoorState::DOOR_CLOSING && doorComponent->getDoorStateInPercent() >= percent)
							{
								continue;
							}
						}
						// At least one door is in the wrong state
						return false;
					}
				}
			}
		}

		// All doors are in the checked state
		return true;
	}

	bool VehicleComponent::getFirstDoorTransformByType(uint32 doorType, qsf::Transform& outDoorTransform) const
	{
		const qsf::TransformComponent& vehicleTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();

		std::vector<uint64> doorIds = getVehicleDoorIdsByType(static_cast<DoorComponent::DoorType>(doorType));
		if (!doorIds.empty())
		{
			const qsf::Entity* doorEntity = getEntity().getMap().getEntityById(doorIds.front());
			if (nullptr != doorEntity)
			{
				const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
				if (nullptr != doorComponent)
				{
					// Door component tells us the correct transformation to use
					doorComponent->getDoorWorldSpaceTransform(outDoorTransform, &vehicleTransformComponent.getTransform());
					return true;
				}
			}
		}

		// Fallback
		outDoorTransform = vehicleTransformComponent.getTransform();
		return false;
	}

	qsf::Entity* VehicleComponent::findNearestDoor(DoorComponent::DoorType doorType, const glm::vec3& targetPoint) const
	{
		qsf::Entity* result = nullptr;
		const std::vector<uint64>& doors = getVehicleDoorIdsByType(doorType);
		if (!doors.empty())
		{
			const qsf::Map& map = getEntity().getMap();
			float nearestDoorSquaredDistance = FLT_MAX;
			for (uint64 doorId : doors)
			{
				qsf::Entity* doorEntity = map.getEntityById(doorId);
				if (doorEntity != nullptr)
				{
					const qsf::TransformComponent* doorTransformComponent = doorEntity->getComponent<qsf::TransformComponent>();
					if (doorTransformComponent != nullptr)
					{
						const float currentSquaredDistance = glm::distance2(doorTransformComponent->getPosition(), targetPoint);
						if (currentSquaredDistance < nearestDoorSquaredDistance)
						{
							nearestDoorSquaredDistance = currentSquaredDistance;
							result = doorEntity;
						}
					}
				}
			}
		}

		return result;
	}

	uint64 VehicleComponent::getDriverVehicleDoorId() const
	{
		auto searchResult = getVehicleDoorIdsByType(DoorComponent::DOORTYPE_DRIVER);
		if (searchResult.empty() == false)
		{
			return searchResult.front();
		}

		// Error!
		QSF_ERROR("VehicleComponent::getVehicleDoorDriverDoor() Can't find driver door entity " << getEntityId() << " in road vehicle component. Check the prefab for editing mistakes.", QSF_REACT_NONE);
		return qsf::getUninitialized<uint64>();
	}

	void VehicleComponent::closeAllDoors()
	{
		const qsf::Map& map = getEntity().getMap();
		for (uint64 doorEntityId : mVehicleDoorsIds)
		{
			// Get the entity
			const qsf::Entity* doorEntity = map.getEntityById(doorEntityId);
			if (nullptr != doorEntity)
			{
				DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
				if (nullptr != doorComponent)
				{
					doorComponent->closeDoor();
				}
			}
		}
	}

	const std::vector<uint64>& VehicleComponent::getVehicleDoorIdsByType(DoorComponent::DoorType doorType) const
	{
		if (mDoorTypeToEntityIdsMap.empty())
		{
			precalculateDoorsToEntitiesMap();
		}
		auto searchResult = mDoorTypeToEntityIdsMap.find(doorType);
		if (searchResult != mDoorTypeToEntityIdsMap.end())
		{
			return searchResult->second;
		}

		// Didn't find entities with this type
		return EMPTY_ENTITY_VECTOR;
	}

	void VehicleComponent::personsFleeFromFire(bool allowSpawning)
	{
		const bool isSquadVehicle = nullptr != getEntity().getComponent<CommandableComponent>();

		// Cycle through all entities in the container
		//  -> Do this in a separate step before actually handling the entities, so we don't run into problems because of manipulation of the iterated list
		qsf::EntityVectorScratchBuffer personsToExit;
		for (auto& iterator : getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& containedEntity = containerLink.getTargetEntity();

			// Don't allow vehicles to leave the container (eg the motorboat from the motorboat transporter)
			if (!EntityHelper(containedEntity).isEntityPerson())
				continue;

			// Only check for injured persons, when not a squad vehicle (otherwise a transported patient by the RTW would be "lost")
			if (!isSquadVehicle)
			{
				HealthComponent* healthComponent = containedEntity.getComponent<HealthComponent>();
				if (nullptr != healthComponent && healthComponent->isInjured())
				{
					// Person is injured (trapped before) and can't leave the vehicle by themself
					continue;
				}
			}

			personsToExit.push_back(&containedEntity);
		}

		if (allowSpawning && personsToExit.empty())
		{
			// In case there's no person inside, spawn one
			const qsf::TransformComponent& vehicleTransformComponent = getEntity().getComponentSafe<qsf::TransformComponent>();
			qsf::Entity* spawnedEntity = SpawnPointComponent::spawnRandomAt(vehicleTransformComponent.getTransform(), getEntity().getMap(), assets::SPAWNPOOL_DEFAULT_PERSON, false);
			if (nullptr != spawnedEntity)
			{
				// Set the person inside the vehicle
				EntityHelper(*spawnedEntity).enterContainer(getEntity(), container::CONTAINERTYPE_PASSENGER);
				personsToExit.push_back(spawnedEntity);
			}
			else
			{
				QSF_ERROR("Unable to spawn person exiting vehicle " << getEntityId(), QSF_REACT_NONE);
			}
		}

		qsf::Transform placeTransform;

		// Get the door position
		getFirstDoorTransformByType(DoorComponent::DOORTYPE_CODRIVER, placeTransform);

		// For leaving the vehicle, we rotate at 180°
		placeTransform.setRotation(placeTransform.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>()));

		// Exit the entities now
		qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
		float placeOffset = 0;
		qsf::Time spawnDelay = qsf::Time::ZERO;
		for (qsf::Entity* containedEntity : personsToExit)
		{
			glm::vec3 newPosition = placeTransform.getPosition() + (transformComponent->getRotation() * qsf::CoordinateSystem::getIn() * placeOffset);
			placeOffset -= 1.5f;	// TODO(mk) Dummy changing of the position, to avoid stacking on the same place

			// Move them 2 meters away from the vehicle (this avoids cliping when an injured person or the paramedic team is unloaded)
			newPosition += placeTransform.getRotation() * qsf::CoordinateSystem::getIn() * 2.0f;

			EntityHelper containedEntityHelper(*containedEntity);
			const bool isInjured = containedEntityHelper.isPersonInjured();

			// Let person exit ...
			qsf::ActionComponent& actionComponent = containedEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();
			actionComponent.pushAction<UnloadPersonFromVehicleAction>(action::DANGER_URGENT).init(getEntity(), newPosition, placeTransform.getRotation(), spawnDelay, !isInjured, true);
			if (!isInjured)
			{
				 // ... and run for its life, but only for not injured persons
				actionComponent.pushAction<MoveAction>(action::DANGER_IMMINENT).init(new qsf::ai::AvoidThreatsGoal(20.0f, 1), MovementModes::MOVEMENT_MODE_RUN_PANIC);
			}
			else
			{
				// Update custom bounding box to lying again for injured persons
				actionComponent.pushAction<InjuredAction>(action::BLOCKING).init(containedEntity->getOrCreateComponentSafe<HealthComponent>().getState(), false, false);

				qsf::ai::NavigationComponent* navigationComponent = containedEntity->getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setActive(false);
				}
				AnimationHelper(*containedEntity).clearAnimation();
			}

			// Handle civilists and gangster
			PersonComponent* personComponent = containedEntity->getComponent<PersonComponent>();
			if (nullptr != personComponent && personComponent->getHandcuffedFlag())
			{
				// Mark the person again as gangster so that the player can interact with him again (only gangster can be arrested)
				personComponent->setGangsterFlag(true);
			}
			else if (containedEntityHelper.isCivilPerson() && !isInjured)
			{
				containedEntity->getOrCreateComponentById("em5::MoveToDespawnComponent");
			}

			spawnDelay += qsf::Time::fromSeconds(0.2f);	// Increase the spawnDelay for everyone after the first
		}
	}

	void VehicleComponent::trapPersons(FreeplayEvent* freeplayEvent, const std::vector<uint32>& injuryPool)
	{
		// Cycle through all entities in the container and injure them
		for (auto& iterator : getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& containedEntity = containerLink.getTargetEntity();

			if (EntityHelper(containedEntity).isCivilPerson())
			{
				uint32 injuryId;
				if (injuryPool.empty())
				{
					// Fallback as defined in GD concept
					static const uint32 defaultInjuries[4] =
					{
						injury::BURN_3_GRADE,
						injury::BURN_4_GRADE,
						injury::BURN_OF_BREATH,
						injury::SMOKE_POISONING
					};
					injuryId = defaultInjuries[qsf::Random::getRandomUint(0, 3)];
				}
				else
				{
					injuryId = qsf::Random::getAnyOf(injuryPool);
				}

				// If there is a pre-edited injury, take this one in any case; the injury we just selected is only the fallback then
				HealthHelper(containedEntity).injurePersonByInjuryAtStartup(freeplayEvent, injuryId, false);
			}

			// TODO(mk) Whats happen with non civil -> squad or gangster units? They are not invincible
		}
	}

	void VehicleComponent::applyDamage(float damageValue, FreeplayEvent* freeplayEvent)
	{
		if (mInvincible)
			return;

		if (damageValue > 0.0f)
		{
			damageValue *= (1.0f - mDamageResistance);

			if (damageValue <= 0.0f)
				return;
		}

		float oldLifeEnergy = mLifeEnergy;
		mLifeEnergy = glm::clamp(mLifeEnergy - damageValue, 0.0f, 1.0f);

		DamageState shouldBeDamageState = getDamageState();
		if (mLifeEnergy >= 0.7f)
		{
			// Vehicle is intact
			shouldBeDamageState = DAMAGESTATE_INTACT;
		}
		else if (mLifeEnergy > 0.0f)
		{
			// Vehicle is only slightly damaged, but still functional
			shouldBeDamageState = DAMAGESTATE_DAMAGE_LEVEL1;
		}
		else
		{
			// Vehicle is damaged or destroyed, and clearly not functional any more
			if (getDamageState() != DAMAGESTATE_DESTROYED)
			{
				shouldBeDamageState = DAMAGESTATE_DAMAGE_LEVEL2;

				// Start burning
				EntityHelper(getEntity()).startFire(freeplayEvent);
			}
		}

		if (shouldBeDamageState != getDamageState())
		{
			// Set new damage state
			setDamageState(shouldBeDamageState);
		}

		showDamageHint(oldLifeEnergy);
	}

	bool VehicleComponent::canBeRepaired() const
	{
		return mLifeEnergy > 0.f && mLifeEnergy < 1.f && mInternalFlags.isSet(CAN_BE_REPAIRED);
	}

	bool VehicleComponent::getCanBeCut() const
	{
		// Is a recovery by the crane vehicle necessary first?
		if (getMustBeRecovered())
		{
			return false;
		}

		FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != fireComponent && fireComponent->isBurning())
		{
			// If vehicle is burning, it can't be cut (don't care if no fire component)
			return false;
		}

		// Usually we can cut open only a damaged vehicle containing a person.
		// But if the "CAN_BE_CUT" flag is set, no person inside is necessary, and the vehicle can still be intact.
		if (mInternalFlags.isSet(CAN_BE_CUT))
		{
			return true;
		}
		else
		{
			// Knock knock, someone inside?
			if (getLinks().empty())
				return false;

			// Damaged or destroyed?
			return (isDamagedLevel2() || isDestroyed());
		}
	}

	void VehicleComponent::setCanBeCut(bool canBeCut, FreeplayEvent* freeplayEvent)
	{
		mInternalFlags.set(CAN_BE_CUT, canBeCut);

		if (canBeCut && nullptr != freeplayEvent)
		{
			// Add to freeplay event
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);	// TODO(fw): Time for another event spread reason?
		}
	}

	void VehicleComponent::setCanBeRepaired(bool canBeRepaired, FreeplayEvent* freeplayEvent)
	{
		mInternalFlags.set(CAN_BE_REPAIRED, canBeRepaired);

		if (canBeRepaired && nullptr != freeplayEvent)
		{
			// Add to freeplay event
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);	// TODO(fw): Time for another event spread reason?
		}
	}

	qsf::Time VehicleComponent::getCutDuration() const
	{
		return mCutDuration;
	}

	void VehicleComponent::setCutDuration(const qsf::Time& duration)
	{
		mCutDuration = duration;
	}

	void VehicleComponent::setCanBeTowed(bool canBeTowed, FreeplayEvent* freeplayEvent)
	{
		mInternalFlags.set(CAN_BE_TOWED, canBeTowed);

		if (canBeTowed && nullptr != freeplayEvent)
		{
			// Add to freeplay event
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);	// TODO(fw): Time for another event spread reason?
		}

		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, getCanBeTowed() && !getCanReallyNotBeTowed());
	}

	void VehicleComponent::setCanReallyNotBeTowed(bool canReallyNotBeTowed, FreeplayEvent* freeplayEvent)
	{
		mInternalFlags.set(CAN_REALLY_NOT_BE_TOWED, canReallyNotBeTowed);

		if (getCanBeTowed() && nullptr != freeplayEvent)
		{
			// Add to freeplay event
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);	// TODO(fw): Time for another event spread reason?
		}

		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, getCanBeTowed() && !getCanReallyNotBeTowed());
	}

	void VehicleComponent::setMustBeRecovered(bool mustBeRecovered, FreeplayEvent* freeplayEvent)
	{
		mInternalFlags.set(MUST_BE_RECOVERED, mustBeRecovered);

		if (mustBeRecovered && nullptr != freeplayEvent)
		{
			// Add to freeplay event
			EventIdComponent::addToEvent(getEntity(), *freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);	// TODO(fw): Time for another event spread reason?
		}
	}

	void VehicleComponent::hideParticleChildrenAtMapSave()
	{
		const qsf::PrototypeManagerHelper helper(getPrototype().getPrototypeManager());
		helper.hideMultipleParticlesById(mDamagedLevel1ParticleIds);
		helper.hideMultipleParticlesById(mDamagedLevel2ParticleIds);
		helper.hideMultipleParticlesById(mDestroyedParticleIds);
	}

	bool VehicleComponent::isInvincible() const
	{
		return mInvincible;
	}

	void VehicleComponent::setInvincible(bool invincible)
	{
		mInvincible = invincible;
	}

	std::string VehicleComponent::getResistancesAsString() const
	{
		std::vector<std::string> resistancesAsString;
		resistancesAsString.push_back(std::to_string(mFireResistance));
		resistancesAsString.push_back(std::to_string(mDamageResistance));
		return qsf::StringParser::combineString(resistancesAsString);
	}

	void VehicleComponent::setResistancesByString(const std::string& resistances)
	{
		std::vector<std::string> resistancesAsString;
		qsf::StringParser::splitString(resistances, resistancesAsString);

		if (resistancesAsString.size() >= 2)
		{
			mFireResistance = std::stof(resistancesAsString[0]);
			mDamageResistance = std::stof(resistancesAsString[1]);
		}
	}

	void VehicleComponent::setSirenEnabled(bool sirenEnabled)
	{
		if (mIsSirenEnabled != sirenEnabled)
		{
			mIsSirenEnabled = sirenEnabled;

			if (!sirenEnabled)
			{
				// Stop playing siren right now
				mSirenSFX.stop();
				mIsSirenSFXSet = false;  // Not actually necessary, but let's play safe
				mSirenSFX.synchronizeWithMultiplay();
			}
		}
	}

	void VehicleComponent::playSirenOnce()
	{
		playSirenInternal(false);
	}

	void VehicleComponent::playSirenLoop()
	{
		playSirenInternal(true);
	}

	void VehicleComponent::stopSiren()
	{
		mSirenSFX.stop();
		mSirenSFX.synchronizeWithMultiplay();
	}

	void VehicleComponent::stopSirenLoop()
	{
		if (mSirenSFX.isPlaying())
		{
			mSirenSFX.oneShot();
			mSirenSFX.synchronizeWithMultiplay(true);
		}
	}

	void VehicleComponent::precalculateDoorsToEntitiesMap() const
	{
		mDoorTypeToEntityIdsMap.clear();

		const qsf::Map& map = getEntity().getMap();
		for (uint64 doorEntityId : mVehicleDoorsIds)
		{
			// Get the entity
			const qsf::Entity* doorEntity = map.getEntityById(doorEntityId);
			if (nullptr != doorEntity)
			{
				const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
				QSF_CHECK(nullptr != doorComponent, "VehicleComponent::precalculateDoorsToEntitiesMap(): entity in door slot has no door component, error in editing, skip this entity", continue);

				// Add entity ID in the precalculated list
				auto searchResult = mDoorTypeToEntityIdsMap.find(doorComponent->getDoorType());
				if (searchResult != mDoorTypeToEntityIdsMap.end())
				{
					// Extent an existing entry
					searchResult->second.push_back(doorEntityId);
				}
				else
				{
					// Add a new entry
					std::vector<uint64> entityList;
					entityList.push_back(doorEntityId);
					mDoorTypeToEntityIdsMap.emplace(doorComponent->getDoorType(), entityList);
				}
			}
		}
	}

	void VehicleComponent::stopActions() const
	{
		qsf::ActionComponent* actionComponent = getEntity().getComponent<qsf::ActionComponent>();
		if (nullptr != actionComponent)
		{
			actionComponent->clearPlan();
		}
	}

	const float VehicleComponent::getFireResistance() const
	{
		return mFireResistance;
	}

	const float VehicleComponent::getDamageResistance() const
	{
		return mDamageResistance;
	}


	//[-------------------------------------------------------]
	//[ Public virtual methods                                ]
	//[-------------------------------------------------------]
	bool VehicleComponent::getDoorPositions(DoorComponent::DoorType doorType, std::vector<qsf::Transform>& outDoorTransform, const qsf::Transform& worldTransform) const
	{
		// Get the door entities
		std::vector<uint64> doorEntitiesIds = getVehicleDoorIdsByType(doorType);
		if (doorEntitiesIds.empty())
		{
			return false;
		}

		// Take all possible doors
		const qsf::Map& map = getEntity().getMap();
		for (uint64 doorEntityId : doorEntitiesIds)
		{
			const qsf::Entity* doorEntity = map.getEntityById(doorEntityId);
			if (nullptr != doorEntity)
			{
				const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
				QSF_CHECK(nullptr != doorComponent, "VehicleComponent::getDoorPosition(): entity in door slot has no door component, error in editing, skip this entity", continue);

				// Add to the position of the door component
				qsf::Transform newTransform;
				doorComponent->getDoorWorldSpaceTransform(newTransform, &worldTransform);
				outDoorTransform.push_back(newTransform);

				#ifndef ENDUSER
					// Warn if door offset is suspiciously far away from the vehicle
					if (glm::distance2(newTransform.getPosition(), worldTransform.getPosition()) > 25.0f * 25.0f)
					{
						QSF_WARN("Vehicle door " << doorEntity->getId() << " offset at position (" << newTransform.getPosition() << ") is suspiciously far away from the vehicle entity " << getEntityId() << " at position (" << worldTransform.getPosition() << ")", QSF_REACT_NONE);
					}
				#endif
			}
		}

		return true;
	}

	bool VehicleComponent::getDoorPosition(uint32 doorIndex, glm::vec3& outDoorPosition) const
	{
		// TODO(mk) This function is not working anymore, i wish a different api here...
		// - Always return the first door ...

		// Get the door entity
		const uint64 entityId = mDoorTypeToEntityIdsMap.begin()->second.front();
		const qsf::Entity* doorEntity = getEntity().getMap().getEntityById(entityId);
		if (nullptr == doorEntity)
			return false;

		// Get the door component
		const DoorComponent* doorComponent = doorEntity->getComponent<DoorComponent>();
		if (nullptr == doorComponent)
			return false;

		// Add to the position the offset of the door component
		outDoorPosition = doorComponent->getDoorOffsetWorldSpacePosition();

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void VehicleComponent::receiveDamage(float amount, damagetype::DamageType damageType, qsf::Component* sender)
	{
		// Call base implementation
		DamageComponent::receiveDamage(amount, damageType, sender);

		if ((damageType == damagetype::FIRE) && (amount > 0.0f))
		{
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSHEATENERGY_VEHICLE, getEntityId(), EntityHelper(getEntity()).isCivilPerson()));
		}
	}

	bool VehicleComponent::isReadyForCleanup() const
	{
		// TODO(mk) Improve this check, check for player units and if the vehicle has actions or is selected by player etc
		// Vehicle is damaged
		return (!isIntact());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	bool VehicleComponent::onStartup()
	{
		if (!DamageComponent::onStartup())
		{
			return false;
		}

		initializePrefabClearComponentExceptions();

		// Detect the initial prefab state
		const qsf::MetadataComponent* metadataComponent = getEntity().getComponent<qsf::MetadataComponent>();
		if (nullptr != metadataComponent)
		{
			const qsf::Asset* asset = metadataComponent->getBasePrefab().getAsset();
			if (nullptr != asset)
			{
				const std::string& assetName = asset->getName();

				size_t findPosition = assetName.rfind("_combined");
				if (findPosition != std::string::npos)
				{
					mCurrentPrefabState = VehiclePrefabState_Combined;
				}

				findPosition = assetName.rfind("_dmg");
				if (findPosition != std::string::npos)
				{
					mCurrentPrefabState = VehiclePrefabState_Destroyed;
				}
			}
		}

		return true;
	}

	void VehicleComponent::onShutdown()
	{
		// Ensure audio is stopped
		mSirenSFX.stop()
			.asset(qsf::AssetProxy::EMPTY);
		mSirenSFX.synchronizeWithMultiplay();

		// Call base class implementation
		DamageComponent::onShutdown();
	}

	void VehicleComponent::onSetSimulating(bool simulating)
	{
		// Call base implementation
		DamageComponent::onSetSimulating(simulating);

		// Inform container
		qsf::game::PrototypeContainer::enableSimulatingMode(simulating);

		if (simulating)
		{
			mHighlightJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&VehicleComponent::updateHighlightJob, this, _1));

			updateShaderHeatValue(0.75f);

			// Configure fire component(s)
			std::vector<FireComponent*> fireComponents;
			if (FireComponent::getAllFireComponents(getEntity(), fireComponents))
			{
				for (FireComponent* fireComponent : fireComponents)
				{
					fireComponent->setReceivedEnergyFactor(1.0f - mFireResistance);
				}
			}
		}
		else
		{
			// We going out of simulation business check if we need to decrement a game counter
			if (isBurning())
			{
				GameCounters::decrementGameCounter(GameCounters::VEHICLES_BURNING);
			}
		}
	}

	void VehicleComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mVehicleDoorsIds);

		serializer.serialize(mIsOnEmergencyOperations);

		serializer.serialize(mDamagedLevel1ParticleIds);
		serializer.serialize(mDamagedLevel2ParticleIds);
		serializer.serialize(mDestroyedParticleIds);

		mInternalFlags.serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	qsf::Component* VehicleComponent::getMovableObjectRenderingListenerComponent()
	{
		return getEntity().getComponent<qsf::RendererComponent>();
	}

	void VehicleComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		DamageComponent::onObjectRenderingStartup();

		mSecondsPassedBetweenUpdates = detail::MATERIALCHECK_UPDATE_FREQUENCY;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void VehicleComponent::onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments)
	{
		mSecondsPassedBetweenUpdates += jobArguments.getSecondsPassed();

		// To prevent performance issues only check materials after time steps
		if (mSecondsPassedBetweenUpdates > detail::MATERIALCHECK_UPDATE_FREQUENCY)
		{
			mSecondsPassedBetweenUpdates = 0.0f;

			// Now we can update the vehicle because we have all childs visible
			if (getDamageState() == DAMAGESTATE_DAMAGE_LEVEL1 || getDamageState() == DAMAGESTATE_DAMAGE_LEVEL2)
			{
				switchToDamagePrefab(false);
				applyDamageLevel1MaterialToIntactMesh(true);

				updateShaderHeatValue(getDamageState() == DAMAGESTATE_DAMAGE_LEVEL1 ? 0.875f : 1.0f);

				setDamageOverlaysActive(isBurning(), false);
				updateTintableMesh();
			}
		}

		// Call the base implementation
		DamageComponent::onUpdateMovableObjectRenderingJob(jobArguments);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	void VehicleComponent::onSetDamageState(DamageState damageState)
	{
		if (damageState == DAMAGESTATE_INTACT || damageState == DAMAGESTATE_DESTROYED)
		{
			// Extinguish fires
			EntityHelper(getEntity()).stopFire();
		}

		const MapHelper mapHelper(getEntity().getMap());
		mapHelper.showEntityParticleIds(DamagedLevel1Particles.getVariable(), damageState == DAMAGESTATE_DAMAGE_LEVEL1, false);
		mapHelper.showEntityParticleIds(DamagedLevel2Particles.getVariable(), damageState == DAMAGESTATE_DAMAGE_LEVEL2, false);
		mapHelper.showEntityParticleIds(DestroyedParticles.getVariable(),     damageState == DAMAGESTATE_DESTROYED,     false);
	}

	void VehicleComponent::onSetIntact()
	{
		// Turn off fire
		setFireStateIntact();

		switchToDamagePrefab(false);

		applyDamageLevel1MaterialToIntactMesh(false);
		updateShaderHeatValue(0.75f);

		setDamageOverlaysActive(false, true);
		updateTintableMesh();
	}

	void VehicleComponent::onSetDamagedLevel1()
	{
		switchToDamagePrefab(false);

		applyDamageLevel1MaterialToIntactMesh(true);
		updateShaderHeatValue(0.875f);

		setDamageOverlaysActive(isBurning(), false);
		updateTintableMesh();
	}

	void VehicleComponent::onSetDamagedLevel2()
	{
		switchFromCombinedToNormalPrefab();
		switchToDamagePrefab(false);

		applyDamageLevel1MaterialToIntactMesh(true);
		updateShaderHeatValue(1.0f);

		setDamageOverlaysActive(isBurning(), false);
		updateTintableMesh();
	}

	void VehicleComponent::onSetDestroyed()
	{
		stopActions();

		// Turn off fire
		setFireStateDestroyed();

		switchFromCombinedToNormalPrefab();
		switchToDamagePrefab(true);

		// Ensure that material overwrites are gone, or we'll might up with some white-window vehicles
		exchangeDamageMaterials(getEntity(), false);

		setDamageOverlaysActive(true, false);

		updateTintableMesh();
	}

	void VehicleComponent::onSetFireLifePercentage(float fireLifePercentage)
	{
		if (fireLifePercentage == 0.0f)
		{
			// Destroy vehicle
			setDamageState(DAMAGESTATE_DESTROYED);
		}
		else if (fireLifePercentage < 1.0f && getDamageState() < DAMAGESTATE_DAMAGE_LEVEL2)
		{
			// Severly damage vehicle
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL2);
		}

		showHitpointBar();
	}

	void VehicleComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// For burning vehicles, use at least damage state 2
		if (getDamageState() < DAMAGESTATE_DAMAGE_LEVEL2)
		{
			setDamageState(DAMAGESTATE_DAMAGE_LEVEL2);
		}

		// Force the damaged hint
		if (mLifeEnergy > 0.1f)
		{
			mLifeEnergy = 0.0f;
			showDamageHint(0.49f);	// Simulate the step between damaged and destroyed
		}

		stopActions();

		setupFireFadeAnimation();

		GameCounters::incrementGameCounter(GameCounters::VEHICLES_BURNING);
	}

	void VehicleComponent::onFireStopped()
	{
		// Vehicles are always destroyed after fire
		if (getDamageState() != DAMAGESTATE_INTACT)
		{
			setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
		}

		GameCounters::decrementGameCounter(GameCounters::VEHICLES_BURNING);

		StatusOverlayComponent::hideBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_SMALL_HEALTH);
	}

	void VehicleComponent::onExplosion(FireComponent* fireComponent)
	{
		if (fireComponent == nullptr)
		{
			fireComponent = getEntity().getComponent<FireComponent>();
		}

		if (fireComponent != nullptr)
		{
			fireComponent->showOnExplosionFireParticles();
		}

		// Play explosion audio
		AudioProxy audioProxy;
		AudioHelper::playExplosionVehicleSound(audioProxy, getEntity());
		audioProxy.detach();
		audioProxy.synchronizeWithMultiplay();

		// Play atmo hint
		AtmosphericRadioMessageHelper::triggerAtmoHintExplosion(getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void VehicleComponent::updateShaderHeatValue(float heat)
	{
		// TODO(co) Update EMERGENCY 5 heat renderer
		/*
		if (isRunning())
		{
			const FireComponent* fireComponent = getEntity().getComponent<FireComponent>();
			if (nullptr != fireComponent && !fireComponent->isBurning())
			{
				const qsf::MeshComponent* meshComponent = getPrototype().getComponent<qsf::MeshComponent>();
				if (nullptr != meshComponent)
				{
					meshComponent->setOgreCustomParameter(20, glm::vec4(heat, 0.0f, 0.0f, 0.0f));
				}
			}
		}
		*/
	}

	void VehicleComponent::setDamageOverlaysActive(bool smutActive, bool emissiveActive)
	{
		if (isRunning())
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			// Set the state of smut & emissive burn overlays
			const uint64 entityId = getEntityId();
			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// Please, don't apply fire glow, damage etc. to persons (e.g. squad units leaving a burning vehicle)
				const qsf::MovableComponent* movableComponent = linkedEntity->getComponent<qsf::MovableComponent>();
				if (nullptr != movableComponent && entityId == movableComponent->getContainerEntityId() && nullptr == linkedEntity->getComponent<PersonComponent>())
				{
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "ApplySmut", qsf::MaterialPropertyValue::fromBoolean(smutActive));				// Enable smut map
					materialVariationManager.setEntityMaterialPropertyValue(*linkedEntity, "UseEmissiveMap", qsf::MaterialPropertyValue::fromBoolean(emissiveActive));	// Enable emissive map (lights in the vehicle case)
				}
			}
		}
	}

	void VehicleComponent::applyDamageLevel1MaterialToIntactMesh(bool materialActive)
	{
		if (mDamageLevel1MaterialActive != materialActive && isRunning())
		{
			mDamageLevel1MaterialActive = materialActive;

			qsf::EntityVectorScratchBuffer linkedEntities;
			qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
			for (qsf::Entity* linkedEntity : linkedEntities)
			{
				// Exchange the current materials with damage or intact materials
				exchangeDamageMaterials(*linkedEntity, materialActive);
			}
		}
	}

	void VehicleComponent::updateHighlightJob(const qsf::JobArguments& arguments)
	{
		// Only own units should be visible through buildings
		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			const Player* localPlayer = PlayerManager::getLocalPlayer();
			const uint32 playerId = commandableComponent->getPlayerId();
			if (nullptr != localPlayer && (localPlayer->getPlayerIndex() == playerId || qsf::isUninitialized(playerId)))
			{
				commandableComponent->delayedShineThrough();
			}
		}

		mHighlightJobProxy.unregister();
	}

	void VehicleComponent::switchToDamagePrefab(bool prefabActive)
	{
		if (mDamagePrefabActive != prefabActive && isSimulating())
		{
			mDamagePrefabActive = prefabActive;

			const qsf::MetadataComponent* metadataComponent = getEntity().getComponent<qsf::MetadataComponent>();
			if (nullptr != metadataComponent)
			{
				std::string localAssetName = metadataComponent->getBasePrefab().getLocalAssetName();
				mPrefabLocalAssetName = localAssetName;

				// Strip "_combined" postfix if exists
				size_t findPosition = localAssetName.rfind("_combined");
				if (findPosition != std::string::npos)
				{
					localAssetName.erase(findPosition);
				}

				mDamagePrefabLocalAssetName = localAssetName + "_dmg";

				if (prefabActive)
				{
					// Current material has no _dmg suffix -> check if there is a material with suffix
					if (!mDamagePrefabLocalAssetName.empty())
					{
						const qsf::AssetProxy replacementAsset(mDamagePrefabLocalAssetName);
						const uint64 replacementGlobalAssetId = replacementAsset.getGlobalAssetId();
						if (replacementAsset.isValid() && qsf::isInitialized(replacementGlobalAssetId))
						{
							setVehiclePrefab(replacementAsset);
							mCurrentPrefabState = VehiclePrefabState_Destroyed;
						}
					}
				}
				else
				{
					if (!mPrefabLocalAssetName.empty())
					{
						const qsf::AssetProxy replacementAsset(mPrefabLocalAssetName);
						const uint64 replacementGlobalAssetId = replacementAsset.getGlobalAssetId();
						if (replacementAsset.isValid() && qsf::isInitialized(replacementGlobalAssetId))
						{
							setVehiclePrefab(replacementAsset);
							mCurrentPrefabState = VehiclePrefabState_Normal;
						}
					}
				}
			}
		}
	}

	void VehicleComponent::switchFromCombinedToNormalPrefab()
	{
		if (mCurrentPrefabState != VehiclePrefabState_Normal)
		{
			const qsf::MetadataComponent* metadataComponent = getEntity().getComponent<qsf::MetadataComponent>();
			if (nullptr != metadataComponent)
			{
				std::string localAssetName = metadataComponent->getBasePrefab().getLocalAssetName();

				// Strip "_dmg" postfix if exists
				size_t findPosition = localAssetName.rfind("_dmg");
				if (findPosition != std::string::npos)
				{
					localAssetName.erase(findPosition);
				}

				findPosition = localAssetName.rfind("_combined");
				if (findPosition != std::string::npos)
				{
					localAssetName.erase(findPosition);
				}

				if (!localAssetName.empty() && mCurrentPrefabState != VehiclePrefabState_Normal)
				{
					const qsf::AssetProxy replacementAsset(localAssetName);
					if (replacementAsset.isValid() && qsf::isInitialized(replacementAsset.getGlobalAssetId()))
					{
						setVehiclePrefab(replacementAsset);
						mCurrentPrefabState = VehiclePrefabState_Normal;
					}
				}
			}
		}
	}

	void VehicleComponent::initializePrefabClearComponentExceptions()
	{
		// Default implementation is empty by intent
	}

	void VehicleComponent::setVehiclePrefab(const qsf::AssetProxy& prefabAssetProxy)
	{
		// Early out if not running
		if (!isRunning())
			return;

		// --- Overview prefab change for vehicles ---
		//
		// There are two use-cases for changing a vehicle's prefab:
		//   a) Switch to/from damage state
		//   b) Switch to/from combined prefab
		//
		// In both cases, we have to make changes to the main entity. This includes only exchanging the mesh, and we don't touch any other components.
		//
		// Things get more difficult when it comes to the child entities. Some need to be preserved (like fire children and their particles), others
		// must be replaced from the new prefab (like vehicle doors). This is partly sub-class specific.
		// There are some child entities that even require some special handling. This is done inside the "restoreEntitiesFromPrefab" implementation.

		// Get the instance of the prototype
		const uint32 localAssetId = prefabAssetProxy.getLocalAssetId();
		const qsf::Prototype* prototype = QSF_MAINPROTOTYPE.getPrefabByLocalAssetId(localAssetId);
		QSF_CHECK(nullptr != prototype, "QSF: Can't create prefab with asset \"" << localAssetId << "\" skip creating process", return);

		const qsf::Entity& entity = getEntity();
		qsf::Map& map = entity.getMap();
		const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();

		// Get a list of the prefab's prototypes, and a mapping of these to newly generated prototype IDs
		std::unordered_map<uint64, uint64> prototypeIdMap;
		map.buildIdMatchingMapWithGeneratedIds(*prototype, prototypeIdMap);

		if (nullptr != linkComponent)
		{
			// First of all, collect the IDs of child entities we have to preserve
			boost::container::flat_set<uint64> childrenToPreserve;
			{
				// Preserve all contained entities, like trapped persons
				for (auto& iterator : getLinks())
				{
					const qsf::ContainerLink& containerLink = *iterator.second;
					childrenToPreserve.insert(containerLink.getTargetEntity().getId());
				}

				// Preserve all cloud array entities
				ContaminationComponent* contaminationComponent = getEntity().getComponent<ContaminationComponent>();
				if (nullptr != contaminationComponent)
				{
					for (uint64 entityId : contaminationComponent->CloudArray.getVariable())
					{
						childrenToPreserve.insert(entityId);
					}
				}

				// Check child entities for certain components
				for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
				{
					const qsf::Entity& childEntity = childLinkComponent->getEntity();
					for (auto componentId : mComponentExceptionIds)
					{
						if (nullptr != childEntity.getComponentById(componentId))
						{
							childrenToPreserve.insert(childEntity.getId());
							break;
						}
					}
				}
			}

			// Everyone not preserved will be deleted; yes, life is hard
			std::vector<uint64> childrenToRemove;
			{
				for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
				{
					const qsf::Entity& childEntity = childLinkComponent->getEntity();
					if (childrenToPreserve.find(childEntity.getId()) == childrenToPreserve.end())
					{
						childrenToRemove.push_back(childEntity.getId());
					}
				}
			}

			// Farewell my children
			for (uint64 entityId : childrenToRemove)
			{
				// Here destroy all linked childs without additional check
				map.destroyObjectById(entityId);
			}

			// Add the preserved children into the ID matching map
			for (uint64 childId : childrenToPreserve)
			{
				prototypeIdMap.emplace(childId, childId);
			}

			// Add the vehicle's parent into the ID matching map (otherwise the slot helper will remove the link)
			const uint64 parentId = linkComponent->getParentId();
			if (qsf::isInitialized(parentId))
			{
				prototypeIdMap.emplace(parentId, parentId);
			}
		}

		// Restore component-specific entities
		restoreEntitiesFromPrefab(*prototype, prototypeIdMap);

		// Replace mesh
		const qsf::MeshComponent* prototypeMeshComponent = prototype->getComponent<qsf::MeshComponent>();
		qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent && nullptr != prototypeMeshComponent)
		{
			meshComponent->setMesh(prototypeMeshComponent->getMesh());
		}

		// Update slots after changes
		qsf::SlotHelper::replaceLinkAndSlotContentsInPrototype(entity, prototypeIdMap);
	}

	void VehicleComponent::cloneEntitiesFromPrefabByComponentId(qsf::Map& map, const qsf::Entity& parentEntity, const std::vector<uint32>& componentIds, const qsf::LinkComponent& prototypeLinkComponent, std::unordered_map<uint64, uint64>& prototypeIdMap)
	{
		std::vector<uint64> sourceEntityIds;
		std::vector<uint64> dummyDestinationEntityIds;

		for (auto childLink : prototypeLinkComponent.getChildLinks())
		{
			qsf::Prototype& prototype = childLink->getPrototype();
			for (auto componentId : componentIds)
			{
				const qsf::Component* component = prototype.getComponentById(componentId);

				if (qsf::MeshComponent::COMPONENT_ID != componentId)
				{
					if (nullptr != component)
					{
						sourceEntityIds.push_back(component->getPrototypeId());
						break;
					}
				}
				else
				{
					// Some vehicles also use meshes as dummy for now, so we need to restore MeshComponents too, but have to look that we don't restore a wheel or door by mistake!
					const WheelComponent* wheelComponent = prototype.getComponent<WheelComponent>();
					const DoorComponent* doorComponent = prototype.getComponent<DoorComponent>();
					if (nullptr != component && nullptr == doorComponent && nullptr == wheelComponent)
					{
						sourceEntityIds.push_back(component->getPrototypeId());
						break;
					}
				}

			}
		}

		// Restore additional component entities
		if (!sourceEntityIds.empty())
		{
			cloneEntitiesFromPrefab(map, parentEntity, sourceEntityIds, dummyDestinationEntityIds, prototypeLinkComponent, prototypeIdMap);
		}
	}

	void VehicleComponent::cloneEntitiesFromPrefab(qsf::Map& map, const qsf::Entity& parentEntity, const std::vector<uint64>& sourceEntityIds, std::vector<uint64>& destinationEntityIds, const qsf::LinkComponent& prototypeLinkComponent, std::unordered_map<uint64, uint64>& prototypeIdMap)
	{
		destinationEntityIds.reserve(sourceEntityIds.size());

		qsf::MetadataComponent* parentMetadataComponent = parentEntity.getComponent<qsf::MetadataComponent>();
		const uint32 layerId = (nullptr != parentMetadataComponent) ? parentMetadataComponent->getLayerId() : qsf::getUninitialized<uint32>();

		qsf::EntityVectorScratchBuffer newEntities;

		// Look for children which we want to clone
		for (auto prototypeId : sourceEntityIds)
		{
			for (auto& childLink : prototypeLinkComponent.getChildLinks())
			{
				if (prototypeId == childLink->getPrototypeId())
				{
					qsf::Prototype& prototype = childLink->getPrototype();

					// Get entity ID
					const uint64 entityId = prototypeIdMap[prototype.getId()];

					// Create a new empty prototype with an invalid ID
					qsf::Entity* entity = map.createEntity();
					QSF_CHECK(nullptr != entity, "Failed to create entity with ID " << entityId, QSF_REACT_THROW);

					newEntities.push_back(entity);
					destinationEntityIds.push_back(entity->getId());

					// Clone all components
					entity->cloneComponentsFrom(prototype, false);

					// Move entity inside parent's layer
					if (qsf::isInitialized(layerId))
					{
						entity->getOrCreateComponentSafe<qsf::MetadataComponent>().setLayerId(layerId);
					}

					// Flag entity as temporary
					entity->setSerializable(false);

					// Replace all mapped ID in slots and links
					qsf::SlotHelper::replaceLinkAndSlotContentsInPrototype(*entity, prototypeIdMap);
				}
			}
		}

		// Transform our newly created entities to the right position
		const bool running = map.isRunning();
		for (qsf::Entity* entity : newEntities)
		{
			// Store original local transform
			qsf::LinkComponent& linkComponent = entity->getOrCreateComponentSafe<qsf::LinkComponent>();
			const qsf::Transform localTransform = linkComponent.getLocalTransform();

			// Start everything up
			entity->startup(running);

			// Link to parent
			linkComponent.linkToParent(parentEntity, qsf::LinkComponent::HARD_LINK);

			// Restore local transformation
			linkComponent.setLocalTransform(localTransform);
		}
	}

	void VehicleComponent::restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap)
	{
		const qsf::Entity& entity = getEntity();
		qsf::Map& map = entity.getMap();

		const VehicleComponent* prototypeVehicleComponent = prototype.getComponent<VehicleComponent>();
		const qsf::LinkComponent* prototypeLinkComponent = prototype.getComponent<qsf::LinkComponent>();
		if (nullptr != prototypeVehicleComponent && nullptr != prototypeLinkComponent)
		{
			// Doors
			cloneEntitiesFromPrefab(map, entity, prototypeVehicleComponent->mVehicleDoorsIds, mVehicleDoorsIds, *prototypeLinkComponent, prototypeIdMap);
			precalculateDoorsToEntitiesMap();
		}
		else
		{
			QSF_WARN("QSF failed to restore the original prefab of entity ID " << entity.getId(), QSF_REACT_NONE);
		}
	}

	void VehicleComponent::showDamageHint(float oldLifeEnergy) const
	{
		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return;

		uint32 playerIndex = commandableComponent->getPlayerId();

		if (mLifeEnergy <= 0.0f && oldLifeEnergy > 0.0f)
		{
			std::string hintId;

			// Firefighter
			if (commandableComponent->isFireFighterRW())
			{
				hintId = "ID_HINT_RW_DAMAGED_02";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_TLF))
			{
				hintId = "ID_HINT_TLF_DAMAGED_02";
			}
			else if (commandableComponent->isFireFighterDECONP())
			{
				hintId = "ID_HINT_DEKONP_DAMAGED_02";
			}
			else if (commandableComponent->isFireFighterDLK())
			{
				hintId = "ID_HINT_DLK_DAMAGED_02";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT))
			{
				hintId = "ID_HINT_TFMB_DAMAGED_02";
			}
			else if (commandableComponent->isFireFighterBoat())
			{
				hintId = "ID_HINT_MB_DAMAGED_02";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_COMMANDOCAR))
			{
				hintId = "ID_HINT_LKF_DAMAGED_02";
			}

			// Fire plane has no damage

			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE_TRANSPORT))
			{
				hintId = "ID_HINT_TFD_DAMAGED_02";
			}

			// The fire drone has no damage but leaves the map if the tfd is destroyed
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT))
			{
				HintHelper::showHint("ID_HINT_TFT_DAMAGED_02", playerIndex);
				hintId = "ID_HINT_FD_DAMAGED_01";
			}
			// Ambulance
			else if (commandableComponent->isAmbulanceNEF())
			{
				hintId = "ID_HINT_NEF_DAMAGED_02";
			}
			else if (commandableComponent->isAmbulanceRTW())
			{
				hintId = "ID_HINT_RTW_DAMAGED_02";
			}
			else if (commandableComponent->isAmbulanceIMT())
			{
				hintId = "ID_HINT_IMT_DAMAGED_02";
			}
			else if (commandableComponent->isAmbulanceHeli())
			{
				hintId = "ID_HINT_RHC_DAMAGED_02";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOGCAR))
			{
				hintId = "ID_HINT_RHF_DAMAGED_02";
			}
			// Tech
			else if (commandableComponent->isTechTowcar())
			{
				hintId = "ID_HINT_ASF_DAMAGED_02";
			}
			else if (commandableComponent->isTechEngineerCar())
			{
				hintId = "ID_HINT_EKW_DAMAGED_02";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_TECH_CRANE))
			{
				hintId = "ID_HINT_BFZ_DAMAGED_02";
			}
			else if (commandableComponent->isTechHeli())
			{
				hintId = "ID_HINT_BHC_DAMAGED_02";
			}
			// Police
			else if (commandableComponent->isPoliceSTW())
			{
				hintId = "ID_HINT_STW_DAMAGED_02";
			}
			else if (commandableComponent->isPoliceMTW())
			{
				hintId = "ID_HINT_MTW_DAMAGED_02";
			}
			else if (commandableComponent->isPoliceHeli())
			{
				hintId = "ID_HINT_PHC_DAMAGED_02";
			}
			else if (commandableComponent->isPoliceSEKHeli())
			{
				hintId = "ID_HINT_SEKHC_DAMAGED_02";
			}
			// The sekd has no damage but leaves the map if the transport vehicle is destroyed
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
			{
				HintHelper::showHint("ID_HINT_TFSEKD_DAMAGED_02", playerIndex);
				hintId = "ID_HINT_SEKD_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceTSEKR())
			{
				hintId = "ID_HINT_TFSEKR_DAMAGED_02";
			}
			else if (commandableComponent->isPoliceSEKR())
			{
				hintId = "ID_HINT_SEKR_DAMAGED_02";
			}
			else if (commandableComponent->isPoliceDogCar())
			{
				hintId = "ID_HINT_PHF_DAMAGED_02";
			}

			if (!hintId.empty())
			{
				HintHelper::showHint(hintId, playerIndex);
			}

			// Flash minimap symbol
			EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(getEntityId());

			// Play atmo hint
			AtmosphericRadioMessageHelper::triggerAtmoHintVehicleUnitDamage(getEntity());
		}

		// Special handling for flb because it can't be destroyed
		else if (mLifeEnergy <= 0.01f && oldLifeEnergy > 0.01f)
		{
			if (commandableComponent->isFireFighterFLB())
			{
				HintHelper::showHint("ID_HINT_FLB_DAMAGED_02", playerIndex);
			}

			// Flash minimap symbol
			EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(getEntityId());
		}

		else if (mLifeEnergy < 0.5f && oldLifeEnergy >= 0.5f)
		{
			std::string hintId;
			// Firefighter
			if (commandableComponent->isFireFighterRW())
			{
				hintId = "ID_HINT_RW_DAMAGED_01";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_TLF))
			{
				hintId = "ID_HINT_TLF_DAMAGED_01";
			}
			else if (commandableComponent->isFireFighterDECONP())
			{
				hintId = "ID_HINT_DEKONP_DAMAGED_01";
			}
			else if (commandableComponent->isFireFighterDLK())
			{
				hintId = "ID_HINT_DLK_DAMAGED_01";
			}
			else if (commandableComponent->isFireFighterFLB())
			{
				hintId = "ID_HINT_FLB_DAMAGED_01";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT))
			{
				hintId = "ID_HINT_TFMB_DAMAGED_01";
			}
			else if (commandableComponent->isFireFighterBoat())
			{
				hintId = "ID_HINT_MB_DAMAGED_01";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_COMMANDOCAR))
			{
				hintId = "ID_HINT_LKF_DAMAGED_01";
			}
			// Fire plane has no damage

			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE_TRANSPORT))
			{
				hintId = "ID_HINT_TFD_DAMAGED_01";
			}
			// The fd has no damage but leaves the map if the tfd is destroyed

			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT))
			{
				hintId = "ID_HINT_TFT_DAMAGED_01";
			}
			// Ambulance
			else if (commandableComponent->isAmbulanceNEF())
			{
				hintId = "ID_HINT_NEF_DAMAGED_01";
			}
			else if (commandableComponent->isAmbulanceRTW())
			{
				hintId = "ID_HINT_RTW_DAMAGED_01";
			}
			else if (commandableComponent->isAmbulanceIMT())
			{
				hintId = "ID_HINT_IMT_DAMAGED_01";
			}
			else if (commandableComponent->isAmbulanceHeli())
			{
				hintId = "ID_HINT_RHC_DAMAGED_01";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOGCAR))
			{
				hintId = "ID_HINT_RHF_DAMAGED_01";
			}
			// Tech
			else if (commandableComponent->isTechTowcar())
			{
				hintId = "ID_HINT_ASF_DAMAGED_01";
			}
			else if (commandableComponent->isTechEngineerCar())
			{
				hintId = "ID_HINT_EKW_DAMAGED_01";
			}
			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_TECH_CRANE))
			{
				hintId = "ID_HINT_BFZ_DAMAGED_01";
			}
			else if (commandableComponent->isTechHeli())
			{
				hintId = "ID_HINT_BHC_DAMAGED_01";
			}
			// Police
			else if (commandableComponent->isPoliceSTW())
			{
				hintId = "ID_HINT_STW_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceMTW())
			{
				hintId = "ID_HINT_MTW_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceHeli())
			{
				hintId = "ID_HINT_PHC_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceSEKHeli())
			{
				hintId = "ID_HINT_SEKHC_DAMAGED_01";
			}
			// The sekd has no damage but leaves the map if the transport vehicle is destroyed

			else if (commandableComponent->isUnitType(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
			{
				hintId = "ID_HINT_TFSEKD_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceTSEKR())
			{
				hintId = "ID_HINT_TFSEKR_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceSEKR())
			{
				hintId = "ID_HINT_SEKR_DAMAGED_01";
			}
			else if (commandableComponent->isPoliceDogCar())
			{
				hintId = "ID_HINT_PHF_DAMAGED_01";
			}

			if (!hintId.empty())
			{
				HintHelper::showHint(hintId, playerIndex);
			}

			// Flash minimap symbol
			EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(getEntityId());
		}
	}


	void VehicleComponent::showHitpointBar()
	{
		StatusOverlayComponent::BarType barType = StatusOverlayComponent::BAR_TYPE_SMALL_HEALTH;
		bool showOverLay = (DAMAGESTATE_DAMAGE_LEVEL2 == getDamageState());

		// Is entity inside a container?
		EntityHelper entityHelper(getEntity());
		if (nullptr != entityHelper.getContainerEntity())
			showOverLay = false;

		if (showOverLay)
		{
			StatusOverlayComponent::showBarOverlay(getEntity(), barType, getFireLifePercentage());
		}
	}

	void VehicleComponent::playSirenInternal(bool loop)
	{
		// Early out if siren is not enabled at all for this vehicle
		//  -> This is actually a modding community request, we don't use this internally
		if (!mIsSirenEnabled)
			return;

		if (!mIsSirenSFXSet)
		{
			mIsSirenSFXSet = true;

			// Maybe the signal sound property is set
			// TODO(fw): Unfortunately this is not the case for most of our units, but it would be much cleaner to use the property instead of hard-coded solutions (think of the modders!)
			if (mSignalSound.isValid())
			{
				mSirenSFX.asset(mSignalSound)
					.channel(qsf::AudioManager::MIXERCHANNEL_SFX)
					.contextEntity(getEntity())
					.spatial()
					.volume(1.0f);
			}
			else
			{
				// We cannot setup sound fx in onStartup because in onStartup the unit type isn't set yet
				// (This information gets set after the entity and their components are created and started)
				EntityHelper entityHelper(getEntity());
				if (entityHelper.isSquadRoadVehicle())
				{
					const UnitHelper unitHelper(getEntity());

					if (unitHelper.isAmbulanceRoadVehicle())
					{
						AudioHelper::setSirenRescue(mSirenSFX, getEntity());
					}
					else if (unitHelper.isPoliceRoadVehicle())
					{
						AudioHelper::setSirenPolice(mSirenSFX, getEntity());
					}
					else if (unitHelper.isTechEngineerCar() || unitHelper.isTechTowcar() || unitHelper.isTechCrane())
					{
						AudioHelper::setSirenTech(mSirenSFX, getEntity());
					}
					else if (unitHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_RW_HISTORIC))
					{
						AudioHelper::setSirenHistoric(mSirenSFX, getEntity());
					}
					else if (unitHelper.isFireFighterRoadVehicle())
					{
						AudioHelper::setSirenFirefighter(mSirenSFX, getEntity());
					}
					else if (entityHelper.isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FLB))
					{
						AudioHelper::setSirenFirefighterFLB(mSirenSFX, getEntity());
					}
				}
			}
		}

		if (loop)
			mSirenSFX.loop();
		else
			mSirenSFX.oneShot();

		mSirenSFX.play();

		mSirenSFX.synchronizeWithMultiplay();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
