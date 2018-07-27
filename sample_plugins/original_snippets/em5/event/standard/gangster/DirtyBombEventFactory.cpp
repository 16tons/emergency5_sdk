// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/DirtyBombEventFactory.h"
#include "em5/event/standard/gangster/DirtyBombEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/objects/ContaminationComponent.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string BOMB_TARGET_TAG("DIRTYBOMB");
	const std::string TERRORIST_TAG("TERRORIST");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DirtyBombEventFactory::DirtyBombEventFactory() :
		mNumberOfTerrorists(2)
	{
		// Nothing here
	}

	DirtyBombEventFactory::~DirtyBombEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* DirtyBombEventFactory::tryTriggerEvent()
	{
		// Find bomb target
		qsf::Entity* bombTarget = findBombTarget();
		if (nullptr == bombTarget)
			return nullptr;

		// Create the event instance
		DirtyBombEvent& freeplayEvent = createAndInitializeEvent<DirtyBombEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		findTerrorists(freeplayEvent.getTerroristIds());
		freeplayEvent.setBombTargetId(bombTarget->getId());

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DirtyBombEventFactory::findTerrorists(std::vector<uint64>& terroristIds) const
	{
		terroristIds.clear();
		int numberOfTerrorists = 0;

		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = TERRORIST_TAG;
		}

		// Get a listing of all spawnpoints
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);

		// No entities with event tag found
		if (tagComponentsInMap.empty())
		{
			setTriggerFailReason("Could not find spawn point by event tag '" + eventTag + "'");
			return nullptr;
		}

		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);
		while (qsf::Entity* spawnPointEntity = randomEnumerator.getNextEntity())
		{
			// Check if the candidate is appropriate
			if (checkSpawnPoint(*spawnPointEntity))
			{
				qsf::Entity* terrorist = EventHelper::spawnEntity(*spawnPointEntity);
				if (nullptr != terrorist)
				{
					terroristIds.push_back(terrorist->getId());
					++numberOfTerrorists;
				}
			}

			// Check if enough terrorists are found
			if (numberOfTerrorists >= mNumberOfTerrorists)
				break;
		}

		// Return "true" if at least one was added
		return !terroristIds.empty();
	}

	bool DirtyBombEventFactory::checkSpawnPoint(qsf::Entity& spawn) const
	{
		EntityHelper entityHelper(spawn);

		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Check if entity has a spawn component
		if (nullptr == spawn.getComponent<PersonSpawnPointComponent>())
			return false;

		// Accepted
		return true;
	}

	qsf::Entity* DirtyBombEventFactory::findBombTarget() const
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = BOMB_TARGET_TAG;
		}

		// Get a listing of all spawn points for the bomb target
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::Entity* entity = randomEnumerator.getNextEntity())
		{
			if (checkBombTarget(*entity))
				return entity;
		}

		return nullptr;
	}

	bool DirtyBombEventFactory::checkBombTarget(qsf::Entity& bombTarget) const
	{
		EntityHelper entityHelper(bombTarget);

		if (!entityHelper.isEntityValidEventTarget())
			return false;

		ContaminationComponent* contaminationComponent = bombTarget.getComponent<ContaminationComponent>();
		if (nullptr == contaminationComponent)
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
