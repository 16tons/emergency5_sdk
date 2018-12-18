// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/BeStunnedAction.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/renderer/animation/MeshAnimationTestComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier BeStunnedAction::ACTION_ID = "em5::BeStunnedAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BeStunnedAction::BeStunnedAction() :
		Action(ACTION_ID),
		mStunDuration(qsf::Time::fromSeconds(0.0)),
		mInitiatorEntityId(qsf::getUninitialized<uint64>()),
		mStunnedType(StunnedType_Pepperspray),
		mRestartStunAnimation(true)
	{
		// Nothing here
	}

	BeStunnedAction::~BeStunnedAction()
	{
		// Nothing here
	}

	void BeStunnedAction::init(uint64 initiatorEntityId, const qsf::Time& stunDuration, StunnedType stunnedType)
	{
		mInitiatorEntityId = initiatorEntityId;
		mStunnedType = stunnedType;
		mRestartStunAnimation = true;

		// This extra check is to avoid reinitializing with lower values
		if (stunDuration > mStunDuration)
		{
			mStunDuration = stunDuration;
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void BeStunnedAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mStunDuration);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool BeStunnedAction::onStartup()
	{
		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			if (personComponent->getPersonFlags(PersonComponent::FLAG_HANDCUFFED))
				return false;

			personComponent->setPersonFlags(PersonComponent::FLAG_STUNNED, true);
		}

		CloseQuartersCombatAction* fightAction = getComponent().getAction<CloseQuartersCombatAction>();
		if (nullptr != fightAction)
		{
			// Gangster was fighting, cancel the fight and remove that action!
			getComponent().removeAction(*fightAction);
		}

		return true;
	}

	void BeStunnedAction::onShutdown()
	{
		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->setPersonFlags(PersonComponent::FLAG_STUNNED, false);
		}

		// Lose the stunned animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	qsf::action::Result BeStunnedAction::updateAction(const qsf::Clock& clock)
	{
		// Time progress
		mStunDuration -= clock.getTimePassed();

		AnimationHelper animationHelper(getEntity());
		std::string animationName = (mStunnedType == StunnedType_Pepperspray ? animationHelper.getAnimationStunnedByPepperspray() : (mStunnedType == StunnedType_DogAttack ? animationHelper.getAnimationGangsterCowerFearfullyLoop() : animationHelper.getAnimationStunnedByFlashGrenade()));

		if (!animationHelper.isAnimationPlaying(animationName) || mRestartStunAnimation)
		{
			mRestartStunAnimation = false;	// Used incase we reinitialize the action
			// Loop flashed animation with blendtime(!)
			// TODO(mk) Here is a bug, with the pushing of repeatly the same animation. My plan was to blend in the animation, but it is not possible with loop
			animationHelper.playAnimation(animationName, true, false, qsf::Time::fromSeconds(0.4f));
		}

		if (mStunDuration > qsf::Time::ZERO)
		{
			return qsf::action::RESULT_CONTINUE;
		}

		// No longer stunned
		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			// Just to be sure the flag is no longer set (e.g. when attacking the initiator)
			personComponent->setPersonFlags(PersonComponent::FLAG_STUNNED, false);
		}

		// Support pre edited animation
		qsf::MeshAnimationTestComponent* meshAnimationTestComponent = getEntity().getComponent<qsf::MeshAnimationTestComponent>();
		if (nullptr != meshAnimationTestComponent && meshAnimationTestComponent->isActive() && meshAnimationTestComponent->isTestComponentUpdatesEnabled())
		{
			// Reactivate the component will force the update again
			meshAnimationTestComponent->setUseRandomOffset(false);	// Start the animation from the beginning in any case, not somewhere in between
			meshAnimationTestComponent->forceRestart(qsf::Time::fromSeconds(0.5f));
		}

		// TODO(mk) I don't think here is a good point for revenge. starting acts of revenge should be part of the gangster action. BTW not all gangster wants to start attacking the target
		if (qsf::isInitialized(mInitiatorEntityId))
		{
			// Let gangster attack the guy who stunned him (if possible)
			qsf::Entity* target = getMap().getEntityById(mInitiatorEntityId);
			if (target != nullptr)
			{
				// Target exists
				qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent)
				{
					GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
					if (gangsterLogic != nullptr)
					{
						// Attack the Stun-Grenade-Thrower with the preferred weapon
						gangsterLogic->attack(*target);
					}
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
