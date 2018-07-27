// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/ReservateTargetAction.h"
#include "em5/logic/local/ReserveLogic.h"

#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ReservateTargetAction::ACTION_ID = "em5::ReservateTargetAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ReservateTargetAction::ReservateTargetAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mReservationTypeId(qsf::getUninitialized<uint32>()),
		mDeleteReservation(false)
	{
		// Nothing here
	}

	ReservateTargetAction::~ReservateTargetAction()
	{
		// Nothing here
	}

	void ReservateTargetAction::init(uint64 targetEntityId, uint32 reservationId)
	{
		mTargetEntityId = targetEntityId;
		mReservationTypeId = reservationId;
	}

	void ReservateTargetAction::initDeleteReservation(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
		mDeleteReservation = true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ReservateTargetAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result ReservateTargetAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			if (mDeleteReservation)
			{
				ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());
			}
			else
			{
				ReserveLogic::createReservationByTypeId(*targetEntity, getEntityId(), mReservationTypeId);
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
