// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterKillerTaskforceLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	namespace detail
	{
		// TODO(ks) Maybe replace it with a configurable value
		// static const float ATTACK_TARGETS_RANGE = 20.0f;						///< How far away may targets be when gangster shall NOT pursue them (aka "Killer taskforce, fixed")	// TODO(co) Unused?
		static const float CHASE_VEHICLES_MAX_SQUARED_RANGE = 100.0f * 100.0f;	///< At what distance will even the "Killer taskforce, move" type stop chasing vehicles?
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterKillerTaskforceLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterKillerTaskforceLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterKillerTaskforceLogic::GangsterKillerTaskforceLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>()),
		mPursueTargets(false)
	{
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::BASEBALLBAT);
		mEquippedWeaponTypes.set(weapon::KNIFE);
		mEquippedWeaponTypes.set(weapon::FIST);
		mEquippedWeaponTypes.set(weapon::STONE);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterKillerTaskforceLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty() && !isCurrentlyFleeing())
		{
			// Gangster got work to do, but is NOT fleeing
			return;
		}

		// Check whether to pursue targets or not
		if (false == mPursueTargets)
		{
			// TODO(ks) Possibility to set "mPursueTargets" via setter? Is this needed?
			if (mEquippedWeaponTypes.isClear(weapon::PISTOL))
			{
				// Gangster does not have a firearm, so he has to pursue his targets...
				mPursueTargets = true;
			}
		}


		qsf::Entity* currentTarget = getMap().getEntityById(mCurrentTargetId);
		if (nullptr != currentTarget)
		{
			// The gangster has a registered target

			EntityHelper targetEntityHelper(*currentTarget);

			if (targetEntityHelper.isValidGangsterEventTarget(*getEntity(), false))
			{
				// And the target is neither injured, nor dead, nor burning, ...

				// Check whether the gangster is currently fighting
				if (!isCurrentlyFighting())
				{
					// Gangster is not fighting. Start a fight!
					attack(*currentTarget);
				}
			}
			else
			{
				// Reset target -> search new one
				mCurrentTargetId = qsf::getUninitialized<uint64>();
				return;
			}
		}
	}

	void GangsterKillerTaskforceLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		qsf::Entity* currentTarget = getMap().getEntityById(mCurrentTargetId);
		if (nullptr != currentTarget)
		{
			// The gangster already has a target he is going to attack
			return;
		}

		// The killer taskforce attacks all units in sight with the following descending priorities:
		// SEK unit -> police unit -> MTW -> STW -> (SEK heli) -> (police heli)

		// Copy vector and sort it according to this gangster's "preferences"
		std::vector<SpottedUnit> sortedPoliceUnits = sightedPoliceUnits;

		// Sorter-struct to sort the police units by priority
		struct PoliceUnitSorter
		{

			int policeUnitPriority(const GangsterBaseLogic::SpottedUnit& policeUnit)
			{
				// Lower number means higher priority, edit values if you want to change the order in which the gangster attacks them
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_ROBOT) // TODO(ks) Only when flag is set, 'cos this is needed for bombplanter event only
					return -1;
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK)
					return 0;
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_POLICE)
					return 1;
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_MTW)
					return 2;
				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_STW)
					return 3;

				HelicopterComponent* heliComp = policeUnit.mEntity->getComponent<HelicopterComponent>();
				if (nullptr != heliComp)
				{
					// This is a Helicopter
					if (heliComp->isFlying())
					{
						// Highest priority, because the gangster shall flee when a flying helicopter is in sight
						// This way we can keep track of that best
						return -5;
					}
				}

				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK_HELI)
					return 4;

				if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_HELI)
					return 5;

				// "Lowest" priority
				return qsf::getUninitialized<int>();
			}

			explicit PoliceUnitSorter(glm::vec3 gangsterPosition)
			{
				gangsterPos = gangsterPosition;
			}
			glm::vec3 gangsterPos;

			bool operator() (const SpottedUnit& lhs, const SpottedUnit& rhs)
			{
				int a = policeUnitPriority(lhs);
				int b = policeUnitPriority(rhs);

				if (a < b)
				{
					return true;
				}

				if (a == b)
				{
					// Same unit type: Compare distances and pick the one closest to the gangster

					const float distA = qsf::game::DistanceHelper::getDistance(gangsterPos, *lhs.mEntity);
					const float distB = qsf::game::DistanceHelper::getDistance(gangsterPos, *rhs.mEntity);

					// Distance-to-gangster comparison
					return distA < distB;
				}

				// b < a
				return false;
			}
		};

		std::sort(sortedPoliceUnits.begin(), sortedPoliceUnits.end(), PoliceUnitSorter(mTransformComponent->getPosition()));

		QSF_CHECK(!sortedPoliceUnits.empty(), "onUnitsSpotted() must not be called with an empty vector!", return);

		if (sortedPoliceUnits[0].mEventTag == CommandableComponent::UNITTAG_POLICE_HELI ||
			sortedPoliceUnits[0].mEventTag == CommandableComponent::UNITTAG_POLICE_SEK_HELI)
		{
			HelicopterComponent* heliComp = sortedPoliceUnits[0].mEntity->getComponent<HelicopterComponent>();
			if (nullptr != heliComp)
			{
				// Helicopter
				if (heliComp->isFlying())
				{
					// Do not mess with a flying helicopter
					if (!isCurrentlyFleeing())
					{
						escape(GangsterBaseLogic::ESCAPE_FOOT_SHORT);
						mCurrentTargetId = qsf::getUninitialized<uint64>();
					}
					return;
				}
			}
		}

		// Only attack vehicles when appropriate weapons
		const bool canAttackVehicles = mEquippedWeaponTypes.isSet(weapon::MOLOTOV) ||
									   mEquippedWeaponTypes.isSet(weapon::STONE) ||
									   mEquippedWeaponTypes.isSet(weapon::PISTOL);

		for (const SpottedUnit& candidate : sortedPoliceUnits)
		{
			qsf::Entity* candidateEntity = candidate.mEntity;

			// Check whether this potential target is valid (checks persons and vehicles)
			EntityHelper targetEntityHelper(*candidateEntity);
			if (!targetEntityHelper.isValidGangsterEventTarget(*getEntity(), false))
			{
				continue;
			}

			if (!canAttackVehicles)
			{
				// Assume only policemen and SEK men are no vehicles
				if (candidate.mEventTag != CommandableComponent::UNITTAG_POLICE_POLICE &&
					candidate.mEventTag != CommandableComponent::UNITTAG_POLICE_SEK)
				{
					// Assume we have found a vehicle (that we cannot attack)
					continue;
				}
			}

			// We found a new target
			mCurrentTargetId = candidateEntity->getId();
			return;
		}

		// No valid target found!
		mCurrentTargetId = qsf::getUninitialized<uint64>();
	}

	void GangsterKillerTaskforceLogic::onNoUnitsSpotted()
	{
		if (qsf::isUninitialized(mLastGangsterLogicId))
		{
			// No former gangster logic available
			return;
		}

		if (mPursueTargets)
		{
			// Gangster shall pursue his targets. Check whether a target exists
			qsf::Entity* currentTarget = getMap().getEntityById(mCurrentTargetId);
			if (nullptr != currentTarget)
			{
				// Current target is valid

				qsf::TransformComponent* transComp = currentTarget->getComponent<qsf::TransformComponent>();
				if (nullptr != transComp)
				{
					RoadVehicleComponent* vehicle = currentTarget->getComponent<RoadVehicleComponent>();
					if (nullptr != vehicle)
					{
						// Target is a vehicle
						if (glm::distance2(transComp->getPosition(), mTransformComponent->getPosition()) <= detail::CHASE_VEHICLES_MAX_SQUARED_RANGE)
						{
							// Still close enough, pursue it
							return;
						}
					}
					else
					{
						// Not a vehicle and thus shall always be pursued
						return;
					}
				}
			}
		}

		// All police units are out of sight, either escaped or injured, so we change the logic back
		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		// Change logic if gangster is idle or moving without police units in sight (This effectively prevents the gangster from chasing units)
		if (actionComponent->getPlan().isEmpty() || actionComponent->getCurrentAction()->getTypeId() == MoveAction::ACTION_ID)
		{
			// The gangster is in idle mode and thus shall change back to whatever gangster type he has been before
			changeLogic(qsf::StringHash(mLastGangsterLogicId));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
