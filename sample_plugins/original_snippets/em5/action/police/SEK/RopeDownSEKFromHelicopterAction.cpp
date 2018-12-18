// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SEK/RopeDownSEKFromHelicopterAction.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"

#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/math/Random.h>
#include <qsf/math/Math.h>
#include <qsf/map/ground/GroundMap.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/helper/OgreAnimationHelper.h>
#include <qsf/time/clock/Clock.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RopeDownSEKFromHelicopterAction::ACTION_ID = "em5::RopeDownSEKFromHelicopterAction";

	static const std::string ROPE_LINKE_BONE = "rope_link_";	///< It exists the bones "rope_link_1" -> "rope_link_3"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RopeDownSEKFromHelicopterAction::RopeDownSEKFromHelicopterAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mFalldownSpeed(3.f),
		mGroundHeight(0.f),
		mRopePlace(1),
		mIsNearGround(false),
		mAnimationState(STATE_INIT)
	{
		// Nothing here
	}

	RopeDownSEKFromHelicopterAction::~RopeDownSEKFromHelicopterAction()
	{
		// Delete the rope
		if (mRopeEntity.valid())
		{
			mRopeEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
		}
	}

	void RopeDownSEKFromHelicopterAction::init(qsf::Entity& vehicleEntity, qsf::Entity& fastRopeEntity, int ropePlace)
	{
		mVehicleEntity = vehicleEntity;
		mFastRopeEntity = fastRopeEntity;
		mRopePlace = ropePlace;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RopeDownSEKFromHelicopterAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RopeDownSEKFromHelicopterAction::onStartup()
	{
		mUpdateAnimation.registerAt(Jobs::ANIMATION_PERSON, boost::bind(&RopeDownSEKFromHelicopterAction::updateAnimation, this, _1));
		return true;
	}

	qsf::action::Result RopeDownSEKFromHelicopterAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				const float SEK_SPECIALIST_MOVEMENT = RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterExitSekHCSpeed();
				mFalldownSpeed = SEK_SPECIALIST_MOVEMENT * qsf::Random::getRandomFloat(0.8f, 1.2f);

				// GroundMapQuery fills mGroundHeight with the height of the ground map
				//  -> Filtering: Include everything, especially walkables (e.g. bridges)
				glm::vec3 position = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
				qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_MASK_ALL).getHeightAt(position, mGroundHeight);

				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistRopingLoop(), true);

				// Make entity visible
				getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
				getEntity().destroyComponent<qsf::game::HiddenComponent>();

				createRope();

				mState = STATE_LOWER_ROPE;
				mAnimationState = STATE_LOWER_ROPE;

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOWER_ROPE:
			{
				// Rope entity should be valid here, but let's play safe
				if (mRopeEntity.valid())
				{
					// Wait till the entity reaches the ground level
					if (mAnimationState == mState)
					{
						return qsf::action::RESULT_CONTINUE;
					}

					// Play audio
					AudioHelper::playSEKRoping(mAudioProxy, getEntity());
					mAudioProxy.synchronizeWithMultiplay();
				}

				mState = STATE_LOWER_UNIT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LOWER_UNIT:
			{
				if (mAnimationState == mState)
				{
					// Wait till the entity reaches the ground level
					return qsf::action::RESULT_CONTINUE;
				}

				// Stop audio
				mAudioProxy.stop();
				mAudioProxy.synchronizeWithMultiplay();

				mState = STATE_FINISH;
				// Fall through by design
			}

			case STATE_FINISH:
			{
				AnimationHelper animationHelper(getEntity());
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationSEKSpecialistRopingEnd()))
				{
					// Wait for finish the animation
					return qsf::action::RESULT_CONTINUE;
				}

				if (mRopeEntity.valid())
				{
					// Delete the rope
					mRopeEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
				}

				// Give the player the control for the unit
				EntityHelper entityHelper(getEntity());
				entityHelper.leaveContainer();
				entityHelper.applyPlacement();

				// Search for a collision-free position
				{
					qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
					glm::vec3 newPosition = transformComponent.getPosition();
					CollisionHelper collisionHelper(getEntity().getMap());
					if (collisionHelper.hasCollisionEM3IgnoreEntity(glm::vec3(0.1f, 0.1f, 0.1f), transformComponent.getTransform(), getEntity()))
						if (collisionHelper.findFreePositionEM3(getEntity(), transformComponent.getTransform(), 4.0f, 0.0f, newPosition))
							transformComponent.setPosition(newPosition);
				}

				// Emit messages
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_EXIT_VEHICLE, getEntityId(), mVehicleEntity->getId()));
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_SQUAD_EXIT_VEHICLE, CommandableComponent::UNITTAG_POLICE_SEK));
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RopeDownSEKFromHelicopterAction::createRope()
	{
		if (mFastRopeEntity.valid())
		{
			// Create object
			mRopeEntity = MapHelper(getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/" + equipment::TECH_HELICOPTER_ROBE));
			if (!mRopeEntity.valid())
			{
				return;	// Error
			}

			// Link rope on fast rope
			mRopeEntity->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(mFastRopeEntity.getSafe(), qsf::LinkComponent::HARD_LINK);
			qsf::game::BoneToBoneLinkComponent& boneToBoneLinkComponent = mRopeEntity->getOrCreateComponentSafe<qsf::game::BoneToBoneLinkComponent>();

			std::stringstream builder;
			builder << ROPE_LINKE_BONE << mRopePlace;	// Create the name ROPE_LINKE_BONE_X"
			boneToBoneLinkComponent.setParentBoneName1(builder.str());
			boneToBoneLinkComponent.setChildBoneName1("bone_stretcher_base");

			// Get the position of the fast rope bone
			glm::vec3 position;
			glm::quat rotation;
			qsf::OgreAnimationHelper::getBoneWorldTransformation(mFastRopeEntity.getSafe(), builder.str(), position, rotation);

			// Place the rope to the unit
			position.y -= 1;
			qsf::MovableComponent& ropeMovableComponent = mRopeEntity->getOrCreateComponentSafe<qsf::MovableComponent>();
			ropeMovableComponent.warpToPositionAndRotation(position, mFastRopeEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getRotation());

			// Set the entity on the right position
			static const glm::vec3 animationOffset(0.737f, -1.078f, -0.1f);
			glm::vec3 newPosition = mRopeEntity->getOrCreateComponentSafe<qsf::TransformComponent>().getTransform().vec3PositionLocalToWorld(animationOffset);
			getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().setPosition(newPosition);
		}
	}

	void RopeDownSEKFromHelicopterAction::updateAnimation(const qsf::JobArguments& jobArgs)
	{
		switch (mAnimationState)
		{
			case STATE_LOWER_ROPE:
			{
				// Rope entity should be valid here, but let's play safe
				if (mRopeEntity.valid())
				{
					qsf::TransformComponent& transformComponent = mRopeEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
					glm::vec3 position = transformComponent.getPosition();

					float heightChangeSpeed = mFalldownSpeed * 1.5f * jobArgs.getSecondsPassed(); // Increase the fall down speed a little bit

					position.y -= heightChangeSpeed;
					if (position.y > mGroundHeight)
					{
						// Wait till the entity reaches the ground level
						transformComponent.setPosition(position);
						return;
					}

					// Set entity on ground
					position.y = mGroundHeight;
					transformComponent.setPosition(position);
				}

				mAnimationState = STATE_LOWER_UNIT;
				break;
			}

			case STATE_LOWER_UNIT:
			{
				qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 position = transformComponent.getPosition();

				float heightChangeSpeed = mFalldownSpeed * jobArgs.getSecondsPassed();

				const float heightDifference = position.y - mGroundHeight;

				static const float NEAR_GROUND_HEIGHT = 1.0f;
				if (!mIsNearGround && heightDifference < NEAR_GROUND_HEIGHT)
				{
					mIsNearGround = true;
					AnimationHelper animationHelper(getEntity());
					animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistRopingEnd());
				}

				if (heightDifference < RoadVehicleSpecsGroup::getInstanceSafe().getHelicopterStartLandingHeightLower())
				{
					heightChangeSpeed *= 0.4f;
				}

				position.y -= heightChangeSpeed;
				if (position.y > mGroundHeight)
				{
					// Wait till the entity reaches the ground level
					transformComponent.setPosition(position);
					return;
				}

				// Set entity on ground
				position.y = mGroundHeight;
				transformComponent.setPosition(position);

				mAnimationState = STATE_FINISH;
				break;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
