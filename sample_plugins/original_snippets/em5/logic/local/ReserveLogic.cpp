// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/ReserveLogic.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ReserveLogic::GAMELOGIC_TYPE_ID					= qsf::StringHash("em5::ReserveLogic");

	const uint32 ReserveParamedicLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveParamedicLogic");
	const uint32 ReserveDoctorLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::ReserveDoctorLogic");
	const uint32 ReserveCutRoadVehicleLogic::GAMELOGIC_TYPE_ID		= qsf::StringHash("em5::ReserveCutRoadVehicleLogic");
	const uint32 ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID		= qsf::StringHash("em5::ReserveEnterVehicleLogic");
	const uint32 ReserveGuidePersonLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveGuidePersonLogic");
	const uint32 ReserveDecontaminatePersonLogic::GAMELOGIC_TYPE_ID	= qsf::StringHash("em5::ReserveDecontaminatePersonLogic");
	const uint32 ReserveDivingSpotLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveDivingSpotLogic");
	const uint32 ReserveWaitForFinishLogic::GAMELOGIC_TYPE_ID		= qsf::StringHash("em5::ReserveWaitForFinishLogic");
	const uint32 ReserveDLKRescueLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveDLKRescueLogic");
	const uint32 ReserveRescueDogLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveRescueDogLogic");
	const uint32 ReserveGangsterLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveGangsterLogic");
	const uint32 ReserveArrestGangsterLogic::GAMELOGIC_TYPE_ID		= qsf::StringHash("em5::ReserveArrestGangsterLogic");
	const uint32 ReserveCloseCombatLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveCloseCombatLogic");
	const uint32 ReserveAttackedByGangsterLogic::GAMELOGIC_TYPE_ID	= qsf::StringHash("em5::ReserveAttackedByGangsterLogic");
	const uint32 ReservePullingLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::ReservePullingLogic");
	const uint32 ReserveMoveToTargetLogic::GAMELOGIC_TYPE_ID		= qsf::StringHash("em5::ReserveMoveToTargetLogic");
	const uint32 ReserveRabidDogLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveRabidDogLogic");
	const uint32 ReserveRepairOrUseLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveRepairOrUseLogic");
	const uint32 ReserveCutTreeLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::ReserveCutTreeLogic");
	const uint32 ReserveTowcarLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::ReserveTowcarLogic");
	const uint32 ReserveHeavyCraneLogic::GAMELOGIC_TYPE_ID			= qsf::StringHash("em5::ReserveHeavyCraneLogic");
	const uint32 SpeakToPersonLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::SpeakToPersonLogic");
	const uint32 RescueWitchLogic::GAMELOGIC_TYPE_ID				= qsf::StringHash("em5::RescueWitchLogic");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool ReserveLogic::checkReservation(qsf::Entity& targetEntity, uint64 reservationId, ReservePriority priority)
	{
		// Check for the reserve logic
		qsf::GameLogicComponent* targetGameLogicComponent = targetEntity.getOrCreateComponent<qsf::GameLogicComponent>();

		// Support multiple gamelogics
		std::vector<ReserveLogic*> reserveLogics = targetGameLogicComponent->getAllGameLogics<ReserveLogic>();
		for (ReserveLogic* reserveLogic : reserveLogics)
		{
			// Someone has reserved the target before...
			if (qsf::isInitialized(reservationId))
			{
				if (reserveLogic->getReservationId() != reservationId)
				{
					// ... and its not me
					if (reserveLogic->mReservePriority >= priority)
					{
						// ... and it is more important than me
						return false;
					}
				}
			}
			else
			{
				// We found an reservation, because we are not looking for any specific, check is failed
				return false;
			}
		}

		// Everything is fine
		return true;
	}

	void ReserveLogic::deleteOwnReservation(qsf::Entity& targetEntity, uint64 reservationId)
	{
		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = targetEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			// Check for the reserve logic
			ReserveLogic* reserveLogic = gameLogicComponent->getGameLogic<ReserveLogic>();
			if (nullptr != reserveLogic)
			{
				// Check for the reserve logic
				if (reserveLogic->getReservationId() == reservationId)
				{
					// Delete my reservation
					reserveLogic->deleteLogic();
				}
			}
		}
	}

	void ReserveLogic::deleteOwnReservation(const qsf::WeakPtr<qsf::Entity>& targetEntity, uint64 reservationId)
	{
		if (targetEntity.valid())
		{
			deleteOwnReservation(*targetEntity, reservationId);
		}
	}

	void ReserveLogic::deleteOwnReservation(qsf::Map& map, uint64 targetEntityId, uint64 reservationId)
	{
		qsf::Entity* targetEntity = map.getEntityById(targetEntityId);
		if (nullptr != targetEntity)
		{
			deleteOwnReservation(*targetEntity, reservationId);
		}
	}

	void ReserveLogic::createReservationByTypeId(qsf::Entity& targetEntity, uint64 reservationId, uint32 typeId)
	{
		// Check for the reserve logic
		qsf::GameLogicComponent* targetGameLogicComponent = targetEntity.getOrCreateComponent<qsf::GameLogicComponent>();
		ReserveLogic* reserveLogic = targetGameLogicComponent->getGameLogic<ReserveLogic>();
		if (nullptr != reserveLogic)
		{
			// Overwrite existing reservation
			targetGameLogicComponent->deleteGameLogic(*reserveLogic);
		}

		// Create reservation
		reserveLogic = static_cast<ReserveLogic*>(targetGameLogicComponent->getOrCreateGameLogicByTypeId(typeId));
		if (nullptr != reserveLogic)
		{
			// Reserve entity
			reserveLogic->setReservationId(reservationId);
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ReserveLogic::ReserveLogic(uint32 gamelogicTypeId) :
		qsf::GameLogic(gamelogicTypeId),
		mReservationId(qsf::getUninitialized<uint64>()),
		mReservePriority(STANDARD)
	{
		// Nothing here
	}

	uint64 ReserveLogic::getReservationId()
	{
		return mReservationId;
	}

	void ReserveLogic::setReservationId(uint64 reservationId)
	{
		mReservationId = reservationId;
	}

	void ReserveLogic::deleteLogic()
	{
		removeThis();
	}

	ReserveParamedicLogic::ReserveParamedicLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveDoctorLogic::ReserveDoctorLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveCutRoadVehicleLogic::ReserveCutRoadVehicleLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveEnterVehicleLogic::ReserveEnterVehicleLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveGuidePersonLogic::ReserveGuidePersonLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveDecontaminatePersonLogic::ReserveDecontaminatePersonLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveDivingSpotLogic::ReserveDivingSpotLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveWaitForFinishLogic::ReserveWaitForFinishLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveDLKRescueLogic::ReserveDLKRescueLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveRescueDogLogic::ReserveRescueDogLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		mReservePriority = LOW;
	}

	ReserveGangsterLogic::ReserveGangsterLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveArrestGangsterLogic::ReserveArrestGangsterLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveCloseCombatLogic::ReserveCloseCombatLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		mReservePriority = NO_UNIT;
	}

	ReserveAttackedByGangsterLogic::ReserveAttackedByGangsterLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		mReservePriority = NO_UNIT;
	}

	ReservePullingLogic::ReservePullingLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveMoveToTargetLogic::ReserveMoveToTargetLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		mReservePriority = MOVETO;
	}

	ReserveRabidDogLogic::ReserveRabidDogLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveRepairOrUseLogic::ReserveRepairOrUseLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveCutTreeLogic::ReserveCutTreeLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveTowcarLogic::ReserveTowcarLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	ReserveHeavyCraneLogic::ReserveHeavyCraneLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	SpeakToPersonLogic::SpeakToPersonLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	RescueWitchLogic::RescueWitchLogic() :
		ReserveLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
