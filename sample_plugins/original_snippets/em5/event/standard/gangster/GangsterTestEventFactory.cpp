// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/GangsterTestEventFactory.h"
#include "em5/event/standard/gangster/GangsterTestEvent.h"
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
	GangsterTestEventFactory::GangsterTestEventFactory()
	{
		// Nothing here
	}

	GangsterTestEventFactory::~GangsterTestEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* GangsterTestEventFactory::tryTriggerEvent()
	{
		// Find potential gangster
		qsf::Entity* candidate = findCandidate();

		// Check for valid candidate
		if (nullptr == candidate)
			return nullptr;

		// Create the event instance
		GangsterTestEvent& freeplayEvent = createAndInitializeEvent<GangsterTestEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*candidate);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* GangsterTestEventFactory::findCandidate() const
	{
		// Find gangster
		return MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
