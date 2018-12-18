// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PutPersonInBoatAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/fire/PutPersonInBoatCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthHelper.h"

#include <qsf_game/QsfGameHelper.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/BoneLinkComponent.h>
#include <qsf_game/command/CommandSystem.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PutPersonInBoatAction::ACTION_ID = "em5::PutPersonInBoatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PutPersonInBoatAction::PutPersonInBoatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PutPersonInBoatAction::~PutPersonInBoatAction()
	{
		// Nothing here
	}

	void PutPersonInBoatAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 PutPersonInBoatAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PutPersonInBoatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PutPersonInBoatAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" and "checkContext" of the commando for validation
		PutPersonInBoatCommand* putPersonInBoatCommand = static_cast<PutPersonInBoatCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PutPersonInBoatCommand>(PutPersonInBoatCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != putPersonInBoatCommand, "PutPersonInBoatCommand::onStartup(): Could not find putPersonInBoatCommand instance", return false);
		if (!(putPersonInBoatCommand->checkCaller(*commandContext.mCaller) && putPersonInBoatCommand->checkContext(commandContext)))
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPlacePersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::get2dDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result PutPersonInBoatAction::updateAction(const qsf::Clock&)
	{
		// Get the caller entity instance
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 place person action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Get the game logic component and the boat
		BoatComponent* boatComponent = targetEntity->getComponent<BoatComponent>();
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == boatComponent || nullptr == gameLogicComponent)
			return qsf::action::RESULT_DONE;

		// Get the diver logic
		DiverLogic* diverLogic = gameLogicComponent->getGameLogic<DiverLogic>();
		if (nullptr == diverLogic)
			return qsf::action::RESULT_DONE;

		// Get the carried person
		qsf::Entity* carriedPerson = getMap().getEntityById(diverLogic->getCarryPerson());
		if (nullptr == carriedPerson)
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mState)
		{
			case STATE_INIT:
			{
				// Show animations for the involved persons
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationPutDrowningInBoat(), true);
				qsf::ActionComponent* actionComponentCarried = carriedPerson->getComponent<qsf::ActionComponent>();
				if (actionComponentCarried != nullptr)
				{
					actionComponentCarried->pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(*carriedPerson).getAnimationDrowningGetsPutInBoat(), true);
				}

				// Immediately go to the next state
				mState = STATE_PLACE_PERSON;

				// Give it 1 frame to start the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLACE_PERSON:
			{
				// Destroy old link
				carriedPerson->destroyComponent<qsf::game::BoneLinkComponent>();

				// Create new link component and set links
				qsf::LinkComponent* linkComponent = carriedPerson->getOrCreateComponent<qsf::LinkComponent>();

				// Register to the new vehicle
				linkComponent->setParentId(targetEntity->getId());

				// Add the person to the boat
				boatComponent->addToContainer(*carriedPerson, container::CONTAINERTYPE_PASSENGER);

				// Get the transform component of the carried person
				qsf::TransformComponent* transformComponentCarriedPerson = carriedPerson->getComponent<qsf::TransformComponent>();
				if (nullptr == transformComponentCarriedPerson)
					return qsf::action::RESULT_DONE;

				// Get the transform component of the vehicle
				const qsf::TransformComponent* transformComponentVehicle = targetEntity->getComponent<qsf::TransformComponent>();
				if (nullptr == transformComponentVehicle)
					return qsf::action::RESULT_DONE;

				// Hide the carried person
				qsf::ActionComponent * actionComponentCarried = carriedPerson->getComponent<qsf::ActionComponent>();
				if (actionComponentCarried != nullptr)
				{
					actionComponentCarried->pushAction<DisappearAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f), false);
				}

				mState = STATE_DISAPPEAR;

				// Immediately go to the next state; no break by intent
			}

			case STATE_DISAPPEAR:
			{
				// Wait until the carried person no longer is disappearing
				qsf::ActionComponent * actionComponentCarried = carriedPerson->getComponent<qsf::ActionComponent>();
				if (actionComponentCarried != nullptr && actionComponentCarried->getActionByTypeId(DisappearAction::ACTION_ID) != nullptr)
				{
					return qsf::action::RESULT_CONTINUE;
				}
				mState = STATE_DONE;

				// Immediately go to the next state; no break by intent
			}

			case STATE_DONE:
			{
				// Emit message "Person placed in ambulance"
				qsf::MessageParameters parameters;
				parameters.setParameter("boatId",  mTargetEntityId);
				parameters.setParameter("diverId", callerEntity.getId());
				parameters.setParameter("targetId",  carriedPerson->getId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PUT_PERSON_IN_BOAT_ACTION, mTargetEntityId, callerEntity.getId(), carriedPerson->getId()), parameters);

				// Delete the carried person from the logic
				//  -> This should always be done in the final state, because after that the action would return RESULT_DONE in the prechecks already
				diverLogic->setCarryPerson(qsf::getUninitialized<uint64>());

				// Diver no longer invincible (but still a hero!)
				HealthHelper(getEntity()).setInvincible(false);

				// Immediately go to the next state; no break by intent
			}
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
