// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/PlacePersonWithDiverAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/fire/PlacePersonWithDiverCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/animation/PersonAnimationController.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/QsfGameHelper.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>

#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Map.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlacePersonWithDiverAction::ACTION_ID = "em5::PlacePersonWithDiverAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonWithDiverAction::PlacePersonWithDiverAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PlacePersonWithDiverAction::~PlacePersonWithDiverAction()
	{
		// Nothing here
	}

	void PlacePersonWithDiverAction::init(const glm::vec3& targetPos)
	{
		mTargetPos = targetPos;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlacePersonWithDiverAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetPos);
		serializer.serializeAs<uint16>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlacePersonWithDiverAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetPosition = mTargetPos;

		// Call "checkCaller" and "checkContext" of the commando for validation
		PlacePersonWithDiverCommand* placePersonWithDiverCommand = static_cast<PlacePersonWithDiverCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlacePersonWithDiverCommand>(PlacePersonWithDiverCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != placePersonWithDiverCommand, "PlacePersonWithDiverAction::onStartup(): Could not find placePersonWithDiverCommand instance", return false);
		if (!placePersonWithDiverCommand->checkContext(commandContext))	// do _not_ check the caller again here (he's blocked by this action itself)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPlacePersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::get2dDistance(commandContext.mTargetPosition.get(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result PlacePersonWithDiverAction::updateAction(const qsf::Clock&)
	{
		// Get the caller entity instance
		qsf::Entity& callerEntity = getEntity();

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
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
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationPutDrowningOnGround(), true);

				qsf::ActionComponent& actionComponentCarried = carriedPerson->getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponentCarried.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(*carriedPerson).getAnimationDrowningGroundPlace(), true, true);

				// The animation for getting pulled has a offset from the caller, so we place them at the same position to avoid "jumping" postions for the carried person.
				EntityHelper(*carriedPerson).setPosition(EntityHelper(callerEntity).getPosition());

				mState = STATE_DONE;

				// Give it 1 frame to start the animation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				{ // Destroy the linkage
					qsf::LinkComponent* linkComponent = carriedPerson->getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						linkComponent->setParentId(qsf::getUninitialized<uint64>());
					}
				}

				{ // Enable movable component of child
					qsf::MovableComponent* movableComponent = carriedPerson->getComponent<qsf::MovableComponent>();
					if (nullptr != movableComponent)
					{
						movableComponent->setActive(true);
					}
				}

				// Update the bounding box (drowning and injured on ground have different bounding boxes)
				PersonComponent* personComponent = carriedPerson->getComponent<PersonComponent>();
				if (nullptr != personComponent)
				{
					personComponent->setBoundingBoxType(PersonComponent::BOX_LYING);
				}

				// Follow up injury for drowning
				HealthComponent* healthComponent = carriedPerson->getComponent<HealthComponent>();
				if (healthComponent != nullptr)
				{
					healthComponent->setDrowningFollowUpInjury();
				}

				{
					// Reposition he carried person to the offset of the diver. Now the person has a nice offset for starting the injured animation
					diverLogic->giveCarryOffsetToCarriedPerson();

					// Delete the carried person from the logic
					// -> This should always be done in the final state, because after that the action would return RESULT_DONE in the prechecks already
					diverLogic->setCarryPerson(qsf::getUninitialized<uint64>());

					// Force new position (TODO(mz): placement must care about floor slope)
					EntityHelper(*carriedPerson).applyPlacement(true);

					// Start animation directly to avoid animation interpolation
					std::string injuryAnimation = personComponent->getPersonAnimationController().getInjuredAnimation(*carriedPerson);
					AnimationHelper carriedAnimationHelper(*carriedPerson);
					if (nullptr != healthComponent && healthComponent->isDead())
					{
						injuryAnimation = carriedAnimationHelper.getAnimationGenericLightLying();
					}

					carriedAnimationHelper.playAnimation(injuryAnimation, false);
				}

				HealthHelper(getEntity()).setInvincible(false);

				// Create a collision for the target, we removed it manually before
				qsf::ai::RouterComponent* routerComponent = carriedPerson->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->createCollisionBox();

				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PLACE_PERSON_FROM_DIVER_ACTION, getEntityId(), carriedPerson->getId()));

				// Immediately go to the next state; no break by intent
			}
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
