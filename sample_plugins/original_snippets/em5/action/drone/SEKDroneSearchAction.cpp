// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/drone/SEKDroneSearchAction.h"
#include "em5/action/drone/StartDroneAction.h"
#include "em5/action/drone/LandDroneAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioProxy.h"
#include "em5/audio/AudioHelper.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/HighPriorityDroneTarget.h"
#include "em5/component/objects/TargetFoundCounterComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/game/Game.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/QsfHelper.h>

#include <glm/gtx/compatibility.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SEKDroneSearchAction::ACTION_ID = "em5::SEKDroneSearchAction";
	const qsf::Time SEKDroneSearchAction::TIME_BETWEEN_CLICKS = qsf::Time::fromSeconds(3.0f);
	const qsf::Time SEKDroneSearchAction::TIME_DRONE_FLY_WITHOUT_TARGET = qsf::Time::fromSeconds(5.0f);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SEKDroneSearchAction::SEKDroneSearchAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mDroneTransportLogic(nullptr),
		mSecondsPassedSinceClick(qsf::Time::ZERO),
		mNewTarget(false)
	{
		// Nothing here
	}

	SEKDroneSearchAction::~SEKDroneSearchAction()
	{
		// Nothing here
	}

	void SEKDroneSearchAction::init(qsf::Entity* droneTransporter)
	{
		mDroneTransporter = droneTransporter;
	}

	void SEKDroneSearchAction::searchNewTarget()
	{
		qsf::Entity* oldTarget = mSearchTarget.get();
		mSearchTarget = getSearchTarget();
		if (!mSearchTarget.valid())
		{
			// Because we skipped the search target, we can use it in case we have no better one
			mSearchTarget = oldTarget;
		}

		mTimeWithoutTarget = qsf::Time::ZERO;
		mState = STATE_SEARCH_GANGSTER;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SEKDroneSearchAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) add serialization
	}

	bool SEKDroneSearchAction::onStartup()
	{
		if (!mDroneTransporter.valid())
			return false;

		const qsf::GameLogicComponent* gameLogicComponent = mDroneTransporter->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false;

		mDroneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
		if (nullptr == mDroneTransportLogic)
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result SEKDroneSearchAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				// Start drone, if necessary
				if (mDroneTransportLogic->getIsDroneUnitedWithLeader())
				{
					qsf::ActionComponent& actionComponent = mDroneTransporter->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.clearPlan();
					actionComponent.pushAction<StartDroneAction>(action::BLOCKING);
				}

				mState = STATE_WAIT_FOR_START;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_START:
			{
				if (mDroneTransportLogic->getIsDroneUnitedWithLeader())
				{
					// Wait till the unlink process is finished
					return qsf::action::RESULT_CONTINUE;
				}

				mState = STATE_SEARCH_GANGSTER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SEARCH_GANGSTER:
			{
				if (mSearchTarget.valid())
				{
					mNewTarget = true;

					mSecondsPassedSinceClick = qsf::Time::ZERO;
					mTimeWithoutTarget = qsf::Time::ZERO;

					getComponent().pushAction<MoveAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(new qsf::ai::ReachObjectGoal(getEntity(), mSearchTarget.getSafe(), AttackPersonTargetPointProvider::TARGET_POINT_ID));
					mState = STATE_GOTO_GANGSTER;
				}
				else
				{
					mTimeWithoutTarget += clock.getTimePassed();
					if (mTimeWithoutTarget > TIME_DRONE_FLY_WITHOUT_TARGET)
					{
						// Send drone back to transporter
						if (mDroneTransporter.valid())
						{
							// The drone transporter calls the drone back, easy
							qsf::ActionComponent& transporterActionComponent = mDroneTransporter->getOrCreateComponentSafe<qsf::ActionComponent>();
							transporterActionComponent.pushAction<LandDroneAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK);
						}
						// else the drone transporter must be destroyed, in this case the drone is flying away or disappears

						// Finish current action
						return qsf::action::RESULT_DONE;
					}
					else
					{
						// Search new target
						mSearchTarget = getSearchTarget();
					}
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_GOTO_GANGSTER:
			{
				// Validate the search target again, maybe something changed
				if (mSearchTarget.valid() && (isValidDroneTarget(mSearchTarget->getComponent<PersonComponent>()) || mSearchTarget->getComponent<HighPriorityDroneTarget>()))
				{
					{
						// Move and rotate closer to target
						const qsf::TransformComponent& targetTransformComponent = mSearchTarget->getOrCreateComponentSafe<qsf::TransformComponent>();
						const qsf::TransformComponent& heliTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

						const glm::vec3& heliPosition = heliTransformComponent.getPosition();
						glm::vec3 targetPosition = targetTransformComponent.getPosition();

						// Eliminate high level
						targetPosition.y = heliPosition.y;

						const glm::quat lookToTargetRotation = qsf::Math::getLookAtQuaternion(heliPosition, targetPosition, qsf::CoordinateSystem::getUp());

						// Move caller closer to target
						glm::vec3 newPosition = lookToTargetRotation * glm::vec3(0, 0, 1.f) + targetPosition;
						newPosition = glm::lerp(newPosition, heliPosition, 0.1f * clock.getTimePassed().getSeconds());

						// Turn to target
						const glm::quat newRotation = glm::lerp(lookToTargetRotation, heliTransformComponent.getRotation(), 0.1f * clock.getTimePassed().getSeconds());

						qsf::MovableComponent& heliMovableComponent = getEntity().getOrCreateComponentSafe<qsf::MovableComponent>();
						heliMovableComponent.moveToPositionAndRotation(newPosition, newRotation);
					}

					if (mSearchTarget->getComponent<HighPriorityDroneTarget>() == nullptr)
					{
						// Make target visible
						qsf::game::HiddenComponent::setHidden(mSearchTarget.getSafe(), false);
					}

					mSecondsPassedSinceClick += clock.getTimePassed();
					if (mSecondsPassedSinceClick > TIME_BETWEEN_CLICKS)
					{
						mSecondsPassedSinceClick = qsf::Time::ZERO;

						AudioProxy audioProxy;
						AudioHelper::playSEKDroneClick(audioProxy, getEntity());
						audioProxy.detach();
						audioProxy.synchronizeWithMultiplay();
					}

					// If the target is just found
					if (mNewTarget)
					{
						mNewTarget = false;
						if (mSearchTarget->getComponent<HighPriorityDroneTarget>() == nullptr)
						{
							// Play hint
							showHint();

							// Send message
							QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FIND_HIDDEN_PERSON_WITH_SEK_DRONE, mSearchTarget->getId(), getEntityId()));
						}
						else
						{
							// Send message
							QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_REACHED_HIGH_PRIORITY_TARGET_WITH_SEK_DRONE, mSearchTarget->getId(), getEntityId()));
						}

						// Increase the number of target searches
						mSearchTarget->getOrCreateComponentSafe<TargetFoundCounterComponent>().increaseNumber();
					}
				}
				else
				{
					// Target is invalid, search new one
					mState = STATE_SEARCH_GANGSTER;
					mSearchTarget = nullptr;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
				// Nothing to do in this case
				break;
		}

		// Error, abort action
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SEKDroneSearchAction::showHint() const
	{
		HintHelper::showHint("ID_HINT_SEKD_GANGSTERFOUND_01");
		EM5_GAME.getMiniMapIconRegistrationLogic().registerHighlightIcon(mSearchTarget->getId());
	}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	qsf::Entity* SEKDroneSearchAction::getSearchTarget() const
	{
		const auto& highPriorityTargets = qsf::ComponentMapQuery(getMap()).getAllInstances<HighPriorityDroneTarget>();
		if (!highPriorityTargets.empty())
		{
			HighPriorityDroneTarget& component = **highPriorityTargets.begin();
			return &component.getEntity();
		}

		// SEK drone has unlimited search radius by intent
		const glm::vec3& currentPosition = EntityHelper(getEntity()).getPosition();
		const auto& personComponentList = qsf::ComponentMapQuery(getMap()).getAllInstances<PersonComponent>();

		qsf::EntityVectorScratchBuffer potentialVisibleTargets;
		qsf::EntityVectorScratchBuffer potentialHiddenTargets;
		for (PersonComponent* personComponent : personComponentList)
		{
			if (!isValidDroneTarget(personComponent))
				continue;

			qsf::Entity& entity = personComponent->getEntity();
			if (EntityHelper(entity).isEntityHidden())
			{
				potentialHiddenTargets.push_back(&entity);
			}
			else
			{
				potentialVisibleTargets.push_back(&entity);
			}
		}

		// Used the same mechanic as in SendDogForSearchCommand::getNextTarget() give priority to targets, add the amount of already searched from TargetFoundComponent to have a priority. Then take the closed
		// Order the targets by priority
		std::multimap<int, qsf::Entity*> priorityMap;
		addListWithPriority(priorityMap, potentialHiddenTargets, 0);
		addListWithPriority(priorityMap, potentialVisibleTargets, 1);

		qsf::EntityVectorScratchBuffer lowestResult;
		if (!priorityMap.empty())
		{
			// Take first element as key
			auto equalListResult = priorityMap.equal_range(priorityMap.begin()->first);
			auto startIterator = equalListResult.first;
			// Return all elements with the lowest key
			while (startIterator != equalListResult.second)
			{
				lowestResult.push_back(startIterator->second);
				++startIterator;
			}
		}

		// Find shortest target (sorted by priority)
		qsf::Entity* shortestEntity = nullptr;
		float shortestDistance = FLT_MAX;
		for (qsf::Entity* target : lowestResult)
		{
			const float distance = qsf::game::DistanceHelper::get2dDistance(currentPosition, *target);
			if (shortestEntity == nullptr || distance < shortestDistance)
			{
				if (mSearchTarget.valid() && mSearchTarget == target)
				{
					// skip already existing target
					continue;
				}
				shortestDistance = distance;
				shortestEntity = target;
			}
		}

		return shortestEntity;
	}

	bool SEKDroneSearchAction::isValidDroneTarget(PersonComponent* personComponent) const
	{
		if (nullptr == personComponent)
			return false;

		if (!personComponent->getGangsterFlag() || personComponent->getHandcuffedFlag())
			return false;

		qsf::Entity& gangsterEntity = personComponent->getEntity();
		const HealthComponent* healthComponent = gangsterEntity.getComponent<HealthComponent>();
		if (nullptr != healthComponent && healthComponent->isInjured())
			return false;

		// Gangsters inside buildings will be ignored (important for EM2017 "Manhunt" mini-event)
		EntityHelper gangsterEntityHelper(gangsterEntity);
		qsf::Entity* container = gangsterEntityHelper.getContainerEntity();
		if (nullptr != container && nullptr != container->getComponentById("em5::BuildingComponent"))
		{
			return false;
		}

		// Check if gangster is part of a hidden event
		FreeplayEvent* freeplayEvent = gangsterEntityHelper.getFreeplayEvent();
		if (nullptr != freeplayEvent)
		{
			if (freeplayEvent->getState() == FreeplayEvent::State::STATE_HIDDEN)
				return false;
		}

		// Maybe anyone will block this entity from being our target, as them:
		{
			qsf::MessageConfiguration msg(Messages::EM5_SEK_DRONE_INVALID_TARGET_REQUEST, getEntityId(), gangsterEntity.getId());
			qsf::MessageParameters params;
			bool response = false;

			// Response is only valid when emit returns true
			if (QSF_MESSAGE.emitMessageWithResponse(msg, response, params))
			{
				// The responders will return "true" if the target is considered invalid. But we return validity, so we need to invert the flag.
				return !response;
			}
		}

		return true;
	}

	// TODO(mk) the same function as SendDogForSearchCommand::addListWithPriority()
	void SEKDroneSearchAction::addListWithPriority(std::multimap<int, qsf::Entity*>& result, std::vector<qsf::Entity*>& list, int priority) const
	{
		for (qsf::Entity* entity : list)
		{
			int key = priority;
			TargetFoundCounterComponent* targetFoundCounterComponent = entity->getComponent<TargetFoundCounterComponent>();
			if (nullptr != targetFoundCounterComponent)
			{
				key += targetFoundCounterComponent->getNumber();
			}

			result.emplace(key, entity);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
