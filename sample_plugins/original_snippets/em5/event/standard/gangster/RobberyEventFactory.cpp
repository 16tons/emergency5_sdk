// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RobberyEventFactory.h"
#include "em5/event/standard/gangster/RobberyEvent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagHelper.h>

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
	RobberyEventFactory::RobberyEventFactory()
	{
		// Nothing here
	}

	RobberyEventFactory::~RobberyEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* RobberyEventFactory::tryTriggerEvent()
	{
		// Find potential pickpocket and victim
		std::pair<qsf::Entity*, qsf::Entity*> candidate = findCandidate();
		if (nullptr == candidate.first)
			return nullptr;

		// Create the event instance
		RobberyEvent& freeplayEvent = createAndInitializeEvent<RobberyEvent>();
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
	std::pair<qsf::Entity*, qsf::Entity*> RobberyEventFactory::findCandidate() const
	{
		// Find gangster
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT, getEventTag());
		QSF_WARN_IF(nullptr == gangster, "Could not find gangster for robbery event!", QSF_REACT_NONE);
		if (nullptr != gangster)
		{
			// Find victim
			qsf::Entity* victim = findVictim(*gangster);
			QSF_WARN_IF(nullptr == victim, "Could not find victim for robbery event!", QSF_REACT_NONE);
			if (nullptr != victim)
			{
				return std::make_pair(gangster, victim);
			}
		}
		return std::make_pair(nullptr, nullptr);
	}

	qsf::Entity* RobberyEventFactory::findVictim(qsf::Entity& candidate) const
	{
		if (mVictimTag.empty())
		{
			std::vector<qsf::Entity*> allSuitableVictimEntities;

			// Get a listing of all person components (cannot be empty since findCandidate already checked)
			const auto& allPersonComponents = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<PersonComponent>();

			// Try to find a victim
			for (PersonComponent* personComponent : allPersonComponents)
			{
				qsf::Entity& entity = personComponent->getEntity();

				// Check for non-injured healthy non-occupied civil target
				if (EntityHelper(entity).isValidGangsterEventTarget(candidate, true))
				{
					allSuitableVictimEntities.push_back(&entity);
				}
			}

			return EntityHelper(candidate).getClosestEntity(allSuitableVictimEntities);
		}
		else
		{
			// Get victim spawn point by event tag
			qsf::Entity* entity = qsf::game::EventTagHelper("RobberyEvent").getRandomTaggedEntity(mVictimTag);
			if (nullptr != entity)
			{
				return entity;
			}
		}

		// None found
		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
