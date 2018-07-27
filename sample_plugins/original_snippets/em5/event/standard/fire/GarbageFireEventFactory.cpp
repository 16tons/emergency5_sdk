// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/GarbageFireEventFactory.h"
#include "em5/event/standard/fire/SimpleFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event tags
	const std::string EVENT_TAG_GARBAGE					= "GARBAGE";
	const std::string EVENT_TAG_GARBAGE_TRASHCAN		= "GARBAGE_TRASHCAN";
	const std::string EVENT_TAG_GARBAGE_TRASHCONTAINER	= "GARBAGE_TRASHCONTAINER";
	const std::string EVENT_TAG_GARBAGE_TRASHPILE		= "GARBAGE_TRASHPILE";

	// Event names
	const std::string EVENT_NAME_TRASHCAN		= "EM5_EVENT_TITLE_05";
	const std::string EVENT_NAME_TRASHCONTAINER	= "EM5_EVENT_TITLE_06";
	const std::string EVENT_NAME_TRASHPILE		= "EM5_EVENT_TITLE_07";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GarbageFireEventFactory::GarbageFireEventFactory()
	{
		// Default event tag, if nothing is defined inside the JSON
		mEventTag = EVENT_TAG_GARBAGE;
	}

	GarbageFireEventFactory::~GarbageFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* GarbageFireEventFactory::tryTriggerEvent()
	{
		// Find a candidate entity
		qsf::Entity* targetEntity = findCandidate();
		if (nullptr == targetEntity)
		{
			setTriggerFailReason("Could not find entity by event tag " + getDifficultyTags());
			return nullptr;
		}

		// Create the event instance
		SimpleFireEvent& freeplayEvent = createAndInitializeEvent<SimpleFireEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Find the event name
		createEventNameByEntityName(*targetEntity, freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetEntity(*targetEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* GarbageFireEventFactory::findCandidate() const
	{
		// Start the helper
		FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
		fireEventCandidateSearchHelper.setDifficultyTagsByString(mDifficultyTags);

		// Get a listing of all "GARBAGE" tagged entities
		const std::vector<qsf::game::EventTagComponent*>& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(getEventTag()), QSF_MAINMAP);
		if (tagComponentsInMap.empty())
			return nullptr;

		// Search for tagged entities
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);
		while (qsf::game::EventTagComponent* eventTagComponent = randomEnumerator.getNext())
		{
			// Check if difficulty tags are set
			if (!fireEventCandidateSearchHelper.hasDifficultyTag(*eventTagComponent))
				continue;

			// Get the entity
			qsf::Entity& entity = eventTagComponent->getEntity();

			// Check if the garbage entity is appropriate
			if (checkCandidate(entity))
			{
				return &entity;
			}
		}

		// None found
		return nullptr;
	}

	bool GarbageFireEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		const EntityHelper entityHelper(entity);

		// Check if entity is a valid event target
		if (!entityHelper.isEntityValidEventTarget())
			return false;

		// Check if entity is not damaged or destroyed
		if (entityHelper.isEntityDamaged())
			return false;

		// Check if entity is burning or burned
		if (entityHelper.isBurning() || entityHelper.isBurned())
			return false;

		// Check if entity can be set on fire
		if (!entityHelper.canBeSetOnFireNow())
			return false;

		// Accepted
		return true;
	}

	void GarbageFireEventFactory::createEventNameByEntityName(qsf::Entity& entity, FreeplayEvent& freeplayEvent)
	{
		const qsf::game::EventTagComponent& eventTagComponent = entity.getComponentSafe<qsf::game::EventTagComponent>();

		if (eventTagComponent.hasTag(EVENT_TAG_GARBAGE_TRASHCAN))
		{
			freeplayEvent.setEventName(EVENT_NAME_TRASHCAN);
		}
		else if (eventTagComponent.hasTag(EVENT_TAG_GARBAGE_TRASHCONTAINER))
		{
			freeplayEvent.setEventName(EVENT_NAME_TRASHCONTAINER);
		}
		else if (eventTagComponent.hasTag(EVENT_TAG_GARBAGE_TRASHPILE))
		{
			freeplayEvent.setEventName(EVENT_NAME_TRASHPILE);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
