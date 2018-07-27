// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DLK/DLKDeployAction.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Map.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DLKDeployAction::ACTION_ID = "em5::DLKDeployAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKDeployAction::DLKDeployAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mGameLogicComponent(nullptr)
	{
		// Nothing here
	}

	DLKDeployAction::~DLKDeployAction()
	{
		// Nothing here
	}

	void DLKDeployAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
	}

	uint64 DLKDeployAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DLKDeployAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DLKDeployAction::onStartup()
	{
		// Get the caller and target entity
		if (nullptr == getEntity().getMap().getEntityById(mTargetEntityId))
			return false;

		updateGameLogicComponent();

		return true;
	}

	qsf::action::Result DLKDeployAction::updateAction(const qsf::Clock&)
	{
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		DlkLadderComponent* dlkLadderComponent = getEntity().getComponent<DlkLadderComponent>();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);

		if (nullptr != supportLegsComponent && nullptr != targetEntity && nullptr != dlkLadderComponent && nullptr != mGameLogicComponent)
		{
			DLKLogic& dlkLogic = mGameLogicComponent->getOrCreateGameLogicSafe<em5::DLKLogic>();
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					// Ignore all damage in this action
					EntityHelper(getEntity()).setInvincible(true);

					// Ensure we undeploy before moving next
					dlkLogic.undeployBeforeMovingNext();

					// Extend the support legs
					supportLegsComponent->startExtending();
					mCurrentState = STATE_EXTENDING_LEGS;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_EXTENDING_LEGS:
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for leg animation to end
						return qsf::action::RESULT_CONTINUE;
					}

					dlkLadderComponent->deployLadder(getTargetPosition(*targetEntity));
					mCurrentState = STATE_LADDER_ALIGNMENT;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_LADDER_ALIGNMENT:
				{
					if (dlkLadderComponent->isAnimationPlaying())
					{
						// Wait until the DLK ladder arm is aligned to the target
						return qsf::action::RESULT_CONTINUE;
					}

					// Allow receiving damage again
					EntityHelper(getEntity()).setInvincible(false);

					mCurrentState = STATE_DONE;

					// Fall through by design
				}

				case STATE_DONE:
				{
					// Fall through by design
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void DLKDeployAction::updateGameLogicComponent()
	{
		mGameLogicComponent = getEntity().getOrCreateComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != mGameLogicComponent, "Could not update the game logic component of the entity: " << getEntityId(), QSF_REACT_THROW);
	}

	const glm::vec3& DLKDeployAction::getTargetPosition(qsf::Entity& targetEntity) const
	{
		// Get deploy target
		return targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
