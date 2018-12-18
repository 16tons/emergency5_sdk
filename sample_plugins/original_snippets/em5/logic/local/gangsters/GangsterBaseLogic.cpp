// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/CloseQuartersCombatAction.h"
#include "em5/action/base/HuntAndShootAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/base/SetFireAction.h"
#include "em5/action/base/ShootAction.h"
#include "em5/action/gangster/ThrowWeaponAction.h"
#include "em5/action/gangster/FearGangsterAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/escape/EscapeCarLongAction.h"
#include "em5/action/move/escape/EscapeCarShortAction.h"
#include "em5/action/move/escape/EscapeFootLongAction.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/move/escape/EscapeHidingFootAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/police/DogAttackGangsterAction.h"
#include "em5/action/police/UsePepperSprayAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/AttackPersonTargetPointProvider.h"
#include "em5/game/targetpoint/GotoObjectTargetPointProvider.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/game/targetpoint/ShootPersonTargetPointProvider.h"
#include "em5/gangster/GangsterType.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/goal/AchieveLineOfSightGoal.h>
#include <qsf_ai/perception/AspectComponent.h>

#include <qsf_logic/targetpoint/TargetPoint.h>
#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/physics/collision/BulletCollisionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used definitions                              ]
	//[-------------------------------------------------------]
	namespace
	{
		const uint32 NUMBER_OF_TRIES_TO_REACH_WAYPOINT = 5;
	}


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{

		weapon::Weapon chooseBestWeapon(const qsf::EnumBitmask<uint16, weapon::Weapon>& availableWeaponSet, const std::vector<weapon::Weapon>& priorizedWeapons)
		{
			for (weapon::Weapon weapon : priorizedWeapons)
			{
				if (availableWeaponSet.isSet(weapon))
				{
					return weapon;
				}
			}
			return weapon::NOFIGHT;
		}

		bool isTargetTryingToArrestGangster(qsf::Entity& targetEntity, qsf::Entity& gangsterEntity)
		{
			qsf::ActionComponent* targetActionComponent = targetEntity.getComponent<qsf::ActionComponent>();
			if (nullptr != targetActionComponent)
			{
				ArrestGangsterAction* targetArrestGangsterAction = targetActionComponent->getAction<ArrestGangsterAction>();
				if (nullptr != targetArrestGangsterAction)
				{
					return (targetArrestGangsterAction->getTargetEntityId() == gangsterEntity.getId());
				}
			}
			return false;
		}

	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterBaseLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterBaseLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterBaseLogic::PossibleTarget::PossibleTarget() :
		mEntity(nullptr),
		mEventTag(qsf::getUninitialized<uint32>()),
		mPriority(0),
		mDistance(0.0f)
	{
		// Nothing here
	}

	GangsterBaseLogic::PossibleTarget::PossibleTarget(qsf::Entity& entity, const qsf::StringHash& eventTag, uint32 priority, float distance) :
		mEntity(&entity),
		mEventTag(eventTag),
		mPriority(priority),
		mDistance(distance)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterBaseLogic::GangsterBaseLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	GangsterBaseLogic::GangsterBaseLogic(uint32 gameLogicId) :
		GameLogic(gameLogicId),
		mGangsterType(nullptr),
		mLastGangsterLogicId(qsf::getUninitialized<uint32>()),
		mSetGangsterFlagOnStartup(true),
		mAlwaysShowWeapon(false),
		mUseOldSpottedUnitsLogic(false),
		mConsiderAllUnitsAsTarget(false),
		mChainReaction(false),
		mIdleReaction(AutoReaction::NONE),
		mAutoTargetReaction(AutoReaction::ATTACK),
		mAutoAggression(false),
		mAutoAggroRange(12.0f),
		mAutoDeAggroRange(15.0f),
		mCelebrateWinTime(qsf::Time::ZERO),
		mPatrolOnWaypoints(false),
		mNeedsUpdateInContainer(false),
		mCurrentState(STATE_NORMAL),
		mResistArrest(true),
		mCurrentAutoTargetPriority(0)
	{
		mWeaponConfigurationForRifle.mWeaponType = em5::weapon::RIFLE;
	}

	GangsterBaseLogic::~GangsterBaseLogic()
	{
		// Nothing here
	}

	void GangsterBaseLogic::applyGangsterType(const GangsterType& gangsterType)
	{
		// Cleanup old gangster type
		if (nullptr != mGangsterType)
		{
			removeGangsterType();
		}

		// Set new gangster type
		mGangsterType = &gangsterType;

		// Copy over property values
		mEquippedWeaponTypes = gangsterType.getWeapons();
		mAlwaysShowWeapon = gangsterType.getAlwaysShowWeapon();
		mResistArrest = gangsterType.getResistArrest();
		mCelebrateWinTime.setSeconds(gangsterType.getCelebrateWinTime());

		mAutoAggroRange = gangsterType.getAutoAggroRange();
		mAutoDeAggroRange = gangsterType.getAutoDeAggroRange();
		mAutoTargetPrioritiesByEventTag = gangsterType.getAutoTargets();
		mAutoTargetReaction = gangsterType.getAutoTargetReaction();
		mIdleReaction = gangsterType.getIdleReaction();
		mChainReaction = gangsterType.getChainReaction();

		// Auto-aggression is activated if there are any targets set
		mAutoAggression = !mAutoTargetPrioritiesByEventTag.empty();

		// Movement speed
		if (gangsterType.getMovementSpeed() > 0.0f)
		{
			qsf::ai::NavigationComponent* navigationComponent = getEntity()->getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				navigationComponent->setNativeMaxForwardSpeed(gangsterType.getMovementSpeed());
			}
		}

		// Copy weapon configuration settings
		mWeaponConfigurationForPistol = QSF_MAKE_REF(gangsterType.getConfigurationForWeapon(weapon::PISTOL));
		mWeaponConfigurationForRifle = QSF_MAKE_REF(gangsterType.getConfigurationForWeapon(weapon::RIFLE));

		startupInternal();

		// Add the gangster type name as event tag to the gangster
		//  -> This way other gangsters can choose him as target by his type
		qsf::game::EventTagComponent& eventTagComponent = QSF_MAKE_REF(getEntity()).getOrCreateComponentSafe<qsf::game::EventTagComponent>();
		eventTagComponent.addEventTag(gangsterType.getName());
	}

	std::string GangsterBaseLogic::getDebugGangsterName() const
	{
		return campClassName() + (mGangsterType != nullptr ? (std::string(": ") + mGangsterType->getName()) : "");
	}

	bool GangsterBaseLogic::getAsGangsterFlagged() const
	{
		PersonComponent* personComponent = getEntitySafe().getComponent<PersonComponent>();
		if (personComponent)
		{
			return personComponent->getGangsterFlag();
		}
		return false;
	}

	void GangsterBaseLogic::setAsGangsterFlagged(bool isGangster)
	{
		PersonComponent* personComponent = getEntitySafe().getComponent<PersonComponent>();
		if (personComponent)
		{
			personComponent->setGangsterFlag(isGangster);
		}
	}

	weapon::WeaponConfiguration* GangsterBaseLogic::getConfigurationForWeapon(weapon::Weapon weapon)
	{
		if (weapon == weapon::PISTOL)
		{
			return &mWeaponConfigurationForPistol;
		}
		else if (weapon == weapon::RIFLE)
		{
			return &mWeaponConfigurationForRifle;
		}
		return nullptr;
	}

	const weapon::WeaponConfiguration* GangsterBaseLogic::getConfigurationForWeapon(weapon::Weapon weapon) const
	{
		if (weapon == weapon::PISTOL)
		{
			return &mWeaponConfigurationForPistol;
		}
		else if (weapon == weapon::RIFLE)
		{
			return &mWeaponConfigurationForRifle;
		}
		return nullptr;
	}

	void GangsterBaseLogic::setCurrentState(GangsterBaseLogic::State newState)
	{
		if (mCurrentState != newState)
		{
			if (newState == STATE_ARRESTED)
				setAsGangsterFlagged(false);	// Gangster game counter must be reduced by this

			mCurrentState = newState;
		}
	}

	void GangsterBaseLogic::addAutoTargetEventTag(const qsf::StringHash& eventTag, uint32 priority)
	{
		mAutoTargetPrioritiesByEventTag.emplace(eventTag, priority);
	}

	void GangsterBaseLogic::setChainReaction(bool active)
	{
		if (mChainReaction != active)
		{
			mChainReaction = active;
			if (active)
			{
				mPersonGetsShootMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_PERSON_GETS_SHOOT), boost::bind(&GangsterBaseLogic::onPersonGetsShoot, this, _1));
			}
			else
			{
				mPersonGetsShootMessageProxy.unregister();
			}
		}
	}


	void GangsterBaseLogic::attack(qsf::Entity& targetEntity)
	{
		attackTarget(targetEntity);
	}

	void GangsterBaseLogic::escape(EscapeType escapeType)
	{
		qsf::Entity* gangster = getEntity();
		QSF_CHECK(nullptr != gangster, "Gangster does not exist!", return);
		QSF_CHECK(!EntityHelper(*gangster).isPersonInjured(), "Gangster is injured and thus should not do anything!", return);

		// Apply the following hack only for EM2017 gangsters (yes, this is itself a hack)
		if (nullptr != mGangsterType && (mGangsterType->getName() == "ME202Terrorist" || mGangsterType->getName() == "WeapondealUnarmed"))
		{
			// HACK: Reset movement speed
			//  -> For some reason, the native forward speed gets multiplied with "run" movement mode's speed multiplicator 3.0f.
			//     So, to get a gangster with the usual run speed, better start with 1.0f as speed value.
			qsf::ai::NavigationComponent* navigationComponent = getEntity()->getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				navigationComponent->setNativeMaxForwardSpeed(1.0f);
			}
		}

		qsf::ActionComponent& actionComponent = getEntity()->getComponentSafe<qsf::ActionComponent>();
		switch (escapeType)
		{
			case ESCAPE_FOOT_LONG:
			{
				actionComponent.pushAction<EscapeFootLongAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_FLEES, getEntityId()));
				break;
			}

			case ESCAPE_FOOT_SHORT:
			{
				actionComponent.pushAction<EscapeFootShortAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_FLEES, getEntityId()));
				break;
			}

			case ESCAPE_HIDE_FOOT:
			{
				// TODO(tl): Seems to not be used anywhere
				actionComponent.pushAction<EscapeHidingFootAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_FLEES, getEntityId()));
				break;
			}

			case ESCAPE_CAR_LONG:
			{
				actionComponent.pushAction<EscapeCarLongAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_FLEES, getEntityId()));
				break;
			}

			case ESCAPE_CAR_SHORT:
			{
				actionComponent.pushAction<EscapeCarShortAction>(action::COMMAND_LOW, qsf::action::APPEND_TO_BACK).init(mEscapeTargetTag);
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_FLEES, getEntityId()));
				break;
			}

			default:
			{
				QSF_ERROR("Illegal Escape Type!", QSF_REACT_NONE);
			}
		}
	}

	void GangsterBaseLogic::setLastGangsterLogic(uint32 lastLogicId)
	{
		mLastGangsterLogicId = lastLogicId;
	}

	qsf::Entity* GangsterBaseLogic::getCurrentAutoAggresionTarget()
	{
		return mCurrentAutoTarget.get();
	}

	// TODO(mk) Rename this function to delete current weapon
	void GangsterBaseLogic::hideCurrentWeapon()
	{
		EntityHelper(getEntitySafe()).deleteActiveEquipment();
	}

	void GangsterBaseLogic::showWeapon(const std::string& equipmentPrefab)
	{
		EntityHelper(getEntitySafe()).showEquipment(equipmentPrefab);

		// Force the right idle animations for the current weapon
		PersonComponent* personComponent = getEntity()->getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->clearAnimation();
		}
	}

	void GangsterBaseLogic::clearWaypoints()
	{
		mWaypoints.clear();
	}

	void GangsterBaseLogic::addWaypoint(qsf::Entity& waypointEntity, uint32 targetPointProviderId, uint32 movementModeId)
	{
		mWaypoints.emplace_back();
		Waypoint& waypoint  = mWaypoints.back();

		waypoint.mEntity = waypointEntity;
		waypoint.mTargetPointProviderId = qsf::isInitialized(targetPointProviderId) ? targetPointProviderId : GotoObjectTargetPointProvider::TARGET_POINT_ID;
		waypoint.mMovementModeId = movementModeId;
		waypoint.mMovementModeId = qsf::isInitialized(waypoint.mMovementModeId) ? waypoint.mMovementModeId : (mPatrolOnWaypoints ? MovementModes::MOVEMENT_MODE_DEFAULT : MovementModes::MOVEMENT_MODE_RUN);
		waypoint.mTriesToReachLeft = NUMBER_OF_TRIES_TO_REACH_WAYPOINT;
		waypoint.mPosition = EntityHelper(waypointEntity).getPosition();
	}

	void GangsterBaseLogic::addWaypoint(glm::vec3 waypointPosition, uint32 movementModeId)
	{
		mWaypoints.emplace_back();
		Waypoint& waypoint  = mWaypoints.back();

		waypoint.mMovementModeId = movementModeId;
		waypoint.mMovementModeId = qsf::isInitialized(waypoint.mMovementModeId) ? waypoint.mMovementModeId : (mPatrolOnWaypoints ? MovementModes::MOVEMENT_MODE_DEFAULT : MovementModes::MOVEMENT_MODE_RUN);
		waypoint.mTriesToReachLeft = NUMBER_OF_TRIES_TO_REACH_WAYPOINT;
		waypoint.mPosition = waypointPosition;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterBaseLogic::onStartup()
	{
		qsf::Entity* entity = getEntity();
		if (nullptr == entity)
			return false;

		startupInternal();

		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&GangsterBaseLogic::updateSimulation, this, _1), qsf::jobs::JobConfiguration());
		return true;
	}

	void GangsterBaseLogic::onShutdown()
	{
		removeGangsterType();

		mSimulationJobProxy.unregister();
		mPersonGetsShootMessageProxy.unregister();
		mGunshotMessageProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	bool GangsterBaseLogic::changeLogic(const qsf::StringHash& newLogicId)
	{
		PersonComponent* personComponent = getEntitySafe().getComponent<PersonComponent>();
		if (personComponent)
		{
			return (nullptr != personComponent->changeGangsterLogic(newLogicId));
		}
		return false;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GangsterBaseLogic::startupInternal()
	{
		// Randomly filter out some weapons where two are set that are comparable
		filterEquippedWeapons();

		// Remove all (civil) equipments/weapons from the attached entity
		hideCurrentWeapon();

		if (mAlwaysShowWeapon)
		{
			if (mEquippedWeaponTypes.isSet(weapon::RIFLE))
			{
				showWeapon(equipment::POLICE_SNIPERRIFLE);
			}
			else if (mEquippedWeaponTypes.isSet(weapon::PISTOL))
			{
				showWeapon(equipment::GANGSTER_PISTOL);
			}
			else if (mEquippedWeaponTypes.isSet(weapon::BASEBALLBAT))
			{
				showWeapon(equipment::BASEBALL_BAT);
			}
			else if (mEquippedWeaponTypes.isSet(weapon::KNIFE))
			{
				showWeapon(equipment::KNIFE);
			}
			else if (mEquippedWeaponTypes.isSet(weapon::TORCH))
			{
				showWeapon(equipment::FANATIC_TORCH);
			}

			// Otherwise, do nothing
			// TODO(ks) what about ThrownWeapons?
		}

		mTransformComponent = getEntity()->getOrCreateComponentSafe<qsf::TransformComponent>();
		mActionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Avoid having behavior of civilians (eg taking pictures at landmarks)
		getEntitySafe().destroyComponent<qsf::ai::AspectComponent>();

		// Flag person as gangster
		if (mSetGangsterFlagOnStartup)
		{
			setAsGangsterFlagged(true);
		}

		// Force a chainpulling switch
		bool chainPullingBackup = mChainReaction;
		setChainReaction(!chainPullingBackup);
		setChainReaction(chainPullingBackup);

		mGunshotMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_GUNSHOT, getEntityId()), boost::bind(&GangsterBaseLogic::onGunShot, this, _1));
	}

	void GangsterBaseLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Only simulate gangsters when they are not (yet) arrested
		if (STATE_ARRESTED == getCurrentState())
		{
			return;
		}

		qsf::Entity& entity = getEntitySafe();

		// Don't simulate injured gangsters
		EntityHelper gangsterEntityHelper(entity);
		if (gangsterEntityHelper.isPersonInjured())
		{
			// We lost the fight
			if (mCurrentAutoTarget.valid())
			{
				loseCurrentAutoTarget();
			}
			mPersonGetsShootMessageProxy.unregister();
			return;
		}

		// No updates when hidden (including inside buildings/vehicles), except if explicitly requested
		if (qsf::game::HiddenComponent::isHidden(entity) && !mNeedsUpdateInContainer)
		{
			return;
		}

		// Call the specialized class update method
		onSimulationUpdate(jobArguments);

		// Check if he does not have anything to do
		if (mActionComponent.getSafe().getPlan().isEmpty())
		{
			updateMoveOnWaypoints();

			// Still idle?
			if (mActionComponent->getPlan().isEmpty())
			{
				// Tell the sub-class implementation that the gangster is bored
				onIdle();

				// Still idle now?
				if (mActionComponent->getPlan().isEmpty())
				{
					if (mIdleReaction == AutoReaction::ESCAPE_LONG)
					{
						// Run away
						escape(ESCAPE_FOOT_LONG);
					}
					else if (mIdleReaction == AutoReaction::ESCAPE_SHORT)
					{
						// Run away
						escape(ESCAPE_FOOT_SHORT);
					}
				}
			}
		}

		// TODO(mk) Replace all use cases for updateLegacyAutoAggression() with the new gangster type system in updateAutoAggression()
		updateLegacyAutoAggression();

		updateAutoAggression();

		// If escaping, look out for policemen around
		updateCatchedByPolice();

		// Give all civil persons around a little fear
		fearNearbyCivilists();
	}

	void GangsterBaseLogic::updateMoveOnWaypoints()
	{
		if (mWaypoints.empty())
			return;

		// Reached the next waypoint now?
		const Waypoint& waypoint = mWaypoints.front();

		const bool invalidWaypoint = (!waypoint.mEntity.valid() && waypoint.mPosition == glm::vec3());
		if (invalidWaypoint)
		{
			// Skip waypoint, entity got invalid and we have no position to go to
			mWaypoints.pop_front();
		}
		else
		{
			bool targetReached = false;
			if (waypoint.mEntity.valid())
			{
				// Check if target point was reached, or if we failed -- for entities with target point
				targetReached = EM5_GAME.getTargetPointManager().isTargetPointReached(waypoint.mTargetPointProviderId, getEntitySafe(), *waypoint.mEntity);
			}
			else
			{
				// Check if target point was reached for points only (or in case we lose the entity)
				targetReached = (qsf::game::DistanceHelper::get2dDistance(waypoint.mPosition, getEntitySafe()) <= 2.0f);
			}

			if (waypoint.mTriesToReachLeft == 0)
			{
				targetReached = true;
			}

			if (targetReached)
			{
				// Make a copy of the waypoint, before we destroy it in the deque
				const Waypoint waypointCopy = waypoint;

				// Reached the waypoint
				mWaypoints.pop_front();

				// When patrolling, add the waypoint to the end again
				if (mPatrolOnWaypoints)
				{
					mWaypoints.push_back(waypointCopy);
					mWaypoints.back().mTriesToReachLeft = NUMBER_OF_TRIES_TO_REACH_WAYPOINT;
				}

				// Message
				const uint64 waypointEntityId = waypointCopy.mEntity.valid() ? waypointCopy.mEntity->getId() : qsf::getUninitialized<uint64>();
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_GANGSTER_REACHED_WAYPOINT, getEntityId(), waypointEntityId));

				// Callback
				onWaypointReached(waypointCopy);
			}
		}

		if (!mWaypoints.empty())
		{
			// On to the next waypoint
			Waypoint& waypoint = mWaypoints.front();

			// Go to the next waypoint (or try the current one once more), only move if target has a minimun distance (to avoid pushing the same points over and over again
			// TODO(fw): Make action priority configurable -- we need something below COMMAND_LOW in case auto-aggression has higher prio than the waypoint (which is mostly the case)
			if (waypoint.mEntity.valid())
			{
				if (qsf::game::DistanceHelper::get2dDistance(*waypoint.mEntity, getEntitySafe()) > 0.2f)
				{
					mActionComponent->pushAction<MoveAction>(action::AUTOMATISM_HIGH).init(new qsf::ai::ReachObjectGoal(getEntitySafe(), *waypoint.mEntity, waypoint.mTargetPointProviderId), waypoint.mMovementModeId);
				}
			}
			else
			{
				if (qsf::game::DistanceHelper::get2dDistance(waypoint.mPosition, getEntitySafe()) > 0.2f)
				{
					mActionComponent->pushAction<MoveAction>(action::AUTOMATISM_HIGH).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(waypoint.mPosition, false, 0.0f, 1.0f)), waypoint.mMovementModeId);
				}
			}

			if (waypoint.mTriesToReachLeft > 0)
				--waypoint.mTriesToReachLeft;

			// Make sure the gangster has no forced animation any more (e.g. for gangsters that are standing around with a cool animation at first)
			EntityHelper(getEntitySafe()).disableMeshAnimationTestComponent();
		}
	}

	void GangsterBaseLogic::updateLegacyAutoAggression()
	{
		if (!mUseOldSpottedUnitsLogic)
			return;

		// Legacy stuff

		// Get all commandable components inside the radius around the gangster
		std::vector<CommandableComponent*> commandableComponents;
		qsf::Entity* container = EntityHelper(getEntitySafe()).getContainerEntity();
		qsf::Entity& positionEntity = (nullptr != container) ? *container : getEntitySafe();
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle<CommandableComponent>(positionEntity.getOrCreateComponent<qsf::TransformComponent>()->getPosition(), ActionRangeSpecsGroup::getInstanceSafe().getGangsterSearchDistance(), commandableComponents);

		// TODO(fw): Filling of this vector should better be done by "findPossibleTargets" -- required the right configuration in "mAutoTargetPrioritiesByEventTag"
		std::vector<SpottedUnit> sightedPoliceUnits;
		for (const CommandableComponent* commandableComponent : commandableComponents)
		{
			// Check if the unit is a police unit
			qsf::StringHash unitType(commandableComponent->getUnitType());

			const bool isAnyPoliceUnit =
				unitType == CommandableComponent::UNITTAG_POLICE_DRONE
				|| unitType == CommandableComponent::UNITTAG_POLICE_HELI
				|| unitType == CommandableComponent::UNITTAG_POLICE_MTW
				|| unitType == CommandableComponent::UNITTAG_POLICE_POLICE
				|| unitType == CommandableComponent::UNITTAG_POLICE_SEK
				|| unitType == CommandableComponent::UNITTAG_POLICE_SEK_HELI
				|| unitType == CommandableComponent::UNITTAG_POLICE_ROBOT
				|| unitType == CommandableComponent::UNITTAG_POLICE_STW;

			if (mConsiderAllUnitsAsTarget || isAnyPoliceUnit)
			{
				// We found a target unit

				// NOTE: We do NOT push back injured, dead, burning or destroyed units!
				qsf::Entity& current = commandableComponent->getEntity();
				EntityHelper targetEntityHelper(current);

				// Target needs to be valid gangster event target
				if (targetEntityHelper.isValidGangsterEventTarget(getEntitySafe(), false))
				{
					// We found a vehicle or a person that is not inside a vehicle
					sightedPoliceUnits.emplace_back(current, unitType, 1, 0.0f);	// TODO(fw): Measure distance
				}
			}
		}

		// This if-block needs to be the final part of this function, since this object may have been replaced by another logic!
		if (sightedPoliceUnits.empty())
		{
			// No units in range
			onNoUnitsSpotted();
		}
		else
		{
			// There are units in sight.
			onUnitsSpotted(sightedPoliceUnits);
		}
	}

	void GangsterBaseLogic::updateAutoAggression()
	{
		// Here check list of targets and active autoAggression (so we can turn it temporary off)
		if (mAutoAggression && !mAutoTargetPrioritiesByEventTag.empty())
		{
			qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

			// Is there a current target?
			if (mCurrentAutoTarget.valid())
			{
				EntityHelper targetEntityHelper(*mCurrentAutoTarget);
				bool stillAttacking = false;

				// Check for current action priority
				if (actionComponent.getCurrentPriority() < action::COMMAND_STD)
				{
					// Check if fight was won
					if (targetEntityHelper.isPersonInjured() || targetEntityHelper.isBurning())
					{
						// Cheering
						if (mCelebrateWinTime > qsf::Time::ZERO)
						{
							actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_STD, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntitySafe()).getAnimationCheer(), true, true, false, qsf::Time::fromSeconds(0.5f), mCelebrateWinTime);
						}
					}
					else
					{
						// Check if outside of deaggro range
						const float distance = qsf::game::DistanceHelper::get2dDistance(getEntitySafe(), *mCurrentAutoTarget);
						if (distance < mAutoDeAggroRange && nullptr == targetEntityHelper.getContainerEntity())
						{
							// Next attack (needed for ranged weapons)
							if (mAutoTargetReaction == AutoReaction::ATTACK)
							{
								// Attack target now
								if (attackTarget(*mCurrentAutoTarget))
								{
									stillAttacking = true;
								}
							}
						}
					}

					if (!stillAttacking)
					{
						loseCurrentAutoTarget();
					}
				}
				else
				{
					// Check if outside of deaggro range
					const float distance = qsf::game::DistanceHelper::get2dDistance(getEntitySafe(), *mCurrentAutoTarget);
					if (distance > mAutoDeAggroRange || nullptr != targetEntityHelper.getContainerEntity())
					{
						// Let him go
						loseCurrentAutoTarget();
						actionComponent.clearPlan();
					}
				}
			}
			else
			{
				// Check for current action priority
				if (actionComponent.getCurrentPriority() < action::COMMAND_LOW)
				{
					const float searchRadius = mAutoAggroRange;

					qsf::StdVectorScratchBuffer<PossibleTarget> possibleTargets;
					findPossibleTargets(possibleTargets, searchRadius, mAutoTargetPrioritiesByEventTag);

					if (!possibleTargets.empty())
					{
						autoAggressionReactOnTargets(possibleTargets);
					}
				}
			}
		}
	}

	void GangsterBaseLogic::findPossibleTargets(std::vector<PossibleTarget>& outTargets, float radius, const PriorityByEventTagMap& targetPrioritiesByEventTag)
	{
		qsf::Entity& entity = getEntitySafe();
		glm::vec3 center;
		{
			EntityHelper gangsterEntityHelper(entity);
			qsf::Entity* container = gangsterEntityHelper.getContainerEntity();
			qsf::Entity& positionEntity = (nullptr != container) ? *container : entity;
			center = positionEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
		}

		qsf::StdVectorScratchBuffer<qsf::game::EventTagComponent*> eventTagComponents;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle<qsf::game::EventTagComponent>(center, radius, eventTagComponents);

		// We search for event tag of the target. Units also have eventTags like "Police Policeman". So we can have individual targets like tagged event-entities or ordered player units
		for (const qsf::game::EventTagComponent* eventTagComponent : eventTagComponents)
		{
			PossibleTarget possibleTarget;
			for (const qsf::NamedIdentifier& tag : eventTagComponent->getEventTags())
			{
				const auto iterator = targetPrioritiesByEventTag.find(tag);
				if (iterator != targetPrioritiesByEventTag.end())
				{
					// Initial value of "possibleTarget.mPriority" is 0, priorities are at least 1, so this is definitely true for the first match
					if (iterator->second > possibleTarget.mPriority)
					{
						possibleTarget.mEntity = &eventTagComponent->getEntity();
						possibleTarget.mEventTag = tag;
						possibleTarget.mPriority = iterator->second;
						possibleTarget.mDistance = qsf::game::DistanceHelper::get2dDistance(entity, *possibleTarget.mEntity);

						// No break by intent
					}
				}
			}

			if (nullptr != possibleTarget.mEntity)
			{
				// Do some checks on the entity
				if (checkTargetEntity(*possibleTarget.mEntity))
				{
					// Any kind of reservation will stop us from attacking a unit
					if (ReserveLogic::checkReservation(*possibleTarget.mEntity, getEntityId(), ReserveLogic::NO_UNIT))
					{
						// Add as possible target
						outTargets.emplace_back(possibleTarget);
					}
				}
			}
		}

		// Sort by priorities
		std::sort(outTargets.begin(), outTargets.end(), [](const PossibleTarget& a, const PossibleTarget& b) {
			return (a.mPriority != b.mPriority) ? (a.mPriority > b.mPriority) : (a.mDistance < b.mDistance);
		});
	}

	bool GangsterBaseLogic::checkTargetEntity(qsf::Entity& entity)
	{
		EntityHelper entityHelper(entity);

		if (entityHelper.checkForEventIdComponent() && (entityHelper.getFreeplayEvent() != EntityHelper(*getEntity()).getFreeplayEvent()))
			return false;

		if (!entityHelper.isInMapBoundaries())
			return false;

		if (entityHelper.isEntityPerson())
		{
			if (entityHelper.isPersonInjured())
				return false;

			if (entityHelper.isArrested())
				return false;

			if (entityHelper.isPullingPerson())
				return false;

			if (entityHelper.isGuidingPerson())
				return false;

			if (nullptr != entityHelper.getContainerEntity())
				return false;
		}
		else if (entityHelper.isVehicle())
		{
			if (!entityHelper.canBeSetOnFireNow() && !entityHelper.isHelicopter())
				return false;

			if (entityHelper.isVehicleDestroyed())
				return false;
		}
		else if (entityHelper.isEntityBuilding())
		{
			if (!entityHelper.canBeSetOnFireNow())
				return false;

			if (entityHelper.isBuildingDamaged())
				return false;
		}

		return true;
	}

	void GangsterBaseLogic::setCurrentAutoTarget(const PossibleTarget& newTarget)
	{
		mCurrentAutoTarget = newTarget.mEntity;
		mCurrentAutoTargetPriority = newTarget.mPriority;
	}

	void GangsterBaseLogic::loseCurrentAutoTarget()
	{
		if (mCurrentAutoTarget.valid())
		{
			ReserveLogic::deleteOwnReservation(getMap(), mCurrentAutoTarget->getId(), getEntityId());
			mCurrentAutoTarget.clear();
			mCurrentAutoTargetPriority = 0;	// invalid
		}
	}

	bool GangsterBaseLogic::isCurrentlyFleeing() const
	{
		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty())
		{
			// Check current action
			const uint32 currentActionTypeId = actionComponent->getCurrentAction()->getTypeId();

			if (EscapeFootLongAction::ACTION_ID == currentActionTypeId ||
				EscapeFootShortAction::ACTION_ID == currentActionTypeId ||
				EscapeCarLongAction::ACTION_ID == currentActionTypeId ||
				EscapeCarShortAction::ACTION_ID == currentActionTypeId ||
				EscapeHidingFootAction::ACTION_ID == currentActionTypeId)
			{
				// Current action is fleeing action
				return true;
			}
		}

		return false;
	}

	bool GangsterBaseLogic::isCurrentlyFighting() const
	{
		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty())
		{
			// There are actions enqueued

			uint32 currentActionTypeId = actionComponent->getCurrentAction()->getTypeId();

			if (CloseQuartersCombatAction::ACTION_ID == currentActionTypeId ||
				ShootAction::ACTION_ID == currentActionTypeId ||
				ThrowWeaponAction::ACTION_ID == currentActionTypeId)
			{
				// Current action is fleeing action
				return true;
			}
		}

		return false;
	}

	void GangsterBaseLogic::fearNearbyCivilists()
	{
		qsf::Entity& entity = getEntitySafe();

		// Do nothing if gangster stands still
		const qsf::MovableComponent* movableComponent = entity.getComponent<qsf::MovableComponent>();
		if (movableComponent == nullptr || !movableComponent->isMoving())
			return;

		// Get all persons components inside the radius around the gangster
		qsf::StdVectorScratchBuffer<PersonComponent*> personComponents;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle<PersonComponent>(EntityHelper(entity).getPosition(), ActionRangeSpecsGroup::getInstanceSafe().getGangsterFearDistance(), personComponents);

		for (const PersonComponent* personComponent : personComponents)
		{
			qsf::Entity& personEntity = personComponent->getEntity();
			EntityHelper entityHelper(personEntity);
			if (entityHelper.isCivilPerson() && entityHelper.isCivilPersonInValidState())
			{
				if (entityHelper.hasEnabledMeshAnimationTestComponent())
					continue;	// Has currently better animations, keep them

				qsf::ActionComponent& actionComponent = personEntity.getOrCreateComponentSafe<qsf::ActionComponent>();
				if (actionComponent.getCurrentPriority() >= action::DISTRACTION_DANGER)
					continue;

				if (nullptr != actionComponent.getAction<FearGangsterAction>())
					continue;

				actionComponent.pushAction<FearGangsterAction>(action::DISTRACTION_DANGER, qsf::action::INSERT_AT_FRONT).init(entity);
			}
		}
	}

	void GangsterBaseLogic::updateCatchedByPolice()
	{
		// TODO(fw): Add a state for escaping
		EscapeAction* escapeAction = mActionComponent->getAction<EscapeFootLongAction>();
		escapeAction = (escapeAction != nullptr) ? escapeAction : mActionComponent->getAction<EscapeFootShortAction>();
		MoveAction* moveAction = (escapeAction != nullptr) ? nullptr : mActionComponent->getAction<MoveAction>();
		if (nullptr != escapeAction || nullptr != moveAction)
		{
			static const PriorityByEventTagMap targetPriorities =
			{
				std::make_pair(CommandableComponent::UNITTAG_POLICE_POLICE.getHash(), 1),
				std::make_pair(CommandableComponent::UNITTAG_POLICE_SEK.getHash(), 1),
				std::make_pair(CommandableComponent::UNITTAG_POLICE_POLICEDOG_LEADER.getHash(), 1),
				std::make_pair(CommandableComponent::UNITTAG_POLICE_POLICEDOG.getHash(), 1),
				std::make_pair(CommandableComponent::UNITTAG_POLICE_GUARDSMAN_HISTORIC.getHash(), 1)
			};
			const float radius = 7.5f;

			qsf::StdVectorScratchBuffer<PossibleTarget> possibleTargets;
			findPossibleTargets(possibleTargets, radius, targetPriorities);

			for (PossibleTarget possibletarget : possibleTargets)
			{
				if (nullptr == possibletarget.mEntity)
					continue;

				// Check if target is attacking the gangster. tell the escape action to slowdown movement to give player units a chance to catch them
				qsf::ActionComponent& actionComponent = possibletarget.mEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
				DogAttackGangsterAction* dogAttackGangsterAction = actionComponent.getAction<DogAttackGangsterAction>();
				ArrestGangsterAction* arrestGangsterAction = actionComponent.getAction<ArrestGangsterAction>();
				UsePepperSprayAction* usePepperSprayAction = actionComponent.getAction<UsePepperSprayAction>();
				if ((nullptr != arrestGangsterAction && arrestGangsterAction->getTargetEntityId() == getEntityId()) ||
					(nullptr != dogAttackGangsterAction && dogAttackGangsterAction->getTargetEntityId() == getEntityId()) ||
					(nullptr != usePepperSprayAction && usePepperSprayAction->getTargetEntityId() == getEntityId()))
				{
					if (nullptr != escapeAction)
					{
						escapeAction->catched();
					}
					else
					{
						if (nullptr == mActionComponent->getAction<qsf::WaitAction>())
						{
							// In case we are not fleeing, just wait (only once, avoid pushing the wait action on stack more than one)
							mActionComponent->pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.0f));
						}
					}
					break;
				}
			}
		}
	}

	void GangsterBaseLogic::onPersonGetsShoot(const qsf::MessageParameters& parameters)
	{
		if (!mAutoAggression || mAutoTargetPrioritiesByEventTag.empty())
			return;	// We can nothing do to react, ignore

		if (mCurrentAutoTarget.valid())
			return;	// We already have a target, ignore

		uint64 targetEntityId = qsf::getUninitialized<uint64>();
		parameters.getParameter("EntityId", targetEntityId);
		uint64 attackerEntityId = qsf::getUninitialized<uint64>();
		parameters.getParameter("AttackerId", attackerEntityId);

		if (targetEntityId == getEntityId())
			return;	// I was hit, ignore

		if (attackerEntityId == getEntityId())
			return;	// I hit a target, ignore

		qsf::Entity* targetEntity = getMap().getEntityById(targetEntityId);
		if (nullptr == targetEntity)
			return; // Error

		qsf::Entity* attackerEntity = getMap().getEntityById(attackerEntityId);
		if (nullptr == attackerEntity)
			return;	// Error

		EntityHelper targetEntityHelper(targetEntity);
		if (targetEntityHelper.isGangsterPerson())
		{
			float distanceTarget = qsf::game::DistanceHelper::get2dDistance(getEntitySafe(), *targetEntity);
			float distanceAttacker = qsf::game::DistanceHelper::get2dDistance(getEntitySafe(), *attackerEntity);
			if (distanceTarget < mAutoAggroRange && distanceAttacker < mAutoDeAggroRange)
			{
				// Check if attacker is valid auto target
				qsf::game::EventTagComponent* eventTagComponent = attackerEntity->getComponent<qsf::game::EventTagComponent>();
				for (const qsf::NamedIdentifier& tag : eventTagComponent->getEventTags())
				{
					const auto iterator = mAutoTargetPrioritiesByEventTag.find(tag);
					if (iterator != mAutoTargetPrioritiesByEventTag.end())
					{
						// Target is a gangster, check if he is in range and we can help him
						qsf::StdVectorScratchBuffer<PossibleTarget> possibleTargets;

						PossibleTarget possibleTarget;
						possibleTarget.mEntity = attackerEntity;
						possibleTarget.mEventTag = tag;
						possibleTarget.mDistance = distanceAttacker;
						possibleTargets.emplace_back(possibleTarget);

						// Start auto aggression reaction on the attacker of your "friend"
						autoAggressionReactOnTargets(possibleTargets);

						break; // Do this only once
					}
				}
			}
		}
	}

	void GangsterBaseLogic::onGunShot(const qsf::MessageParameters& parameters)
	{
		if (!mAutoAggression || mAutoTargetPrioritiesByEventTag.empty())
			return;	// We can nothing do to react, ignore

		uint64 attackerEntityId = qsf::getUninitialized<uint64>();
		parameters.getParameter("ShooteeId", attackerEntityId);

		if (mCurrentAutoTarget.valid() && attackerEntityId != mCurrentAutoTarget->getId())
			return;	// Error, we should shoot on our auto target

		qsf::Entity* attackerEntity = getMap().getEntityById(attackerEntityId);
		if (nullptr == attackerEntity)
			return;	// Error

		// Search for new auto attack target with higher priority
		qsf::StdVectorScratchBuffer<PossibleTarget> possibleTargets;
		findPossibleTargets(possibleTargets, mAutoAggroRange, mAutoTargetPrioritiesByEventTag);

		if (!possibleTargets.empty())
		{
			if (possibleTargets[0].mPriority > mCurrentAutoTargetPriority)
			{
				autoAggressionReactOnTargets(possibleTargets);
			}
		}
	}

	void GangsterBaseLogic::removeGangsterType()
	{
		if (nullptr != mGangsterType)
		{
			// Remove the gangster type name as event tag
			qsf::game::EventTagComponent* eventTagComponent = QSF_MAKE_REF(getEntity()).getComponent<qsf::game::EventTagComponent>();
			if (nullptr != eventTagComponent)
			{
				eventTagComponent->removeEventTag(mGangsterType->getId());
			}

			mGangsterType = nullptr;
		}
	}

	void GangsterBaseLogic::autoAggressionReactOnTargets(std::vector<PossibleTarget>& possibleTargets)
	{
		if (mAutoTargetReaction == AutoReaction::ATTACK)
		{
			// Just begin with the first one, as list is sorted
			for (const PossibleTarget& target : possibleTargets)
			{
				qsf::Entity& targetEntity = *target.mEntity;
				setCurrentAutoTarget(target);

				// Attack target now
				if (attackTarget(targetEntity))
				{
					// Reserve if it is a close combat
					qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
					if (nullptr != actionComponent.getAction<CloseQuartersCombatAction>())
					{
						ReserveLogic::createReservation<ReserveAttackedByGangsterLogic>(targetEntity, getEntityId());
					}

					// If waypoint movement gets interrupted by attacking, fill up remaining tries by one
					if (!mWaypoints.empty())
					{
						Waypoint& waypoint = mWaypoints.front();
						if (waypoint.mTriesToReachLeft < NUMBER_OF_TRIES_TO_REACH_WAYPOINT)
							++waypoint.mTriesToReachLeft;
					}

					break;
				}

				// Try the next target
			}
		}
		else if (mAutoTargetReaction == AutoReaction::ESCAPE_LONG)
		{
			// Run away
			escape(ESCAPE_FOOT_LONG);
		}
		else if (mAutoTargetReaction == AutoReaction::ESCAPE_SHORT)
		{
			// Run away
			escape(ESCAPE_FOOT_SHORT);
		}
	}

	void GangsterBaseLogic::filterEquippedWeapons()
	{
		// Only one of: knife and baseball bat
		if (mEquippedWeaponTypes.isSet(weapon::BASEBALLBAT) && mEquippedWeaponTypes.isSet(weapon::KNIFE))
		{
			if (qsf::Random::getRandomChance(0.5f))
				mEquippedWeaponTypes.clear(weapon::BASEBALLBAT);
			else
				mEquippedWeaponTypes.clear(weapon::KNIFE);
		}

		// Only one of: pistol and rifle
		if (mEquippedWeaponTypes.isSet(weapon::PISTOL) && mEquippedWeaponTypes.isSet(weapon::RIFLE))
		{
			if (qsf::Random::getRandomChance(0.5f))
				mEquippedWeaponTypes.clear(weapon::PISTOL);
			else
				mEquippedWeaponTypes.clear(weapon::RIFLE);
		}

		// Only one of: molotov and stone
		if (mEquippedWeaponTypes.isSet(weapon::MOLOTOV) && mEquippedWeaponTypes.isSet(weapon::STONE))
		{
			if (qsf::Random::getRandomChance(0.5f))
				mEquippedWeaponTypes.clear(weapon::MOLOTOV);
			else
				mEquippedWeaponTypes.clear(weapon::STONE);
		}
	}

	bool GangsterBaseLogic::attackTarget(qsf::Entity& targetEntity)
	{
		qsf::Entity& entity = getEntitySafe();
		qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

		// First some sanity checks
		QSF_CHECK(!EntityHelper(entity).isPersonInjured(), "Gangster is injured and thus should not do anything", return false);

		EntityHelper targetEntityHelper(targetEntity);
		if (targetEntityHelper.isEntityPerson())
		{
			// Choose a good weapon
			static const std::vector<weapon::Weapon> closeCombatWeaponsPriorized = { weapon::KNIFE, weapon::BASEBALLBAT, weapon::FIST };
			const weapon::Weapon bestCloseCombatWeapon = chooseBestWeapon(mEquippedWeaponTypes, closeCombatWeaponsPriorized);

			static const std::vector<weapon::Weapon> rangedWeaponsPriorized = { weapon::RIFLE, weapon::PISTOL, weapon::MOLOTOV, weapon::STONE };
			const weapon::Weapon bestRangedWeapon = chooseBestWeapon(mEquippedWeaponTypes, rangedWeaponsPriorized);

			// Close combat or ranged attack?
			const weapon::Weapon usedWeapon = (bestRangedWeapon != weapon::NOFIGHT) ? bestRangedWeapon : bestCloseCombatWeapon;
			if (usedWeapon == weapon::NOFIGHT)
				return false;

			// Go for it
			return attackWithWeapon(targetEntity, usedWeapon);
		}
		else if (targetEntityHelper.isVehicle())
		{
			// Choose a good ranged weapon
			static const std::vector<weapon::Weapon> rangedWeaponsPriorized = { weapon::RIFLE, weapon::PISTOL, weapon::MOLOTOV, weapon::STONE };
			const weapon::Weapon usedWeapon = chooseBestWeapon(mEquippedWeaponTypes, rangedWeaponsPriorized);
			if (usedWeapon == weapon::NOFIGHT)
				return false;

			// Go for it
			return attackWithWeapon(targetEntity, usedWeapon);
		}
		else if (targetEntityHelper.isEntityBuilding())
		{
			// Let arsonists be immune to fire
			entity.getOrCreateComponentSafe<FireReceiverComponent>().setActive(false);

			// Cheer at the same position we started all
			glm::vec3 cheerPosition = entity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

			// Turn to vehicle or building and set it on fire
			actionComponent.clearPlan();
			actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH).init(new qsf::ai::ReachObjectGoal(entity, targetEntity, PullPersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
			actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH).init(targetEntity.getId());
			actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init(AnimationHelper(entity).getAnimationManipulateObjectKneeling(), true, false, false, qsf::Time::fromSeconds(0.5f));
			actionComponent.pushAction<SetFireAction>(action::COMMAND_HIGH).init(targetEntity);
			actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH).init(new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(cheerPosition, false, 0.0f, 2.0f)), MovementModes::MOVEMENT_MODE_RUN);
			actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH).init(targetEntity.getId());

			return true;
		}

		return false;
	}

	bool GangsterBaseLogic::attackWithWeapon(qsf::Entity& targetEntity, weapon::Weapon usedWeapon)
	{
		QSF_CHECK(mTransformComponent.valid(), "Gangster transform component is invalid", return false);

		qsf::Entity& entity = getEntitySafe();
		qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

		// Get distance to target
		const float squaredDistanceBetweenGangsterAndTarget = qsf::game::DistanceHelper::getSquaredDistance(mTransformComponent->getPosition(), targetEntity);

		switch (weapon::getWeaponCategory(usedWeapon))
		{
			case weapon::Category::CLOSE_COMBAT:
			{
				if (isTargetTryingToArrestGangster(targetEntity, entity))
				{
					// Our target is already attacking us, no need to move
					actionComponent.clearPlan();
					actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH).init(targetEntity.getId());
					return true;
				}

				weapon::Weapon targetWeapon = weapon::FIST;		// TODO(fw): Target should select his weapon just as we do

				// Move to target, then start a close quarters combat
				actionComponent.clearPlan();
				actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH).init(new qsf::ai::ReachObjectGoal(entity, targetEntity, AttackPersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
				actionComponent.pushAction<CloseQuartersCombatAction>(action::COMMAND_HIGH).init(targetEntity, usedWeapon, targetWeapon);
				return true;
			}

			case weapon::Category::FIREARM:
			{
				const weapon::WeaponConfiguration* weaponConfiguration = getConfigurationForWeapon(usedWeapon);
				QSF_CHECK(nullptr != weaponConfiguration, "No weapon configuration for " << static_cast<int>(usedWeapon), QSF_REACT_THROW);

				if (usedWeapon == weapon::RIFLE)
				{
					showWeapon(equipment::POLICE_SNIPERRIFLE);
				}
				else if (usedWeapon == weapon::PISTOL)
				{
					showWeapon(equipment::GANGSTER_PISTOL);
				}

				actionComponent.clearPlan();
				actionComponent.pushAction<HuntAndShootAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(targetEntity, *weaponConfiguration);

				// But before that, move to target if it is too far way!
				if (squaredDistanceBetweenGangsterAndTarget > weaponConfiguration->mShootRange * weaponConfiguration->mShootRange)
				{
					actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(entity, targetEntity, ShootPersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
				}
				return true;
			}

			case weapon::Category::THROWABLE:
			{
				// Wait a short amount of time -- this is mainly done to avoid multiple gangsters throwing exactly synchronously
				actionComponent.pushAction<qsf::WaitAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(qsf::Random::getRandomFloat(0.0f, 0.5f)));

				// Throw weapon action
				actionComponent.pushAction<ThrowWeaponAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(targetEntity, usedWeapon);

				// No line-of-sight check here, as it makes problems only...
				//const short traceLineCollisionFlags = qsf::BulletCollisionComponent::mDefaultCollisionMask;
				//actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::AchieveLineOfSightGoal(entity, targetEntity, ShootPersonTargetPointProvider::TARGET_POINT_ID, traceLineCollisionFlags), MovementModes::MOVEMENT_MODE_RUN);

				// But before that, move to target if it is too far way!
				if (squaredDistanceBetweenGangsterAndTarget > 20.0f * 20.0f) // TODO(ks) Configurable!
				{
					actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(entity, targetEntity, ShootPersonTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
				}
				return true;
			}

			default:
				// This includes weapons that gangsters should not possess, like pepper spray and stun grenade
				QSF_ERROR("Unhandled weapon type " << usedWeapon, QSF_REACT_NONE);
				return false;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
