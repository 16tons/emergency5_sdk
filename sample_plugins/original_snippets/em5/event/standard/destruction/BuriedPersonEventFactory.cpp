// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuriedPersonEventFactory.h"
#include "em5/event/standard/destruction/BuriedPersonEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuriedPersonEventFactory::BuriedPersonEventFactory()
	{
		// Nothing here
	}

	BuriedPersonEventFactory::~BuriedPersonEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BuriedPersonEventFactory::tryTriggerEvent()
	{
		qsf::Entity* targetEntity = nullptr;
		qsf::Entity* spawnPointEntity = nullptr;

		// Check if there is an event tag set. Use spawn point.
		if (!getEventTag().empty())
		{
			// Find spawn point
			spawnPointEntity = EventHelper::findSpawnPoint(qsf::StringHash(getEventTag()));
			if (nullptr == spawnPointEntity)
			{
				setTriggerFailReason("Could not find spawn point by event tag " + getEventTag());
				return nullptr;
			}

			// Spawn a candidate entity
			targetEntity = EventHelper::spawnEntity(*spawnPointEntity);
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("Could not find spawn entity from spawn point with event tag " + getEventTag());
				return nullptr;
			}
		}

		// Otherwise search an existing person
		else
		{
			// Find a candidate entity
			targetEntity = findCandidate();
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("No valid victim candidate found");
				return nullptr;
			}
		}

		// Create the event instance
		BuriedPersonEvent& freeplayEvent = createAndInitializeEvent<BuriedPersonEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// TODO(co) Why is this commented without a commented why it's commented?
		// Lock the spawnpoint with eventid
		//if (nullptr != spawnPointEntity)
		//{
		//	EventIdComponent::addToEvent(*spawnPointEntity, freeplayEvent, eventspreadreason::Reason::EVENT_SPECIFIC);
		//}

		// Initialize the event
		freeplayEvent.setTargetPerson(*targetEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	qsf::Entity* BuriedPersonEventFactory::findCandidate() const
	{
		ComponentsRandomEnumerator<PersonComponent> randomEnumerator(40);	// Limited number of tries
		while (qsf::Entity* personEntity = randomEnumerator.getNextEntity())
		{
			// Check if the person is appropriate
			if (checkCandidate(*personEntity))
			{
				return personEntity;
			}
		}

		// None found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool BuriedPersonEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		EntityHelper entityHelper(entity);

		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Check if entity is a correctly initialized person
		if (!entityHelper.isCivilPerson())
			return false;

		// Check if person is still healthy and not injured
		if (entityHelper.isPersonInjured())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
