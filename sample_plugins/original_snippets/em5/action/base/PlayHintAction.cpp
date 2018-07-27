// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/PlayHintAction.h"
#include "em5/game/hint/HintSystem.h"
#include "em5/game/Game.h"
#include "em5/EM5Helper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlayHintAction::ACTION_ID	= "em5::PlayHintAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlayHintAction::PlayHintAction() :
		Action(ACTION_ID),
		mHintType(HintMessage::MESSAGETYPE_HINT)
	{
		// Nothing here
	}

	PlayHintAction::~PlayHintAction()
	{
		// Nothing here
	}

	void PlayHintAction::init(const std::string& hintId, HintMessage::MessageType messageType)
	{
		mHintId = hintId;
		mHintType = messageType;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlayHintAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mHintId);
		serializer.serializeAs<uint8>(mHintType);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result PlayHintAction::updateAction(const qsf::Clock&)
	{
		// Send hint
		EM5_GAME.getHintSystem().addMessage(mHintId, mHintType);

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
