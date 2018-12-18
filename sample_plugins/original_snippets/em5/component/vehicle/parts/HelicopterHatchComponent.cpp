// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/BoneToBoneLinkComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HelicopterHatchComponent::COMPONENT_ID = qsf::StringHash("em5::HelicopterHatchComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterHatchComponent::HelicopterHatchComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mHelicopterHatchState(CLOSED),
		mTargetIsInjured(true),
		mLowerHatchSpeed(2.0f),
		mRaiseHatchSpeed(1.0f),
		mInjuredPersonFlyHigh(2.0f)
	{
		// Nothing to do in here
	}

	HelicopterHatchComponent::~HelicopterHatchComponent()
	{
		// Nothing to do in here
	}

	void HelicopterHatchComponent::openHatch(qsf::Entity& targetEntity, bool targetIsInjured)
	{
		if (mHelicopterHatchState != CLOSED)
			return;

		mHelicopterHatchState = START_OPENING;
		mTargetEntity = targetEntity;
		mTargetIsInjured = targetIsInjured;
		mTargetPosition = EntityHelper(targetEntity).getPosition();

		// Activate job so we get regular update calls
		activateJob();
	}

	void HelicopterHatchComponent::startLowerStretcherWithInjured(const glm::vec3& targetPosition)
	{
		if (mHelicopterHatchState != TRANSPORT_INJURED)
			return;

		mHelicopterHatchState = LOWER_STRETCHER;
		mTargetEntity = nullptr;
		mTargetPosition = targetPosition;

		// Activate job so we get regular update calls
		activateJob();
	}

	void HelicopterHatchComponent::raiseStretcher(bool targetIsInjured)
	{
		mHelicopterHatchState = HIGHER_STRETCHER;
		mTargetIsInjured = targetIsInjured;

		// Activate job so we get regular update calls
		activateJob();
	}

	bool HelicopterHatchComponent::isLowerStretcher() const
	{
		return (mHelicopterHatchState == LOWER_STRETCHER || mHelicopterHatchState == OPENING_HATCH || mHelicopterHatchState == START_OPENING);
	}

	bool HelicopterHatchComponent::isRaiseStretcher() const
	{
		return (mHelicopterHatchState == HIGHER_STRETCHER || mHelicopterHatchState == CLOSEING_HATCH);
	}

	bool HelicopterHatchComponent::isTransportingInjuredPerson() const
	{
		return (mHelicopterHatchState == TRANSPORT_INJURED);
	}

	qsf::Entity* HelicopterHatchComponent::getStretcherEntity() const
	{
		return mStretcherEntity.valid() ? mStretcherEntity.get() : nullptr;
	}

	qsf::Entity* HelicopterHatchComponent::getTargetFromStretcher()
	{
		if (mStretcherEntity.valid())
		{
			const qsf::LinkComponent& linkComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			for (const qsf::LinkComponent* currentLinkComponent : linkComponent.getChildLinks())
			{
				qsf::Entity& entity = currentLinkComponent->getEntity();
				if (EntityHelper(entity).isEntityPerson())
				{
					return &entity;
				}
			}
		}
		// Error
		return nullptr;
	}

	float HelicopterHatchComponent::getLowerStretcherSpeed() const
	{
		return mLowerHatchSpeed;
	}

	void HelicopterHatchComponent::setLowerStretcherSpeed(float meterPerSecond)
	{
		mLowerHatchSpeed = meterPerSecond;
	}

	float HelicopterHatchComponent::getRaiseStretcherSpeed() const
	{
		return mRaiseHatchSpeed;
	}

	void HelicopterHatchComponent::setRaiseStretcherSpeed(float meterPerSecond)
	{
		mRaiseHatchSpeed = meterPerSecond;
	}

	float HelicopterHatchComponent::getInjuredPersonFlyHigh() const
	{
		return mInjuredPersonFlyHigh;
	}

	void HelicopterHatchComponent::setInjuredPersonFlyHigh(float injuredPersonFlyHigh)
	{
		mInjuredPersonFlyHigh = injuredPersonFlyHigh;
	}

	void HelicopterHatchComponent::placeTargetOnStretcher()
	{
		// TODO(fw): How come the stretcher entity does not exist?
		if (!mStretcherEntity.valid())
			return;

		qsf::Entity& targetEntity = mTargetEntity.getSafe();

		qsf::LinkComponent& linkComponent = targetEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
		linkComponent.setParentId(mStretcherEntity->getId());
		linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);

		// Add bone link
		qsf::game::BoneToBoneLinkComponent& boneToBoneLinkComponent = targetEntity.getOrCreateComponentSafe<qsf::game::BoneToBoneLinkComponent>();
		boneToBoneLinkComponent.setLocalRotation1(qsf::EulerAngles::eulerToQuaternion(glm::radians(90.f), 0, 0));
		boneToBoneLinkComponent.setLocalPosition1(glm::vec3(0, 0.1f, 0));
		boneToBoneLinkComponent.setBoneName(equipment::EQUIPMENT_BONE_TECH_STRETCHER);
		boneToBoneLinkComponent.setParentBoneName1(equipment::EQUIPMENT_BONE_TECH_STRETCHER);
		boneToBoneLinkComponent.setChildBoneName1(equipment::STANDARD_BONE_PELVIS);

		// Set movable component inactive to have an clean object movement when we raise the stretcher (the movable component handles movement for itself, looks like small "hangers" in movement)
		qsf::MovableComponent* movableComponent = targetEntity.getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
			movableComponent->setActive(false);

		// Start animation
		qsf::ActionComponent& actionComponent = targetEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
		std::string animationname;
		if (mTargetIsInjured)
		{
			animationname = AnimationHelper(targetEntity).getAnimationVictimOnStretcher();
		}
		else
		{
			animationname = AnimationHelper(targetEntity).getAnimationRescueWithHeli02();
		}
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationname, false, true);

		HealthHelper(targetEntity).setInvincible(true);

		// Disable patient collision
		EntityHelper(targetEntity).setCollisionEnabled(false);

		// Don't mark entity as rescue anymore
		targetEntity.destroyComponent<CallForRescueComponent>();
	}

	void HelicopterHatchComponent::placeTargetOnGround(qsf::Entity& targetEntity)
	{
		qsf::TransformComponent& transformComponent = targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::Transform currentTransform = transformComponent.getTransform();

		// Destroy links
		targetEntity.destroyComponent<qsf::game::BoneLinkComponent>();
		targetEntity.destroyComponent<qsf::game::BoneToBoneLinkComponent>();
		targetEntity.destroyComponent<qsf::LinkComponent>();

		transformComponent.setTransform(currentTransform);

		// Create placement component if it doesn't already exist
		targetEntity.getOrCreateComponentById(qsf::StringHash("em5::PersonPlacementComponent"));

		// Apply placement to avoid hovering in air
		EntityHelper(targetEntity).applyPlacement();
		HealthHelper(targetEntity).setInvincible(false);
		HealthComponent* healthComponent = targetEntity.getComponent<HealthComponent>();
		if (healthComponent != nullptr)
		{
			healthComponent->setInvincible(false);
			healthComponent->updateInjuredIcon();
		}

		// Enable patient collision again
		EntityHelper(targetEntity).setCollisionEnabled(true);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void HelicopterHatchComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mHelicopterHatchState);
		serializer.serialize(mTargetIsInjured);
		serializer.serialize(mLowerHatchSpeed);
		serializer.serialize(mRaiseHatchSpeed);
		serializer.serialize(mInjuredPersonFlyHigh);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void HelicopterHatchComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&HelicopterHatchComponent::updateJob, this, _1));
		}
	}

	void HelicopterHatchComponent::deactivateJob()
	{
		mUpdateJobProxy.unregister();
	}

	void HelicopterHatchComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		switch (mHelicopterHatchState)
		{
			case START_OPENING:
			{
				// Play open hatch animation
				qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
				actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationHelicopterHatchOpen(), true);

				mHelicopterHatchState = OPENING_HATCH;
				return;
			}
			case OPENING_HATCH:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait till animation is played
					return;
				}

				createStretcher();

				mHelicopterHatchState = LOWER_STRETCHER;
				return;
			}

			case LOWER_STRETCHER:
			{
				if (updateLowerStretcher(jobArguments.getSecondsPassed()))
				{
					// Lower the stretcher till the final position is reached
					return;
				}

				// Everything is finished, stop job update
				deactivateJob();

				// Pickup person is handed by an action, deactivate job here
				mHelicopterHatchState = PICKUP_PERSON;
				return;
			}
			case HIGHER_STRETCHER:
			{
				if (updateHigherStretcher(jobArguments.getSecondsPassed()))
				{
					// High the stretcher till the final position is reached
					return;
				}

				if (!mTargetIsInjured)
				{
					// Handle person inside helicopter
					closeHatch();
					destroyStretcher();

					// Start different control flow
					mHelicopterHatchState = CLOSEING_HATCH;
					return;
				}

				// Everything is finished, stop job update
				deactivateJob();

				mHelicopterHatchState = TRANSPORT_INJURED;
				return;
			}
			case CLOSEING_HATCH:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait till animation is played
					return;
				}

				// Everything is finished, stop job update
				deactivateJob();

				mHelicopterHatchState = CLOSED;
				return;
			}
			case CLOSED:
			case PICKUP_PERSON:
			case TRANSPORT_INJURED:
				// Nothing to do in this case
				break;
		}
	}

	void HelicopterHatchComponent::createStretcher()
	{
		// Lazy evaluation, create the stretcher in the first time we need it
		if (mStretcherEntity.valid())
		{
			return;
		}

		std::string stretcherName = equipment::TECH_STRETCHER;
		if (!mTargetIsInjured)
		{
			// If the hatch has to be close we need a stretcher, able to carry non injured persons
			stretcherName = equipment::TECH_HELICOPTER_ROBE;
		}

		mStretcherEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/" + stretcherName));
		QSF_CHECK(mStretcherEntity.valid(), "HelicopterHatchComponent::createStretcher(): Can't create recoveryHelicopter stretcher. Check if prefab is existing.", return);

		qsf::Entity* boneDummyEntity = getEntity().getMap().createObjectByLocalPrefabAssetId(equipment::PREFAB_DUMMY_BONE);
		QSF_CHECK(boneDummyEntity, "HelicopterHatchComponent::createStretcher(): Can't create dummy bone entity. Check if prefab is existing.", return);

		// Set stretcher on the position of the hatch
		qsf::TransformComponent& entityTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		// Set dummy bone position
		qsf::TransformComponent& boneDummyEntityTransformComponent = boneDummyEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		boneDummyEntityTransformComponent.setTransform(entityTransformComponent.getTransform());

		// Link the dummy bone to the helicopter
		qsf::LinkComponent& boneDummyLinkComponent = boneDummyEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
		boneDummyLinkComponent.setParentId(getEntityId());

		// Set stretcher position
		qsf::TransformComponent& stretcherTransformComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		stretcherTransformComponent.setTransform(entityTransformComponent.getTransform());

		// Link the stretcher entity
		qsf::LinkComponent& stretcherLinkComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
		stretcherLinkComponent.setParentId(boneDummyEntity->getId());

		// Enable bone linking
		qsf::game::BoneToBoneLinkComponent& boneToBoneLinkComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::game::BoneToBoneLinkComponent>();
		boneToBoneLinkComponent.setChildBoneName1("bone_stretcher_base");
		boneToBoneLinkComponent.setParentBoneName1("dummy_bone");
	}

	bool HelicopterHatchComponent::updateLowerStretcher(float secondsPassed)
	{
		if (!mStretcherEntity.valid())
		{
			// Stop further updates of this state
			return false;
		}

		qsf::TransformComponent& transformComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		glm::vec3 stretcherPosition = transformComponent.getPosition();

		// Check if stretcher is in final position
		const float offsetOverGround = 1.5f;
		if (mTargetPosition.y <= stretcherPosition.y - offsetOverGround)
		{
			// Move the stretcher
			// Calculate the speed the stretcher is moving this update
			const float movementDistance = mLowerHatchSpeed * secondsPassed;
			stretcherPosition.y -= movementDistance;
			transformComponent.setPosition(stretcherPosition);
			return true;
		}

		// Stretcher is in end position, don't lower it more
		return false;
	}

	bool HelicopterHatchComponent::updateHigherStretcher(float secondsPassed)
	{
		if (!mStretcherEntity.valid())
		{
			// Stop further updates of this state
			return true;
		}

		qsf::TransformComponent& transformComponent = mStretcherEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		glm::vec3 stretcherPosition = transformComponent.getPosition();

		qsf::TransformComponent& ownerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		glm::vec3 finalPosition = ownerTransformComponent.getPosition();

		if (mTargetIsInjured)
		{
			finalPosition.y -= mInjuredPersonFlyHigh;
		}

		// Check if stretcher is in final position
		if (finalPosition.y > stretcherPosition.y)
		{
			// Move the stretcher
			// Calculate the speed the stretcher is moving this update
			const float movementDistance = mRaiseHatchSpeed * secondsPassed;
			stretcherPosition.y += movementDistance;
			if (stretcherPosition.y > finalPosition.y)
			{
				stretcherPosition.y = finalPosition.y;
			}
			transformComponent.setPosition(stretcherPosition);
			return true;
		}

		return false;
	}

	void HelicopterHatchComponent::closeHatch()
	{
		// Play close hatch animation
		qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationHelicopterHatchOpen(), true, false, true);
	}

	void HelicopterHatchComponent::destroyStretcher()
	{
		// Delete the link between stretcher and target entity, to avoid deleting the target with the stretcher
		if (mTargetEntity.valid())
		{
			mTargetEntity->destroyComponent<qsf::LinkComponent>();
			// Remember that the target entity is now without linkage, an mechanism has to take care about it, e.g. "pickupPersonByFlyingHeliAction"
		}

		// Delete the stretcher entity
		if (mStretcherEntity.valid())
		{
			qsf::Map& map = mStretcherEntity->getMap();
			qsf::LinkComponent* stretcherLinkComponent = mStretcherEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != stretcherLinkComponent)
			{
				// Delete dummy bone
				qsf::Entity* dummyBoneEntity = map.getEntityById(stretcherLinkComponent->getParentId());
				if (nullptr != dummyBoneEntity)
				{
					MapHelper(map).destroyEntity(*dummyBoneEntity);
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
