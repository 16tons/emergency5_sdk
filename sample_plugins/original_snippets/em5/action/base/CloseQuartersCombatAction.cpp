// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/component/GameComponent.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#ifndef ENDUSER
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CloseQuartersCombatAction::ACTION_ID = "em5::CloseQuartersCombatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CloseQuartersCombatAction::CloseQuartersCombatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(COMBATSTATE_INIT),
		mAttackerWeapon(weapon::NOFIGHT),
		mTargetWeapon(weapon::NOFIGHT),
		mLowerTargetLife(0.0f),
		mLowerAttackerLife(0.0f),
		mMaxCombatRounds(0),
		mCurrentRound(0),
		mTargetInjured(false),
		mAttackerInjured(false),
		mCounterAttack(false),
		mLowerHealthLimitPercentage(0.0f)
	{
		// Nothing here
	}

	CloseQuartersCombatAction::~CloseQuartersCombatAction()
	{
		// Nothing here
	}

	void CloseQuartersCombatAction::init(const qsf::Entity& targetEntity, weapon::Weapon attackerWeapon, weapon::Weapon targetWeapon, uint64 maxCombatRounds, float lowerTargetLife, float lowerAttackerLife, float healthRestrictionPercentage)
	{
		mTargetEntityId = targetEntity.getId();
		mAttackerWeapon = attackerWeapon;
		mTargetWeapon = targetWeapon;
		mMaxCombatRounds = maxCombatRounds;
		mLowerTargetLife = lowerTargetLife;
		mLowerAttackerLife = lowerAttackerLife;
		mLowerHealthLimitPercentage = glm::clamp(healthRestrictionPercentage, 0.0f, 1.0f);

		// If uninitialized values have been set for weapon damages (lowerXyzLife), set the default damage for the specific weapons here
		if (qsf::isUninitialized(mLowerTargetLife))
		{
			mLowerTargetLife = getDefaultWeaponDamage(attackerWeapon);
		}
		if (qsf::isUninitialized(mLowerAttackerLife))
		{
			mLowerAttackerLife = getDefaultWeaponDamage(targetWeapon);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CloseQuartersCombatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
		serializer.serializeAs<uint16>(mAttackerWeapon);
		serializer.serializeAs<uint16>(mTargetWeapon);
		serializer.serialize(mLowerTargetLife);
		serializer.serialize(mLowerAttackerLife);
		serializer.serialize(mMaxCombatRounds);
		serializer.serialize(mTargetInjured);
		serializer.serialize(mAttackerInjured);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CloseQuartersCombatAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Check for reservation
		if (!ReserveLogic::checkReservation(*targetEntity, callerEntity.getId(), ReserveLogic::NO_UNIT))
			return false;

		EntityHelper callerEntityHelper(callerEntity);
		EntityHelper targetEntityHelper(*targetEntity);

		GangsterBaseLogic* targetGangsterLogic = targetEntityHelper.getGameLogic<GangsterBaseLogic>();
		if (nullptr != targetGangsterLogic && GangsterBaseLogic::STATE_ARRESTED == targetGangsterLogic->getCurrentState())
		{
			// Target is arrested, no fight possible!
			return false;
		}

		// Check if target does not have someone arrested
		if (callerEntityHelper.isGuidingPerson() || targetEntityHelper.isGuidingPerson())
			return false;

		// Check for healthy caller and target
		if (callerEntityHelper.isPersonInjured() || targetEntityHelper.isPersonInjured())
			return false;

		// Check if the entities are not inside a container by this time (building or vehicle)
		QSF_CHECK(nullptr == callerEntityHelper.getContainerEntity(), "The EM5 close quarters combat action was unable to start because the caller entity is inside a container (vehicle or building)", return false);
		QSF_CHECK(nullptr == targetEntityHelper.getContainerEntity(), "The EM5 close quarters combat action was unable to start because the target entity is inside a container (vehicle or building)", return false);

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullObjectDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);

		if (currentDistance > maxDistance)
			return false;

		// If we attack a police man, we need to check that this policeman is trying to arrest us
		const bool isTutorialActive = getMap().getCoreEntity().getOrCreateComponentSafe<GameComponent>().isInTutorial();	// Disable this technique for tutorial
		if ((targetEntityHelper.isUnit("Policeman") || targetEntityHelper.isUnit("PoliceSekman")) && !isTutorialActive)
		{
			qsf::ActionComponent& targetActionComponent = targetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			ArrestGangsterAction* targetArrestGangsterAction = targetActionComponent.getAction<ArrestGangsterAction>();
			if (nullptr == targetArrestGangsterAction || targetArrestGangsterAction->getTargetEntityId() != callerEntity.getId())
			{
				targetActionComponent.clearPlan();
				targetActionComponent.pushAction<GetEquipmentAction>(action::BLOCKING).init(equipment::POLICE_HANDCUFFS_OPEN);
				targetActionComponent.pushAction<ArrestGangsterAction>(action::BLOCKING).init(callerEntity);
				return false;
			}
			else if (mMaxCombatRounds != ArrestGangsterAction::FIGHT_ROUNDS)	// TODO(tl): using the number of fight rounds is a bad hack
			{
				return false;	// Wait for the existing arrest to arrest the gangster
			}
		}

		// Add reservation to prevent multiple attackers at once
		ReserveLogic::createReservation<ReserveCloseCombatLogic>(*targetEntity, callerEntity.getId());

		return true;
	}

	void CloseQuartersCombatAction::onShutdown()
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);

		// In case the combat was interrupted, this clears any pending animations
		AnimationHelper(callerEntity).clearAnimation();

		// Get health components
		HealthComponent* healthComponentCaller = callerEntity.getComponent<HealthComponent>();
		if (nullptr != healthComponentCaller)
		{
			// Allow persons to regenerate again
			healthComponentCaller->setRegenerationDisabled(false);
		}

		// Assuming only gangsters start fights and have weapons like baseball-bat or knife equipped...

		// Get weapon from gangster logic
		GangsterBaseLogic* gangsterLogic = EntityHelper(callerEntity).getGameLogic<GangsterBaseLogic>();
		if (nullptr != gangsterLogic)
		{
			// Hide weapon, if any...
			// TODO(fw): Is this wise? What if gangster wins?
			gangsterLogic->hideCurrentWeapon();
		}

		if (nullptr != targetEntity)
		{
			AnimationHelper(*targetEntity).clearAnimation();
			HealthComponent* healthComponentTarget = targetEntity->getComponent<HealthComponent>();
			if (nullptr != healthComponentTarget)
			{
				// Allow persons to regenerate again
				healthComponentTarget->setRegenerationDisabled(false);
			}

			// In case the combat was interrupted (or even when it was finished the usual way), remove the hacky wait action that ensures synchronization of the two opponents
			qsf::ActionComponent* targetActionComponent = targetEntity->getOrCreateComponent<qsf::ActionComponent>();
			qsf::WaitAction* waitAction = targetActionComponent->getAction<qsf::WaitAction>();
			if (nullptr != waitAction)
			{
				// Found the wait action, remove it
				targetActionComponent->removeAction(*waitAction);
			}

			// Remove reservation again
			ReserveLogic::createReservation<ReserveCloseCombatLogic>(*targetEntity, callerEntity.getId());
		}
	}

	qsf::action::Result CloseQuartersCombatAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		PersonComponent* callerPersonComponent = callerEntity.getComponent<PersonComponent>();
		QSF_CHECK(nullptr != callerPersonComponent, "The EM5 close quarters combat action was unable to retrieve the caller entity's person component", return qsf::action::RESULT_DONE);

		HealthComponent* healthComponentCaller = callerEntity.getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponentCaller, "The EM5 close quarters combat action was unable to retrieve the caller entity's health component", return qsf::action::RESULT_DONE);

		EntityHelper callerEntityHelper(callerEntity);
		QSF_CHECK(nullptr == callerEntityHelper.getContainerEntity(), "The EM5 close quarters combat action was unable to continue because the caller entity is inside a container (vehicle or building)", return qsf::action::RESULT_DONE);

		// Get the target entity instance
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 close quarters combat action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		PersonComponent* targetPersonComponent = targetEntity->getComponent<PersonComponent>();
		QSF_CHECK(nullptr != targetPersonComponent, "The EM5 close quarters combat action was unable to retrieve the target entity's person component", return qsf::action::RESULT_DONE);

		HealthComponent* healthComponentTarget = targetEntity->getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponentTarget, "The EM5 close quarters combat action was unable to retrieve the target entity's health component", return qsf::action::RESULT_DONE);

		EntityHelper targetEntityHelper(*targetEntity);
		QSF_CHECK(nullptr == targetEntityHelper.getContainerEntity(), "The EM5 close quarters combat action was unable to continue because the target entity is inside a container (vehicle or building)", return qsf::action::RESULT_DONE);

		// When fight is not yet over...
		if (COMBATSTATE_DONE != mState)
		{
			// Check for healthy caller and target
			if ((callerEntityHelper.isPersonInjured() && !mAttackerInjured) ||	// Caller injured (but not by this fight)
				(targetEntityHelper.isPersonInjured() && !mTargetInjured))		// Target injured (but not by this fight)
			{
				if (COMBATSTATE_INIT == mState)
				{
					// Fight has not yet begun
					return qsf::action::RESULT_DONE;
				}
				else
				{
					// Fight already in progress, some "cleanup" work to do
					mState = COMBATSTATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}
			}
		}

		// This a special case (unarmed fight, i.e. fists only), where the role of the attacker alternates each round
		const bool fistFight = (mAttackerWeapon == weapon::FIST && mTargetWeapon == weapon::FIST);

		// Set a flag whether the persons fight until one of them has 0 life
		const bool fightToInjury = (0 == mMaxCombatRounds);

		// Target health component
		switch (mState)
		{
			// Initialize fight, set flags etc.
			case COMBATSTATE_INIT:
			{
				// Check if the target is already in a fight (maybe even with the caller, happens if both are started at the same time)
				qsf::ActionComponent& targetActionComponent = targetEntity->getComponentSafe<qsf::ActionComponent>();
				const CloseQuartersCombatAction* targetCombatAction = targetActionComponent.getAction<CloseQuartersCombatAction>();
				if (nullptr != targetCombatAction && targetCombatAction->mState > COMBATSTATE_INIT)
				{
					return qsf::action::RESULT_DONE;
				}

				// Persons are now involved in a fight and can't turn to other attackers
				targetPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, true);
				callerPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, true);

				// Prevent persons in combat from regenerating
				healthComponentCaller->setRegenerationDisabled(true);
				healthComponentTarget->setRegenerationDisabled(true);

				// This action prevents the target from going on with his/her own actions. It is removed once the fight is over.
				// TODO(fw) Find a more elegant way to do that (without clearing the action plan of course)
				targetActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromHours(999.0f));

				// Enqueue fight-start animations to attacker and target, depending on respective weapon
				// We assume that the gangster is always the attacker

				// Get weapon from gangster logic
				GangsterBaseLogic* gangsterLogic = callerEntityHelper.getGameLogic<GangsterBaseLogic>();
				if (nullptr != gangsterLogic)
				{
					// Equip (close quarters) weapons
					switch (mAttackerWeapon)
					{
						case weapon::FIST:
							// Make sure the gangster does not equip any (visible) weapons
							gangsterLogic->hideCurrentWeapon();
							break;

						case weapon::TORCH:
							gangsterLogic->showWeapon(equipment::FANATIC_TORCH);
							break;

						case weapon::SWORD:
							gangsterLogic->showWeapon(equipment::POLICE_SWORD);
							break;

						case weapon::BASEBALLBAT:
							gangsterLogic->showWeapon(equipment::BASEBALL_BAT);
							break;

						case weapon::KNIFE:
							gangsterLogic->showWeapon(equipment::KNIFE);
							break;

						default:
							break;
					}
				}

				// Play start animation for both fighters
				playAnimationStartByWeapon(callerEntity, mAttackerWeapon);
				playAnimationStartByWeapon(*targetEntity, mTargetWeapon);

				// Let persons face each other
				getComponent().pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(targetEntity->getId());
				targetActionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(callerEntity.getId());

				mState = COMBATSTATE_ATTACK;

				return qsf::action::RESULT_CONTINUE;
			}

			case COMBATSTATE_ATTACK:
			{
				// Wait for target to become "idle"
				qsf::Action* currentAction = targetEntity->getComponent<qsf::ActionComponent>()->getCurrentAction();
				if (nullptr != currentAction && currentAction->getTypeId() != qsf::WaitAction::ACTION_ID)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				#ifndef ENDUSER
					// Register debug proxy to display debug text on top of fighting persons
					mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
				#endif

				// Keep track of the number of the current combat round
				++mCurrentRound;

				if (fistFight)
				{
					if (mCurrentRound % 2 == 1)
					{
						mTargetInjured = damageEntity(*healthComponentTarget, mLowerTargetLife, callerEntity);

						// Attacker's turn; If the target gets injured in this round, do not play full animations
						enqueueFightingAnimations(callerEntity, *targetEntity, mAttackerWeapon, mTargetWeapon, mTargetInjured ? FIRST_PART : FULL_ANIMATION);
					}
					else
					{
						mAttackerInjured = damageEntity(*healthComponentCaller, mLowerAttackerLife, *targetEntity);

						// Target's turn; If the attacker gets injured in this round, do not play full animations
						enqueueFightingAnimations(*targetEntity, callerEntity, mTargetWeapon, mAttackerWeapon, mAttackerInjured ? FIRST_PART : FULL_ANIMATION);
					}
				}
				else
				{
					// No fist-fight (i.e. at least one opponent has a weapon)
					mTargetInjured = damageEntity(*healthComponentTarget, mLowerTargetLife, callerEntity);

					// Enqueue the attacker's fighting animations (and the target's defense animations)
					enqueueFightingAnimations(callerEntity, *targetEntity, mAttackerWeapon, mTargetWeapon, mTargetInjured ? FIRST_PART : FULL_ANIMATION);
				}

				// Currently, it is the attacker's turn!
				mCounterAttack = false;
				mState = COMBATSTATE_CHECK_INJURIES;
				return qsf::action::RESULT_CONTINUE;
			}

			case COMBATSTATE_CHECK_INJURIES:
			{
				if (!mTargetInjured)
				{
					qsf::Action* currentAction = targetEntity->getComponent<qsf::ActionComponent>()->getCurrentAction();
					if (nullptr != currentAction && currentAction->getTypeId() != qsf::WaitAction::ACTION_ID)
					{
						return qsf::action::RESULT_CONTINUE;
					}
				}

				if (!mTargetInjured && !mAttackerInjured)
				{
					// No one is injured

					if (fistFight)
					{
						// Check whether too many rounds were fought. This is skipped when there is no max. number of rounds
						// For non-fistfight, this is checked in COMBATSTATE_COUNTERATTACK
						if (!fightToInjury && mCurrentRound >= mMaxCombatRounds)
						{
							mState = COMBATSTATE_DONE;
						}
						else
						{
							// In the case of a fist fight, the same case "COMBATSTATE_ATTACK" is executed in the next combat round
							mState = COMBATSTATE_ATTACK;
						}
					}
					else
					{
						if (mCounterAttack)
						{
							// This just has been a counter attack, so the round is over
							if (!fightToInjury && mCurrentRound >= mMaxCombatRounds)
							{
								mState = COMBATSTATE_DONE;
							}
							else
							{
								// Now it is the attacker's turn
								mState = COMBATSTATE_ATTACK;
							}
						}
						else
						{
							// Now it is the target's turn
							mState = COMBATSTATE_COUNTERATTACK;
						}
					}

					return qsf::action::RESULT_CONTINUE;
				}

				// Someone is injured!

				// TODO(db): Is this control flow really necessary? Because on this point the target or attacker are already injured.
				// Try to get the event ID component from the target
				//EventIdComponent* eventComp = targetEntity->getComponent<EventIdComponent>();
				//if (nullptr == eventComp)
				//{
				//	eventComp = callerEntity.getComponent<EventIdComponent>();
				//}

				//if (mTargetInjured)
				//{
				//	if (nullptr != eventComp)
				//	{
				//		AnimationHelper(*targetEntity).clearAnimation();

				//		// Apply injury
				//		if (!healthComponentTarget->isInjured())
				//		{
				//			healthComponentTarget->applyInjuryById(getInjuryCausedByWeapon(mAttackerWeapon), eventComp->getFreeplayEvent());
				//		}
				//	}

				//	// Let the attacker finish his attacking animation
				//	enqueueFightingAnimations(callerEntity, *targetEntity, mAttackerWeapon, weapon::NO_FIGHT, LAST_PART);
				//}
				//else // Attacker is injured
				//{
				//	if (nullptr != eventComp)
				//	{
				//		AnimationHelper(callerEntity).clearAnimation();

				//		// Apply injury
				//		if (!healthComponentCaller->isInjured())
				//		{
				//			healthComponentCaller->applyInjuryById(getInjuryCausedByWeapon(mTargetWeapon), eventComp->getFreeplayEvent());
				//		}
				//	}

				//	// Let the target finish his attacking animation
				//	enqueueFightingAnimations(*targetEntity, callerEntity, mTargetWeapon, weapon::NO_FIGHT, LAST_PART);
				//}

				mState = COMBATSTATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case COMBATSTATE_COUNTERATTACK:
			{
				// This is not a (pure) fist fight, otherwise we would never have got here.

				qsf::Action* currentAction = targetEntity->getComponent<qsf::ActionComponent>()->getCurrentAction();
				if (nullptr != currentAction && currentAction->getTypeId() != qsf::WaitAction::ACTION_ID)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				// Lower attacker's health energy
				healthComponentCaller->setHealthEnergy(healthComponentCaller->getHealthEnergy() - (healthComponentCaller->getMaximumHealthEnergy() * mLowerAttackerLife));
				mAttackerInjured = healthComponentCaller->getHealthEnergy() <= 0.0f;

				// Enqueue the target's fighting animations (and the attacker's defense animations)
				enqueueFightingAnimations(*targetEntity, callerEntity, mTargetWeapon, mAttackerWeapon, mAttackerInjured ? FIRST_PART : FULL_ANIMATION);

				// This is the target's turn!
				mCounterAttack = true;
				mState = COMBATSTATE_CHECK_INJURIES;
				return qsf::action::RESULT_CONTINUE;
			}

			case COMBATSTATE_DONE:
			{
				#ifndef ENDUSER
					// Remove debug text
					mDebugDrawProxy.unregister();
				#endif

				if (!mAttackerInjured)
				{
					// Let the attacker end her/his fighting animation
					playAnimationEndByWeapon(callerEntity, mAttackerWeapon);
				}

				if (!mTargetInjured)
				{
					// Let the target end her/his fighting animation
					playAnimationEndByWeapon(*targetEntity, mTargetWeapon);
				}

				// Reset flags
				targetPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, false);
				callerPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, false);

				// End fight
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void CloseQuartersCombatAction::enqueueFightingAnimations(qsf::Entity& attackerEntity, qsf::Entity& targetEntity, weapon::Weapon attackerWeapon, weapon::Weapon targetWeapon, FightAnimationMode animationMode)
	{
		#ifndef ENDUSER
			// Debug drawing
			if (GameDebugGroup::getInstanceSafe().getShowActivitiesDebug())
			{
				// Display debug text above the current attacker's head
				const glm::vec3& attackerDebugTextPosition = attackerEntity.getComponentSafe<qsf::TransformComponent>().getPosition();
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(std::to_string(mCurrentRound) + ". Round!", attackerDebugTextPosition + glm::vec3(0.0f, 2.0f, 0.0f), qsf::Color4::YELLOW));
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(getDebugWeaponName(attackerWeapon), attackerDebugTextPosition + glm::vec3(0.0f, 2.2f, 0.0f), qsf::Color4::CYAN));
			}
			else
			{
				mDebugDrawProxy.unregister();
			}
		#endif

		// Increase the person gets injured after the hit, we stop the fighting animation in the moment the "deadly hit" happens
		float startOffset = 0.0f;
		float endOffset = 1.0f;
		switch (animationMode)
		{
			case FULL_ANIMATION:
			{
				if (attackerWeapon == weapon::FIST)
				{
					// To tweak the fighting speed and make the look more "smooth", we cut some animation time (probably 90% of the fighting animation)
					startOffset = 0.1f;
					endOffset = 0.90f;
					break;
				}
				break;
			}

			case FIRST_PART:
			{
				switch (attackerWeapon)
				{
					case weapon::FIST:
						endOffset = 0.68f;
						break;

					case weapon::BASEBALLBAT:
						endOffset = 0.57f;
						break;

					case weapon::KNIFE:
						endOffset = 0.28f;
						break;

					case weapon::TORCH:
					case weapon::SWORD:
					default:
						break;
				}
				break;
			}

			case LAST_PART:
			{
				switch (attackerWeapon)
				{
					case weapon::FIST:
						startOffset = 0.70f;
						break;

					case weapon::BASEBALLBAT:
						startOffset = 0.61f;
						break;

					case weapon::KNIFE:
						startOffset = 0.31f;
						break;

					case weapon::TORCH:
					case weapon::SWORD:
					default:
						break;
				}

				break;
			}

			default:
				break;
		}

		// The animation assets not matching fully together. The time for attacking and defending is not matching. So we only use the shortest animation as maxPlayTime (also use the Offset). The hard coded values are the time the the animation takes
		const qsf::Time blendTime = qsf::Time::fromSeconds(0.4f);
		float maxPlayTime = 0.0f;

		switch (attackerWeapon)
		{
			case weapon::FIST:
				maxPlayTime = 3.208333f * (endOffset - startOffset);
				break;

			case weapon::BASEBALLBAT:
				maxPlayTime = 1.458333f * (endOffset - startOffset);
				break;

			case weapon::KNIFE:
				maxPlayTime = 3.583333f * (endOffset - startOffset);
				break;

			case weapon::TORCH:
				maxPlayTime = 3.625f * (endOffset - startOffset);
				break;

			case weapon::SWORD:
				maxPlayTime = 4.0f * (endOffset - startOffset);
				break;

			default:
				// Nothing to do in this case
				break;
		}
		switch (targetWeapon)
		{
			case weapon::FIST:
				maxPlayTime = glm::min(maxPlayTime, 3.208333f * (endOffset - startOffset));
				break;

			case weapon::BASEBALLBAT:
				maxPlayTime = glm::min(maxPlayTime, 1.875f * (endOffset - startOffset));
				break;

			case weapon::KNIFE:
				maxPlayTime = glm::min(maxPlayTime, 3.625f * (endOffset - startOffset));
				break;

			case weapon::TORCH:
				maxPlayTime = glm::min(maxPlayTime, 3.625f * (endOffset - startOffset));
				break;

			case weapon::SWORD:
				maxPlayTime = glm::min(maxPlayTime, 4.0f * (endOffset - startOffset));
				break;

			default:
				// Nothing to do in this case
				break;
		}

		// Enqueue animations for the actual attack
		AnimationHelper animationHelper(attackerEntity);
		switch (attackerWeapon)
		{
			case weapon::FIST:
			{
				playAnimationWithOffset(attackerEntity, animationHelper.getAnimationFistFight01Loop(), blendTime, startOffset, endOffset);

				// Play audio
				if (qsf::Random::getRandomChance(0.5f))
				{
					AudioHelper::playGangsterFistVersion1(mAudioProxy, getEntity(), qsf::Time::fromSeconds(0.5f));
					mAudioProxy.synchronizeWithMultiplay();
				}
				else
				{
					AudioHelper::playGangsterFistVersion2(mAudioProxy, getEntity(), qsf::Time::fromSeconds(0.5f));
					mAudioProxy.synchronizeWithMultiplay();
				}

				if (qsf::Random::getRandomChance(0.5f))
				{
					AudioHelper::playGangsterKickVersion1(mKickAudioProxy, getEntity(), qsf::Time::fromSeconds(2.0f));
					mKickAudioProxy.synchronizeWithMultiplay();
				}
				else
				{
					AudioHelper::playGangsterKickVersion2(mKickAudioProxy, getEntity(), qsf::Time::fromSeconds(2.0f));
					mKickAudioProxy.synchronizeWithMultiplay();
				}

				break;
			}

			case weapon::BASEBALLBAT:
			{
				playAnimationWithOffset(attackerEntity, animationHelper.getAnimationFightBaseballBat01Loop(), blendTime, startOffset, endOffset);
				break;
			}

			case weapon::KNIFE:
			{
				playAnimationWithOffset(attackerEntity, animationHelper.getAnimationFightKnife01Loop(), blendTime, startOffset, endOffset);
				break;
			}

			case weapon::SWORD:
			{
				playAnimationWithOffset(attackerEntity, animationHelper.getAnimationFightSword01Loop(), blendTime, startOffset, endOffset);
				break;
			}

			case weapon::TORCH:
			{
				playAnimationWithOffset(attackerEntity, animationHelper.getAnimationFightTorch01Loop(), blendTime, startOffset, endOffset);
				break;
			}

			default:
				// Either wrong weapon type equipped (should be impossible) or "WEAPON_NO_FIGHT" is equipped. I.e. the opponent will not do anything (e.g. when he was just defeated)
				break;
		}

		qsf::Time maxPlayingTime(qsf::Time::fromSeconds(maxPlayTime));
		AnimationHelper defenderAnimationHelper(targetEntity);
		// The defense animation depends on the weapon the defender currently uses. Careful, we reusing the animation helper here
		switch (targetWeapon)
		{
			case weapon::FIST:
			{
				playAnimationWithOffset(targetEntity, defenderAnimationHelper.getAnimationFistFight02(), blendTime, startOffset, endOffset, maxPlayingTime);

				// Play audio
				if (qsf::Random::getRandomChance(0.5f))
				{
					AudioHelper::playGangsterFistVersion1(mAudioProxy, getEntity(), qsf::Time::fromSeconds(0.5f));
					mAudioProxy.synchronizeWithMultiplay();
				}
				else
				{
					AudioHelper::playGangsterFistVersion2(mAudioProxy, getEntity(), qsf::Time::fromSeconds(0.5f));
					mAudioProxy.synchronizeWithMultiplay();
				}
				break;
			}

			case weapon::BASEBALLBAT:
			{
				playAnimationWithOffset(targetEntity, defenderAnimationHelper.getAnimationFightBaseballBat02(), blendTime, startOffset, endOffset, maxPlayingTime);
				break;
			}

			case weapon::KNIFE:
			{
				playAnimationWithOffset(targetEntity, defenderAnimationHelper.getAnimationFightKnife02(), blendTime, startOffset, endOffset, maxPlayingTime);
				break;
			}

			case weapon::TORCH:
			{
				playAnimationWithOffset(targetEntity, defenderAnimationHelper.getAnimationFightTorch02(), blendTime, startOffset, endOffset, maxPlayingTime);
				break;
			}

			case weapon::SWORD:
			{
				playAnimationWithOffset(targetEntity, defenderAnimationHelper.getAnimationFightSword02(), blendTime, startOffset, endOffset, maxPlayingTime);
				break;
			}

			default:
				// Either wrong weapon type equipped (should be impossible) or "WEAPON_NO_FIGHT" is equipped. I.e. the opponent will not do anything (e.g. when he was just defeated)
				break;
		}
	}

	std::string CloseQuartersCombatAction::getDebugWeaponName(weapon::Weapon weapon) const
	{
		switch (weapon)
		{
			case weapon::FIST:
				return "Fists";

			case weapon::BASEBALLBAT:
				return "Baseball bat";

			case weapon::KNIFE:
				return "Knife";

			case weapon::SWORD:
				return "Sword";

			case weapon::TORCH:
				return "Torch";

			default:
				return ""; // Should not happen
		}
	}

	float CloseQuartersCombatAction::getDefaultWeaponDamage(weapon::Weapon weapon) const
	{
		switch (weapon)
		{
			case weapon::FIST:
				return 0.20f;	// Lower life by 20% when fists are used

			case weapon::BASEBALLBAT:
				return 0.33f;	// Lower life by 33% when BBB is used

			case weapon::KNIFE:
				return 0.60f;	// Lower life by 60% when knife is used

			case weapon::SWORD:
			case weapon::TORCH:
				return 0.0f;	// No damage, because we have no medieval injured solution

			default:
				return 0.10f;	// Lower life by 10% in all other cases
		}
	}

	qsf::StringHash CloseQuartersCombatAction::getInjuryCausedByWeapon(weapon::Weapon weapon) const
	{
		std::vector<qsf::StringHash> possibleInjuries;

		switch (weapon)
		{
			// Fist and baseball bat can cause the same injuries
			case weapon::FIST:	// Fall-through by design
			case weapon::BASEBALLBAT:
				possibleInjuries.push_back(injury::BROKEN_ARM);
				possibleInjuries.push_back(injury::BROKEN_LEG);
				possibleInjuries.push_back(injury::BROKEN_SKULL);
				possibleInjuries.push_back(injury::INNER_BLEEDING);
				possibleInjuries.push_back(injury::BRAIN_BLEEDING);
				possibleInjuries.push_back(injury::HEAD_BODY_LACERATION);
				break;

			case weapon::KNIFE:
				possibleInjuries.push_back(injury::STAB_WOUND_BODY_INNER_BLEEDING);
				possibleInjuries.push_back(injury::STAB_WOUND_LUNG);
				possibleInjuries.push_back(injury::STAB_WOUND_BODY);
				break;

			default:
				break;
		}

		// Check for valid injuries
		QSF_CHECK(!possibleInjuries.empty(), "No injuries found that could have been caused by that weapon!", return qsf::getUninitialized<qsf::StringHash>());

		// Pick a random one
		return qsf::Random::getAnyOf(possibleInjuries);
	}

	bool CloseQuartersCombatAction::damageEntity(HealthComponent& healthComponent, float damage, qsf::Entity& damgeSourceEntity)
	{
		// Lower target's health energy only when not the limit is reached
		const float currentHealthPercentage = healthComponent.getHealthEnergy() / healthComponent.getMaximumHealthEnergy();
		if (currentHealthPercentage > mLowerHealthLimitPercentage)
		{
			// Calculate damage
			const float amountDamage = healthComponent.getMaximumHealthEnergy() * damage;

			// Make damage (sender is attacker)
			healthComponent.receiveDamage(amountDamage, damagetype::OTHER, damgeSourceEntity.getComponent<qsf::TransformComponent>());
		}
		return EntityHelper(healthComponent.getEntity()).isPersonInjured();
	}

	void CloseQuartersCombatAction::playAnimationWithOffset(qsf::Entity& entity, std::string animationPfath, qsf::Time blendTime, float startOffset, float endOffset, const boost::optional<qsf::Time> maxPlayTime)
	{
		PlayAnimationAction& playAnimationAction = entity.getComponent<qsf::ActionComponent>()->pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
		playAnimationAction.init(animationPfath, true, false, false, blendTime, maxPlayTime);
		playAnimationAction.setAnimationOffsetRange(startOffset, endOffset);
	}

	void CloseQuartersCombatAction::playAnimationStartByWeapon(qsf::Entity& entity, weapon::Weapon weaponType)
	{
		std::string animationPath;
		AnimationHelper animationHelper(entity);
		switch (weaponType)
		{
			case weapon::FIST:
				animationPath = animationHelper.getAnimationFistFight01Start();
				break;

			case weapon::BASEBALLBAT:
				animationPath = animationHelper.getAnimationFightBaseballBat01Start();
				break;

			case weapon::KNIFE:
				animationPath = animationHelper.getAnimationFightKnife01Start();
				break;

			case weapon::TORCH:
				animationPath = animationHelper.getAnimationFightTorch01Start();
				break;

			case weapon::SWORD:
				animationPath = animationHelper.getAnimationFightSword01Start();
				break;

			default:
				// Nothing to do in this case
				break;
		}
		if (!animationPath.empty())
		{
			// Push the animation
			entity.getComponentSafe<qsf::ActionComponent>().pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationPath, true);
		}
	}

	void CloseQuartersCombatAction::playAnimationEndByWeapon(qsf::Entity& entity, weapon::Weapon weaponType)
	{
		std::string animationPath;
		AnimationHelper animationHelper(entity);
		switch (weaponType)
		{
			case weapon::FIST:
				animationPath = animationHelper.getAnimationFistFight01End();
				break;

			case weapon::BASEBALLBAT:
				animationPath = animationHelper.getAnimationFightBaseballBat01End();
				break;

			case weapon::KNIFE:
				animationPath = animationHelper.getAnimationFightKnife01End();
				break;

			case weapon::TORCH:
				animationPath = animationHelper.getAnimationFightTorch01End();
				break;

			case weapon::SWORD:
				animationPath = animationHelper.getAnimationFightSword01End();
				break;

			default:
				// Nothing to do in this case
				break;
		}
		if (!animationPath.empty())
		{
			// Push the animation
			entity.getComponentSafe<qsf::ActionComponent>().pushAction<PlayAnimationAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(animationPath, true);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
