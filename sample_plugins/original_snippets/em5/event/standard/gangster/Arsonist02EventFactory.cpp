// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/Arsonist02EventFactory.h"
#include "em5/event/standard/gangster/ArsonistEvent.h"
#include "em5/map/MapHelper.h"

#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Arsonist02EventFactory::Arsonist02EventFactory()
	{
		// Nothing here
	}

	Arsonist02EventFactory::~Arsonist02EventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* Arsonist02EventFactory::tryTriggerEvent()
	{
		// Find potential arsonist
		qsf::Entity* arsonist = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT_EXTENDED);
		QSF_WARN_IF(nullptr == arsonist, "Could not find arsonist for arsonist 02 event!", QSF_REACT_NONE);
		if (nullptr == arsonist)
			return nullptr;

		// Create the event instance
		ArsonistEvent& freeplayEvent = createAndInitializeEvent<ArsonistEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setFirstBuildingMustHaveBalcony(true);
		freeplayEvent.setArsonist(*arsonist);
		freeplayEvent.setCheeringTime(mCheeringTime);

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
