// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/DeployRescueBusAction.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"

#include <qsf/component/link/LinkComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DeployRescueBusAction::ACTION_ID = "em5::DeployRescueBusAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DeployRescueBusAction::DeployRescueBusAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	DeployRescueBusAction::~DeployRescueBusAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DeployRescueBusAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DeployRescueBusAction::onStartup()
	{
		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (linkComponent == nullptr)
			return false;

		mRescueBusComponent = linkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();
		if (!mRescueBusComponent.valid())
			return false;

		// Always possible
		return true;
	}

	qsf::action::Result DeployRescueBusAction::updateAction(const qsf::Clock&)
	{
		if (!mRescueBusComponent.valid())
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				if (mRescueBusComponent->isHidden())
				{
					mRescueBusComponent->startExtending();
				}
				else
				{
					mRescueBusComponent->hideExtending();
				}

				mState = STATE_DEPLOY;

				// No break by intent
			}

			case STATE_DEPLOY:
			{
				if (mRescueBusComponent->isAnimationPlaying())
				{
					return qsf::action::RESULT_CONTINUE;
				}

				mState = STATE_END;

				// No break by intent
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
