// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterFanaticArsonistLogic.h"
#include "em5/ai/MovementModes.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/component/objects/StakeComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/GotoObjectFrontsideTargetPointProvider.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagHelper.h>
#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Random.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{
		qsf::logic::TargetPoint cheeringTargetPoint(qsf::Entity& callerEntity, qsf::Entity* targetEntity)
		{
			float radius = 0.0f;

			if (nullptr != targetEntity)
			{
				const ComplexFireComponent* complexFireComponent = targetEntity->getComponent<ComplexFireComponent>();
				if (nullptr != complexFireComponent)
				{
					std::vector<FireComponent*> allFireComponents;
					complexFireComponent->getAllFireComponents(allFireComponents);
					for (FireComponent* fireComponent : allFireComponents)
					{
						// Find the radius in which fire damage is taken
						float dangerRadius = glm::max(fireComponent->getSoftRadius(), fireComponent->getHardRadius());
						radius = glm::max(radius, dangerRadius);
					}
				}
				else
				{
					// Only relevant when no complex fire component exists
					const FireComponent* fireComponent = targetEntity->getComponent<FireComponent>();
					if (nullptr != fireComponent)
					{
						radius = glm::max(radius, fireComponent->getSoftRadius());
					}
				}
			}
			else
			{
				QSF_WARN("Target for gangster fanatic arsonist logic is not valid anymore", QSF_REACT_NONE);
			}

			// Fallback solution in case no reasonable soft/hard-radius exists
			radius = glm::min(radius, 5.0f);

			qsf::TransformComponent* gangsterTransformComponent = callerEntity.getOrCreateComponent<qsf::TransformComponent>();
			const glm::quat rot = gangsterTransformComponent->getRotation();
			const glm::quat rotationXZ = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), rot);

			glm::vec3 cheerVector = radius * (rotationXZ * qsf::CoordinateSystem::getIn());

			// Step back from target to cheer
			glm::vec3 cheerPos = gangsterTransformComponent->getPosition() - cheerVector;
			return qsf::logic::TargetPoint(cheerPos, false, 0.0f, 0.0f, rotationXZ);
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterFanaticArsonistLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterFanaticArsonistLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterFanaticArsonistLogic::GangsterFanaticArsonistLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>()),
		mIgniteTimeStake(qsf::Time::fromSeconds(20.0f)),
		mIgniteTimeObject(qsf::Time::fromSeconds(4.0f)),
		mCheeringTime(qsf::Time::fromSeconds(20.0f)),
		mCurrentState(STATE_ANGRY_PROTEST),
		mCurrentTargetIndex(-1)
	{
		mEquippedWeaponTypes.set(weapon::TORCH);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;
	}

	void GangsterFanaticArsonistLogic::setArsonistTargets(std::vector<std::string>& targetEventTags)
	{
		mTargetEventTags = targetEventTags;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterFanaticArsonistLogic::onStartup()
	{
		if (nullptr == getEntity())
			return false;	// Error

		mStartingTransform = EntityHelper(getEntitySafe()).getTransform();

		FireReceiverComponent* fireReceiverComponent = getEntity()->getComponent<FireReceiverComponent>();
		if (fireReceiverComponent)
		{
			// Make arsonist invulnerable against fire
			fireReceiverComponent->setActive(false);
		}

		// Call base class for future startup routines
		return GangsterBaseLogic::onStartup();
	}

	void GangsterFanaticArsonistLogic::onShutdown()
	{
		mIgniteTargetMessageProxy.unregister();

		FireReceiverComponent* fireReceiverComponent = getEntity()->getComponent<FireReceiverComponent>();
		if (fireReceiverComponent)
		{
			fireReceiverComponent->setActive(true);
		}

		// Call base class
		GangsterBaseLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterFanaticArsonistLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		if (isTargetInvalid())
		{
			// Current target was empty or ignited by other arsonists (No Error). Search next target.
			actionComponent.clearPlan();
			mCurrentState = STATE_GO_TO_NEXT_TARGET;
		}

		// Check if gangster still got work to do
		if (!actionComponent.getPlan().isEmpty())
		{
			// Gangster still busy, so we do nothing
			return;
		}

		switch (mCurrentState)
		{
			case STATE_ANGRY_PROTEST:
			{
				// To avoid an armada of synchronous borgs, add small delay before starting
				actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_LOW).init( qsf::Time::fromMilliseconds(qsf::Random::getRandomInt(0, 500)));

				// Push animation
				AnimationHelper animationHelper(getEntitySafe());
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_LOW).init(animationHelper.getAnimationAngry(), true, false, false, qsf::Time::fromSeconds(0.3f));

				mCurrentState = STATE_ANGRY_PROTEST_END;
				break;
			}

			case STATE_ANGRY_PROTEST_END:
			{
				mCurrentState = STATE_GO_TO_NEXT_TARGET;
				// No break by intent
			}

			case STATE_GO_TO_NEXT_TARGET:
			{
				// Find a valid target
				qsf::Entity* nextTarget = getNextTarget();
				if (nullptr == nextTarget)
				{
					// Move to home position and go in idle state
					qsf::logic::TargetPoint homePosition(mStartingTransform.getPosition(), false, 0.0f, 0.0f, mStartingTransform.getRotation());
					actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(homePosition), MovementModes::MOVEMENT_MODE_RUN);

					mIgniteTargetMessageProxy.unregister();

					mCurrentTargetId = qsf::getUninitialized<uint64>();
					mCurrentState = STATE_DO_NOTHING;
					break;
				}

				mCurrentTargetId = nextTarget->getId();

				// Run to next target
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *nextTarget, GotoObjectFrontsideTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);

				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Register messageProxy to detect if someone else ignite the current target
				mIgniteTargetMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_OBJECT_START_BURNING, mCurrentTargetId), boost::bind(&GangsterFanaticArsonistLogic::onTargetWasIgnited, this, _1));

				mCurrentState = STATE_SET_TARGET_ON_FIRE;
				break;
			}

			case STATE_SET_TARGET_ON_FIRE:
			{
				mIgniteTargetMessageProxy.unregister();

				// Entity was check before
				qsf::Entity* currentTargetEntity = getMap().getEntityById(mCurrentTargetId);

				const float distance = qsf::game::DistanceHelper::getDistance(*currentTargetEntity, getEntitySafe());
				if (distance > 15.f)
				{
					// Error, distance to target is way to far, search next target
					QSF_WARN("GangsterFanaticArsonist can`t reach target " << mCurrentTargetId << " skip this target.", QSF_REACT_NONE)
					mCurrentState = STATE_GO_TO_NEXT_TARGET;
					break;
				}

				// Play gangster animation
				const qsf::Time& incinerateTime = (nullptr != currentTargetEntity->getComponent<StakeComponent>()) ? mIgniteTimeStake : mIgniteTimeObject;
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(AnimationHelper(*getEntity()).getAnimationManipulateObjectKneeling(), true, true, false, qsf::Time::ZERO, incinerateTime);

				// Wait till the ignite animation is over
				mCurrentState = STATE_WATCH_AND_CHEER;
				break;
			}

			case STATE_WATCH_AND_CHEER:
			{
				// The target started burning, so the next step for the gangster is to step away and watch the work / cheer
				// Set target on fire (Id is valid at this point, so the pointer has to as well)
				qsf::Entity* currentTargetEntity = getMap().getEntityById(mCurrentTargetId);
				EntityHelper(*currentTargetEntity).startFire(EntityHelper(*getEntity()).getFreeplayEvent());

				qsf::logic::TargetPoint cheerPosition = cheeringTargetPoint(*getEntity(), getMap().getEntityById(mCurrentTargetId));

				// Move away from target
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(cheerPosition), MovementModes::MOVEMENT_MODE_RUN);

				// Turn to target
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(mCurrentTargetId);

				// Cheer
				AnimationHelper animationHelper(*getEntity());
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(animationHelper.getAnimationCheer(), true, true, false, qsf::Time::ZERO, mCheeringTime);
				actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(qsf::Time::fromSeconds(2.0f));

				// The gangster is done with that target, reset the target
				mCurrentTargetId = qsf::getUninitialized<uint64>();

				// Wait for end of the watching animation
				mCurrentState = STATE_WATCH_AND_CHEER_END;
				break;
			}

			case STATE_WATCH_AND_CHEER_END:
			{
				// One target is not enough; the gangster shall repeat the whole sequence
				mCurrentState = STATE_GO_TO_NEXT_TARGET;
				break;
			}

			case STATE_DO_NOTHING:
			{
				break;
			}

			default:
			{
				// Should not happen, but please prove me wrong if so
				QSF_CHECK(false, "ArsonistEvent: Gangster has invalid next action!", QSF_REACT_NONE);
			}
		}
	}

	void GangsterFanaticArsonistLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	void GangsterFanaticArsonistLogic::onNoUnitsSpotted()
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	qsf::Entity* GangsterFanaticArsonistLogic::getNextTarget()
	{
		qsf::Entity* nextTarget = nullptr;
		while (nextTarget == nullptr)
		{
			// Increase the counter
			++mCurrentTargetIndex;

			if (mCurrentTargetIndex >= static_cast<int>(mTargetEventTags.size()))
				return nullptr;

			qsf::game::EventTagHelper eventTagHelper("GangsterFanaticArsonistLogic");
			eventTagHelper.tryAcquireTaggedEntity(nextTarget, mTargetEventTags[mCurrentTargetIndex]);

			if (nullptr == nextTarget)
				continue;

			// Validate next target
			if (EntityHelper(*nextTarget).isBurning())
			{
				// Target is not valid -> continue search for target
				nextTarget = nullptr;
			}
		}

		return nextTarget;
	}

	void GangsterFanaticArsonistLogic::onTargetWasIgnited(const qsf::MessageParameters& parameters)
	{
		// The current target was ignited, so choose next target
		mCurrentState = STATE_GO_TO_NEXT_TARGET;

		mActionComponent->clearPlan();
	}

	bool GangsterFanaticArsonistLogic::isTargetInvalid() const
	{
		if (nullptr != getEntity() && nullptr != getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>().getAction<CloseQuartersCombatAction>())
		{
			// Gangster has currently other problems, but return false to avoid clearing the action stack and finish the fight
			return false;
		}

		if (mCurrentState != STATE_GO_TO_NEXT_TARGET && mCurrentState != STATE_SET_TARGET_ON_FIRE)
			return false;	// abort, now we don't care about a target, just idle

		qsf::Entity* currentTargetEntity = getMap().getEntityById(mCurrentTargetId);
		if (nullptr == currentTargetEntity)
			return true;

		if (EntityHelper(*currentTargetEntity).isBurning())
			return true;

		StakeComponent* stakeComponent = currentTargetEntity->getComponent<StakeComponent>();
		if (nullptr != stakeComponent && !stakeComponent->getcanBeIgnited())
			return true;

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
