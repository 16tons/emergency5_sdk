// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/multiplayer/MultiplayerDummyAction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier MultiplayerDummyAction::ACTION_ID = "em5::MultiplayerDummyAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MultiplayerDummyAction::MultiplayerDummyAction() :
		Action(ACTION_ID)
	{
		// Nothing here
	}

	MultiplayerDummyAction::~MultiplayerDummyAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void MultiplayerDummyAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result MultiplayerDummyAction::updateAction(const qsf::Clock&)
	{
		// Do nothing

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
