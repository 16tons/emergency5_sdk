// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/base/StopGuidePersonAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/equipment/DropEquipmentAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/audio/AudioHelper.h"
#include "em5/command/police/ArrestGangsterCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/time/Time.h>
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
	const qsf::NamedIdentifier ArrestGangsterAction::ACTION_ID = "em5::ArrestGangsterAction";

	const uint64 ArrestGangsterAction::FIGHT_ROUNDS = 3;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void ArrestGangsterAction::setArrestedParameters(qsf::Entity& arrestedEntity, bool arrested)
	{
		PersonComponent& targetPersonComponent = arrestedEntity.getComponentSafe<PersonComponent>();	// Usually we have tones of checks before reaching this

		qsf::GameLogicComponent* gameLogicComponentTarget = arrestedEntity.getComponent<qsf::GameLogicComponent>();
		GangsterBaseLogic* gangsterLogic = nullptr;
		if (nullptr != gameLogicComponentTarget)
		{
			// Only get the logic
			gangsterLogic = gameLogicComponentTarget->getGameLogic<GangsterBaseLogic>();
		}

		// Set the gangster's state to ARRESTED
		if (nullptr != gangsterLogic)
		{
			GangsterBaseLogic::State gangsterState = (arrested ? GangsterBaseLogic::STATE_ARRESTED : GangsterBaseLogic::STATE_NORMAL);
			gangsterLogic->setCurrentState(gangsterState);
		}
		else
		{
			// Ensure gangster game counter is reduced even if he has no logic
			targetPersonComponent.setGangsterFlag(!arrested);
		}

		// Add handcuffs to gangster person
		if (arrested)
		{
			EntityHelper(arrestedEntity).showEquipment(equipment::POLICE_HANDCUFFS);
		}
		else
		{
			EntityHelper(arrestedEntity).deleteActiveEquipment();
		}
		targetPersonComponent.setPersonFlags(PersonComponent::FLAG_HANDCUFFED, arrested);

		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_ARREST_STARTED, arrestedEntity.getId()));

		// Make sure the gangster movement is correct (stop moving for arrested)
		MoveToDespawnComponent* moveToDespawnComponent = arrestedEntity.getComponent<MoveToDespawnComponent>();
		if (moveToDespawnComponent != nullptr)
		{
			moveToDespawnComponent->setActive(!arrested);
		}
		qsf::ai::NavigationComponent* navigationComponent = arrestedEntity.getComponent<qsf::ai::NavigationComponent>();
		if (navigationComponent != nullptr)
		{
			navigationComponent->setActive(!arrested);
		}
	}

	glm::vec3 ArrestGangsterAction::getArrestOffset()
	{
		return glm::vec3(0.129f, 0.f, 0.658f);
	}

	glm::vec3 ArrestGangsterAction::getFightingOffset()
	{
		return glm::vec3(0.f, 0.f, 0.8f);
	}

	glm::vec3 ArrestGangsterAction::getFightingLongRangeOffset()
	{
		return glm::vec3(0.f, 0.f, 1.3f);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ArrestGangsterAction::ArrestGangsterAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	ArrestGangsterAction::~ArrestGangsterAction()
	{
		// Nothing here
	}

	void ArrestGangsterAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 ArrestGangsterAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ArrestGangsterAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ArrestGangsterAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		mTargetEntity = getMap().getEntityById(mTargetEntityId);
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getArrestGangsterDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetEntity.getSafe(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		ArrestGangsterCommand* arrestGangsterCommand = static_cast<ArrestGangsterCommand*>(QSFGAME_COMMAND.getCommandManager().getById<ArrestGangsterCommand>(ArrestGangsterCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != arrestGangsterCommand, "ArrestGangsterCommand::onStartup(): Could not find arrest gangster command instance", return false);
		if (!(arrestGangsterCommand->checkCallerWithoutPriority(*commandContext.mCaller) && arrestGangsterCommand->checkContext(commandContext)))
			return false;

		// Check for healthy gangster (injured gangsters need to be treated instead of arrested)
		if (EntityHelper(mTargetEntity.getSafe()).isPersonInjured())
		{
			return false;
		}

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
			return false;

		if (!ReserveLogic::checkReservation(callerEntity, mTargetEntityId))
			return false;

		// Reserve the entity
		ReserveLogic::createReservation<ReserveArrestGangsterLogic>(mTargetEntity.getSafe(), getEntityId());

		ReserveLogic::createReservation<ReserveArrestGangsterLogic>(callerEntity, mTargetEntityId);

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(mTargetEntity.getSafe(), callerEntity))
		{
			return false;
		}

		return true;
	}

	void ArrestGangsterAction::onShutdown()
	{
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
		ReserveLogic::deleteOwnReservation(getMap(), getEntityId(), mTargetEntityId);

		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());

		if (!mTargetEntity.valid())
			return; // Nothing to do here...

		// Remove the hacky wait action that prevents the gangster from doing something while being arrested
		qsf::ActionComponent& targetActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		qsf::WaitAction* waitAction = targetActionComponent.getAction<qsf::WaitAction>();
		if (nullptr != waitAction)
		{
			// Found the WaitAction, remove it!
			targetActionComponent.removeAction(*waitAction);
		}

		// If interrupted while arresting, free the gangster again
		if ((mCurrentState == STATE_ARRESTING || mCurrentState == STATE_WAIT_FOR_ANIMATIONS) && mTargetEntity.valid())
		{
			StopGuidePersonAction::freeGangster(getEntity(), *mTargetEntity);
		}
	}

	qsf::action::Result ArrestGangsterAction::updateAction(const qsf::Clock&)
	{
		// Get the caller entity
		qsf::Entity& callerEntity = getEntity();

		if (!mTargetEntity.valid())
			return qsf::action::RESULT_DONE;

		// Get the game logic component
		const qsf::GameLogicComponent* gameLogicComponent = callerEntity.getComponent<qsf::GameLogicComponent>();
		QSF_CHECK(nullptr != gameLogicComponent, "The EM5 arrest gangster action was unable to retrieve the game logic component", return qsf::action::RESULT_DONE);

		// Get the person component
		PersonComponent* targetPersonComponent = mTargetEntity->getComponent<PersonComponent>();
		QSF_CHECK(nullptr != targetPersonComponent, "The EM5 arrest gangster action was unable to retrieve the person component", return qsf::action::RESULT_DONE);

		// Gangster is already "wearing" handcuffs, probably an arresting action was canceled (TODO(mz): would be better to make that impossible, but not trivial, so not today...)
		// TODO(fw): Does this code make sense any more? There is handling of this situation in "onShutdown", or isn't it?
		if (mCurrentState <= STATE_DONE && targetPersonComponent->getPersonFlags(PersonComponent::FLAG_HANDCUFFED))
		{
			ensureRelativePositions(true);
			AnimationHelper(*mTargetEntity).clearAnimation();
			mCurrentState = STATE_DONE;
		}

		// Check for healthy gangster (injured gangsters need to be treated instead of arrested)
		if (EntityHelper(mTargetEntity.getSafe()).isPersonInjured())
		{
			return qsf::action::RESULT_DONE;
		}

		qsf::GameLogicComponent* gameLogicComponentTarget = mTargetEntity->getComponent<qsf::GameLogicComponent>();
		GangsterBaseLogic* gangsterLogic = nullptr;
		if (nullptr != gameLogicComponentTarget)
		{
			gangsterLogic = gameLogicComponentTarget->getGameLogic<GangsterBaseLogic>();
		}

		// If the target person is already stunned, just arrest it.
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				if (!UnitHelper(getEntity()).isMedievalGuardsman())
				{
					// Don't fight with handcuffs in hand (for Guardsman, but with swords)
					EntityHelper(getEntity()).deleteActiveEquipment();
				}

				// Stop the gangster's current action and let the two persons face each other
				getComponent().pushAction<TurnToAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(mTargetEntityId);
				qsf::ActionComponent& gangsterActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

				// Gangster should not have any other concerns besides this arrest attempt right now.
				if (!targetPersonComponent->getPersonFlags(PersonComponent::FLAG_STUNNED))
				{
					// Gangster is not stunned, so it is okay to clear the plan
					// When gangster is stunned, this would remove the BeStunnedAction (very bad!)
					// On the other hand, clearing the action plan is not necessary when gangster is stunned
					gangsterActionComponent.clearPlan();
				}
				// This WaitAction prevents the gangster from escaping the arresting WITHOUT having defeated the policeman
				gangsterActionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromHours(999.0));

				// Whatever happens, the gangster drops his firearms or thrown weapons, if any.
				if (EntityHelper(mTargetEntity.getSafe()).checkIsActiveEquipment(equipment::GANGSTER_PISTOL))
				{
					// Pistol equipped. Drop it.
					gangsterActionComponent.pushAction<DropEquipmentAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init();

					// Get weapon from gangster logic
					if (nullptr != gangsterLogic)
					{
						if (gangsterLogic->getEquippedWeaponTypes().isSet(weapon::PISTOL))
						{
							// Remove weapon from gangster logic
							gangsterLogic->getEquippedWeaponTypes().clear(weapon::PISTOL);
						}
						else
						{
							QSF_CHECK(false, "Pistol was active (and shown) weapon, but not equipped!", QSF_REACT_NONE);
						}
					}
				}

				if (targetPersonComponent->getPersonFlags(PersonComponent::FLAG_STUNNED))
				{
					// Stunned gangster can easily be arrested without resistance
					mCurrentState = STATE_ARRESTING;
				}
				else if (nullptr != gangsterLogic && gangsterLogic->getResistArrest())
				{
					// This gangster won't give up without a fight
					mCurrentState = STATE_FIGHT;
				}
				else
				{
					// Not a gangster at all, or he won't resist, so he can easily be arrested
					mCurrentState = STATE_ARRESTING;
				}

				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FIGHT:
			{
				// Put both in perfect fighting positions
				ensureRelativePositions(false);

				// Default weapon for gangsters when they resist arresting is 'fists'. If they equip any close quarters weapon, they use those.
				weapon::Weapon attackerWeapon = getWeaponFromGangster();

				qsf::ActionComponent& targetActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

				// Support special weapon for medieval guardsman. All other policeman starts fighting with fists
				weapon::Weapon defenseWeapon = (UnitHelper(callerEntity).isMedievalGuardsman() ? weapon::SWORD : weapon::FIST);

				targetActionComponent.pushAction<CloseQuartersCombatAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(callerEntity, attackerWeapon, defenseWeapon, FIGHT_ROUNDS, 0.2f, 0.0f);

				mCurrentState = STATE_ARRESTING;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_ARRESTING:
			{
				ensureRelativePositions(true);

				// Play animation for police man
				AnimationHelper animationHelper(getEntity());
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationPoliceArrestGangster03(), true);
				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationPoliceArrestGangster01(), true);

				// Clear action plan of target entity
				qsf::ActionComponent& targetActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				targetActionComponent.clearPlan();
				targetActionComponent.pushAction<PlayAnimationAction>(action::BLOCKING).init(animationHelper.getAnimationGangsterGettingArrested01(), true);
				targetActionComponent.pushAction<PlayAnimationAction>(action::BLOCKING).init(animationHelper.getAnimationGangsterGettingArrested03(), true);

				setArrestedParameters(mTargetEntity.getSafe(), true);

				if (UnitHelper(getEntity()).isMedievalGuardsman())
				{
					// Medieval solution for arresting
					// Fight is over, guardsman don`t need the sword anymore, gangster don`t get handcuffs
					EntityHelper(getEntity()).deleteActiveEquipment();
					EntityHelper(mTargetEntity.getSafe()).deleteActiveEquipment();
				}

				// Play audio
				AudioHelper::playPoliceHandcuffs(mAudioProxy, getEntity(), qsf::Time::fromSeconds(4.0f));
				mAudioProxy.synchronizeWithMultiplay();

				// Emit a message telling the event that the gangster is being arrested. This makes sure the gangster does not go on with his crimes.
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_ARREST, mTargetEntityId, getEntityId()));

				mCurrentState = STATE_WAIT_FOR_ANIMATIONS;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_ANIMATIONS:
				mCurrentState = STATE_DONE;
				// Fallthrough by design

			case STATE_DONE:
			{
				// Enqueue a GuidePersonAction
				getComponent().pushAction<GuidePersonAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(mTargetEntityId, GuidePersonAction::ARREST, true);

				return qsf::action::RESULT_DONE;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	weapon::Weapon ArrestGangsterAction::getWeaponFromGangster() const
	{
		weapon::Weapon gangsterWeapon = weapon::FIST;

		// Get Weapon from gangster logic
		qsf::GameLogicComponent* gameLogicComponent = mTargetEntity->getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			GangsterBaseLogic* gangsterLogic = gameLogicComponent->getGameLogic<GangsterBaseLogic>();
			if (nullptr != gangsterLogic)
			{
				// Find (close quarters) weapons the gangster can attack with
				std::vector<weapon::Weapon> possibleWeapons;

				if (gangsterLogic->getEquippedWeaponTypes().isSet(weapon::BASEBALLBAT))
					possibleWeapons.push_back(weapon::BASEBALLBAT);

				if (gangsterLogic->getEquippedWeaponTypes().isSet(weapon::KNIFE))
					possibleWeapons.push_back(weapon::KNIFE);

				if (gangsterLogic->getEquippedWeaponTypes().isSet(weapon::TORCH))
					possibleWeapons.push_back(weapon::TORCH);

				if (!possibleWeapons.empty())
				{
					// Pick a random one
					gangsterWeapon = qsf::Random::getAnyOf(possibleWeapons);
				}
			}
		}

		return gangsterWeapon;
	}

	void ArrestGangsterAction::ensureRelativePositions(bool arrestSituation)
	{
		qsf::TransformComponent& transformComponentUnit = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		// Turn gangster away from police (aka same rotation as police)
		qsf::TransformComponent& transformComponent = mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		transformComponent.setRotation(transformComponentUnit.getRotation());

		glm::vec3 localOffset = getArrestOffset();
		if (!arrestSituation)
		{
			// Turn 180° to the unit
			transformComponent.setRotation(transformComponentUnit.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>()));
			localOffset = getFightingOffset();

			if (UnitHelper(getEntity()).isMedievalGuardsman())
			{
				localOffset = getFightingLongRangeOffset();
			}
		}

		// Position gangster at correct offset
		const glm::vec3 offsetWorld = transformComponentUnit.getTransform().vec3DirectionLocalToWorld(localOffset);
		transformComponent.setPosition(transformComponentUnit.getPosition() - offsetWorld);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
