// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuildingCollapseEventFactory.h"
#include "em5/event/standard/destruction/BuildingCollapseEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/PlayerHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/physics/collision/CollisionHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string EVENT_TAG_BUILDING_GAMESCOM("GC_CRASH_BUILDING");
	const std::string EVENT_TAG_BUILDING("BUILDING_COLLAPSE");

	const std::string EVENT_TAG_DEBRIS("BUILDING_COLLAPSE_DEBRIS");
	const std::string EVENT_TAG_EXPLOSION("BUILDING_COLLAPSE_EXPLOSION");	// TODO(mz) Generic solution might come then those explosions can be ignored.

	const bool PERSONS_UNDER_DEBRIS = true;
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingCollapseEventFactory::BuildingCollapseEventFactory()
	{
		// Nothing here
	}

	BuildingCollapseEventFactory::~BuildingCollapseEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BuildingCollapseEventFactory::tryTriggerEvent()
	{
		if (PERSONS_UNDER_DEBRIS)
		{
			// Player must already have the heavy truck crane
			Player* localPlayer = PlayerManager::getLocalPlayer();
			if (localPlayer == nullptr)
				return nullptr;

			PlayerHelper playerHelper(*localPlayer);
			if (0 == playerHelper.getNumberOfUnit(CommandableComponent::UNITTAG_TECH_CRANE))
				return nullptr;
		}

		// Find a house to collapse entity
		qsf::Entity* targetEntity = findHouseCandidate();
		if (!targetEntity)
			return nullptr;

		// Create the event instance
		BuildingCollapseEvent& freeplayEvent = createAndInitializeEvent<BuildingCollapseEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event (Don't use explosions because it looks crappy, explosions wasnt meant to use with collapse building only for explosion event)
		freeplayEvent.init(targetEntity->getId(), EVENT_TAG_DEBRIS, "", PERSONS_UNDER_DEBRIS);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* BuildingCollapseEventFactory::findHouseCandidate() const
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = EVENT_TAG_BUILDING;
		}

		// Search for tagged entities
		const std::vector<qsf::game::EventTagComponent*>& tagComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponents);

		while (qsf::Entity* entity = randomEnumerator.getNextEntity())
		{
			BuildingComponent* buildingComponent = entity->getComponent<BuildingComponent>();

			// Check if the candidate is appropriate
			if (nullptr != buildingComponent && checkHouseCandidate(*buildingComponent))
			{
				return &buildingComponent->getEntity();
			}
		}

		return nullptr;
	}

	bool BuildingCollapseEventFactory::checkHouseCandidate(em5::BuildingComponent& buildingComponent) const
	{
		qsf::Entity& entityBuilding = buildingComponent.getEntity();
		const EntityHelper entityHelper(entityBuilding);

		// Check if entity has an event ID component already
		if (entityHelper.checkForEventIdComponent())
			return false;

		// Building not damaged or destroyed
		if (!buildingComponent.isIntact())
			return false;

		// Check if building has all destruction stages
		// TODO(mk) The next commented line is not clear to me, what are "standard building for living"?
		// Also check if this is a standard building for living (special buildings shouldn't have all destruction stages, otherwise we need a special flag for those)
		if (!buildingComponent.getIntactMesh().isValid()
			|| !buildingComponent.getDamagedMesh().isValid()
			|| !buildingComponent.getDestroyedMesh().isValid())
			return false;

		// Building not burning
		if (entityHelper.isBurning())
			return false;

		// Check if there is a person inside the building
		if (!entityHelper.isContainerEmpty())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Get entities linked to the house
		qsf::LinkComponent* buildingLinkComponent = entityBuilding.getComponent<qsf::LinkComponent>();
		if (buildingLinkComponent == nullptr)
			return false;

		// Check if the house enough debris elements free of collisions
		int countDebris = 0;
		qsf::CollisionHelper collisionHelper(entityBuilding.getMap());
		for (qsf::LinkComponent* linkComponent : buildingLinkComponent->getChildLinks())
		{
			qsf::Entity& childEntity = linkComponent->getEntity();
			qsf::game::EventTagComponent* tagComponent = childEntity.getComponent<qsf::game::EventTagComponent>();
			if (nullptr != tagComponent && tagComponent->hasTag(EVENT_TAG_DEBRIS) && collisionHelper.canBeAddedWithoutCollision(childEntity))
			{
				// Got unblocked debris
				++countDebris;
			}
		}

		const bool hasDebris = (countDebris >= 3);	// Extra variable for easier debugging
		return hasDebris;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
