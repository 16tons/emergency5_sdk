// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/PickpocketEventFactory.h"
#include "em5/event/standard/gangster/PickpocketEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickpocketEventFactory::PickpocketEventFactory()
	{
		// Nothing here
	}

	PickpocketEventFactory::~PickpocketEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* PickpocketEventFactory::tryTriggerEvent()
	{
		// Find potential pickpocket and victim
		std::pair<qsf::Entity*, qsf::Entity*> candidate = findCandidate();

		if (nullptr == candidate.first)
			return nullptr;

		// Create the event instance
		PickpocketEvent& freeplayEvent = createAndInitializeEvent<PickpocketEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*candidate.first);
		freeplayEvent.setVictim(*candidate.second);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::pair<qsf::Entity*, qsf::Entity*> PickpocketEventFactory::findCandidate() const
	{
		// Find gangster
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT, getEventTag());
		QSF_WARN_IF(gangster == nullptr, "Could not find gangster in Pickpocket Event!", QSF_REACT_NONE);
		if (nullptr != gangster)
		{
			// Find victim
			qsf::Entity* victim = findVictim(*gangster);
			QSF_WARN_IF(victim == nullptr, "Could not find victim in Pickpocket Event!", QSF_REACT_NONE);
			if (nullptr != victim)
			{
				return std::make_pair(gangster, victim);
			}
		}
		return std::make_pair(nullptr, nullptr);
	}

	qsf::Entity* PickpocketEventFactory::findVictim(qsf::Entity& candidate) const
	{
		if (mVictimTag.empty())
		{
			// Get a listing of all person components (cannot be empty since findCandidate already checked)
			std::vector<qsf::Entity*> allPersonEntities;

			ComponentsRandomEnumerator<PersonComponent> randomEnumerator;
			while (PersonComponent* personComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = personComponent->getEntity();
				if (checkVictim(candidate, entity))
				{
					allPersonEntities.push_back(&entity);
				}
			}

			return EntityHelper(candidate).getClosestEntity(allPersonEntities);
		}
		else
		{
			// Get victim spawn point by event tag
			qsf::Entity* entity = qsf::game::EventTagHelper("PickpocketEvent").getRandomTaggedEntity(mVictimTag);
			if (nullptr != entity)
			{
				// No further check here, the entity is possibly just a spawn point
				return entity;
			}
		}

		// None found
		return nullptr;
	}

	bool PickpocketEventFactory::checkVictim(qsf::Entity& candidate, qsf::Entity& victim) const
	{
		EntityHelper entityHelper(victim);

		if (!entityHelper.isValidGangsterEventTarget(candidate, true))
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
