// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/HistoricPlagueBuildingComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/spawnpoint/SpawnPointComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>
#include <qsf/math/Transform.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/time/Time.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HistoricPlagueBuildingComponent::COMPONENT_ID = qsf::StringHash("em5::HistoricPlagueBuildingComponent");
	const std::string HistoricPlagueBuildingComponent::DECAL_X = "decal_x";


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	HistoricPlagueBuildingComponent& HistoricPlagueBuildingComponent::getInstance()
	{
		// Return the current instance of this component or create a new
		return QSF_MAINMAP.getCoreEntity().getOrCreateComponentSafe<HistoricPlagueBuildingComponent>();
	}

	void HistoricPlagueBuildingComponent::showSpawnedEntityIcons(qsf::Entity& entity, bool show)
	{
		if (show)
		{
			// Register mini map icon of the fleeing person
			MinimapHelper::registerIcon(entity, MiniMapIconType::EVENT);

			// Show overlay icon
			StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS, true);
		}
		else
		{
			// Remove the entity from the mini map icon
			MinimapHelper::unregisterIcon(entity);

			// Hide the overlay icon
			StatusOverlayComponent::hideIcon(entity, StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS);
		}
	}

	qsf::Entity* HistoricPlagueBuildingComponent::getMarkXDecalFromEntity(qsf::Entity& buildingEntity)
	{
		// The x-mark decal is edited as child links for the plague buildings
		qsf::LinkComponent* linkComponent = buildingEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (auto child : linkComponent->getChildLinks())
			{
				qsf::Entity& childEntity = child->getEntity();
				qsf::MetadataComponent* metadataComponent = childEntity.getComponent<qsf::MetadataComponent>();
				if (nullptr != metadataComponent)
				{
					if (metadataComponent->getTags() == DECAL_X)
					{
						// Found x-mark decal
						return &childEntity;
					}
				}
			}
		}

		// No entity found
		return nullptr;
	}

	void HistoricPlagueBuildingComponent::showPestIconForBuilding(qsf::Entity& entity)
	{
		MinimapHelper::unregisterIcon(entity);
		MinimapHelper::registerIcon(entity, MiniMapIconType::PEST_BUILDING);
		StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_POSSIBLY_INFECTED_BUILDING, false);
		StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_INFECTED, true);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HistoricPlagueBuildingComponent::HistoricPlagueBuildingComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mParentEvent(nullptr),
		mDestinationTrigger(nullptr),
		mLayerId(qsf::getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	HistoricPlagueBuildingComponent::~HistoricPlagueBuildingComponent()
	{
		// Nothing to do in here
	}

	void HistoricPlagueBuildingComponent::activateCleanHousesByEntityTag(FreeplayEvent& freeplayEvent, const std::string& entityTag, qsf::Entity& despawnPoint, qsf::Entity& destinationTrigger, bool spawnPersons)
	{
		// Just initialize the parameters
		mParentEvent = &freeplayEvent;
		mEntityTag = entityTag;
		mDestinationPosition = despawnPoint.getComponentSafe<qsf::TransformComponent>().getPosition();
		mDestinationTrigger = &destinationTrigger;

		// Check if the component is already running
		if (isRunning())
		{
			// Collect the spawn data
			std::vector<BuildingComponent*> buildings = collectSpawnBuildingsFromMap();

			if (spawnPersons)
			{
				spawnPersonsFromBuildings(buildings);
			}
			else
			{
				// Show cleanup icons for buildings
				for (BuildingComponent* buildingComponent : buildings)
				{
					this->setMustClean(buildingComponent->getEntity(), true);
				}
			}
		}
	}

	bool HistoricPlagueBuildingComponent::getMustClean(uint64 entityId) const
	{
		for (const MustCleanData& mustCleanData : mMustCleanBuildings)
		{
			if (mustCleanData.entityId == entityId)
			{
				return mustCleanData.mustClean;
			}
		}

		// Not in list, so no need for cleaning
		return false;
	}

	void HistoricPlagueBuildingComponent::setMustClean(qsf::Entity& buildingEntity, bool mustClean)
	{
		for (MustCleanData& mustCleanData : mMustCleanBuildings)
		{
			if (mustCleanData.entityId == buildingEntity.getId())
			{
				mustCleanData.mustClean = mustClean;
				showBuildingIcons(buildingEntity, mustClean);
			}
		}

		// Not in list, bad using
	}

	void HistoricPlagueBuildingComponent::spawnPersonsFromBuildings(std::vector<BuildingComponent*>& spawnBuildings)
	{
		for (BuildingComponent* buildingComponent: spawnBuildings)
		{
			if (nullptr == buildingComponent)
				continue;

			// Create new transformation for the spawned entity
			qsf::Transform transform(buildingComponent->getDoorPosition(), buildingComponent->getDoorRotation());

			// Spawn the new entity at the given spawn pool
			qsf::Entity* spawnedEntity = SpawnPointComponent::spawnRandomAt(transform, getEntity().getMap(), assets::SPAWNPOOL_MEDIEVAL_PERSON, false);

			if (nullptr != spawnedEntity)
			{
				// Place the entity at the building door
				EntityHelper(*spawnedEntity).placeEntityAtBuildingDoor(buildingComponent->getEntity(), false);

				setupAndMovePersonToDespawn(*spawnedEntity, buildingComponent->getEntityId());

				// Unregister the map icon
				MinimapHelper::unregisterIcon(buildingComponent->getEntity());

				// Hide the overlay icon
				StatusOverlayComponent::hideIcon(buildingComponent->getEntity(), StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS);
				StatusOverlayComponent::hideIcon(buildingComponent->getEntity(), StatusOverlayComponent::STATUS_ICON_INFECTED);
			}
			else
			{
				QSF_ERROR_ONCE("HistoricPlagueBuildingComponent::spawnPersonsFromBuildings() can't spawn person at entity" << buildingComponent->getEntityId(), QSF_REACT_NONE);
			}
		}
	}

	void HistoricPlagueBuildingComponent::setLayerName(const std::string& layername)
	{
		qsf::Layer* eventLayer = getEntity().getMap().getLayerManager().getLayerByName(qsf::StringHash(layername));
		if (nullptr != eventLayer)
		{
			mLayerId = eventLayer->getId();
		}
	}

	bool HistoricPlagueBuildingComponent::isPersonReachedFleeingTrigger() const
	{
		for (auto fleeingPersonData : mFleeingPersons)
		{
			if (fleeingPersonData.personEntity.valid() && fleeingPersonData.triggerEntity.valid())
			{
				if (EntityHelper(*fleeingPersonData.personEntity).isInsideBox(fleeingPersonData.triggerEntity->getComponentSafe<qsf::TransformComponent>(), true))
				{
					return true;
				}
			}
		}
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	bool HistoricPlagueBuildingComponent::onStartup()
	{
		// Register the message proxy
		mEntityReturnedProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_PLAGUEPERSON_REMOVED), boost::bind(&HistoricPlagueBuildingComponent::onEntityRemoved, this, _1));

		if (isRunning())
		{
			// Hide all X-Decal entities in the map
			for (qsf::MetadataComponent* metadataComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<qsf::MetadataComponent>())
			{
				if (nullptr != metadataComponent && DECAL_X == metadataComponent->getTags())
				{
					// The mesh component must be inactive to hide it
					metadataComponent->getEntity().getComponentSafe<qsf::MeshComponent>().setActive(false);
					break;
				}
			}
		}

		// Just return success
		return true;
	}

	void HistoricPlagueBuildingComponent::onShutdown()
	{
		mEntityReturnedProxy.unregister();
	}

	void HistoricPlagueBuildingComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::vector<BuildingComponent*> HistoricPlagueBuildingComponent::collectSpawnBuildingsFromMap()
	{
		std::vector<BuildingComponent*> result;

		// Gather the tagged entities from the map
		std::vector<qsf::game::EventTagComponent*> tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(mEntityTag), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* eventTagComponent : tagComponentsInMap)
		{
			qsf::Entity& taggedEntity = eventTagComponent->getEntity();

			BuildingComponent* buildingComponent = taggedEntity.getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				result.push_back(buildingComponent);

				// Adding data in internal data structure
				bool unique = true;
				for (const MustCleanData& mustCleanData : mMustCleanBuildings)
				{
					if (mustCleanData.entityId == buildingComponent->getEntityId())
					{
						unique = false;
						break;
					}
				}

				if (unique)
				{
					// Set a new event ID component
					EventSpreadHelper::setEventIdComponent(mParentEvent, taggedEntity, eventspreadreason::EVENT_SPECIFIC);

					MustCleanData mustCleanData;
					mustCleanData.entityId = buildingComponent->getEntityId();
					mustCleanData.mustClean = true;
					mMustCleanBuildings.push_back(mustCleanData);
				}
			}
		}
		return result;
	}

	void HistoricPlagueBuildingComponent::onEntityRemoved(const qsf::MessageParameters& parameters)
	{
		// Get the entity ID from the message
		uint64 entityId = parameters.getFilter(2);

		// Get the removed entity
		qsf::Entity* removedEntity = getEntity().getMap().getEntityById(entityId);
		if (nullptr != removedEntity)
		{
			// Remove the icons
			showSpawnedEntityIcons(*removedEntity, false);
		}
	}

	void HistoricPlagueBuildingComponent::showBuildingIcons(qsf::Entity& buildingEntity, bool show)
	{
		if (show)
		{
			// Register the map icon
			MinimapHelper::registerIcon(buildingEntity, MiniMapIconType::POSSIBLY_PEST_BUILDING);

			// Show overlay icon
			StatusOverlayComponent::showIcon(buildingEntity, StatusOverlayComponent::STATUS_ICON_POSSIBLY_INFECTED_BUILDING, true);
		}
		else
		{
			// Unregister the map icon
			MinimapHelper::unregisterIcon(buildingEntity);

			// Hide the overlay icon
			StatusOverlayComponent::hideIcon(buildingEntity, StatusOverlayComponent::STATUS_ICON_POSSIBLY_INFECTED_BUILDING);
			StatusOverlayComponent::hideIcon(buildingEntity, StatusOverlayComponent::STATUS_ICON_INFECTED);
		}
	}

	void HistoricPlagueBuildingComponent::setupAndMovePersonToDespawn(qsf::Entity& personEntity, uint64 homeBuildingId)
	{
		// Disable potential ambient animation
		EntityHelper(personEntity).disableMeshAnimationTestComponent();

		// Get the person component of the spawned entity
		PersonComponent& personComponent = personEntity.getComponentSafe<PersonComponent>();
		// The medieval persons uses a different skeleton for animation
		personComponent.setPersonFlags(PersonComponent::FLAG_NEW_SKELETON, true);
		personComponent.setHomeBuildingId(homeBuildingId);
		personComponent.giveRandomNameHistoric();

		// Moving the new spawned entity in the event layer is essential for the cleanup mechanic. Otherwise the person would stay in the map after main event restart.
		personEntity.getOrCreateComponentSafe<qsf::MetadataComponent>().setLayerId(mLayerId);

		// Make these persons visible behind walls as well
		EntityHelper(personEntity).enableShineThroughEffect();

		// The Person should move to the given destination point
		qsf::logic::TargetPoint moveTargetPoint;
		// Set the tolerance of the target point
		moveTargetPoint.mTolerance = 1.0f;
		moveTargetPoint.mPosition = mDestinationPosition;

		// Get the action component of the spawned person
		qsf::ActionComponent& actionComponent = personEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<MoveAction>(action::COMMAND_LOW).init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint));	// TODO(tl): Use MovementModes::MOVEMENT_MODE_FREE_WALK to use free area

		// Register person in internal logic
		FleeingPersonData fleePersonData;
		fleePersonData.personEntity = &personEntity;
		fleePersonData.triggerEntity = mDestinationTrigger;
		mFleeingPersons.push_back(fleePersonData);

		// Register the icons
		showSpawnedEntityIcons(personEntity, true);

		EventSpreadHelper::setEventIdComponent(mParentEvent, personEntity, eventspreadreason::Reason::EVENT_SPECIFIC);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
