// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/MoverType.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::CollisionCallback methods     ]
	//[-------------------------------------------------------]
	inline qsf::ai::CollisionCallback::Reaction AiCollisionDetectedFunctor::onEntityCollided(qsf::Entity& entity, qsf::Entity& other)
	{
		// Switch/case on entity types here, world reactions...

		// Dummy implementation returns all collisions to steernig system, let it decide what to do with them.
		return qsf::ai::CollisionCallback::HANDLE_BY_STEERING;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
