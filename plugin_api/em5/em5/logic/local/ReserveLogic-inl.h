// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename T>
	void ReserveLogic::createReservation(qsf::Entity& targetEntity, uint64 reservationId)
	{
		createReservationByTypeId(targetEntity, reservationId, T::GAMELOGIC_TYPE_ID);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
