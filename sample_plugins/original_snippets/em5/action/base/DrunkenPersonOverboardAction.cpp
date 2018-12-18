// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/DrunkenPersonOverboardAction.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DrunkenPersonOverboardAction::ACTION_ID = "em5::DrunkenPersonOverboardAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DrunkenPersonOverboardAction::DrunkenPersonOverboardAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mJumpVector(0.0f, qsf::Random::getRandomFloat(3.5f, 5.5f), -qsf::Random::getRandomFloat(1.5f, 2.5f)),
		mJumpWaitTime(qsf::Time::fromMilliseconds(500))
	{
		// Nothing here
	}

	DrunkenPersonOverboardAction::~DrunkenPersonOverboardAction()
	{
		// Nothing here
	}

	void DrunkenPersonOverboardAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
		mCurrentState = STATE_INIT;
	}

	uint64 DrunkenPersonOverboardAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DrunkenPersonOverboardAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DrunkenPersonOverboardAction::onStartup()
	{
		return true;
	}

	qsf::action::Result DrunkenPersonOverboardAction::updateAction(const qsf::Clock& clock)
	{
		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				AnimationHelper personAnimationHelper(getEntity());
				personAnimationHelper.playAnimation(personAnimationHelper.getAnimationJumpDown());

				mCurrentState = STATE_JUMP_FROM_SHIP_START;

				// Fall through by design
			}

			case STATE_JUMP_FROM_SHIP_START:
			{
				if (mJumpWaitTime > qsf::Time::ZERO)
				{
					mJumpWaitTime -= clock.getTimePassed();
					return qsf::action::RESULT_CONTINUE;
				}

				mCurrentState = STATE_JUMP_FROM_SHIP;

				// Fall through by design
			}

			case STATE_JUMP_FROM_SHIP:
			{
				if (handleJumpFromShip(clock.getTimePassed()))
				{
					mCurrentState = STATE_CREATE_DIVINGSPOT;
				}
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CREATE_DIVINGSPOT:
			{
				if (!linkOwnerToNewDivingSpot())
				{
					return qsf::action::RESULT_DONE;
				}

				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DrunkenPersonOverboardAction::handleJumpFromShip(const qsf::Time& timePassed)
	{
		qsf::Entity& personEntity = getEntity();

		qsf::MovableComponent& movableComponent = personEntity.getComponentSafe<qsf::MovableComponent>();
		const qsf::PhysicsWorldComponent& physicsWorldComponent = personEntity.getMap().getCoreEntity().getComponentSafe<qsf::PhysicsWorldComponent>();

		const float secondsPassed = timePassed.getSeconds();

		mJumpVector = mJumpVector + (physicsWorldComponent.getGravity() * secondsPassed);

		glm::vec3 newPosition = movableComponent.getPosition() + ((movableComponent.getRotation() * mJumpVector) * secondsPassed);

		float currentMapHeight = 0.0f;
		qsf::GroundMapQuery(personEntity.getMap()).getHeightAtXZPosition(newPosition.x, newPosition.z, currentMapHeight);

		// Check if the person hits the ground
		if (newPosition.y < currentMapHeight)
		{
			newPosition.y = currentMapHeight;
			return true;
		}

		movableComponent.moveToPosition(newPosition);
		return false;
	}

	bool DrunkenPersonOverboardAction::linkOwnerToNewDivingSpot() const
	{
		qsf::Entity& ownerEntity = getEntity();
		qsf::Entity* divingSpotEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(equipment::PREFAB_FIRE_DIVINGSPOT);
		if (nullptr == divingSpotEntity)
			return false;

		// Get diving spot component
		DivingSpotComponent* divingSpotComponent = divingSpotEntity->getComponent<DivingSpotComponent>();
		if (nullptr == divingSpotComponent)
			return false;

		// Link target to diving spot
		divingSpotComponent->setAlternativeTargetId(getEntityId());

		// Positioning of the diving spot
		qsf::TransformComponent& transformComponent = ownerEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& divingspotTransformComponent = divingSpotEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		divingspotTransformComponent.setPosition(transformComponent.getPosition());

		// Send message for new created diving spot
		qsf::MessageParameters parameters;
		parameters.setParameter("DivingSpotId", divingSpotEntity->getId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CREATED_DIVINGSPOT, divingSpotEntity->getId()), parameters);

		// Hide the person
		qsf::game::HiddenComponent::setHidden(ownerEntity, true);

		// Recreate the person placement component
		ownerEntity.getOrCreateComponent<PersonPlacementComponent>();

		// Done
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
