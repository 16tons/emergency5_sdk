// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/DrunkenOverboardEventFactory.h"
#include "em5/event/standard/medical/DrunkenOverboardEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DrunkenOverboardEventFactory::DrunkenOverboardEventFactory()
	{
		// Nothing here
	}

	DrunkenOverboardEventFactory::~DrunkenOverboardEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* DrunkenOverboardEventFactory::tryTriggerEvent()
	{
		// TODO(db) check if player has diver/ motorboat

		qsf::Entity* shipEntity = findShip();
		if (nullptr == shipEntity)
			return nullptr;

		// Create a candidate entity
		qsf::Entity* targetEntity = spawnCandidate(*shipEntity);
		if (nullptr == targetEntity)
			return nullptr;

		// Create the event instance
		DrunkenOverboardEvent& freeplayEvent = createAndInitializeEvent<DrunkenOverboardEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetPersonId(targetEntity->getId());
		freeplayEvent.setTargetShipId(shipEntity->getId());

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* DrunkenOverboardEventFactory::findShip() const
	{
		// Get a listing of all ships for drunken overboard event
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash("DRUNKEN_OVERBOARD_EVENT"), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::Entity* shipEntity = randomEnumerator.getNextEntity())
		{
			// Check if the boat is appropriate
			if (checkShip(*shipEntity))
				return shipEntity;
		}

		// None found
		return nullptr;
	}

	qsf::Entity* DrunkenOverboardEventFactory::spawnCandidate(qsf::Entity& shipEntity) const
	{
		// Get a listing of all ships for drunken overboard event
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash("DRUNKEN"), QSF_MAINMAP);

		// No entities with event tag found
		if (tagComponentsInMap.empty())
			return nullptr;

		return &tagComponentsInMap[0]->getEntity();
	}

	bool DrunkenOverboardEventFactory::checkShip(qsf::Entity& shipEntity) const
	{
		// TODO(db) ship has to be a cutter or container ship
		// TODO(db) check if ship is damaged/ destroyed
		// TODO(db) check if ship is part of an other event

		EntityHelper entityHelper(shipEntity);

		// Check if ship has event ID component
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (nullptr == shipEntity.getComponent<BoatComponent>())
			return false;

		// Accepted
		return true;
	}

	qsf::Entity* DrunkenOverboardEventFactory::findSpawnPointOnShip(qsf::Entity& shipEntity) const
	{
		qsf::LinkComponent* linkComponent = shipEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		// Search the spawnpoint
		for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
		{
			qsf::Entity& childEntity = childComponent->getEntity();
			const qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();

			// TODO(db) Remove hardcoded name
			if (metadataComponent->getName() == "spawn_civil_person")
			{
				return &childEntity;
			}
		}

		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
