// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/UseStunGrenadeAction.h"
#include "em5/action/gangster/BeThrownWeaponAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/police/UseStunGrenadeCommand.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/physics/collision/BulletBoxCollisionComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>
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
	const qsf::NamedIdentifier UseStunGrenadeAction::ACTION_ID = "em5::UseStunGrenadeAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseStunGrenadeAction::UseStunGrenadeAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	UseStunGrenadeAction::~UseStunGrenadeAction()
	{
		// Nothing here
	}

	void UseStunGrenadeAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
	}

	uint64 UseStunGrenadeAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UseStunGrenadeAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool UseStunGrenadeAction::onStartup()
	{
		return validateUserAndTarget();
	}

	qsf::action::Result UseStunGrenadeAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					// Play throw animation. Do not use an action for the animation as we have to start the real throw in the middle of it.
					qsf::AssetProxy animation(AnimationHelper(getEntity()).getAnimationSEKSpecialistThrowGrenade());
					qsf::MeshAnimationComponent& meshAnimationComponent = getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
					meshAnimationComponent.playAnimation(animation, false);

					// Time to wait based on animation
					mTimeStartThrowing = clock.getCurrentTime() + qsf::Time::fromSeconds(2.55);
					mCurrentState = STATE_START_THROW;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_START_THROW:
				{
					if (clock.getCurrentTime() < mTimeStartThrowing)
						return qsf::action::RESULT_CONTINUE;

					qsf::TransformComponent* unitTransformComponent = getEntity().getComponent<qsf::TransformComponent>();
					QSF_CHECK(unitTransformComponent != nullptr, "Got a unit without TransformComponent", return qsf::action::RESULT_DONE);

					// Create the object that is flying through the air
					const std::string PREFAB_STUN_GRENADE = "em5/prefab/equipment/" + em5::equipment::POLICE_STUN_GRENADE;
					qsf::Entity* grenadeEntity = getMap().createObjectByLocalPrefabAssetId(qsf::StringHash(PREFAB_STUN_GRENADE));
					QSF_CHECK(grenadeEntity != nullptr, "Could not create the grenade", return qsf::action::RESULT_DONE);

					// It needs a MovableComponent and a BulletCollisionComponent, so ensure it has those
					grenadeEntity->getOrCreateComponent<qsf::MovableComponent>();
					grenadeEntity->getOrCreateComponent<qsf::BulletBoxCollisionComponent>();

					// Set the correct weapon position (position of thrower + some offset)
					qsf::TransformComponent& grenadeTransformComponent = grenadeEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
					glm::vec3 offsetLocal(0.2, 1.8f, -0.8f);	// approximately where the hand should be now (guess-values)
					glm::vec3 offsetWorld = unitTransformComponent->getTransform().vec3DirectionLocalToWorld(offsetLocal);
					glm::vec3 weaponPos = unitTransformComponent->getPosition() + offsetWorld;
					grenadeTransformComponent.setPosition(weaponPos);
					grenadeTransformComponent.setScale(glm::vec3(1.0f));

					// Send it on it's way
					qsf::ActionComponent& grenadeActionComponent = grenadeEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					grenadeActionComponent.pushAction<BeThrownWeaponAction>(action::BLOCKING).init(getEntityId(), mTargetEntityId, weapon::STUNGRENADE, true);

					mCurrentState = STATE_WAIT_FOR_ANIMATION_END;

					// Fall through by design
				}

				case STATE_WAIT_FOR_ANIMATION_END:
				{
					qsf::AssetProxy animation(AnimationHelper(getEntity()).getAnimationSEKSpecialistThrowGrenade());
					qsf::MeshAnimationComponent& meshAnimationComponent = getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
					if (meshAnimationComponent.isAnimationPlaying(animation))
						return qsf::action::RESULT_CONTINUE;

					// Fall through by design
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool UseStunGrenadeAction::validateUserAndTarget()
	{
		// Build context
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (targetEntity == nullptr)
			return false;

		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = targetEntity;

		UseStunGrenadeCommand* useStunGrenadeCommand = static_cast<UseStunGrenadeCommand*>(QSFGAME_COMMAND.getCommandManager().getById<UseStunGrenadeCommand>(UseStunGrenadeCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != useStunGrenadeCommand, "UseStunGrenadeAction::validateUserAndTarget(): Could not find UseStunGrenadeCommand instance", return false);

		if (!useStunGrenadeCommand->checkContext(commandContext))
			return false;

		// We allow throwing somewhat above the ThrowDistance which was from the command to allow some movement for the target.
		// Only ensure it's not getting too crazy (throwing over half the map)
		const float TRHOW_DISTANCE = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeThrowDistance() * 1.5f; // 1.5 just some factor to prevent ever throwing on moving targets
		float currentDistanceToTarget = qsf::game::DistanceHelper::get2dDistance(getEntity(), *targetEntity);
		if (currentDistanceToTarget > TRHOW_DISTANCE)
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
