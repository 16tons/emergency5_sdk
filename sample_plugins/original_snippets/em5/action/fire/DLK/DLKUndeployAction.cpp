// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DLK/DLKUndeployAction.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"

#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DLKUndeployAction::ACTION_ID = "em5::DLKUndeployAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKUndeployAction::DLKUndeployAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mGameLogicComponent(nullptr)
	{
		// Nothing here
	}

	DLKUndeployAction::~DLKUndeployAction()
	{
		// Nothing here
	}

	void DLKUndeployAction::init()
	{
		updateGameLogicComponent();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DLKUndeployAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DLKUndeployAction::onStartup()
	{
		updateGameLogicComponent();

		return true;
	}

	qsf::action::Result DLKUndeployAction::updateAction(const qsf::Clock&)
	{
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		DlkLadderComponent* dlkLadderComponent = getEntity().getComponent<DlkLadderComponent>();

		if (nullptr != supportLegsComponent && nullptr != dlkLadderComponent && nullptr != mGameLogicComponent)
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					dlkLadderComponent->foldLadder();

					mCurrentState = STATE_FOLD_LADDER;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_FOLD_LADDER:
				{
					if (dlkLadderComponent->isAnimationPlaying())
					{
						// Wait until the ladder is fold back
						return qsf::action::RESULT_CONTINUE;
					}

					supportLegsComponent->hideExtending();
					mCurrentState = STATE_HIDE_LEGS;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_HIDE_LEGS:
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for leg animation to end
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					// Fall through by design
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void DLKUndeployAction::updateGameLogicComponent()
	{
		mGameLogicComponent = getEntity().getOrCreateComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != mGameLogicComponent, "Could not update the game logic component of the entity: " << getEntityId(), QSF_REACT_THROW);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
