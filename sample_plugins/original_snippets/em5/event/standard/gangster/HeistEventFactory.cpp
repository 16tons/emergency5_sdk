// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/HeistEventFactory.h"
#include "em5/event/standard/gangster/HeistEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
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
	HeistEventFactory::HeistEventFactory() :
		mHeistDuration(0.0f),
		mDebugTarget(false)
	{
		// Nothing here
	}

	HeistEventFactory::~HeistEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* HeistEventFactory::tryTriggerEvent()
	{
		// Find potential pickpocket and victim
		std::pair<qsf::Entity*, qsf::Entity*> candidate = findCandidate();

		if (nullptr == candidate.first)
			return nullptr;

		// Create the event instance
		HeistEvent& freeplayEvent = createAndInitializeEvent<HeistEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*candidate.first, true);
		freeplayEvent.setBuilding(*candidate.second);
		freeplayEvent.setHeistDuration(mHeistDuration);
		freeplayEvent.setVictimPool(mVictimPool);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::pair<qsf::Entity*, qsf::Entity*> HeistEventFactory::findCandidate() const
	{
		// Find gangster
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT, getEventTag());
		QSF_WARN_IF(nullptr == gangster, "Could not find gangster for heist event!", QSF_REACT_NONE);
		if (nullptr != gangster)
		{
			// Find building
			qsf::Entity* building = findBuilding(*gangster);
			QSF_WARN_IF(nullptr == building, "Could not find building for heist event!", QSF_REACT_NONE);
			if (nullptr != building)
			{
				return std::make_pair(gangster, building);
			}
		}

		return std::make_pair(nullptr, nullptr);
	}

	qsf::Entity* HeistEventFactory::findBuilding(qsf::Entity& candidate) const
	{
		// Get a listing of all building components
		std::vector<qsf::Entity*> allSuitableBuildingEntities;

		ComponentsRandomEnumerator<BuildingComponent> randomEnumerator;
		while (BuildingComponent* buildingComponent = randomEnumerator.getNext())
		{
			qsf::Entity& entity = buildingComponent->getEntity();
			if (checkBuilding(candidate, entity))
			{
				allSuitableBuildingEntities.push_back(&entity);
			}
		}

		return EntityHelper(candidate).getClosestEntity(allSuitableBuildingEntities);
	}

	bool HeistEventFactory::checkBuilding(qsf::Entity& candidate, qsf::Entity& building) const
	{
		EntityHelper entityHelper(building);

		if (entityHelper.isBuildingDamaged())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		BuildingComponent& buildingComponent = building.getComponentSafe<BuildingComponent>();
		if (!buildingComponent.hasDoor())
			return false;

		if (mDebugTarget)
			return true;

		qsf::game::EventTagComponent* eventTagComponent = building.getComponent<qsf::game::EventTagComponent>();
		if (nullptr == eventTagComponent)
			return false;

		if (!eventTagComponent->hasTag(mBuildingTag))
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
