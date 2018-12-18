// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterKillerAllLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>


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
		static const float ATTACK_TARGETS_RANGE = 30.0f;	///< How far away may targets be so that the gangster attacks them?
		static const float CHASE_MAX_RANGE = 100.0f;		///< At what distance will the gangster stop chasing his targets?
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterKillerAllLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterKillerAllLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterKillerAllLogic::GangsterKillerAllLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>())
	{
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::KNIFE);
		mEquippedWeaponTypes.set(weapon::FIST);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;
	}


	/*  Target Priorities (as defined by Mick)

	Priority - Description - Components

	1  - SEK-Spezialist zu Fuﬂ								(PC, CC)
	2  - Polizist zu Fuﬂ									(PC, CC)
	3  - Mannschaftstransportwagen							(VC, CC)
	4  - Streifenwagen										(VC, CC)
	5  - Zivilperson (!) zu Fuﬂ								(PC)
	6  - Feuerwehrmann zu Fuﬂ								(PC, CC)
	7  - Notarzt zu Fuﬂ										(PC, CC)
	8  - Sanit‰terteam zu Fuﬂ								(PC, CC)
	9  - andere Einheiten (Ingenieur, Taucher) zu Fuﬂ		(PC, CC)
	10 - Tanklˆschfahrzeug									(VC, CC)
	11 - Drehleiterfahrzeug									(VC, CC)
	12 - R¸stwagen											(VC, CC)
	13 - Notarzteinsatzfahrzeug								(VC, CC)
	14 - Rettungstransportwagen								(VC, CC)
	15 - Andere Einsatzfahrzeuge							(VC, CC)

	PC = PersonComponent
	VC = VehicleComponent
	CC = CommandableComponent

	*/

	int GangsterKillerAllLogic::getSquadPriority(const CommandableComponent* comp) const
	{
		QSF_CHECK(nullptr != comp, "CommandableComponent is invalid!", return qsf::getUninitialized<int>());

		uint32 unitType = comp->getUnitType();

		// Lower number means higher priority, edit values if you want to change the order in which the gangster attacks them
		if (unitType == CommandableComponent::UNITTAG_POLICE_SEK)
			return 1;
		if (unitType == CommandableComponent::UNITTAG_POLICE_POLICE)
			return 2;
		if (unitType == CommandableComponent::UNITTAG_POLICE_MTW)
			return 3;
		if (unitType == CommandableComponent::UNITTAG_POLICE_STW)
			return 4;

		// (_Walking_ Civil persons have priority 5)

		if (unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER ||
			unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC)
			return 6;
		if (unitType == CommandableComponent::UNITTAG_AMBULANCE_DOCTOR)
			return 7;
		if (unitType == CommandableComponent::UNITTAG_AMBULANCE_PARAMEDICS)
			return 8;

		// Other _walking_ units get priority 9 (engineer, diver, ...?)

		if (unitType == CommandableComponent::UNITTAG_TECH_ENGINEER)
			return 9;
		if (unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER)
			return 9;

		if (unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_TLF)
			return 10;
		if (unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_DLK)
			return 11;
		if (unitType == CommandableComponent::UNITTAG_FIREFIGHTERS_RW)
			return 12;
		if (unitType == CommandableComponent::UNITTAG_AMBULANCE_NEF)
			return 13;
		if (unitType == CommandableComponent::UNITTAG_AMBULANCE_RTW)
			return 14;

		HelicopterComponent* heliComp = comp->getEntity().getComponent<HelicopterComponent>();
		if (nullptr != heliComp)
		{
			// Helicopter
			if (heliComp->isFlying())
			{
				// Do not attack flying helicopter
				return qsf::getUninitialized<int>();
			}
			else
			{
				// Helicopter on the ground
				return 15;
			}

		}

		// All other commandable vehicles
		if (EntityHelper(comp->getEntity()).isVehicle())
			return 15;

		// Something bad - we ignore that
		return qsf::getUninitialized<int>();
	}

	qsf::Entity* GangsterKillerAllLogic::getNextTarget() const
	{
		qsf::Entity* gangster = getEntity();
		QSF_CHECK(nullptr != gangster, "Gangster does not exist!", return nullptr);

		qsf::TransformComponent* tc = gangster->getComponent<qsf::TransformComponent>();
		QSF_CHECK(nullptr != tc, "Gangster does not have a transform component!", return nullptr);

		// Gangster position
		const glm::vec3& gangsterPos = tc->getPosition();

		// Create a component map query to search for suitable targets
		qsf::ComponentMapQuery componentMapQuery(getMap());

		// Entity and priority
		std::vector<std::pair<qsf::Entity*, int> > validTargetsInRange;

		std::vector<CommandableComponent*> commandableCompsInRange;
		componentMapQuery.getInstancesInCircle<CommandableComponent>(gangsterPos, detail::ATTACK_TARGETS_RANGE, commandableCompsInRange);

		for (const CommandableComponent* commandableComponent : commandableCompsInRange)
		{
			qsf::Entity& targetEntity = commandableComponent->getEntity();

			// Do the usual tests (no injuries, deaths, no children, not part of an event, ...)
			if (!EntityHelper(targetEntity).isValidGangsterEventTarget(*gangster, false))
				continue;

			int squadPriority = getSquadPriority(commandableComponent);

			if (qsf::isUninitialized(squadPriority))
				continue;

			// Assume that every CommandableComponent in range, that is not injured etc., is valid
			validTargetsInRange.emplace_back(&targetEntity, squadPriority);
		}


		// ks: As requested by Mick on 8.8.14, civil persons are no longer targeted by this gangster type
		// (Explanation: It would be way too hard for the player to win the game - too many persons would be shot)
		/*
		// There might be civil persons that are in range
		std::vector<PersonComponent*> civilPersonsInRange;
		componentMapQuery.getInstancesInCircle<PersonComponent>(gangsterPos, detail::ATTACK_TARGETS_RANGE, civilPersonsInRange);

		for (const PersonComponent* personComponent : civilPersonsInRange)
		{
			qsf::Entity& targetEntity = personComponent->getEntity();

			if (!EntityHelper(targetEntity).isValidGangsterEventTarget(*gangster, true))
				continue;

			// Add civil person to valid targets (They always have priority 5)
			validTargetsInRange.emplace_back(&targetEntity, 5);
		}
		*/

		if (!validTargetsInRange.empty())
		{
			// Sorter-struct to sort the targets by priority
			struct TargetSorter
			{
				explicit TargetSorter(const glm::vec3& _gangsterPosition) :
					gangsterPosition(_gangsterPosition)
				{ }

				glm::vec3 gangsterPosition;

				bool operator() (const std::pair<qsf::Entity*, int>& lhs, const std::pair<qsf::Entity*, int>& rhs)
				{
					int a = lhs.second;
					int b = rhs.second;

					if (a < b)
					{
						// a got higher priority than b
						return true;
					}

					if (a == b)
					{
						// Same priority: Compare distances and pick the one closest to the gangster

						const float squaredDistanceA = qsf::game::DistanceHelper::getSquaredDistance(gangsterPosition, *lhs.first);
						const float squaredDistanceB = qsf::game::DistanceHelper::getSquaredDistance(gangsterPosition, *rhs.first);

						// Distance-to-gangster comparison
						return (squaredDistanceA < squaredDistanceB);
					}

					// b < a
					return false;
				}
			};

			return std::min_element(validTargetsInRange.begin(), validTargetsInRange.end(), TargetSorter(gangsterPos))->first;
		}

		// Nothing found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterKillerAllLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty())
		{
			// Gangster got work to do, let him finish that
			return;
		}

		qsf::Entity* currentTarget = nullptr;

		if (qsf::isUninitialized(mCurrentTargetId))
		{
			// Gangster needs a new target!
			currentTarget = getNextTarget();
		}
		else
		{
			// Stick with the current target 8as it is still valid
			currentTarget = getMap().getEntityById(mCurrentTargetId);
		}

		if (nullptr == currentTarget)
		{
			// No target in range. Become a Guard passage
			changeLogic(qsf::StringHash("em5::GangsterGuardPassageLogic"));
		}
		else
		{
			// The gangster has a registered target

			mCurrentTargetId = currentTarget->getId();
			EntityHelper targetEntityHelper(*currentTarget);
			bool isValid = targetEntityHelper.isValidGangsterEventTarget(*getEntity(), false);
			if (isValid && qsf::game::DistanceHelper::getDistance(*getEntity(), *currentTarget) < detail::CHASE_MAX_RANGE)
			{
				// And the target is neither injured, nor dead, nor burning, ...
				attack(*currentTarget);
			}
			else
			{
				// Reset target -> search new one
				mCurrentTargetId = qsf::getUninitialized<uint64>();
				return;
			}
		}
	}

	void GangsterKillerAllLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	void GangsterKillerAllLogic::onNoUnitsSpotted()
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
