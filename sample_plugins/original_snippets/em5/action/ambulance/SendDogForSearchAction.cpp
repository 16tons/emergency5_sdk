// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/SendDogForSearchAction.h"
#include "em5/action/ambulance/RescueDogLocateVictimAction.h"
#include "em5/action/ambulance/CombineDogWithLeaderAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/PlayHintAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/police/DogAttackGangsterAction.h"
#include "em5/action/police/PoliceDogLocateObjectAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/audio/AudioHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/DebugDrawManager.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SendDogForSearchAction::ACTION_ID = "em5::SendDogForSearchAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SendDogForSearchAction::SendDogForSearchAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mUseCase(UseCase::RESCUE_DOG_SEARCH_VICTIM),
		mAnyFound(false)
	{
		// Nothing here
	}

	SendDogForSearchAction::~SendDogForSearchAction()
	{
		// Nothing here
	}

	void SendDogForSearchAction::init(qsf::Entity* targetEntity, UseCase useCase, bool* anyFound)
	{
		mTargetEntity = targetEntity;
		mUseCase = useCase;
		mAnyFound = (nullptr != anyFound) ? *anyFound : (nullptr != targetEntity);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SendDogForSearchAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SendDogForSearchAction::onStartup()
	{
		mRescueDogEntity = RescueDogLeaderLogic::getRescueDogEntityFromLeader(getEntity());
		if (!mRescueDogEntity.valid())
			return false; // Error

		PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->forceIdleAnimationUpdate();
		}

		// Action is send without move order before, no additional checks necessary
		return true;
	}

	qsf::action::Result SendDogForSearchAction::updateAction(const qsf::Clock&)
	{
		// Here we don't check the target because the action can handle both cases. In case we haven't a target we push x random positions in the search range of the dog.
		if (!mRescueDogEntity.valid())
			return qsf::action::RESULT_DONE;	//Error

		switch (mState)
		{
			case STATE_INIT:
			{
				if (mTargetEntity.valid())
				{
					// Create reservation to avoid multiple targets
					ReserveLogic::createReservation<ReserveRescueDogLogic>(mTargetEntity.getSafe(), mRescueDogEntity->getId());
				}

				playRescueDogLeaderAnimation();

				// Play audio
				AudioHelper::playRescueDogWhistle(mAudioProxy, getEntity(), qsf::Time::fromSeconds(3.2f));
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_SENDDOG;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SENDDOG:
			{
				AnimationHelper animationHelper(getEntity());
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationRescueDogLeaderSendDog()))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}
				RescueDogLeaderLogic::playDogPantAudio(getEntity(), true);
				RescueDogLeaderLogic::delinkDogFromLeader(getEntity());
				if (mTargetEntity.valid())
				{
					pushMoveToTarget();
				}
				else
				{
					pushMoveToRandomPositions();
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SendDogForSearchAction::playRescueDogLeaderAnimation() const
	{
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationRescueDogLeaderSendDog(), true);
	}

	void SendDogForSearchAction::pushMoveToTarget()
	{
		qsf::Entity* targetEntity = mTargetEntity.get();

		// Find a target for the rescue dog. In case the target is in an vehicle or blocked, use this entity.
		qsf::LinkComponent* targetLinkComponent = mTargetEntity->getComponent<qsf::LinkComponent>();
		if (nullptr != targetLinkComponent)
		{
			if (targetLinkComponent->getParentLinkType() == qsf::LinkComponent::HARD_LINK && nullptr != targetLinkComponent->getParentPrototype())
			{
				targetEntity = static_cast<qsf::Entity*>(targetLinkComponent->getParentPrototype());
			}
		}

		QSF_CHECK(nullptr != targetEntity, "SendDogForSearchAction::pushMoveToTarget() No valid target for rescue dog. Data must be corrupted.", return);

		qsf::ActionComponent& actionComponent = mRescueDogEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();

		// Push action
		switch (mUseCase)
		{
			case UseCase::RESCUE_DOG_SEARCH_VICTIM:
			{
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*mRescueDogEntity, *targetEntity, OnTouchTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_SEARCHING);
				actionComponent.pushAction<RescueDogLocateVictimAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(mTargetEntity.getSafe(), getEntity());
				break;
			}
			case UseCase::POLICE_DOG_SEARCH_OBJECT:
			{
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*mRescueDogEntity, *targetEntity, OnTouchTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_SEARCHING);
				actionComponent.pushAction<PoliceDogLocateObjectAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(mTargetEntity.getSafe(), getEntity());
				break;
			}
			case UseCase::POLICE_DOG_ATTACK_GANGSTER:
			{
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*mRescueDogEntity, *targetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
				actionComponent.pushAction<DogAttackGangsterAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(mTargetEntity.getSafe(), getEntity());
				break;
			}
		}

		// Reserve the target
		ReserveLogic::createReservation<ReserveRescueDogLogic>(mTargetEntity.getSafe(), mRescueDogEntity->getId());
	}

	void SendDogForSearchAction::pushMoveToRandomPositions()
	{
		// Reset the search range
		float searchRange = 8.0f; // Here we don`t want the rescue dog search action range. This is to far. Take hardcoded value.
		qsf::ActionComponent& actionComponent = mRescueDogEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		qsf::Map& map = getMap();
		const glm::vec3& entityPosition = EntityHelper(getEntity()).getPosition();
		const bool showDebug = GameDebugGroup::getInstanceSafe().getShowAutomatismDebug();

		// Push x random positions in range
		const int numberFailMoves = LogicSpecsGroup::getInstanceSafe().getRescueDogNumberFailMoves();
		for (int i = 0; i < numberFailMoves; ++i)
		{
			const glm::vec3& position = getRandomPositionInRange(entityPosition, map, searchRange);

			// Check if position is reachable
			// Pushing random positions can lead to errors in the AI, check if the position is in an valid place
			if (CollisionHelper(map).isInBlockedArea(getEntity(), position))
			{
				--i;
			}

			qsf::logic::TargetPoint moveTargetConfig;
			moveTargetConfig.mPosition = position;
			actionComponent.pushAction<MoveAction>(action::BLOCKING).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_SEARCHING);

			if (showDebug)
			{
				QSF_DEBUGDRAW.requestDraw(qsf::CircleDebugDrawRequest(position, qsf::CoordinateSystem::getUp(), 0.5f, qsf::Color4::GREEN), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
			}
		}
		if (showDebug)
		{
			QSF_DEBUGDRAW.requestDraw(qsf::CircleDebugDrawRequest(entityPosition, qsf::CoordinateSystem::getUp(), searchRange, qsf::Color4::WHITE), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
		}

		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING).init(AnimationHelper(mRescueDogEntity.getSafe()).getAnimationRescueDogFoundVictim(), true);

		// Push a hint via action
		if (UnitHelper(*mRescueDogEntity).isAmbulanceRescueDog())
		{
			if (mAnyFound)
				actionComponent.pushAction<PlayHintAction>(action::BLOCKING).init("ID_RESCUEDOG_NO_WAY_01", HintMessage::MESSAGETYPE_HINT);
			else
				actionComponent.pushAction<PlayHintAction>(action::BLOCKING).init("ID_RESCUEDOG_NO_VICTIM_01", HintMessage::MESSAGETYPE_TEXTHINT);	// We have no audio for this one
		}
		else
		{
			actionComponent.pushAction<PlayHintAction>(action::BLOCKING).init("EM2017_MINI04_HINT01", HintMessage::MESSAGETYPE_HINT);
		}

		// Send the dog back to the rescue dog leader
		actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(mRescueDogEntity.getSafe(), getEntity(), OnTouchTargetPointProvider::TARGET_POINT_ID));
		actionComponent.pushAction<CombineDogWithLeaderAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(getEntity());
	}

	glm::vec3 SendDogForSearchAction::getRandomPositionInRange(const glm::vec3& entityPosition, const qsf::Map& map, float searchRange)
	{
		// Using http://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
		// random a[0,1], random b[0,1] and a < b: (b*R*cos(2*pi*a/b), b*R*sin(2*pi*a/b))

		float randomValueA = qsf::Random::getRandomFloat(0, 1);
		float randomValueB = qsf::Random::getRandomFloat(0, 1);

		// A must be smaller than B
		if (randomValueA > randomValueB)
		{
			// Swap A and B
			float dummy = randomValueA;
			randomValueA = randomValueB;
			randomValueB = dummy;
		}

		float xPosition = entityPosition.x + (randomValueB * searchRange * cos(2 * glm::pi<float>()* randomValueA/randomValueB));
		float yPosition = entityPosition.y;
		float zPosition = entityPosition.z + (randomValueB * searchRange * sin(2 * glm::pi<float>()* randomValueA/randomValueB));
		glm::vec3 dummy;
		if (qsf::GroundMapQuery(map).getTerrainHeightAndNormalAtXZPosition(xPosition, zPosition, yPosition, dummy))
		{
			return glm::vec3(xPosition, yPosition, zPosition);
		}

		// Error
		return qsf::Math::GLM_VEC3_ZERO;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
