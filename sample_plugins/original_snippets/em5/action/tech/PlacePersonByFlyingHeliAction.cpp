// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/PlacePersonByFlyingHeliAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/tech/PlacePersonByFlyingHeliCommand.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/audio/AudioHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlacePersonByFlyingHeliAction::ACTION_ID = "em5::PlacePersonByFlyingHeliAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonByFlyingHeliAction::PlacePersonByFlyingHeliAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
	}

	PlacePersonByFlyingHeliAction::~PlacePersonByFlyingHeliAction()
	{
		// Nothing here
	}

	void PlacePersonByFlyingHeliAction::init(const glm::vec3& targetPosition)
	{
		mTargetPosition = targetPosition;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlacePersonByFlyingHeliAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mTargetPosition);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlacePersonByFlyingHeliAction::onStartup()
	{
		//if (!checkActionCondition(mTargetEntity.getSafe()))
		//	return false;

		// Check if entity is right and get the helicopter hatch component
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		qsf::Entity* specialEntity = getMap().getEntityById(helicopterComponent->getSpecialEntity());
		if (nullptr == specialEntity)
			return false;

		mHelicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
		if (!mHelicopterHatchComponent.valid())
			return false;

		if (!mHelicopterHatchComponent->isTransportingInjuredPerson())
			return false;

		// Check for collisions
		qsf::Entity* targetEntity = mHelicopterHatchComponent->getTargetFromStretcher();
		if (nullptr != targetEntity)
		{
			qsf::TransformComponent& transformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 groundPosition = transformComponent.getPosition();

			float height;
			if (qsf::GroundMapQuery(getMap()).getHeightAt(groundPosition, height))
				groundPosition.y = height;

			CollisionHelper collisionHelper(getMap());
			if (collisionHelper.hasCollisionEM3(*targetEntity, groundPosition, 1.0f))
				return false;
		}

		return true;
	}

	qsf::action::Result PlacePersonByFlyingHeliAction::updateAction(const qsf::Clock&)
	{
		// Check for components
		if (!mHelicopterHatchComponent.valid())
			return qsf::action::RESULT_DONE;

		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return qsf::action::RESULT_DONE;

		switch (mState)
		{
			case STATE_INIT:
			{
				mHelicopterHatchComponent->startLowerStretcherWithInjured(mTargetPosition);

				AudioHelper::playHelicopterRopeMove(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_LOWER_STRETCHER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOWER_STRETCHER:
			{
				if (mHelicopterHatchComponent->isLowerStretcher())
				{
					// Wait till the stretcher is on ground
					return qsf::action::RESULT_CONTINUE;
				}

				placePersonOnGround();

				getComponent().pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f));

				AudioHelper::playHelicopterRopeAttach(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_WAIT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT:
			{
				// Blocked for 1 second by WaitAction
				mHelicopterHatchComponent->raiseStretcher(false);

				AudioHelper::playHelicopterRopeMove(mAudioProxy, getEntity());
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_RAISE_STRETCHER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_RAISE_STRETCHER:
			{
				if (mHelicopterHatchComponent->isRaiseStretcher())
				{
					// Wait till the stretcher is on final position
					return qsf::action::RESULT_CONTINUE;
				}

				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_FINISH;
				// Fall through by design
			}

			case STATE_FINISH:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlacePersonByFlyingHeliAction::checkActionCondition(qsf::Entity& targetEntity)
	{
		//// Get the caller and target entity
		//qsf::Entity& callerEntity = getEntity();

		//// Build context
		//CommandContext commandContext;
		//commandContext.mCaller = &callerEntity;
		//commandContext.mTargetEntity = &targetEntity;

		//// Call "checkCaller" && "checkContext" of the command for validation
		//PlacePersonByFlyingHeliCommand* placePersonByFlyingHeliCommand = static_cast<PlacePersonByFlyingHeliCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlacePersonByFlyingHeliCommand>(PlacePersonByFlyingHeliCommand::PLUGINABLE_ID));
		//QSF_CHECK(nullptr != placePersonByFlyingHeliCommand, "PlacePersonByFlyingHeliAction::onStartup(): Could not found place person by flying helicopter command instance", return false);
		//if (!(placePersonByFlyingHeliCommand->checkCallerWithoutPriority(*commandContext.mCaller) && placePersonByFlyingHeliCommand->checkContext(commandContext)))
		//	return false;

		//// Make the distance check
		//float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
		//const float currentDistance = qsf::game::DistanceHelper::getDistance(targetEntity, callerEntity);

		// TODO(mk) Temporary disable the distance checks
		//if (currentDistance > (maxDistance))
		//	return false;

		// Every check is valid, return true
		return true;
	}

	void PlacePersonByFlyingHeliAction::placePersonOnGround()
	{
		qsf::Entity* targetEntity = mHelicopterHatchComponent->getTargetFromStretcher();
		if (nullptr == targetEntity)
			return;

		// Let the helicopter hatch component do the work
		mHelicopterHatchComponent->placeTargetOnGround(*targetEntity);

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PLACE_PERSON_FROM_FLYING_HELI_ACTION, targetEntity->getId(), getEntityId()));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
