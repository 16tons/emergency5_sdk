// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/TerrorAttackEventFactory.h"
#include "em5/event/standard/gangster/TerrorAttackEvent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

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
	TerrorAttackEventFactory::TerrorAttackEventFactory() :
		mBombTime(qsf::Time::fromSeconds(30.0f)),
		mTerroristCount(3)
	{
		// Nothing here
	}

	TerrorAttackEventFactory::~TerrorAttackEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* TerrorAttackEventFactory::tryTriggerEvent()
	{
		// Find potential pickpocket and victim
		std::vector<std::pair<qsf::Entity*, qsf::Entity*>> candidates;
		findCandidates(mTerroristCount, candidates);
		if (candidates.size() != mTerroristCount)
			return nullptr;

		// Create the event instance
		TerrorAttackEvent& freeplayEvent = createAndInitializeEvent<TerrorAttackEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		for (auto& candidate : candidates)
		{
			freeplayEvent.addTerrorist(*candidate.first, *candidate.second);
		}

		freeplayEvent.setBombTime(mBombTime);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void TerrorAttackEventFactory::findCandidates(int count, std::vector<std::pair<qsf::Entity*, qsf::Entity*>>& candidates) const
	{
		// Get a listing of all tagged entities
		const auto& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag("BOMB_SPOT", QSF_MAINMAP);
		if (tagComponentsInMap.size() < static_cast<size_t>(count))
		{
			// Not enough tagged entities there... early out with empty vector
			return;
		}

		// Collect all bomb spots
		std::vector<qsf::Entity*> allTaggedEntities;
		{
			for (qsf::game::EventTagComponent* eventTagComponent : tagComponentsInMap)
			{
				qsf::Entity& entity = eventTagComponent->getEntity();
				if (checkBombSpot(entity))
				{
					allTaggedEntities.push_back(&entity);
				}
			}
		}

		for (int i = 0; i < count; ++i)
		{
			// TODO(fw): This looks weird, does it work correctly?
			qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::TERRORIST);
			for (std::pair<qsf::Entity*, qsf::Entity*>& candidate : candidates)
			{
				if (candidate.first == gangster)
				{
					gangster = MapHelper(QSF_MAINMAP).createGangsterFromPoolAndSpawnItInGangsterEventArea(GangsterPool::TERRORIST);
					break;
				}
			}

			qsf::Entity* bombSpot = EntityHelper(*gangster).getClosestEntity(allTaggedEntities);
			if (nullptr == bombSpot)
			{
				return;
			}
			else
			{
				// Erase the bomb spot from the list, so we don't use it twice
				for (auto iter = allTaggedEntities.begin(); iter != allTaggedEntities.end(); ++iter)
				{
					if ((*iter) == bombSpot)
					{
						allTaggedEntities.erase(iter);
						break;
					}
				}
			}

			candidates.emplace_back(gangster, bombSpot);
		}
		return;
	}

	bool TerrorAttackEventFactory::checkBombSpot(qsf::Entity& bombSpot) const
	{
		// Bomb spot needs to be within map boundaries
		if (!EntityHelper(bombSpot).isInMapBoundaries())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
