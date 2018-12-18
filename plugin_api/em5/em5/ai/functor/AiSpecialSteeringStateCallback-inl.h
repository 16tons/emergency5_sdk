// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::ApproachedSpecialStateCallback methods ]
	//[-------------------------------------------------------]
	inline qsf::ai::ApproachedSpecialStateCallback::Reaction AiSpecialSteeringStateCallback::onEntityApproachesArea(qsf::Entity& entity, const qsf::ai::worldElement::State& state, qsf::UnsignedFloat distance)
	{
		return onEntityApproachesNode(entity, state, distance); // treat like an approached node
	}

	inline qsf::ai::ApproachedSpecialStateCallback::Reaction AiSpecialSteeringStateCallback::onEntityInCurrentArea(qsf::Entity& entity, const qsf::ai::worldElement::State& state)
	{
		return qsf::ai::ApproachedSpecialStateCallback::NONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
