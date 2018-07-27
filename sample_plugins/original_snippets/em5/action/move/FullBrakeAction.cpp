// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/FullBrakeAction.h"

#include <qsf/component/move/MovableComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier FullBrakeAction::ACTION_ID = "em5::FullBrakeAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FullBrakeAction::FullBrakeAction() :
		qsf::Action(ACTION_ID),
		mClear(false)
	{
		// Nothing here
	}

	FullBrakeAction::~FullBrakeAction()
	{
		// Nothing to do here
	}

	void FullBrakeAction::init(bool clear)
	{
		mClear = clear;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void FullBrakeAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mClear);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result FullBrakeAction::updateAction(const qsf::Clock&)
	{
		// TODO(jm): Make that car stop a realistic way and play some annoying sounds
		if (mClear)
		{
			// That's all folks
			return qsf::action::RESULT_CLEARLIST;
		}
		else
		{
			const qsf::MovableComponent* movableComponent = getEntity().getComponent<qsf::MovableComponent>();
			return (nullptr != movableComponent && 0.0f != movableComponent->getSpeed()) ? qsf::action::RESULT_CONTINUE : qsf::action::RESULT_DONE;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
