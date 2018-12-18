// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/HuntAndShootAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/ShootAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/ShootPersonTargetPointProvider.h"
#include "em5/game/Game.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/AchieveLineOfSightGoal.h>

#include <qsf/QsfHelper.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/SegmentDebugDrawRequest.h>
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier HuntAndShootAction::ACTION_ID = "em5::HuntAndShootAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HuntAndShootAction::HuntAndShootAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mNumberOfShots(0)
	{
		// Nothing here
	}

	HuntAndShootAction::~HuntAndShootAction()
	{
		// Nothing to do in here
	}

	void HuntAndShootAction::init(qsf::Entity& targetEntity, const weapon::WeaponConfiguration& weaponConfiguration)
	{
		mTargetEntity = targetEntity;

		mWeaponConfiguration = weaponConfiguration;
	}

	float HuntAndShootAction::getShootRange() const
	{
		return mWeaponConfiguration.mShootRange;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void HuntAndShootAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Serialize this
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool HuntAndShootAction::onStartup()
	{
		// We only support pistols and rifle as shoot weapons
		if (mWeaponConfiguration.mWeaponType != weapon::PISTOL &&
			mWeaponConfiguration.mWeaponType != weapon::RIFLE)
		{
			return false;
		}

		return true;
	}

	void HuntAndShootAction::onShutdown()
	{
		AnimationHelper animationHelper(getEntity());
		animationHelper.clearAnimation();
	}

	qsf::action::Result HuntAndShootAction::updateAction(const qsf::Clock&)
	{
		if (!isTargetValid())
		{
			mCurrentState = STATE_END;
		}

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// TODO(mk) Nothing here?
			}

			case STATE_GOTOTARGET:
			{
				const short traceLineCollisionFlags = qsf::BulletCollisionComponent::mDefaultCollisionMask;
				const MovementModes::Modes& movementMode = EntityHelper(getEntity()).isGangsterPerson() ? MovementModes::MOVEMENT_MODE_RUN : MovementModes::MOVEMENT_MODE_DEFAULT;
				getComponent().pushAction<MoveAction>(action::COMMAND_STD, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::AchieveLineOfSightGoal(getEntity(), mTargetEntity.getSafe(), ShootPersonTargetPointProvider::TARGET_POINT_ID, traceLineCollisionFlags), movementMode);

				mCurrentState = STATE_SHOOT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_SHOOT:
			{
				const qsf::TransformComponent& callerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				const qsf::TransformComponent& targetTransformComponent = mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

				const float distance = qsf::game::DistanceHelper::get2dDistance(callerTransformComponent.getPosition(), targetTransformComponent.getPosition());
				if (distance < (mWeaponConfiguration.mShootRange * 1.2f)) // Allow a little bit tolerance
					// TODO(tl): Line to sight check removed or we could put gangster and police in a state being helpless and unable to shoot
					// (hasFreeSightlineTo(getEntity().getMap().getCoreEntity().getComponent<qsf::PhysicsWorldComponent>(), callerTransformComponent, targetTransformComponent, mTargetEntity.get()))
				{
					getComponent().pushAction<ShootAction>(action::COMMAND_STD, qsf::action::INSERT_AT_FRONT).init(mTargetEntity.getSafe(), mWeaponConfiguration);

					++mNumberOfShots;	// Count every shot
					if (mWeaponConfiguration.mMaxNumberShots > 0 && mNumberOfShots >= mWeaponConfiguration.mMaxNumberShots)
					{
						// Reached maximum number of shots for this target, abort action
						mCurrentState = STATE_END;
					}
				}
				else
				{
					// Check if it's time to give up (for vehicles which drive faster than we walk)
					const float GIVE_UP_VEHICLE_PURSUIT = LogicSpecsGroup::getInstanceSafe().getSEKGiveUpVehiclePursuit();
					if (distance >= GIVE_UP_VEHICLE_PURSUIT && EntityHelper(mTargetEntity.getSafe()).isRoadVehicle())
					{
						showOutOfRangeHint(targetTransformComponent.getPosition());
						mCurrentState = STATE_END;
					}
					else
					{
						// Move closer to the target
						mCurrentState = STATE_GOTOTARGET;
					}
				}

				// Loop the action
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END:
			{
				playStandUpAnimation();

				// Action is finished, abort
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool HuntAndShootAction::isTargetValid() const
	{
		if (!mTargetEntity.valid())
			return false;

		EntityHelper entityHelper(mTargetEntity.getSafe());
		if (entityHelper.isEntityPerson())
		{
			if (entityHelper.isPersonInjured())
			{
				return false;
			}
		}
		else
		{
			if (nullptr == mTargetEntity->getComponent<VehicleComponent>())
				return false;

			if (entityHelper.isBurning() || entityHelper.isBurned())
				return false;

			if (entityHelper.isVehicleDestroyed())
			{
				return false;
			}
		}

		if (!entityHelper.isInMapBoundaries())
			return false;

		if (entityHelper.getContainerEntity() != nullptr)
			return false;

		return true;
	}

	void HuntAndShootAction::playStandUpAnimation()
	{
		if (mWeaponConfiguration.mWeaponType != weapon::RIFLE)
			return;

		qsf::MeshAnimationComponent* meshAnimationComponent = getEntity().getComponent<qsf::MeshAnimationComponent>();
		if (nullptr == meshAnimationComponent)
			return;

		// (mk) was previously "if (animationHelper.isAnimationPlaying(animationHelper.getAnimationSEKSpecialistShootRifle()))" but this code was somehow never true (channel with animation never finished). Blending here is also good
		AnimationHelper animationHelper(getEntity());
		if (meshAnimationComponent->getCurrentAnimation() != animationHelper.getAnimationSEKSpecialistShootRifle())
			return;

		// In this case stand up after shooting	(use action here because we blend the animation)
		getComponent().pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationSEKSpecialistKneelRifleEnd(), false, false, false, qsf::Time::fromSeconds(0.5f));
	}

	void HuntAndShootAction::showOutOfRangeHint(const glm::vec3& position) const
	{
		// Blink minimap at position
		MiniMapIconRegistrationLogic& minimapIconRegistrationLogic = EM5_GAME.getMiniMapIconRegistrationLogic();
		minimapIconRegistrationLogic.registerPingByWorldPosition(position);
		minimapIconRegistrationLogic.registerHighlightIcon(mTargetEntity->getId());

		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			HintHelper::showHint("ID_SEKSPECIALIST_CARTARGET_ESCAPES_01", commandableComponent->getPlayerId(), qsf::Time::fromSeconds(20.0f));
		}
	}

	bool HuntAndShootAction::hasFreeSightlineTo(qsf::PhysicsWorldComponent* physicsWorldComponent, const qsf::TransformComponent& startTransform, const qsf::TransformComponent& targetTransform, qsf::Entity* targetEntity)
	{
		QSF_ASSERT(nullptr != physicsWorldComponent, "SEKSpecialistLogic::hasFreeSightlineTo needs the PhysicWorldComponent on the core entity", QSF_REACT_NONE);
		if (nullptr == physicsWorldComponent)
			return false;

		const float eyeHeight = 1.8f;			// SEK experts are big strong men - eye-height at 1.80
		const float targetHeight = 1.4f;		// Shoot in the heart
		glm::vec3 rayStart(startTransform.getPosition());
		glm::vec3 rayEnd(targetTransform.getPosition());
		rayStart.y += eyeHeight;
		rayEnd.y += targetHeight;

		// Move start a little bit away to avoid hitting the unit itself.
		rayStart += glm::normalize(rayEnd-rayStart) * 0.9f;	// TODO(mz): Should use real radius (0.9 is just a guess)

		#ifndef ENDUSER
			QSF_DEBUGDRAW.requestDraw(qsf::SegmentDebugDrawRequest(qsf::Segment::fromTwoPoints(rayStart, rayEnd), qsf::Color4::RED), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(1.f)));
		#endif

		glm::vec3 hitPositionBullet;
		qsf::Entity* firstHitEntity = physicsWorldComponent->rayTestFetchFirstHitEntity(rayStart, rayEnd, &hitPositionBullet);
		return (firstHitEntity == targetEntity || firstHitEntity == nullptr);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
