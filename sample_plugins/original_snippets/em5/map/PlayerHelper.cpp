// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/map/PlayerHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/damage/DamageComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/game/player/Player.h"
#include "em5/game/units/UnitPool.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	bool PlayerHelper::isUnitAvailable(const qsf::Map& map, const qsf::StringHash& identifier) const
	{
		return (getUnitAvailableCount(map, identifier) > 0);
	}

	uint32 PlayerHelper::getUnitAvailableCount(const qsf::Map& map, const qsf::StringHash& identifier) const
	{
		// Get the number of available units (includes the unit count inside the HQ which arn't blocked)
		uint32 availableNumber = getNumberOfUnit(identifier);
		if (availableNumber > 0)
		{
			// Check if the entities inside the map aren't injured
			for (uint64 unitEntityId : mPlayer.getUnitPool().getUnitsInUseByName(identifier))
			{
				// Get the unit entity
				qsf::Entity* unitEntity = map.getEntityById(unitEntityId);

				// When unit entity is a nullptr then the unit is in use but "gone" -> not available (This is mostly the case for the paramedic team, when this team is injured the unit entity gets currently "lost")
				if (nullptr != unitEntity)
				{
					// What kind of entity is it
					const HealthComponent* healthComponent = unitEntity->getComponent<HealthComponent>();
					if (nullptr != healthComponent && (healthComponent->isInjured() || healthComponent->isDead()))
					{
						// Injured / dead person
						--availableNumber;
					}
					else
					{
						const DamageComponent* damageComponent = unitEntity->getComponent<DamageComponent>();
						if (nullptr != damageComponent && !damageComponent->isIntact())
						{
							// Damaged vehicle
							--availableNumber;
						}
					}
				}
				else
				{
					// Entity not found
					--availableNumber;
				}
			}
		}

		return availableNumber;
	}

	uint32 PlayerHelper::getNumberOfUnit(const qsf::StringHash& identifier) const
	{
		UnitPool& unitPool = mPlayer.getUnitPool();

		// Remove blocked unit count from the maximal available number. Blocked units aren't available
		return unitPool.getNumberMaxAvailableByName(identifier) - unitPool.getNumberBlockedByName(identifier);
	}

	uint32 PlayerHelper::getNumberAmbulanceMedics() const
	{
		return getNumberOfUnit(CommandableComponent::UNITTAG_AMBULANCE_DOCTOR);
	}

	uint32 PlayerHelper::getNumberAmbulanceParamedics() const
	{
		return getNumberOfUnit(CommandableComponent::UNITTAG_AMBULANCE_PARAMEDICS);
	}

	uint32 PlayerHelper::getNumberFireFightersFirefighter() const
	{
		return getNumberOfUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER)
			 + getNumberOfUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC);
	}

	uint32 PlayerHelper::getNumberPolicePolice() const
	{
		return getNumberOfUnit(CommandableComponent::UNITTAG_POLICE_POLICE);
	}

	uint32 PlayerHelper::getNumberRescueDogLeader() const
	{
		return getNumberOfUnit(CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOG_LEADER);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
