// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/JailbreakEventFactory.h"
#include "em5/event/standard/gangster/JailbreakEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string JAILBREAK_SPAWN_TAG("JAILBREAK");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	JailbreakEventFactory::JailbreakEventFactory() :
		mMinNumberOfPrisoners(2),
		mMaxNumberOfPrisoners(3)
	{
		// Nothing here
	}

	JailbreakEventFactory::~JailbreakEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* JailbreakEventFactory::tryTriggerEvent()
	{
		// Create the event instance
		JailbreakEvent& freeplayEvent = createAndInitializeEvent<JailbreakEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		findPrisoners(freeplayEvent.getPrisonerIds());

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool JailbreakEventFactory::findPrisoners(std::vector<uint64>& prisonerIds) const
	{
		prisonerIds.clear();
		const int maxNumberOfPrisoners = qsf::Random::getRandomInt(mMinNumberOfPrisoners, mMaxNumberOfPrisoners);

		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = JAILBREAK_SPAWN_TAG;
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
				qsf::Entity* prisoner = EventHelper::spawnEntity(*spawnPointEntity);
				if (nullptr != prisoner)
				{
					prisonerIds.push_back(prisoner->getId());

					// Check if enough terrorists are found
					if (prisonerIds.size() >= static_cast<size_t>(maxNumberOfPrisoners))
						break;
				}
			}
		}

		return !prisonerIds.empty();
	}

	bool JailbreakEventFactory::checkSpawnPoint(qsf::Entity& spawn) const
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
