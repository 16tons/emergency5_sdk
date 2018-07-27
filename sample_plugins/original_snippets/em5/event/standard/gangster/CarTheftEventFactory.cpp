// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/CarTheftEventFactory.h"
#include "em5/event/standard/gangster/CarTheftEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CarTheftEventFactory::CarTheftEventFactory()
	{
		// Nothing here
	}

	CarTheftEventFactory::~CarTheftEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* CarTheftEventFactory::tryTriggerEvent()
	{
		// Find potential thief and car
		std::pair<qsf::Entity*, qsf::Entity*> candidatePair = findCandidate();
		if (nullptr == candidatePair.first)
			return nullptr;

		// Create the event instance
		CarTheftEvent& freeplayEvent = createAndInitializeEvent<CarTheftEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*candidatePair.first);
		freeplayEvent.setCar(*candidatePair.second);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	std::pair<qsf::Entity*, qsf::Entity*> CarTheftEventFactory::findCandidate() const
	{
		qsf::Entity* carEntity = findCar();
		QSF_WARN_IF(nullptr == carEntity, "Could not find car for car theft event!", QSF_REACT_NONE)
		if (nullptr != carEntity)
		{
			// TODO(mk) Here the factory Creates a gangster, normally this job is part for the event
			// Find gangster
			qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT);
			QSF_WARN_IF(nullptr == gangster, "Could not find gangster for car theft event!", QSF_REACT_NONE);
			if (nullptr != gangster)
			{
				// Try to replace the entity with the real one, when the given entity is a combined one
				carEntity = MapHelper(carEntity->getMap()).replaceCombinedEntityWithRealOne(*carEntity);

				return std::make_pair(gangster, carEntity);
			}
		}
		// Nothing found
		return std::make_pair(nullptr, nullptr);
	}

	qsf::Entity* CarTheftEventFactory::findCar() const
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			// "Stealable" cars are supposed to be tagged with CARTHEFT
			eventTag = "CARTHEFT";
		}

		// Get a listing of all tagged cars
		const auto& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::Entity* entity = randomEnumerator.getNextEntity())
		{
			if (checkCar(*entity))
				return entity;
		}

		// None found
		return nullptr;
	}

	bool CarTheftEventFactory::checkCar(qsf::Entity& entity) const
	{
		RoadVehicleComponent* roadVehicleComponent = entity.getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return false;

		const EntityHelper entityHelper(entity);
		if (entityHelper.checkForEventIdComponent())
			return false;

		if (!entityHelper.isCivilRoadVehicle())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
