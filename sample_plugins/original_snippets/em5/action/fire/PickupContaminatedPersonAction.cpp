// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PickupContaminatedPersonAction.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/LeadPersonTargetPointProvider.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/TurnToAction.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupContaminatedPersonAction::ACTION_ID = "em5::PickupContaminatedPersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupContaminatedPersonAction::PickupContaminatedPersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mAllowLyingPersons(true)
	{
		// Nothing here
	}

	PickupContaminatedPersonAction::~PickupContaminatedPersonAction()
	{
		// Nothing here
	}

	void PickupContaminatedPersonAction::init(uint64 targetEntityId, bool allowLyingPersons)
	{
		mTargetEntityId = targetEntityId;
		mAllowLyingPersons = allowLyingPersons;
	}

	uint64 PickupContaminatedPersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupContaminatedPersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupContaminatedPersonAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (targetEntity == nullptr)
			return false;

		// Squad already carrying someone/something
		if (EntityHelper(callerEntity).isCarrying())
			return false;

		// Check if the target is already linked to some parent
		qsf::LinkComponent* linkComponent = targetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent && linkComponent->hasParent())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Try to get event ID component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result PickupContaminatedPersonAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (targetEntity == nullptr)
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					// For plague infected persons, make sure they can't lose energy any more
					//  -> This is especially important as they are placed inside the RTW later on without being healed
					if (healthComponent->getContaminationType() == Injury::CONTAMINATION_PLAGUE)
					{
						healthComponent->setInvincible(true);
					}

					const HealthComponent::State state = healthComponent->getState();
					if (state == HealthComponent::STATE_HEALTHY)
					{
						getComponent().pushAction<MoveAction>(getPriority()).init(new qsf::ai::ReachObjectGoal(getEntity(), *targetEntity, LeadPersonTargetPointProvider::TARGET_POINT_ID));
						getComponent().pushAction<qsf::TurnToAction>(getPriority()).init(targetEntity->getId());

						mState = STATE_REACHED_LEAD_TARGET;
						return qsf::action::RESULT_CONTINUE;
					}
					else if (mAllowLyingPersons && (state == HealthComponent::STATE_INJURED || state == HealthComponent::STATE_DEATHFIGHT))
					{
						getComponent().pushAction<qsf::TurnToAction>(getPriority()).init(targetEntity->getId());

						mState = STATE_REACHED_PULL_TARGET;
						return qsf::action::RESULT_CONTINUE;
					}
				}

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_REACHED_PULL_TARGET:
			{
				getComponent().pushAction<PullPersonAction>(getPriority()).init(mTargetEntityId, PullPersonAction::START, true);

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_REACHED_LEAD_TARGET:
			{
				getComponent().pushAction<GuidePersonAction>(getPriority()).init(mTargetEntityId, GuidePersonAction::LEAD, true);

				// Ensure animation updates
				AnimationHelper(getEntity()).clearAnimation();
				AnimationHelper(*targetEntity).clearAnimation();

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}

	void PickupContaminatedPersonAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
