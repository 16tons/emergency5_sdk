// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterWithWaypointReachedCallback.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterWithWaypointReachedCallback::GangsterWithWaypointReachedCallback() :
		GangsterBaseLogic()
	{
		// Nothing to do in here
	}

	GangsterWithWaypointReachedCallback::GangsterWithWaypointReachedCallback(uint32 gameLogicId) :
		GangsterBaseLogic(gameLogicId)
	{
		// Nothing to do in here
	}

	GangsterWithWaypointReachedCallback::~GangsterWithWaypointReachedCallback()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterWithWaypointReachedCallback::onWaypointReached(const Waypoint& waypoint)
	{
		waypointReached();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
