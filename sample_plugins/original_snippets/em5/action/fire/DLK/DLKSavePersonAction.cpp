// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DLK/DLKSavePersonAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/firefighters/DLKLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DLKSavePersonAction::ACTION_ID = "em5::DLKSavePersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DLKSavePersonAction::DLKSavePersonAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mSecondsPassed(0.0f),
		mGameLogicComponent(nullptr),
		mInjurePersonAfterRescue(false),
		mActivateFireReceiverAfterRescue(false)
	{
		// Nothing here
	}

	DLKSavePersonAction::~DLKSavePersonAction()
	{
		// Nothing here
	}

	void DLKSavePersonAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
		updateGameLogicComponent();
	}

	uint64 DLKSavePersonAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DLKSavePersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DLKSavePersonAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		updateGameLogicComponent();

		// Check if is a target is to low
		qsf::TransformComponent& dlkTransformComponent = callerEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		const float personHeight = getTargetPosition(*targetEntity).y;
		const float dlkHeight = dlkTransformComponent.getPosition().y;
		if (personHeight < dlkHeight)
			return false;

		// Check distance
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, getEntity());
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getDlkDistance();
		if (currentDistance >= maxDistance)
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, getEntity()))
		{
			return false;
		}

		mDlkLadderComponent = getEntity().getComponent<DlkLadderComponent>();

		return true;
	}

	qsf::action::Result DLKSavePersonAction::updateAction(const qsf::Clock& clock)
	{
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);

		if (nullptr != supportLegsComponent && nullptr != targetEntity && mDlkLadderComponent.valid() && nullptr != mGameLogicComponent)
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					// Check for the reserve logic and if it isn't reserved we reserve it for us
					if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
						return qsf::action::RESULT_DONE;

					ReserveLogic::createReservation<ReserveDLKRescueLogic>(*targetEntity, getEntityId());

					// Extend the support legs
					supportLegsComponent->startExtending();
					mCurrentState = STATE_EXTENDING_LEGS;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_EXTENDING_LEGS:
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for leg animation to end
						return qsf::action::RESULT_CONTINUE;
					}

					mDlkLadderComponent->alignLadder(getTargetPosition(*targetEntity));
					mCurrentState = STATE_LADDER_ALIGNMENT;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_LADDER_ALIGNMENT:
				{
					if (mDlkLadderComponent->isAnimationPlaying())
					{
						// Wait until the DLK ladder arm is aligned to the target
						return qsf::action::RESULT_CONTINUE;
					}

					fadeOutPerson();
					mSecondsPassed = 0.0f;
					mCurrentState = STATE_WAIT_1;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_WAIT_1:
				{
					// Wait 1 second
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed <= 1.0f)
					{
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_RESCUE_FROM_BUILDING;

					// Fall through by design
				}

				case STATE_RESCUE_FROM_BUILDING:
				{
					// Rescue the person and place him/her on the ladder stage
					rescuePersonFromBuilding();
					mSecondsPassed = 0.0f;
					mCurrentState = STATE_WAIT_2;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_WAIT_2:
				{
					// Wait 1 second
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed <= 1.0f)
					{
						return qsf::action::RESULT_CONTINUE;
					}

					mDlkLadderComponent->foldLadder();
					mCurrentState = STATE_FOLD_LADDER;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_FOLD_LADDER:
				{
					mSecondsPassed = 0.0f;
					if (mDlkLadderComponent->isAnimationPlaying())
					{
						// Fold back the DLK ladder arm and transport the person down
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_WAIT_3;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_WAIT_3:
				{
					// Wait 1 second
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed < 1.0f)
					{
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_RESCUE_FROM_DLK;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_RESCUE_FROM_DLK:
				{
					// Person will exit from the ladder stage
					rescuePersonFromDlk();

					// Delete Reservation after releasing the person
					ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());

					// Hide the support legs
					supportLegsComponent->hideExtending();
					mCurrentState = STATE_HIDE_LEGS;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_HIDE_LEGS:
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for leg animation to end
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					// Fall through by design
				}
			}
		}

		if (nullptr != targetEntity)
		{
			// Delete Reservation for sure if anything goes wrong
			ReserveLogic::deleteOwnReservation(*targetEntity, getEntityId());
		}
		return qsf::action::RESULT_DONE;
	}

	void DLKSavePersonAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DLKSavePersonAction::updateGameLogicComponent()
	{
		mGameLogicComponent = getEntity().getOrCreateComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != mGameLogicComponent, "Could not update the game logic component of the entity: " << getEntityId(), QSF_REACT_THROW);
	}

	void DLKSavePersonAction::fadeOutPerson()
	{
		qsf::Entity* personEntity = nullptr;
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			CallForRescueComponent* callForRescueComponent = targetEntity->getComponent<CallForRescueComponent>();
			if (nullptr != callForRescueComponent && callForRescueComponent->getCanRescuedByDLK())
			{
				personEntity = targetEntity;
			}
			else
			{
				// Find a contained target of a building
				BuildingComponent* buildingComponent = targetEntity->getComponent<BuildingComponent>();
				if (nullptr != buildingComponent)
				{
					personEntity = buildingComponent->getPersonWavingOutOfWindow();
				}
			}
		}
		if (nullptr != personEntity)
		{
			qsf::game::HiddenComponent::setHidden(*personEntity, false);

			personEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f));
		}
	}

	void DLKSavePersonAction::rescuePersonFromBuilding()
	{
		qsf::Entity* personEntity = nullptr;
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr != targetEntity)
		{
			CallForRescueComponent* callForRescueComponent = targetEntity->getComponent<CallForRescueComponent>();
			if (nullptr != callForRescueComponent && callForRescueComponent->getCanRescuedByDLK())
			{
				mInjurePersonAfterRescue = false;
				personEntity = targetEntity;
			}
			else
			{
				mInjurePersonAfterRescue = true;

				// Find a contained target of a building with a health component
				BuildingComponent* buildingComponent = targetEntity->getComponent<BuildingComponent>();
				if (nullptr != buildingComponent)
				{
					personEntity = buildingComponent->getPersonWavingOutOfWindow();

					if (nullptr != personEntity)
					{
						buildingComponent->removeFromContainer(*personEntity);	// Destroy the linkage between building and person
						buildingComponent->stopPersonWaveOutOfWindow();

						// We want to see the person
						// TODO(sw) Don't interact directly with the render component, this makes problems in multiplay
						qsf::game::HiddenComponent::setHidden(*personEntity, false);
					}
				}
			}
		}
		if (nullptr != personEntity)
		{
			personEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::fromSeconds(1.0f));

			// Delete the call for rescue component
			personEntity->destroyComponent<CallForRescueComponent>();

			// Hide person trapped icon
			StatusOverlayComponent::showIcon(*personEntity, StatusOverlayComponent::STATUS_ICON_CONDITION_RESCUE_DLK, false);

			// Make person temporarily immune to fire
			FireReceiverComponent& fireReceiverComponent = personEntity->getOrCreateComponentSafe<FireReceiverComponent>();
			mActivateFireReceiverAfterRescue = fireReceiverComponent.isActive();
			fireReceiverComponent.setActive(false);

			// Put target on DLK stage
			putEntityOnDlk(personEntity);

			// Get an animation
			qsf::MeshAnimationComponent* meshAnimation = personEntity->getOrCreateComponent<qsf::MeshAnimationComponent>();
			QSF_CHECK(nullptr != meshAnimation, "Couldn't create or get meshAnimationComponent in personEntity", return);

			const qsf::AssetProxy animationAsset(AnimationHelper(*personEntity).getAnimationBreatheDeeply());
			meshAnimation->playAnimation(animationAsset, true);
		}
	}

	void DLKSavePersonAction::rescuePersonFromDlk()
	{
		qsf::Entity* stageEntity = mDlkLadderComponent->getStageEntity();
		QSF_CHECK(nullptr != stageEntity, "DLKSavePersonAction: No stageEntity found", return);

		qsf::LinkComponent* stageLinkComponent = stageEntity->getComponent<qsf::LinkComponent>();
		QSF_CHECK(nullptr != stageLinkComponent, "No link component in ladderEntity found", return);

		qsf::Entity* personEntity = nullptr;

		// Find the target with a person component
		for (const qsf::LinkComponent* childLink : stageLinkComponent->getChildLinks())
		{
			PersonComponent* personComponent = childLink->getEntity().getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personEntity = &childLink->getEntity();
				break;
			}
		}

		if (nullptr != personEntity)
		{
			// Delete link
			qsf::LinkComponent* personLinkComponent = personEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != personLinkComponent)
			{
				personLinkComponent->setParentId(qsf::getUninitialized<uint64>());
			}

			// Get person the correct placement type again
			personEntity->getOrCreateComponent<PersonPlacementComponent>();

			// Set target to caller position
			const qsf::TransformComponent& callerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& targetTransformComponent = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

			qsf::Transform targetTransform = targetTransformComponent.getTransform();

			targetTransform.setPositionAndRotation(callerTransformComponent.getPosition() + callerTransformComponent.getRotation() * glm::vec3(2, 0, -2), callerTransformComponent.getRotation());

			// Search for a free position first with 3 meter radius and if we find nothing with 6 and 12
			glm::vec3 freePosition;
			for (int i = 0; i < 3; ++i)
			{
				const float radius = (i == 0 ? 3.0f : (i == 1 ? 6.0f : 12.0f));
				if (CollisionHelper(getMap()).findFreePositionEM3(*personEntity, targetTransform, radius, 0.1f, freePosition))
				{
					break;
				}
			}

			qsf::MovableComponent* movableComponent = personEntity->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				movableComponent->warpToPositionAndRotation(freePosition, targetTransform.getRotation());
			}
			else
			{
				targetTransformComponent.setPositionAndRotation(freePosition, targetTransform.getRotation());
			}

			if (mInjurePersonAfterRescue)
			{
				// Get freeplay event from target building
				FreeplayEvent* freeplayEvent = nullptr;
				EventIdComponent* eventIdComponent = personEntity->getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent)
				{
					freeplayEvent = eventIdComponent->getFreeplayEvent();
				}

				HealthComponent* healthComponent = personEntity->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					// Injure the person
					if (nullptr != personEntity->getComponentById("em5::InjuryAtStartupComponent"))
						HealthHelper(*healthComponent).injurePersonByInjuryAtStartup(freeplayEvent, false);
					else
						HealthHelper(*healthComponent).injurePersonBySmoke(freeplayEvent, false);
				}
			}
			else if (mActivateFireReceiverAfterRescue)
			{
				personEntity->getOrCreateComponentSafe<FireReceiverComponent>().setActive(true);
			}

			// Register the player as "has interacted with" event object
			EventIdComponent::registerPlayer(*personEntity, getEntity());

			// Emit message that the person is rescued
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_TOOKOUTOFWINDOW_PERSON, personEntity->getId()));
		}
	}

	void DLKSavePersonAction::putEntityOnDlk(qsf::Entity* entity)
	{
		qsf::Entity* stageEntity = mDlkLadderComponent->getStageEntity();
		QSF_CHECK(nullptr != stageEntity, "DLKSavePersonAction: No stageEntity found", return);

		qsf::TransformComponent& stageTransformComponent = stageEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& entityTransformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();

		entityTransformComponent.setPositionAndRotation(stageTransformComponent.getPosition(), stageTransformComponent.getRotation());

		{
			qsf::LinkComponent& entityLinkComponent = entity->getOrCreateComponentSafe<qsf::LinkComponent>();
			entityLinkComponent.setParentId(stageEntity->getId());
			entityLinkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

			// Get a nice position on the stage
			entityLinkComponent.setLocalPosition(entityLinkComponent.getLocalPosition() + glm::vec3(0.0f, 0.1f, -0.5f));
		}

		// Emit message
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_LOAD_PERSON_ON_DLK, entity->getId()));
	}

	glm::vec3 DLKSavePersonAction::getTargetPosition(qsf::Entity& targetEntity)
	{
		glm::vec3 result = targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		// Check if the target isn't the person we want to safe, so we need to find the real target position
		BuildingComponent* buildingComponent = DLKLogic::getPlaceOfActionBuilding(targetEntity);
		if (nullptr != buildingComponent)
		{
			qsf::Entity* personEntity = buildingComponent->getPersonWavingOutOfWindow();
			if (nullptr != personEntity)
			{
				const qsf::TransformComponent& personTransformComponent = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				result = personTransformComponent.getPosition();
			}
		}
		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
