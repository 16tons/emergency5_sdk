// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/WaitForEntityAction.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier WaitForEntityAction::ACTION_ID = "em5::WaitForEntityAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WaitForEntityAction::WaitForEntityAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mDistance(0.0f),
		mFinished(false)
	{
		// Nothing here
	}

	WaitForEntityAction::~WaitForEntityAction()
	{
		// Nothing here
	}

	void WaitForEntityAction::init(const qsf::Entity& targetEntity, float distance)
	{
		mTargetEntityId = targetEntity.getId();
		mDistance = distance;
	}

	void WaitForEntityAction::finish()
	{
		mFinished = true;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void WaitForEntityAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result WaitForEntityAction::updateAction(const qsf::Clock&)
	{
		const qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (mFinished || nullptr == targetEntity || qsf::game::DistanceHelper::getDistance(*targetEntity, getEntity()) < mDistance)
			return qsf::action::RESULT_DONE;

		return qsf::action::RESULT_CONTINUE;
	}

	uint64 WaitForEntityAction::getTargetId() const
	{
		return mTargetEntityId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
