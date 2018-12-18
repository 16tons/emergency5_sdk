// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/StormDamageEventFactory.h"
#include "em5/event/standard/destruction/StormDamageEvent.h"
#include "em5/freeplay/factory/FreeplayEventTriggerInfo.h"

#include <qsf_game/environment/weather/WeatherComponent.h>

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StormDamageEventFactory::StormDamageEventFactory()
	{
		// Nothing here
	}

	StormDamageEventFactory::~StormDamageEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* StormDamageEventFactory::tryTriggerEvent()
	{
		// Make sure event cannot trigger twice
		if (isEventAlreadyRunning())
			return nullptr;

		if (!getTriggerInfo().mTriggeredByDebug)
		{
			// Check for weather condition, must be storm
			qsf::game::WeatherComponent* weatherComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::WeatherComponent>();
			if (nullptr != weatherComponent)
			{
				if (weatherComponent->getLastSetWeatherStateName() != "Storm")
				{
					return nullptr;
				}
			}
		}

		// Create the event instance
		StormDamageEvent& freeplayEvent = createAndInitializeEvent<StormDamageEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
