// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/helper/CarAccidentHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/health/injury/Injuries.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/prototype/helper/SlotHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{

		std::vector<DoorComponent*> getDoorsOfEntity(qsf::Entity& entity)
		{
			std::vector<DoorComponent*> result;

			qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				for (const qsf::LinkComponent* childLinkComponent : linkComponent->getChildLinks())
				{
					DoorComponent* doorComponent = childLinkComponent->getEntity().getComponent<DoorComponent>();
					if (nullptr != doorComponent)
					{
						result.push_back(doorComponent);
					}
				}
			}
			return result;
		}

	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity& CarAccidentHelper::prepareAccidentCar(qsf::Entity& vehicleEntity, FreeplayEvent& freeplayEvent, bool startFire, bool replaceVehicle)
	{
		qsf::Layer& layer = QSF_MAKE_REF(EntityHelper(vehicleEntity).getLayer());
		return prepareAccidentCar(vehicleEntity, layer, freeplayEvent, startFire, replaceVehicle);
	}

	qsf::Entity& CarAccidentHelper::prepareAccidentCar(qsf::Entity& vehicleEntity, qsf::Layer& layer, FreeplayEvent& freeplayEvent, bool startFire, bool replaceVehicle)
	{
		// Injuries for persons inside cars
		static const std::vector<uint32> injuryPoolTrapped =
		{
			injury::HEAD_BODY_LACERATION.getHash(),
			injury::BROKEN_ARM.getHash(),
			injury::BROKEN_LEG.getHash(),
			injury::INNER_BLEEDING.getHash(),
			injury::BROKEN_SKULL.getHash(),
			injury::BRAIN_BLEEDING.getHash(),
			injury::WHIPLASH.getHash(),
			injury::SHOCK.getHash()
		};

		// Wrong editing (missing fire and collision components), so we have to replace vehicle by another one
		qsf::Entity& replacementVehicleEntity = replaceVehicle ? replaceVehicleEntity(vehicleEntity) : vehicleEntity;

		// Move entity and all of its children into the right layer
		{
			qsf::EntityVectorScratchBuffer entities;
			qsf::LinkComponent::getConnectedLinkGroup(replacementVehicleEntity, 0xff, true, entities);
			for (qsf::Entity* entity : entities)
			{
				layer.registerEntityById(entity->getId());
			}
		}

		VehicleComponent* vehicleComponent = replacementVehicleEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			if (startFire)
			{
				EntityHelper(replacementVehicleEntity).startFire(&freeplayEvent);
			}
			else
			{
				// Mark this as an dynamic vehicle, important for right cleanup behavior
				vehicleComponent->setIsParkingEntity(false);

				// Set damage level
				vehicleComponent->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);

				// Tell everyone that the vehicle is now damaged
				EventIdComponent::addToEvent(replacementVehicleEntity, freeplayEvent, eventspreadreason::DAMAGE_VEHICLE);
			}

			// Injure and trap the persons in the car
			vehicleComponent->trapPersons(&freeplayEvent, injuryPoolTrapped);
		}

		return replacementVehicleEntity;
	}

	qsf::Entity& CarAccidentHelper::replaceVehicleEntity(qsf::Entity& vehicleEntity)
	{
		const uint64 vehicleEntityId = vehicleEntity.getId();

		VehicleComponent* vehicleComponent = vehicleEntity.getComponent<VehicleComponent>();
		if (nullptr == vehicleComponent)
		{
			QSF_WARN("Vehicle " << vehicleEntityId << " has no vehicle component", QSF_REACT_NONE);
			return vehicleEntity;
		}

		// Find the asset name of the entity which should be replaced
		const std::string assetName = getUndamagedEntityName(vehicleEntity);

		// Create the replacement vehicle
		qsf::Entity* replacementVehicle = MapHelper(vehicleEntity.getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash(assetName));
		QSF_CHECK(replacementVehicle != nullptr, "TrafficAccident01Event::setupScene(): Could not create the vehicle " + assetName, return vehicleEntity);

		// Move contained persons from old vehicle to the new one
		qsf::EntityVectorScratchBuffer entitiesInVehicle;
		getPersonEntitiesInVehicle(*vehicleComponent, entitiesInVehicle);
		for (qsf::Entity* personEntity : entitiesInVehicle)
		{
			EntityHelper personEntityHelper(*personEntity);
			personEntityHelper.leaveContainer();
			personEntityHelper.enterContainer(*replacementVehicle, container::CONTAINERTYPE_PASSENGER);
		}

		// Position new vehicle at the same place the old one had
		const qsf::TransformComponent& origTransformComponent = vehicleEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& replacementTransformComponent = replacementVehicle->getOrCreateComponentSafe<qsf::TransformComponent>();
		replacementTransformComponent.setTransform(origTransformComponent.getTransform());

		// Keep vehicle tint
		qsf::compositing::TintableMeshComponent* origTintableMeshComponent = vehicleEntity.getComponent<qsf::compositing::TintableMeshComponent>();
		qsf::compositing::TintableMeshComponent* replacementTintableMeshComponent = replacementVehicle->getComponent<qsf::compositing::TintableMeshComponent>();
		if (nullptr != origTintableMeshComponent && nullptr != replacementTintableMeshComponent)
		{
			replacementTintableMeshComponent->setTintPalette(origTintableMeshComponent->getTintPalette());
			replacementTintableMeshComponent->setTintPaletteIndex(origTintableMeshComponent->getTintPaletteIndex());
		}

		// Keep edited doors (especially because door offsets are edited often per instance)
		{
			// Throw away all doors in replacement vehicle
			const std::vector<DoorComponent*> newDoors = getDoorsOfEntity(*replacementVehicle);
			for (DoorComponent* doorComponent : newDoors)
			{
				doorComponent->getEntity().getMap().destroyEntityById(doorComponent->getEntityId());
			}

			VehicleComponent& newVehicleComponent = replacementVehicle->getComponentSafe<VehicleComponent>();
			newVehicleComponent.VehicleDoorsArray.getVariable().clear();

			// Move new doors
			const std::vector<DoorComponent*> oldDoors = getDoorsOfEntity(vehicleEntity);
			for (DoorComponent* doorComponent : oldDoors)
			{
				qsf::LinkComponent& doorLink = doorComponent->getPrototype().getComponentSafe<qsf::LinkComponent>();
				doorLink.unlinkFromParent();
				doorLink.linkToParent(*replacementVehicle);

				// Update slot
				newVehicleComponent.VehicleDoorsArray.getVariable().push_back(doorComponent->getPrototype().getId());
			}

			// Vehicle's internal doors map needs an update now
			newVehicleComponent.precalculateDoorsToEntitiesMap();
		}

		// Things get a bit complicated if vehicle was a child (for example, child of a burying object like a broken tree)
		qsf::Entity* parent = nullptr;
		{
			qsf::LinkComponent* linkComponent = vehicleEntity.getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				parent = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			}
		}

		// Special case: Chemical transport
		ContaminationComponent* contaminationComponent = vehicleEntity.getComponent<ContaminationComponent>();
		if (nullptr != contaminationComponent)
		{
			// Create a new contamination component
			replacementVehicle->cloneComponent(*contaminationComponent, true);

			// Copy the particle effects to the new contamination component
			for (size_t index = 0; index < contaminationComponent->CloudArray.size(); ++index)
			{
				uint64 entityId = contaminationComponent->CloudArray.get(index);

				// Find the link that is part of ContaminationComponent
				// TODO(sw) This might not work in multiplay thus the contamination cloud optical effect won't be seen on client side
				qsf::LinkComponent* vehicleLinkComponent = vehicleEntity.getComponent<qsf::LinkComponent>();
				for (qsf::LinkComponent* childLink : vehicleLinkComponent->getChildLinks())
				{
					if (entityId == childLink->getEntity().getId())
					{
						childLink->unlinkFromParent();
						childLink->linkToParent(*replacementVehicle, qsf::LinkComponent::GROUP_LINK);
						break;
					}
				}
			}
		}

		// Destroy the old vehicle
		MapHelper::destroyEntity(vehicleEntity);

		// TODO(fw): This code does not work as intended, see todo inside
		//  Use-case was car under fallen tree in EM2017 storm damage event, which was solved by introducing the "replaceVehicle" parameter
/*
		// Link to the same parent again
		if (nullptr != parent)
		{
			// TODO(fw): For some reason, linking makes the vehicle's main mesh invisible
			qsf::LinkComponent& linkComponent = replacementVehicle->getOrCreateComponentSafe<qsf::LinkComponent>();
			//linkComponent.linkToParent(*parent);

			// Update all slots
			std::unordered_map<uint64, uint64> uniqueIdMap;
			uniqueIdMap.emplace(vehicleEntityId, replacementVehicle->getId());
			qsf::SlotHelper::replaceLinkAndSlotContentsInPrototype(*parent, uniqueIdMap);
		}
*/
		return *replacementVehicle;
	}

	std::string CarAccidentHelper::getUndamagedEntityName(qsf::Entity& damagedEntity)
	{
		// Find the asset name of damagedEntity
		qsf::MetadataComponent& metadataComponent = damagedEntity.getComponentSafe<qsf::MetadataComponent>();
		const std::string assetName(metadataComponent.getBasePrefab().getLocalAssetName());

		// Remove the "_dmg" ending in the name to get the name of the new asset
		size_t newEnd = assetName.rfind("_dmg");
		QSF_CHECK(newEnd != std::string::npos, "TrafficAccident01Event::getUndamagedEntityName(): Some vehicles placed are not _dmg ", return assetName);
		return assetName.substr(0, newEnd);
	}

	void CarAccidentHelper::getPersonEntitiesInVehicle(VehicleComponent& vehicleComponent, std::vector<qsf::Entity*>& entities)
	{
		for (auto iterator : vehicleComponent.getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& linkedEntity = containerLink.getTargetEntity();

			if (EntityHelper(linkedEntity).isEntityPerson())
			{
				entities.push_back(&linkedEntity);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
