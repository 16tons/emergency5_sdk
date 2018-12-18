// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/ArrivedAtDespawnAction.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ArrivedAtDespawnAction::ACTION_ID = "em5::ArrivedAtDespawnAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ArrivedAtDespawnAction::ArrivedAtDespawnAction() :
		Action(ACTION_ID)
	{
		// Nothing here
	}

	ArrivedAtDespawnAction::~ArrivedAtDespawnAction()
	{
		// Nothing here
	}

	void ArrivedAtDespawnAction::init(const qsf::logic::TargetPoint& targetPoint)
	{
		mTargetPoints.push_back(targetPoint);
	}

	void ArrivedAtDespawnAction::init(const std::vector<qsf::logic::TargetPoint>& targetPoints)
	{
		mTargetPoints = targetPoints;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ArrivedAtDespawnAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetPoints);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result ArrivedAtDespawnAction::updateAction(const qsf::Clock&)
	{
		for (auto targetPoint: mTargetPoints)
		{
			// Range check if entity is arrived at his target point
			if (qsf::game::DistanceHelper::get2dDistance(targetPoint.mPosition, getEntity()) < targetPoint.mTolerance)
			{
				// Destroy the entity
				MapHelper::destroyEntity(getEntity());

				return qsf::action::RESULT_CLEARLIST;
			}
		}

		// Error, entity can not reach its goal
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
