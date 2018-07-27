// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/PoliceDogLocateObjectAction.h"
#include "em5/plugin/Messages.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PoliceDogLocateObjectAction::ACTION_ID = "em5::PoliceDogLocateObjectAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PoliceDogLocateObjectAction::PoliceDogLocateObjectAction() :
		RescueDogLocateVictimAction(ACTION_ID)
	{
		// Police dog knows where its target is
		mTurnToTarget = true;
	}

	PoliceDogLocateObjectAction::~PoliceDogLocateObjectAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::RescueDogLocateVictimAction methods ]
	//[-------------------------------------------------------]
	uint32 PoliceDogLocateObjectAction::getFoundMessageId() const
	{
		return Messages::EM5_POLICE_DOG_FOUND_OBJECT;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
