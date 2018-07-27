// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/SetFireAction.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SetFireAction::ACTION_ID = "em5::SetFireAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SetFireAction::SetFireAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	SetFireAction::~SetFireAction()
	{
		// Nothing here
	}

	void SetFireAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SetFireAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result SetFireAction::updateAction(const qsf::Clock& clock)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			// Set target on fire
			EntityHelper(*targetEntity).startFire(EntityHelper(getEntity()).getFreeplayEvent());
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
