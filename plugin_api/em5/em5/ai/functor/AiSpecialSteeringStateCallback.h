// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/navigation/steering/ApproachedSpecialStateCallback.h>


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
	*    Example of special state callback for EM5. Allows ignoring blocked nodes if emergency lighting is switched on.
	*
	*    This is currently only a stub and not needed at the moment because the only feature of ignoring traffic lights has moved to the movement modes.
	*    We should remove it in the end if it is not needed but I think there will be further use cases if we implement the more specific features later on.
	*/
	class AiSpecialSteeringStateCallback : public qsf::ai::ApproachedSpecialStateCallback
	{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::ApproachedSpecialStateCallback methods ]
	//[-------------------------------------------------------]
	public:
		virtual Reaction onEntityApproachesNode(qsf::Entity& entity, const qsf::ai::worldElement::State& state, qsf::UnsignedFloat distance) override;
		inline virtual Reaction onEntityApproachesArea(qsf::Entity& entity, const qsf::ai::worldElement::State& state, qsf::UnsignedFloat distance) override;
		inline virtual Reaction onEntityInCurrentArea(qsf::Entity& entity, const qsf::ai::worldElement::State& state) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/ai/functor/AiSpecialSteeringStateCallback-inl.h"
