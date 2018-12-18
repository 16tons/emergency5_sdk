// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/BomberEventFactory.h"
#include "em5/event/standard/gangster/BomberEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BomberEventFactory::BomberEventFactory() :
		mBombTime(qsf::getUninitialized<float>()),
		mEscapeTime(qsf::getUninitialized<float>()),
		mBombTargetTag("BOMB_SPOT")
	{
		// Nothing here
	}

	BomberEventFactory::~BomberEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BomberEventFactory::tryTriggerEvent()
	{
		// Find potential spots for event
		std::pair<qsf::Entity*, qsf::Entity*> candidate = findCandidate();

		if (nullptr == candidate.first)
			return nullptr;

		// Create the event instance
		BomberEvent& freeplayEvent = createAndInitializeEvent<BomberEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*candidate.first);
		freeplayEvent.setBombSpot(*candidate.second);
		freeplayEvent.setBombTime(mBombTime);
		freeplayEvent.setEscapeTime(mEscapeTime);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::pair<qsf::Entity*, qsf::Entity*> BomberEventFactory::findCandidate() const
	{
		// Find a gangster
		// TODO(fw): Choosing a gangster is actually not necessary here, for the gangster is placed at the bomb spot anyway
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::TERRORIST, getEventTag());
		QSF_WARN_IF(nullptr == gangster, "Could not find gangster for bomber event!", QSF_REACT_NONE)
		if (nullptr != gangster)
		{
			// Find a bomb spot
			qsf::Entity* bombSpot = findBombSpot(*gangster);
			QSF_WARN_IF(nullptr == bombSpot, "Could not find bomb spot for bomber event!", QSF_REACT_NONE)
			if (nullptr != bombSpot)
				return std::make_pair(gangster, bombSpot);
		}

		return std::make_pair(nullptr, nullptr);
	}

	qsf::Entity* BomberEventFactory::findBombSpot(qsf::Entity& candidate) const
	{
		// TODO(fw): Do not cycle through all event tag components, take only those with the bomb target tag
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator;
		while (qsf::game::EventTagComponent* eventTagComponent = randomEnumerator.getNext())
		{
			qsf::Entity& entity = eventTagComponent->getEntity();
			if (checkBombSpot(candidate, entity))
			{
				return &entity;
			}
		}
		return nullptr;
	}

	bool BomberEventFactory::checkBombSpot(qsf::Entity& candidate, qsf::Entity& bombSpot) const
	{
		qsf::game::EventTagComponent* eventTagComponent = bombSpot.getComponent<qsf::game::EventTagComponent>();

		if (nullptr == eventTagComponent)
			return false;

		if (!eventTagComponent->hasTag(qsf::StringHash(getBombTargetTag())))
			return false;

		// Bomb spot needs to be within map boundaries
		if (!EntityHelper(bombSpot).isInEventBoundaries())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
