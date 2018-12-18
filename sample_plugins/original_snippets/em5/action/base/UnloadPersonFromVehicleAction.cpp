// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/UnloadPersonFromVehicleAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/placement/PlacementComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/math/Math.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace
	{
		bool getPlaceTransform(qsf::Entity& entity, qsf::Entity& vehicleEntity, qsf::Transform& transform)
		{
			CollisionHelper collisionHelper(entity.getMap());

			// Test the place where we want to spawn the unit (position is out parameter)
			glm::vec3 newPosition = transform.getPosition();
			if (!collisionHelper.findFreePositionEM3(entity, transform, 2.0f, 0.1f, newPosition))
			{
				// New check with higher searching radius, avoid aborting the action here
				if (!collisionHelper.findFreePositionEM3(entity, transform, 5.0f, 0.1f, newPosition))
				{
					// No free position found
					return false;
				}
			}

			// Check for height difference
			const qsf::Transform& vehicleTransform = vehicleEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
			if (std::abs(vehicleTransform.getPosition().y - newPosition.y) > 1.0f)
			{
				// It would be okay if the exiting person is roughly inside the vehicle's floor plane
				//  -> This is relevant when vehicle stands on a slope
				if (std::abs(vehicleTransform.vec3PositionWorldToLocal(newPosition).y) > 0.5f)
				{
					// The distance is too large, can't place the person there
					return false;
				}
			}

			if (collisionHelper.isInWater(newPosition))
			{
				// Can't exit unit in water, except when unloading diver from boat
				bool isBoatAndDiver = false;
				if (nullptr != vehicleEntity.getComponentById("em5::BoatComponent"))
				{
					CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
					isBoatAndDiver = (nullptr != commandableComponent && commandableComponent->isFireFighterDiver());
				}

				if (!isBoatAndDiver)
				{
					return false;
				}
			}

			// Everything okay
			transform.setPosition(newPosition);
			return true;
		}
	}



	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UnloadPersonFromVehicleAction::ACTION_ID = "em5::UnloadPersonFromVehicleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadPersonFromVehicleAction::UnloadPersonFromVehicleAction() :
		Action(ACTION_ID),
		mPlaceTransformInitialized(false),
		mPushAnimation(true),
		mEnforceUnloading(false),
		mDoorType(0),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	UnloadPersonFromVehicleAction::~UnloadPersonFromVehicleAction()
	{
		// Nothing here
	}

	void UnloadPersonFromVehicleAction::init(qsf::Entity& exitVehicle, uint32 doorType)
	{
		mVehicleEntity = exitVehicle;
		mDoorType = doorType;
	}

	void UnloadPersonFromVehicleAction::init(qsf::Entity& exitVehicle, const glm::vec3& placePosition, const glm::quat& placeRotation, qsf::Time spawnDelay, bool pushAnimation, bool enforceUnloading)
	{
		mVehicleEntity = exitVehicle;
		mPlaceTransform.setPositionAndRotation(placePosition, placeRotation);
		mPlaceTransformInitialized = true;
		mPushAnimation = pushAnimation;
		mSpawnDelay = spawnDelay;
		mEnforceUnloading = enforceUnloading;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UnloadPersonFromVehicleAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(co) Anything to do in here?
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result UnloadPersonFromVehicleAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				mSpawnDelay -= clock.getTimePassed();

				if (mSpawnDelay > qsf::Time::ZERO)
				{
					// Continue waiting
					return qsf::action::RESULT_CONTINUE;
				}

				if (!mPlaceTransformInitialized)
				{
					if (mVehicleEntity.valid())
					{
						// Get the door position
						mVehicleEntity->getComponentSafe<VehicleComponent>().getFirstDoorTransformByType(mDoorType, mPlaceTransform);

						// For leaving the vehicle, we rotate at 180°
						mPlaceTransform.setRotation(mPlaceTransform.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>()));
					}
					else
					{
						mPlaceTransform = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
					}
					mPlaceTransformInitialized = true;
				}

				mState = STATE_START_EFFECT;

				// Fall through by design
			}

			case STATE_START_EFFECT:
			{
				// Place person to final position
				qsf::Entity& entity = getEntity();
				qsf::MovableComponent& personMovableComponent = entity.getOrCreateComponentSafe<qsf::MovableComponent>();
				entity.getOrCreateComponentSafe<qsf::TransformComponent>();

				if (!getPlaceTransform(entity, mVehicleEntity.getSafe(), mPlaceTransform))
				{
					if (!mEnforceUnloading)
					{
						// Better give up
						return qsf::action::RESULT_DONE;
					}

					// Otherwise use the original place transform and place the person right there (yes, it's ugly, but we have to)
				}

				{ // Fixup rotation only rotation around y-axis are allowed
					glm::vec3 eulerAngles = qsf::EulerAngles::quaternionToEuler(mPlaceTransform.getRotation());

					// y and z component contains rotation around x/z-axis -> zero those values
					eulerAngles.y = 0.0f;
					eulerAngles.z = 0.0f;
					mPlaceTransform.setRotation(qsf::EulerAngles::eulerToQuaternion(eulerAngles));
				}

				bool doDropToGround = true;

				// Place object onto ground. Try first the placement component of the entity itself. If not successfully fall back to simple groundmap placement
				qsf::PlacementComponent* placementComponent = getEntity().getComponent<qsf::PlacementComponent>();
				if (nullptr != placementComponent)
				{
					// Use entity placement component to place the entity
					glm::vec3 placementPosition = mPlaceTransform.getPosition();
					glm::quat placementRotation = mPlaceTransform.getRotation();
					if (placementComponent->applyPlacement(placementPosition, placementRotation))
					{
						// Placement was successfull take over values
						mPlaceTransform.setPositionAndRotation(placementPosition, placementRotation);
						doDropToGround = false;
					}
				}

				if (doDropToGround)
				{
					// Drop place position to the ground
					glm::vec3 position = mPlaceTransform.getPosition();
					if (qsf::PlacementComponent::dropToGround(getMap(), position))
					{
						mPlaceTransform.setPosition(position);
					}
				}

				EntityHelper entityHelper(entity);
				entityHelper.leaveContainer(false);
				personMovableComponent.warpToPositionAndRotation(mPlaceTransform.getPosition(), mPlaceTransform.getRotation());

				{ // Say no to T-pose by ensuring there's always at least the idle/deboard animation active
					AnimationHelper animationHelper(entity);
					qsf::MeshAnimationComponent* meshAnimationComponent = entity.getOrCreateComponent<qsf::MeshAnimationComponent>();
					CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
					if (nullptr != commandableComponent && commandableComponent->isAmbulanceParamedics())
					{
						AnimationHelper::AnimationMaskData animationMaskData;
						meshAnimationComponent->playAnimation(qsf::AssetProxy(animationHelper.getIdleAnimation(animationMaskData)), false);
					}
					else
					{
						meshAnimationComponent->playAnimation(qsf::AssetProxy(animationHelper.getAnimationDeboardAnimation()), false);
					}

					// Avoid visual one frame glitches
					if (mVehicleEntity.valid() && EntityHelper(*mVehicleEntity).isEntityVisibleOnScreen())
					{
						meshAnimationComponent->enforceGraphicsSynchronization();
					}
				}

				// Reactivate movable component
				personMovableComponent.setActive(true);

				// Apply placement to have a valid position on the ground
				entityHelper.applyPlacement();

				mState = STATE_END_EFFECT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END_EFFECT:
			{
				// Start fade effect one frame later to assume the right animation / position
				EntityHelper(getEntity()).fadeIn();

				if (mPushAnimation)
				{
					CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
					if (nullptr != commandableComponent && commandableComponent->isAmbulanceParamedics())
					{
						// Extra handling for the paramedic team, it currently has no deboard animation and needs one, so we force an update for the idle action here
						getEntity().getComponentSafe<PersonComponent>().forceIdleAnimationUpdate();
					}
					else
					{
						// Play deboard animation for contained entity (with wait for finish flag, to finish the animation before the player can interact with them
						qsf::ActionComponent& actionComponent = getComponent();
						actionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).initWithAnimationRetargeting(AnimationHelper(getEntity()).getAnimationDeboardAnimation(), qsf::MeshAnimationComponent::ANIMATION_BONE_SCALE_RETARGETING, true);
					}
				}

				const uint64 vehicleEntityId = mVehicleEntity.valid() ? mVehicleEntity->getId() : qsf::getUninitialized<uint64>();
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_EXIT_VEHICLE, getEntityId(), vehicleEntityId));

				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
