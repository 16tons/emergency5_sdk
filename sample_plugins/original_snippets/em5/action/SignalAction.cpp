// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/SignalAction.h"

#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SignalAction::ACTION_ID = "em5::SignalAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SignalAction::SignalAction() :
		qsf::Action(ACTION_ID),
		mSignalOnAbort(true),
		mMessageAtUpdate(qsf::getUninitialized<qsf::StringHash>()),
		mMessageAtAbort(qsf::getUninitialized<qsf::StringHash>())
	{
		// Nothing here
	}

	SignalAction::~SignalAction()
	{
		// Nothing here
	}

	void SignalAction::init(const qsf::MessageConfiguration& message, const qsf::MessageParameters& messageParameters, bool emitMessageOnAbort)
	{
		mMessageAtUpdate = message;
		mMessageParametersAtUpdate = messageParameters;

		// Set and check if the message at abort should be emitted
		mSignalOnAbort = emitMessageOnAbort;
		if (mSignalOnAbort)
		{
			mMessageAtAbort = message;
			mMessageParametersAtAbort = messageParameters;
		}
	}

	void SignalAction::init(const qsf::MessageConfiguration& messageAtUpdate, const qsf::MessageConfiguration& messageAtAbort,
							const qsf::MessageParameters& messageParametersAtUpdate, const qsf::MessageParameters& messageParametersAtAbort)
	{
		mSignalOnAbort = true;
		mMessageAtUpdate = messageAtUpdate;
		mMessageAtAbort = messageAtAbort;
		mMessageParametersAtUpdate = messageParametersAtUpdate;
		mMessageParametersAtAbort = messageParametersAtAbort;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void SignalAction::serialize(qsf::BinarySerializer& serializer)
	{
		mMessageAtUpdate.serialize(serializer);
		mMessageAtAbort.serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result SignalAction::updateAction(const qsf::Clock&)
	{
		QSF_MESSAGE.emitMessage(mMessageAtUpdate, mMessageParametersAtUpdate);
		return qsf::action::RESULT_DONE;
	}

	void SignalAction::onAbort()
	{
		if (mSignalOnAbort)
		{
			QSF_MESSAGE.emitMessage(mMessageAtAbort, mMessageParametersAtAbort);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
