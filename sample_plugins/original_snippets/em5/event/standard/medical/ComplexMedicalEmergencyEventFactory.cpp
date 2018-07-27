// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/medical/ComplexMedicalEmergencyEventFactory.h"
#include "em5/event/standard/medical/ComplexMedicalEmergencyEvent.h"
#include "em5/event/EventHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ComplexMedicalEmergencyEventFactory::ComplexMedicalEmergencyEventFactory()
	{
		// Nothing here
	}

	ComplexMedicalEmergencyEventFactory::~ComplexMedicalEmergencyEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* ComplexMedicalEmergencyEventFactory::tryTriggerEvent()
	{
		// Wrong weather early out
		if (!checkWeatherConditions())
		{
			setTriggerFailReason("Wrong weather condition. Weather must be: " + getWeatherAsString());
			return nullptr;
		}

		qsf::Entity* targetEntity = nullptr;

		// Check if there is an event tag set. Use spawn point
		if (!getEventTag().empty())
		{
			// Find spawn point
			qsf::Entity* spawnPointEntity = EventHelper::findSpawnPoint(qsf::StringHash(getEventTag()));
			if (nullptr == spawnPointEntity)
			{
				setTriggerFailReason("Could not find spawn point by event tag " + getEventTag());
				return nullptr;
			}

			// Spawn a candidate entity
			targetEntity = EventHelper::spawnEntity(*spawnPointEntity);
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("Could not find spawn entity from spawn point with event tag " + getEventTag());
				return nullptr;
			}
		}

		// Otherwise search an existing person
		else
		{
			// Find a candidate entity
			targetEntity = findCandidate();
			if (nullptr == targetEntity)
			{
				setTriggerFailReason("No valid victim candidate found");
				return nullptr;
			}
		}

		// Create the event instance
		ComplexMedicalEmergencyEvent& freeplayEvent = createAndInitializeEvent<ComplexMedicalEmergencyEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetPerson(*targetEntity);

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
