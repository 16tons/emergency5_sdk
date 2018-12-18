// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/ShootAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/base/HuntAndShootAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/GameplayAssets.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/spawnpoint/civilist/CivilistSpawnPointComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ShootAction::ACTION_ID = "em5::ShootAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ShootAction::ShootAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	ShootAction::~ShootAction()
	{
		// Nothing here
	}

	void ShootAction::init(const qsf::Entity& targetEntity, const weapon::WeaponConfiguration& weaponConfiguration)
	{
		mTargetEntityId = targetEntity.getId();
		mWeaponConfig = weaponConfiguration;
	}

	uint64 ShootAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	float ShootAction::getShootRange() const
	{
		return mWeaponConfig.mShootRange;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ShootAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);

		// TODO(fw): Add weapon configuration serialization
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ShootAction::onStartup()
	{
		// No distance- or visibility-check here, do those before calling the action and just shoot when we get here.
		// Alternative would be to add another parameter for that in init, but I'd prefer not to do that.
		// This way we avoid things like shooter kneeling for a shot and then not shooting because he is now
		// a little bit too far away.

		return validateUserAndTarget();
	}

	qsf::action::Result ShootAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
			if (nullptr == targetEntity)
			{
				// No valid target! Abort action!
				return qsf::action::RESULT_DONE;
			}

			// Show weapon if not happened already
			// Get weapon from gangster logic
			GangsterBaseLogic* gangsterLogic = EntityHelper(getEntity()).getGameLogic<GangsterBaseLogic>();
			if (nullptr != gangsterLogic)
			{
				// Equip (shooting) weapons
				switch (mWeaponConfig.mWeaponType)
				{
					case weapon::PISTOL:
						gangsterLogic->showWeapon(equipment::GANGSTER_PISTOL);
						break;

					default:
						break;
				}
			}

			// Turn to target at every time he wants to shoot
			const float targetYaw = TurnToAction::calcYawRotationToTarget(getEntity(), *targetEntity, false);
			TurnToAction::turnToTarget(getEntity(), clock.getTimePassed().getSeconds(), targetYaw);

			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					const qsf::TransformComponent& callerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
					const qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

					const float distance = qsf::game::DistanceHelper::get2dDistance(callerTransformComponent.getPosition(), targetTransformComponent.getPosition());
					if (distance > mWeaponConfig.mShootRange * 1.2f)
					{
						// Failed: Target is too far away
						return qsf::action::RESULT_DONE;
					}

					// The following actions ensure that the shooter aim his weapon to the target
					AnimationHelper animationHelper(getEntity());
					switch (mWeaponConfig.mWeaponType)
					{
						case weapon::PISTOL: // Same animation used by SEK and gangster
							animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistPistol02(), false, false, qsf::Time::fromSeconds(0.1f));
							break;

						case weapon::RIFLE: // Rifle is SEK-only
							animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistKneelRifleLoop(), false, false, qsf::Time::fromSeconds(0.5f));
							break;
					}

					mCurrentState = STATE_AIM_AT_TARGET;
					mWaitTimeToAim = mWeaponConfig.mTimeToAim;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_AIM_AT_TARGET:
				{
					// Disable the particle effect now, so it will be synchronized across network
					showMuzzleFlash(false);

					if (mWaitTimeToAim > qsf::Time::ZERO)
					{
						// Wait till the wait time is over
						mWaitTimeToAim -= clock.getTimePassed();
						return qsf::action::RESULT_CONTINUE;
					}

					mCurrentState = STATE_FINISH_SHOOT;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_FINISH_SHOOT:
				{
					// Enqueue the actual shooting animation(s)
					AnimationHelper animationHelper(getEntity());
					switch (mWeaponConfig.mWeaponType)
					{
						case weapon::PISTOL: // Same animation used by SEK and gangster
							animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistPistolShoot(), false, false, qsf::Time::fromSeconds(0.1f));
							break;

						case weapon::RIFLE: // Rifle is SEK-only
							animationHelper.playAnimation(animationHelper.getAnimationSEKSpecialistShootRifle(), false, false, qsf::Time::fromSeconds(0.5f));
							break;
					}

					// Enable the particle effect, so it is played once (we assume auto-repeat is off on all correctly edited weapons)
					showMuzzleFlash(true);
					injureTarget();

					if (mWeaponConfig.mWeaponType == weapon::PISTOL)
					{
						// Play audio
						AudioProxy audioProxy;
						if (EntityHelper(getEntity()).isGangsterPerson())
						{
							AudioHelper::playGangsterPistoleShot(audioProxy, getEntity());
						}
						else
						{
							AudioHelper::playSEKPistoleShot(audioProxy, getEntity());
						}
						audioProxy.detach();
						audioProxy.synchronizeWithMultiplay();
					}
					else // if (mUsedWeapon == weapon::RIFLE)
					{
						// Play audio
						AudioProxy audioProxy;
						AudioHelper::playSEKRifleShot(audioProxy, getEntity());
						audioProxy.detach();
						audioProxy.synchronizeWithMultiplay();
					}

					// Avoid repeated shooting is happening too fast and it's nicer if the unit doesn't drop the weapon immediately after a shot
					getComponent().pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mWeaponConfig.mTimeToWaitAfterShot);

					qsf::MessageParameters parameters;
					parameters.setParameter("ShooterId", getEntityId());
					parameters.setParameter("ShooteeId", mTargetEntityId);
					parameters.setParameter("GunType", int(mWeaponConfig.mWeaponType));
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_GUNSHOT, getEntityId(), mTargetEntityId), parameters);

					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					// TODO(mk) Code here is never reached (the pre-check stops this)
					break;
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool ShootAction::validateUserAndTarget() const
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);

		if (nullptr == targetEntity)
			return false;

		HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			// Only for persons
			if (healthComponent->isDead() || healthComponent->isInjured())
			{
				// It's enough, stop shooting here
				return false;
			}
		}

		// Everything OK
		return true;
	}

	void ShootAction::showMuzzleFlash(bool visible)
	{
		// The weapon (pistol and rifle) have edited muzzle flash entities
		qsf::Entity* equipmentEntity = EntityHelper(getEntity()).getActiveEquipmentEntity();
		if (nullptr != equipmentEntity)
		{
			qsf::ParticlesComponent* particleComponent = equipmentEntity->getComponent<qsf::ParticlesComponent>();
			if (nullptr == particleComponent)
			{
				// As fallback, search the particleComponent via the linkComponent
				qsf::LinkComponent* linkComponent = equipmentEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					particleComponent = linkComponent->getComponentFromEntityOrLinkedChild<qsf::ParticlesComponent>();
				}
			}

			if (nullptr != particleComponent)
			{
				particleComponent->setActive(visible);
			}
		}
	}

	void ShootAction::injureTarget()
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return;	// Error

		EntityHelper targetEntityHelper(*targetEntity);
		EntityHelper entityHelper(getEntity());

		// Getting shot with injuring and particle effects
		{
			qsf::TransformComponent* targetTransformComponent = targetEntity->getComponent<qsf::TransformComponent>();
			HealthComponent* healthComponent = targetEntity->getComponent<HealthComponent>();
			if (nullptr != healthComponent)
			{
				// Entity with health component (likely a person)
				float amount = healthComponent->getMaximumHealthEnergy() * mWeaponConfig.mDamageHealth;

				// Take a unspecific component from event sender
				healthComponent->receiveDamage(amount, damagetype::WEAPONS, getEntity().getComponent<qsf::TransformComponent>());
			}
			else
			{
				// Maybe a car was shot at?
				VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
				if (nullptr != vehicleComponent)
				{
					float damage = (targetEntityHelper.isFlying() ? 0 : mWeaponConfig.mDamageVehicle);

					// Yes, it's a car!

					// TODO(mk) here use the receiveDamage() function
					vehicleComponent->applyDamage(damage, entityHelper.getFreeplayEvent());

					// Spark particles
					if (nullptr != targetTransformComponent)
					{
						qsf::Map& map = targetEntity->getMap();
						const float particleTimeout = 0.75f;
						MapHelper(map).spawnParticle(assets::PREFAB_SHOOT_WEAPON_EFFECT.getHash(), particleTimeout,
							targetTransformComponent->getPosition() + glm::vec3(0, 1, 0), // Let spark particles appear somewhere around middle of body
							glm::vec3(0.5f));
					}
				}
			}

		}

		// Civil persons should flee
		if (targetEntityHelper.isCivilPerson())
		{
			fleeCivilPerson(*targetEntity);
		}
		else if (MapHelper::isUnitAutomatismActive())
		{
			unitShootBackAutomatisim(*targetEntity);
		}
	}

	void ShootAction::fleeCivilPerson(qsf::Entity& civilEntity)
	{
		qsf::ActionComponent& actionComponent = civilEntity.getOrCreateComponentSafe<qsf::ActionComponent>();

		qsf::logic::TargetPoint moveTargetPoint;
		moveTargetPoint.mTolerance = 1.0f;

		if (actionComponent.getCurrentPriority() >= action::DANGER_URGENT)
			return;	// Not affected

		MoveToDespawnComponent* moveToDespawnComponent = civilEntity.getComponent<MoveToDespawnComponent>();
		if (nullptr == moveToDespawnComponent)
			return;	// Civil with movement target are not affected

		const glm::vec3& gangsterPosition = EntityHelper(getEntity()).getPosition();
		const glm::vec3& personPosition = EntityHelper(civilEntity).getPosition();
		const glm::vec3 gangsterToPerson = glm::normalize(personPosition - gangsterPosition);

		// Collect all relevant spawn points
		std::vector<CivilistSpawnPointComponent*> spawnPoints;	// Exit and transit spawn points in one vector
		moveToDespawnComponent->getExitAndTransitSpawnPoints(spawnPoints, spawnPoints);

		// Randomly iterate over the spawn points
		ComponentsRandomEnumerator<CivilistSpawnPointComponent> randomEnumerator(constructionpolicy::SWAP, spawnPoints);
		while (CivilistSpawnPointComponent* spawnPoint = randomEnumerator.getNext())
		{
			const glm::vec3& spawnPosition = spawnPoint->getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
			const glm::vec3 personToSpawn = glm::normalize(spawnPosition - personPosition);

			// We should not move to a despawn point which we can reach before the 10 seconds are over (with a run speed of 3 m/s we can reach 30 meter)
			const float personToSpawnSquaredDistance = qsf::game::DistanceHelper::getSquaredDistance(spawnPosition, personPosition);
			if (personToSpawnSquaredDistance <= (30.0f * 30.0f))
				continue; // out of Range

			// We search for a good angle
			const float angle = glm::dot(gangsterToPerson, personToSpawn);
			if (angle > 0.0f)
			{
				// We found a point we can run to
				moveTargetPoint.mPosition = spawnPosition;

				actionComponent.clearPlan();
				actionComponent.pushAction<MoveAction>(action::DANGER_URGENT).init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint), MovementModes::MOVEMENT_MODE_FREE_RUN, nullptr, qsf::Time::fromSeconds(10.0f));
				break;
			}
		}
	}

	void ShootAction::unitShootBackAutomatisim(qsf::Entity& targetEntity)
	{
		// Automatic reaction of shooting back from targets
		EntityHelper targetEntityHelper(targetEntity);
		if (!targetEntityHelper.isUnit(CommandableComponent::UNITTAG_POLICE_POLICE) && !targetEntityHelper.isUnit(CommandableComponent::UNITTAG_POLICE_SEK))
			return;	// Not affected

		qsf::ActionComponent& actionComponent = targetEntity.getOrCreateComponentSafe<qsf::ActionComponent>();

		// Only shoot if nothing better to do
		qsf::Action* currentAction = actionComponent.getCurrentAction();
		if (currentAction == nullptr && actionComponent.getAction<HuntAndShootAction>() == nullptr && actionComponent.getCurrentPriority() < action::DANGER_URGENT)
		{
			// Only if its possible
			if (!targetEntityHelper.isSquadPersonInValidState())
				return;

			if (targetEntityHelper.isCarryingAnotherPerson())
				return;

			// Only if unit is not selected
			if (EM5_GAME.getSelectionManager().isIdSelected(targetEntity.getId()))
				return;

			weapon::WeaponConfiguration weaponConfiguration;
			weaponConfiguration.mWeaponType = weapon::PISTOL;
			weaponConfiguration.mDamageHealth = LogicSpecsGroup::getInstanceSafe().getSEKPistolHealthDamage();
			weaponConfiguration.mDamageVehicle = LogicSpecsGroup::getInstanceSafe().getSEKPistolVehicleDamage();
			weaponConfiguration.mShootRange = LogicSpecsGroup::getInstanceSafe().getSEKPistolShootRange();
			weaponConfiguration.mTimeToAim = qsf::Time::fromSeconds(0.5f);
			weaponConfiguration.mTimeToWaitAfterShot = qsf::Time::fromSeconds(0.7f);
			actionComponent.clearPlan();

			targetEntityHelper.showEquipment(equipment::POLICE_PISTOL);
			actionComponent.pushAction<HuntAndShootAction>(action::DANGER_URGENT).init(getEntity(), weaponConfiguration);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
