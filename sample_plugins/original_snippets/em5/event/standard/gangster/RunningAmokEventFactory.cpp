// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RunningAmokEventFactory.h"
#include "em5/event/standard/gangster/RunningAmokEvent.h"
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
	RunningAmokEventFactory::RunningAmokEventFactory() :
		mPeopleToAttack(0)
	{
		// Nothing here
	}

	RunningAmokEventFactory::~RunningAmokEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* RunningAmokEventFactory::tryTriggerEvent()
	{
		// Find gangster
		qsf::Entity* gangster = MapHelper(QSF_MAINMAP).getOrCreateGangsterCandidateInGangsterEventArea(GangsterPool::DEFAULT_EXTENDED);
		QSF_WARN_IF(nullptr == gangster, "Could not find gangster for ammok event!", QSF_REACT_NONE);
		if (nullptr == gangster)
			return nullptr;

		// Create the event instance
		RunningAmokEvent& freeplayEvent = createAndInitializeEvent<RunningAmokEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setGangster(*gangster);
		freeplayEvent.setPeopleToAttack(mPeopleToAttack);

		// Done
		return &freeplayEvent;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
