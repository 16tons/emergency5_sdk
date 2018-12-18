// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/GrandTheftAutoEventFactory.h"
#include "em5/event/standard/gangster/GrandTheftAutoEvent.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/base/ContainerCategory.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/traffic/TrafficLightComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GrandTheftAutoEventFactory::GrandTheftAutoEventFactory()
	{
		// Nothing here
	}

	GrandTheftAutoEventFactory::~GrandTheftAutoEventFactory()
	{
		// Nothing here
	}

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* GrandTheftAutoEventFactory::tryTriggerEvent()
	{
		// Find car
		 qsf::Entity* car = findNextCar();

		QSF_WARN_IF(nullptr == car, "Could not find a car for GTA Event!", return nullptr;);

		// Create the event instance
		GrandTheftAutoEvent& freeplayEvent = createAndInitializeEvent<GrandTheftAutoEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setCar(*car);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* GrandTheftAutoEventFactory::findNextCar() const
	{
		qsf::Map& map = MapHelper(QSF_MAINMAP).getMap();

		std::vector<qsf::Entity*> vehicleEntities;

		for (RoadVehicleComponent* roadVehicleComponents : qsf::ComponentMapQuery(map).getAllInstances<RoadVehicleComponent>())
		{
			qsf::Entity& vehicle = roadVehicleComponents->getEntity();

			// Exclude parking or inactive vehicles
			qsf::ai::NavigationComponent* navigationComponent = vehicle.getComponent<qsf::ai::NavigationComponent>();
			if (nullptr == navigationComponent || !navigationComponent->isActive())
				continue;

			// Another check for parking vehicle, just to be sure (see EM_2017 bug 2003)
			if (vehicle.getComponent<RoadVehicleComponent>()->VehicleDoorsArray.size() == 0)
				continue;

			qsf::TransformComponent* transformComponent = vehicle.getComponent<qsf::TransformComponent>();
			if (nullptr == transformComponent)
				continue;

			EntityHelper entityHelper(vehicle);

			if (!entityHelper.isCivilRoadVehicle())
				continue;

			if (entityHelper.isEntityVisibleOnScreen())
				continue;

			if (!entityHelper.isEntityValidEventTarget())
				continue;

			if (entityHelper.isCloseToEventBoundaries(DISTANCE_TO_BORDER))
				continue;

			std::vector<qsf::Transform> driverDoors;
			roadVehicleComponents->getDoorPositions(DoorComponent::DOORTYPE_DRIVER, driverDoors, transformComponent->getTransform());

			if (driverDoors.empty())
				continue;

			vehicleEntities.push_back(&vehicle);
		}

		if (vehicleEntities.empty())
			return nullptr;

		return qsf::Random::getAnyOf(vehicleEntities);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
