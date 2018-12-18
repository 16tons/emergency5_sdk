// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/DataCacheItemFactory.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/damage/DamageComponent.h"
#include "em5/component/door/DoorComponent.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/FireParticleOptionsComponent.h"
#include "em5/fire/component/WaterjetComponent.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/component/movement/SwingComponent.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/objects/ContaminationCloudComponent.h"
#include "em5/component/objects/EboxComponent.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/network/multiplayer/datacache/datacacheitems/ActionData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BarrierTapeData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoatData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoneLinkData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BoneToBoneLinkData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BuildingData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BuildingOverlayData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/BuryData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DamageData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DlkLadderData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/DoorData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/EventIdData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/FireData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/GameLogicData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/GhostData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HealthData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HelicopterData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/HiddenData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/InventoryData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/LinkData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MovableData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MeshData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/MeshAnimationData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/NavigationData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/ParticleData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/PersonData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/PersonMagnetData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/RescueBusData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/SupportLegData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/StatusOverlayData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/SwingData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/TimeOfDayData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/TransformData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/TreeData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/UsableByEngineerData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/VehicleData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/VehicleCraneData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/WaterCannonData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/WaterjetData.h"
#include "em5/network/multiplayer/datacache/datacacheitems/WeatherData.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/environment/time/TimeOfDayComponent.h>
#include <qsf_game/environment/weather/WeatherComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/nodes/NodesComponent.h>
#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/renderer/component/CameraComponent.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		std::vector<DataCacheItemFactory::Listener*> DataCacheItemFactory::mListeners;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		std::string DataCacheItemFactory::componentIdToString(uint8 componentId)
		{
			TrackedComponentType trackedComponentType = static_cast<TrackedComponentType>(componentId);

			switch (trackedComponentType)
			{
				case TrackedComponentType::Hidden:
					return "Hidden";
				case TrackedComponentType::Transform:
					return "Transform";
				case TrackedComponentType::Movable:
					return "Movable";
				case TrackedComponentType::Fire:
					return "Fire";
				case TrackedComponentType::Damage:
					return "Damage";
				case TrackedComponentType::Health:
					return "Health";
				case TrackedComponentType::Navigation:
					return "AI Navigation";
				case TrackedComponentType::Inventory:
					return "Inventory";
				case TrackedComponentType::Link:
					return "Link";
				case TrackedComponentType::BoneToBoneLink:
					return "BoneToBoneLink";
				case TrackedComponentType::GameLogic:
					return "GameLogic";
				case TrackedComponentType::TimeOfDay:
					return "TimeOfDay";
				case TrackedComponentType::MeshAnimation:
					return "MeshAnimation";
				case TrackedComponentType::VehicleCrane:
					return "VehicleCrane";
				case TrackedComponentType::Waterjet:
					return "Waterjet";
				case TrackedComponentType::BoneLink:
					return "BoneLink";
				case TrackedComponentType::WaterCannon:
					return "WaterCannon";
				case TrackedComponentType::Mesh:
					return "Mesh";
				case TrackedComponentType::Door:
					return "Door";
				case TrackedComponentType::EventId:
					return "EventId";
				case TrackedComponentType::Tree:
					return "Tree";
				case TrackedComponentType::DlkLadder:
					return "DlkLadder";
				case TrackedComponentType::Helicopter:
					return "Helicopter";
				case TrackedComponentType::Swing:
					return "Swing";
				case TrackedComponentType::Boat:
					return "Boat";
				case TrackedComponentType::Action:
					return "Action";
				case TrackedComponentType::Ghost:
					return "Ghost";
				case TrackedComponentType::BuildingOverlay:
					return "BuildingOverlay";
				case TrackedComponentType::StatusOverlay:
					return "StatusOverlay";
				case TrackedComponentType::Particles:
					return "Particles";
				case TrackedComponentType::Building:
					return "Building";;
				case TrackedComponentType::Person:
					return "Person";
				case TrackedComponentType::PersonMagnet:
					return "PersonMagnet";
				case TrackedComponentType::BarrierTape:
					return "BarrierTape";
				case TrackedComponentType::RescueBus:
					return "RescueBus";
				case TrackedComponentType::SupportLegs:
					return "SuppoLegs";
				case TrackedComponentType::Weather:
					return "Weather";
			}

			// Ask listeners (used for modding only)
			for (Listener* listener : mListeners)
			{
				std::string result = listener->componentIdToString(componentId);
				if (!result.empty())
				{
					return result;
				}
			}

			return "unknown";
		}

		bool DataCacheItemFactory::isEntityCacheRelevant(qsf::Entity& entity)
		{
			// TODO(sw) Add here checks to filter out entities which won't change their states due simulation updates -> They are static entities
			//          Or are generally not suitable for updates from the host (e.g. the camera entity)

			// Ask listeners (used for modding only)
			//  -> If any one replies "true", the entity is relevant
			for (Listener* listener : mListeners)
			{
				if (listener->isEntityCacheRelevant(entity))
				{
					return true;
				}
			}

			// All walkable components are not cache relevant
			if (nullptr != entity.getComponent<qsf::WalkableComponent>())
			{
				return false;
			}

			// All node components are not cache relevant
			if (nullptr != entity.getComponent<qsf::NodesComponent>())
			{
				// But only when it doesn't have a fire component/complex fire component
				if (nullptr == entity.getComponent<FireComponent>() && nullptr == entity.getComponent<ComplexFireComponent>())
				{
					return false;
				}
			}

			// If the entity has a movable component then this entity is always cache relevant, regardless if it is a child entity
			if (nullptr != entity.getComponent<qsf::MovableComponent>())
			{
				return true;
			}

			if (nullptr != entity.getComponent<RescueBusComponent>())
			{
				return true;
			}

			if (nullptr != entity.getComponent<SupportLegsComponent>())
			{
				return true;
			}

			{ // Special check for child entities which parent has a fire component but no mesh component
				qsf::Prototype& parentPrototype = qsf::LinkComponent::getTopmostAncestorPrototype(entity, qsf::LinkComponent::SELECT_PARENT);
				if (&parentPrototype != &entity)
				{
					// Entity is a child entity check if the toplevel parent has a fire component
					if (nullptr != parentPrototype.getComponent<FireComponent>())
					{
						// Toplevel parent has a fire component check if this child would be selected as target entity for the status overlay component
						//  -> As a fix for MP disconnect due unknown entity in map cache when a trash group gets burned, add all children
						//  -> The actual problem here is that "StatusOverlayComponent::getEntityOrChildWithMesh" can deliver different children on different machines, so we don't use that one here
						if (entity.getComponent<qsf::MeshComponent>() != nullptr)
						{
							// The status overlay component would be created in this instance instead of the toplevel parent -> add this to the cache
							return true;
						}
					}
				}
			}

			// Check if it is a top level entity (no parent)
			qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				// If parent ID is set and it is a hard link -> no top level entity (Group and soft links are ignored)
				if (qsf::isInitialized(linkComponent->getParentId()) && linkComponent->getParentLinkType() == qsf::LinkComponent::HARD_LINK)
				{
					// Ignore only hard links, when the child has no fire component and no door component and no particles component
					if (nullptr == entity.getComponent<FireComponent>() && nullptr == entity.getComponent<DoorComponent>() && nullptr == entity.getComponent<qsf::ParticlesComponent>())
					{
						return false;
					}
				}
			}

			// Exclude also all cameras
			if (nullptr != entity.getComponent<qsf::CameraComponent>())
			{
				return false;
			}

			// An building is only relevant, when it has a damage and a fire/complex fire component
			if (nullptr != entity.getComponent<BuildingComponent>())
			{
				if ((nullptr == entity.getComponent<FireComponent>() && nullptr == entity.getComponent<ComplexFireComponent>()) || nullptr == entity.getComponent<DamageComponent>())
				{
					// The building entity has no fire or complex fire component and nor damage component
					return false;
				}
			}

			bool includeEntityWithParticle = false;
			if (nullptr != entity.getComponent<qsf::ParticlesComponent>() && nullptr != entity.getComponent<ContaminationCloudComponent>())
			{
				// For entities already in the map only select particle entities which have a contamination cloud
				includeEntityWithParticle = true;
			}

			// Must have a fire, damage, building, health or movable component
			// TODO(sw) add tracking for complex fire?
			return (nullptr != entity.getComponent<FireComponent>() || nullptr != entity.getComponent<DamageComponent>()
					|| nullptr != entity.getComponent<HealthComponent>() || nullptr != entity.getComponent<DoorComponent>()
					|| nullptr != entity.getComponent<BuryComponent>() || includeEntityWithParticle);
		}

		void DataCacheItemFactory::createCacheItemsForEntity(qsf::Entity& entity, ComponentCacheMap& componentCacheMap)
		{
			// Ask listeners (used for modding only)
			for (Listener* listener : mListeners)
			{
				listener->createCacheItemsForEntity(entity, componentCacheMap);
			}

			// Safety check
			qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Movable), std::unique_ptr<DataCacheBase>(new MovableData(entity)));
			}

			// TODO(sw) Is it wise to cache data for all fire components? Wouldn't it be better to react on EM5_FIRECOMPONENT_START_BURNING -> they are temporary dynamic
			FireComponent* fireComponent = entity.getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Fire), std::unique_ptr<DataCacheBase>(new FireData(entity)));
			}

			// TODO(sw) Is it wise to cache data for all damage components? Wouldn't it be better to react on something like EM5_COMPONENT_GETSDAMAGED_BUILDING -> they are temporary dynamic
			DamageComponent* damageComponent = entity.getComponent<DamageComponent>();
			if (nullptr != damageComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Damage), std::unique_ptr<DataCacheBase>(new DamageData(entity)));
			}

			// TODO(sw) Is it wise to cache data for all health components? Wouldn't it be better to react on something like EM5_COMPONENT_GETSINJURED_PERSON -> they are temporary dynamic
			HealthComponent* healthComponent = entity.getComponent<HealthComponent>();
			if (nullptr != healthComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Health), std::unique_ptr<DataCacheBase>(new HealthData(entity)));
			}

			// TODO(sw) Do we need this?
			/*qsf::ai::NavigationComponent* navigationComponent = entity.getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Navigation), std::unique_ptr<DataCacheBase>(new NavigationData(entity)));
			}*/

			if (nullptr != entity.getComponent<VehicleCraneComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::VehicleCrane), std::unique_ptr<DataCacheBase>(new VehicleCraneData(entity)));
			}

			if (nullptr != entity.getComponent<WaterjetComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Waterjet), std::unique_ptr<DataCacheBase>(new WaterjetData(entity)));
			}

			if (nullptr != entity.getComponent<WaterCannonComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::WaterCannon), std::unique_ptr<DataCacheBase>(new WaterCannonData(entity)));
			}

			qsf::Entity& parentEntity = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(entity, qsf::LinkComponent::SELECT_PARENT));

			if (nullptr == movableComponent)
			{
				// Add transform data, but only for special cases
				// First: Entity must have no movable component
				if (nullptr != entity.getComponent<qsf::TransformComponent>())
				{
					// Only add the tracking for transform data changes, when
					// - It is an equipment, but not if it is the paramedic stretcher and has a link parent, because this makes problems on client side (e.g. the stretcher is positioned way above the paramedic team)
					qsf::game::EquipmentComponent* equipmentComponent = entity.getComponent<qsf::game::EquipmentComponent>();
					if (nullptr != equipmentComponent)
					{
						const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();

						// If it is the paramedic stretcher and has a link component active then ignore it
						if (equipmentComponent->getEquipmentName() != "equipment_ambulance_stretcher" || nullptr == linkComponent || !linkComponent->hasParent())
						{
							componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Transform), std::unique_ptr<DataCacheBase>(new TransformData(entity)));
						}
					}
					else
					{
						// Special handling for the tow car special entity (the tarpaulin which is used as replacement for too big to tow items)
						// For this we need also track the transform data because this get changed via the tow action when the tarpaulin gets placed at the wreckage location
						CommandableComponent* parentCommandableComponent = parentEntity.getComponent<CommandableComponent>();
						if (nullptr != parentCommandableComponent && parentCommandableComponent->isTechTowcar())
						{
							componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Transform), std::unique_ptr<DataCacheBase>(new TransformData(entity)));
						}
					}
				}
			}

			qsf::MeshComponent* meshComponent = entity.getComponent<qsf::MeshComponent>();
			if (nullptr != meshComponent && (meshComponent->getId() == qsf::MeshComponent::COMPONENT_ID || meshComponent->getId() == qsf::compositing::TintableMeshComponent::COMPONENT_ID ))
			{
				// Only pure mesh or tintable mesh components are tracked
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Mesh), std::unique_ptr<DataCacheBase>(new MeshData(entity)));
			}

			CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
			PersonComponent* personComponent = entity.getComponent<PersonComponent>();
			VehicleComponent* vehicleComponent = entity.getComponent<VehicleComponent>();
			BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();

			// Add entries for components who's existence is tracked
			// Hidden is mainly only used for persons and vehicles
			// TODO(sw) This is an candidate that the host get informed, when this component gets created
			if (nullptr != personComponent || nullptr != vehicleComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Hidden), std::unique_ptr<DataCacheBase>(new HiddenData(entity)));
			}

			// Currently only persons can have active equipments (which is here tracked)
			if (nullptr != personComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Inventory), std::unique_ptr<DataCacheBase>(new InventoryData(entity)));
			}

			// Don't track link data for buildings, trees, doors, fire without damage
			const bool donotTrackLinkForFire = (nullptr != entity.getComponent<FireComponent>() && nullptr == damageComponent);
			if (nullptr == buildingComponent && nullptr == entity.getComponent<TreeComponent>()
				&& nullptr == entity.getComponent<DoorComponent>() && !donotTrackLinkForFire)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Link), std::unique_ptr<DataCacheBase>(new LinkData(entity)));
			}

			// Bone to bone links are only used for persons and equipments or particles (effect entities)
			if (nullptr != personComponent || nullptr != entity.getComponent<qsf::game::EquipmentComponent>() || nullptr != entity.getComponent<qsf::ParticlesComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::BoneToBoneLink), std::unique_ptr<DataCacheBase>(new BoneToBoneLinkData(entity)));
			}

			// The game logic tracking only tracks information about pulling/carrying persons/objects which can only be done by unit persons and the gangster in building logic
			// We track now for all commandable object the game logic data.
			if (nullptr != commandableComponent || nullptr != buildingComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::GameLogic), std::unique_ptr<DataCacheBase>(new GameLogicData(entity)));
			}

			componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::MeshAnimation), std::unique_ptr<DataCacheBase>(new MeshAnimationData(entity)));
			componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::BoneLink), std::unique_ptr<DataCacheBase>(new BoneLinkData(entity)));
			componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::EventId), std::unique_ptr<DataCacheBase>(new EventIdData(entity)));

			if (nullptr != entity.getComponent<DoorComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Door), std::unique_ptr<DataCacheBase>(new DoorData(entity)));
			}

			if (nullptr != entity.getComponent<TreeComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Tree), std::unique_ptr<DataCacheBase>(new TreeData(entity)));
			}

			if (nullptr != entity.getComponent<DlkLadderComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::DlkLadder), std::unique_ptr<DataCacheBase>(new DlkLadderData(entity)));
			}

			bool isHelicopter = nullptr != entity.getComponent<HelicopterComponent>();
			if (isHelicopter)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Helicopter), std::unique_ptr<DataCacheBase>(new HelicopterData(entity)));
			}

			bool isBoat = nullptr != entity.getComponent<BoatComponent>();
			if (isBoat)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Boat), std::unique_ptr<DataCacheBase>(new BoatData(entity)));
			}

			if (isBoat || isHelicopter)
			{
				if (nullptr != entity.getComponent<SwingComponent>())
				{
					componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Swing), std::unique_ptr<DataCacheBase>(new SwingData(entity)));
				}
			}

			// We track only the action priority thus currently only relevant for units
			if (nullptr != commandableComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Action), std::unique_ptr<DataCacheBase>(new ActionData(entity)));
			}

			// Ghost is only relevant for units, vehicles, persons, objects with a bury component
			if (nullptr != commandableComponent || nullptr != personComponent || nullptr != vehicleComponent || nullptr != entity.getComponent<BuryComponent>() || nullptr != entity.getComponent<qsf::game::EquipmentComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Ghost), std::unique_ptr<DataCacheBase>(new GhostData(entity)));
			}

			// For the diving spot we need to check if this is the child-entity containing the invisible mesh component, since the status overlay component wants to attach to this one really badly for some reason beyond my understanding even though that just makes things a lot more complicated
			// Overlay is only relevant for health, damage, fire (only when toplevel), usablebyengineer and diving spot
			// We can ignore the fire check, because in EM5 all toplevel burnable objects have a damage component
			if (nullptr != healthComponent || nullptr != damageComponent || nullptr != entity.getComponent<UsableByEngineerComponent>() || (nullptr != parentEntity.getComponent<DivingSpotComponent>() && nullptr != entity.getComponent<qsf::MeshComponent>()))
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::StatusOverlay), std::unique_ptr<DataCacheBase>(new StatusOverlayData(entity)));
			}
			else
			{
				// Special check for child entities which parent has a fire component but no mesh component
				qsf::Prototype& parentPrototype = qsf::LinkComponent::getTopmostAncestorPrototype(entity, qsf::LinkComponent::SELECT_PARENT);
				if (&parentPrototype != &entity)
				{
					// Entity is a child entity check if the toplevel parent has a fire component
					if (nullptr != parentPrototype.getComponent<FireComponent>())
					{
						// Toplevel parent has a fire component check if this child would be selected as target entity for the status overlay component
						//  -> As a fix for MP disconnect due unknown entity in map cache when a trash group gets burned, add all children
						//  -> The actual problem here is that "StatusOverlayComponent::getEntityOrChildWithMesh" can deliver different children on different machines, so we don't use that one here
						if (entity.getComponent<qsf::MeshComponent>() != nullptr)
						{
							// The status overlay component would be created in this instance instead of the toplevel parent -> add this to the cache
							componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::StatusOverlay), std::unique_ptr<DataCacheBase>(new StatusOverlayData(entity)));
						}
					}
				}
			}

			if (nullptr != entity.getComponent<UsableByEngineerComponent>() || nullptr != entity.getComponent<EboxComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::UsableByEngineer), std::unique_ptr<DataCacheBase>(new UsableByEngineerData(entity)));
			}

			if (nullptr != entity.getComponent<qsf::ParticlesComponent>() && nullptr == entity.getComponent<FireParticleOptionsComponent>())
			{
				if (nullptr == parentEntity.getComponent<TreeComponent>() && (nullptr == parentEntity.getComponent<BuildingComponent>() || nullptr != entity.getComponent<ContaminationCloudComponent>()))
				{
					componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Particles), std::unique_ptr<DataCacheBase>(new ParticleData(entity)));
				}
			}

			if (nullptr != buildingComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Building), std::unique_ptr<DataCacheBase>(new BuildingData(entity)));
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::BuildingOverlay), std::unique_ptr<DataCacheBase>(new BuildingOverlayData(entity)));
			}

			if (nullptr != personComponent && nullptr == commandableComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Person), std::unique_ptr<DataCacheBase>(new PersonData(entity)));
			}

			if (nullptr != entity.getComponent<PersonMagnetComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::PersonMagnet), std::unique_ptr<DataCacheBase>(new PersonMagnetData(entity)));
			}

			if (nullptr != vehicleComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Vehicle), std::unique_ptr<DataCacheBase>(new VehicleData(entity)));
			}

			if (nullptr != entity.getComponent<BarrierTapeComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::BarrierTape), std::unique_ptr<DataCacheBase>(new BarrierTapeData(entity)));
			}

			if (nullptr != entity.getComponent<RescueBusComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::RescueBus), std::unique_ptr<DataCacheBase>(new RescueBusData(entity)));
			}

			if (nullptr != entity.getComponent<SupportLegsComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::SupportLegs), std::unique_ptr<DataCacheBase>(new SupportLegData(entity)));
			}

			if (nullptr != personComponent)
			{
				// For "SpeakToPersonComponent", we really only have to track the component's existence, so there is no need for a "SpeakToPersonData" class
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::SpeakToPerson), std::unique_ptr<DataCacheBase>(new DataCacheComponentExistenceTrackingBase<SpeakToPersonComponent>(entity)));
			}

			// For "PoliceDogTargetComponent", we really only have to track the component's existence, so there is no need for a "PoliceDogTargetData" class
			componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::PoliceDogTarget), std::unique_ptr<DataCacheBase>(new DataCacheComponentExistenceTrackingBase<PoliceDogTargetComponent>(entity)));

			if (nullptr != entity.getComponent<BuryComponent>())
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Bury), std::unique_ptr<DataCacheBase>(new BuryData(entity)));
			}
		}

		void DataCacheItemFactory::createCacheItemsForCoreEntity(const qsf::Entity& entity, ComponentCacheMap& componentCacheMap)
		{
			// Ask listeners (used for modding only)
			for (Listener* listener : mListeners)
			{
				listener->createCacheItemsForCoreEntity(entity, componentCacheMap);
			}

			qsf::game::TimeOfDayComponent* timeOfDayComponent = entity.getComponent<qsf::game::TimeOfDayComponent>();
			if (nullptr != timeOfDayComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::TimeOfDay), std::unique_ptr<DataCacheBase>(new TimeOfDayData(entity)));
			}

			qsf::game::WeatherComponent* weatherComponent = entity.getComponent<qsf::game::WeatherComponent>();
			if (nullptr != weatherComponent)
			{
				componentCacheMap.emplace(static_cast<uint8>(TrackedComponentType::Weather), std::unique_ptr<DataCacheBase>(new WeatherData(entity)));
			}
		}

		void DataCacheItemFactory::addListener(Listener& listener)
		{
			if (std::find(mListeners.begin(), mListeners.end(), &listener) != mListeners.end())
			{
				QSF_ERROR("Double insertion of DataCacheItemFactory::Listener instance", return);
			}

			mListeners.push_back(&listener);
		}

		void DataCacheItemFactory::removeListener(Listener& listener)
		{
			const auto iterator = std::find(mListeners.begin(), mListeners.end(), &listener);
			if (iterator == mListeners.end())
			{
				QSF_ERROR("DataCacheItemFactory::Listener instance to remove could not be found", return);
			}

			mListeners.erase(iterator);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5
