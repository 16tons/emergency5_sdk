// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/PickupDrowningPersonByBoatAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/tech/PickupDrowningPersonByBoatCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupDrowningPersonByBoatAction::ACTION_ID = "em5::PickupDrowningPersonByBoatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupDrowningPersonByBoatAction::PickupDrowningPersonByBoatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mSecondsPassed(qsf::Time::ZERO)
	{
		// Nothing here
	}

	PickupDrowningPersonByBoatAction::~PickupDrowningPersonByBoatAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void PickupDrowningPersonByBoatAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 PickupDrowningPersonByBoatAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupDrowningPersonByBoatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupDrowningPersonByBoatAction::onStartup()
	{
		// Get the target entity
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		if (!checkActionCondition(*targetEntity))
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result PickupDrowningPersonByBoatAction::updateAction(const qsf::Clock& clock)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;	// Target entity is not existing, error

		// TODO(co) Is it save to remove this?
		targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

		switch (mState)
		{
			case STATE_INIT:
			{
				if (!checkActionCondition(*targetEntity))
				{
					return qsf::action::RESULT_DONE;
				}

				// Check for the reserve logic
				if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
				{
					// Wait till the reservation is unlocked
					return qsf::action::RESULT_CONTINUE;
				}

				// Reserve the entity
				ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*targetEntity, getEntityId());

				// Target is out of danger now
				targetEntity->destroyComponent<CallForRescueComponent>();

				targetEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut();

				mSecondsPassed = qsf::Time::ZERO;

				mState = STATE_PLAY_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAY_ANIMATION:
			{
				mSecondsPassed += clock.getTimePassed();
				if (mSecondsPassed.getSeconds() >= 1.0f)
				{
					// Link person to boat
					linkTargetEntity(*targetEntity);

					mState = STATE_FINISH;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINISH:
			{
				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void PickupDrowningPersonByBoatAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PickupDrowningPersonByBoatAction::linkTargetEntity(qsf::Entity& targetEntity)
	{
		// Make person invincible
		targetEntity.getOrCreateComponentSafe<HealthComponent>().setInvincible(true);

		// Enter the vehicle now
		EntityHelper(targetEntity).enterContainer(getEntity(), container::CONTAINERTYPE_PASSENGER);
	}

	bool PickupDrowningPersonByBoatAction::checkActionCondition(qsf::Entity& targetEntity)
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = &targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		PickupDrowningPersonByBoatCommand* pickupDrowningPersonWithBoatCommand = static_cast<PickupDrowningPersonByBoatCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PickupDrowningPersonByBoatCommand>(PickupDrowningPersonByBoatCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != pickupDrowningPersonWithBoatCommand, "PickupDrowningPersonWithBoatCommand::checkActionCondition(): Could not pickup person by boat command instance", return false);
		if (!(pickupDrowningPersonWithBoatCommand->checkCallerWithoutPriority(*commandContext.mCaller) && pickupDrowningPersonWithBoatCommand->checkContext(commandContext)))
			return false;

		// Make the distance check
		float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(targetEntity, callerEntity);

		if (currentDistance > (maxDistance*2))
			return false;

		// Every check is valid, return true
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
