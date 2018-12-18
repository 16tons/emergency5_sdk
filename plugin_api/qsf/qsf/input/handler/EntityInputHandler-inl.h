// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EntityInputHandler::EntityInputHandler(uint32 id) :
		InputHandler(id),
		mEntityId(getUninitialized<uint64>())
	{
		// Nothing to do in here
	}

	inline EntityInputHandler::~EntityInputHandler()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::EntityInputHandler methods     ]
	//[-------------------------------------------------------]
	inline void EntityInputHandler::startupForEntity(Entity& entity)
	{
		// Nothing here
	}

	inline void EntityInputHandler::shutdownForEntity(Entity& entity)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
