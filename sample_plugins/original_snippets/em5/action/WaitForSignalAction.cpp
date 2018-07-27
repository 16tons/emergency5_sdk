// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/WaitForSignalAction.h"

#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier WaitForSignalAction::ACTION_ID = "em5::WaitForSignalAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WaitForSignalAction::WaitForSignalAction() :
		qsf::Action(ACTION_ID),
		mMessageReceived(false),
		mWaitInfinite(false)
	{
		// Nothing here
	}

	WaitForSignalAction::~WaitForSignalAction()
	{
		// Nothing here
	}

	void WaitForSignalAction::init(const qsf::MessageConfiguration& message)
	{
		mWaitInfinite = true;
		mMessageReceived = false;

		// Init max wait time with a value > zero
		mMaxWaitTime = qsf::Time::fromMicroseconds(1);
		mMessageProxy.registerAt(message, boost::bind(&WaitForSignalAction::onMessage, this, _1));
	}

	void WaitForSignalAction::init(const qsf::MessageConfiguration& message, const qsf::Time& maxWaitTime)
	{
		mWaitInfinite = false;
		mMaxWaitTime = maxWaitTime;
		mMessageReceived = false;
		mMessageProxy.registerAt(message, boost::bind(&WaitForSignalAction::onMessage, this, _1));
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	void WaitForSignalAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mMessageReceived);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result WaitForSignalAction::updateAction(const qsf::Clock& clock)
	{
		if (!mWaitInfinite)
		{
			mMaxWaitTime -= clock.getTimePassed();
		}

		if (mMessageReceived || mMaxWaitTime <= qsf::Time::ZERO)
			return qsf::action::RESULT_DONE;

		return qsf::action::RESULT_CONTINUE;
	}

	void WaitForSignalAction::onMessage(const qsf::MessageParameters&)
	{
		mMessageProxy.unregister();
		mMessageReceived = true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
