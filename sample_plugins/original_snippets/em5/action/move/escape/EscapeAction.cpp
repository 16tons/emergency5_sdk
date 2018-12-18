// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/escape/EscapeAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/component/GameComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/math/Random.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void EscapeAction::catched()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::MoveAction methods            ]
	//[-------------------------------------------------------]
	bool EscapeAction::onStartup()
	{
		// Call base class implementation
		if (!qsf::ai::MoveAction::onStartup())
			return false;

		const EntityHelper entityHelper(getEntity());
		mIsCivilPerson = entityHelper.isCivilPerson();
		if (mIsCivilPerson)
		{
			getEntity().getOrCreateComponent<PersonPlacementComponent>();
		}

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	EscapeAction::EscapeAction(uint32 actionId) :
		qsf::ai::MoveAction(actionId),
		mIsCivilPerson(false)
	{
		// Nothing here
	}

	EscapeAction::~EscapeAction()
	{
		// Nothing here
	}

	qsf::Entity* EscapeAction::getTargetEntity() const
	{
		if (!mEscapeTargetTag.empty())
		{
			const auto& list = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(mEscapeTargetTag), QSF_MAINMAP);
			if (!list.empty())
			{
				return &qsf::Random::getAnyOf(list)->getEntity();
			}
		}
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	bool EscapeAction::checkAllowUpdate() const
	{
		if (mGameComponent.valid() && mIsCivilPerson)
		{
			return mGameComponent->getCivilPersonMovement();
		}

		return true;
	}

	void EscapeAction::pushBreathAnimation(qsf::Time animationTime)
	{
		// Wait some seconds and play animations for breathing (gangster is out of power and need to recharge)
		qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
		AnimationHelper animationHelper(getEntity());
		actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationBreatheDeeply(), true, false, false, qsf::Time::ZERO, animationTime);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
