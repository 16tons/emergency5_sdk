// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/navigation/steering/CollisionCallback.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Example on collision detected functor for EMERGENCY 5. Currently ignores collisions.
	*
	*  @todo
	*    - TODO(bs) This is an implementation stub, e.g. game reactions still need to be triggered.
	*/
	class AiCollisionDetectedFunctor : public qsf::ai::CollisionCallback
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::CollisionCallback methods     ]
	//[-------------------------------------------------------]
	public:
		virtual Reaction onEntityCollided(qsf::Entity& entity, qsf::Entity& other) override;

		virtual bool shouldPotentialCollisionBeConsidered(qsf::Entity& entity, qsf::Entity& other, qsf::Time when) override;
		virtual bool shouldReactToCollision(qsf::Entity& entity, qsf::Entity& other, qsf::Time when) override;

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/ai/functor/AiCollisionDetectedFunctor-inl.h"
