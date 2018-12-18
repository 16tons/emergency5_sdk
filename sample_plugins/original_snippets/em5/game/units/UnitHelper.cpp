// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/base/PrototypeContainer.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/link/link/prototype/ContainerLink.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnitHelper::UnitHelper(qsf::Entity& entity) :
		mEntity(entity),
		mCommandableComponent(entity.getComponent<CommandableComponent>())
	{
		// Nothing here
	}

	UnitHelper::~UnitHelper()
	{
		// Nothing here
	}

	bool UnitHelper::containsInjuredPersons()
	{
		EntityHelper entityHelper(mEntity);
		if (entityHelper.isSquadRoadVehicle())
		{
			qsf::game::PrototypeContainer* prototypeContainer = entityHelper.getPrototypeContainer();
			if (nullptr != prototypeContainer)
			{
				for (auto& iterator : prototypeContainer->getLinks())
				{
					// Get the person in the vehicle
					qsf::ContainerLink& containerLink = *iterator.second;
					qsf::Entity& personEntity = containerLink.getTargetEntity();

					// Check if the person is injured
					if (EntityHelper(personEntity).isPersonInjured())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool UnitHelper::containsGangster()
	{
		EntityHelper entityHelper(mEntity);
		if (entityHelper.isSquadRoadVehicle())
		{
			qsf::game::PrototypeContainer* prototypeContainer = entityHelper.getPrototypeContainer();
			if (nullptr != prototypeContainer)
			{
				for (auto& iterator : prototypeContainer->getLinks())
				{
					// Get the person in the vehicle
					qsf::ContainerLink& containerLink = *iterator.second;
					qsf::Entity& personEntity = containerLink.getTargetEntity();

					// Check if the person is injured
					if (EntityHelper(personEntity).isGangsterPerson())
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool UnitHelper::isAmbulanceNEF() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceNEF() : false;
	}

	bool UnitHelper::isAmbulanceRTW() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceRTW() : false;
	}

	bool UnitHelper::isAmbulanceIMT() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceIMT() : false;
	}

	bool UnitHelper::isAmbulanceHelicopter() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceHeli() : false;
	}

	bool UnitHelper::isAmbulanceDoctor() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceDoctor() : false;
	}

	bool UnitHelper::isAmbulanceParamedics() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceParamedics() : false;
	}

	bool UnitHelper::isAmbulanceRoadVehicle() const
	{
		if (nullptr != mCommandableComponent)
		{
			return (mCommandableComponent->isAmbulanceNEF() || mCommandableComponent->isAmbulanceRTW() || mCommandableComponent->isAmbulanceIMT()
					|| mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOGCAR));
		}
		return false;
	}

	bool UnitHelper::isAmbulanceRescueDogLeader() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceRescueDogLeader() : false;
	}

	bool UnitHelper::isAmbulanceRescueDog() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isAmbulanceRescueDog() : false;
	}

	bool UnitHelper::isPoliceSTW() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceSTW() : false;
	}

	bool UnitHelper::isPoliceMTW() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceMTW() : false;
	}

	bool UnitHelper::isPoliceSEKHeli() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceSEKHeli() : false;
	}

	bool UnitHelper::isPoliceTSEKR() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceTSEKR() : false;
	}

	bool UnitHelper::isPoliceSEKR() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceSEKR() : false;
	}

	bool UnitHelper::isPoliceDogCar() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceDogCar() : false;
	}

	bool UnitHelper::isPoliceRoadVehicle() const
	{
		if (nullptr != mCommandableComponent)
		{
			return (mCommandableComponent->isPoliceMTW() || mCommandableComponent->isPoliceSTW() || mCommandableComponent->isPoliceTSEKR() || mCommandableComponent->isPoliceDogCar());
		}
		return false;
	}

	bool UnitHelper::isPolicePerson() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPolicePerson() : false;
	}

	bool UnitHelper::isPoliceSEKPerson() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceSEKPerson() : false;
	}

	bool UnitHelper::isPoliceDog() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceDog() : false;
	}

	bool UnitHelper::isFireFighterRW() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterRW() : false;
	}

	bool UnitHelper::isFireFighterDECONP() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterDECONP() : false;
	}

	bool UnitHelper::isFireFighterDLK() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterDLK() : false;
	}

	bool UnitHelper::isFireFighterFLB() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterFLB() : false;
	}

	bool UnitHelper::isFireFighterBoat() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT) : false;
	}

	bool UnitHelper::isFireFighterBoatTransporter() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT) : false;
	}

	bool UnitHelper::isFireFighterCommandoCar() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_COMMANDOCAR) : false;
	}

	bool UnitHelper::isFireFighterTLF() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_TLF) : false;
	}

	bool UnitHelper::isFireFighterDiverTransporter() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT) : false;
	}

	bool UnitHelper::isFireFighterRoadVehicle() const
	{
		if (nullptr != mCommandableComponent)
		{
			return (mCommandableComponent->isFireFighterDECONP() || mCommandableComponent->isFireFighterDLK() || mCommandableComponent->isFireFighterRW()
					|| mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_BOAT_TRANSPORT)
					|| mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_COMMANDOCAR)
					|| mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_TLF)
					|| mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DIVER_TRANSPORT));
		}
		return false;
	}

	bool UnitHelper::isFireFighterPerson() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterPerson() : false;
	}

	bool UnitHelper::isFireFighterPersonWithABCSuitActive() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC) : false;
	}

	bool UnitHelper::isFireFighterDrone() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isFireFighterDrone() : false;
	}

	bool UnitHelper::isTechEngineerCar() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isTechEngineerCar() : false;
	}

	bool UnitHelper::isTechTowcar() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isTechTowcar() : false;
	}

	bool UnitHelper::isTechCrane() const
	{
		return isUnit(CommandableComponent::UNITTAG_TECH_CRANE);
	}

	bool UnitHelper::isMedievalDoctor() const
	{
		return isUnit(CommandableComponent::UNITTAG_AMBULANCE_DOCTOR_HISTORIC);
	}

	bool UnitHelper::isMedievalGuardsman() const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isPoliceGuardsmanHistoric() : false;
	}

	bool UnitHelper::isUnit(const qsf::NamedIdentifier& unitName) const
	{
		return (nullptr != mCommandableComponent) ? mCommandableComponent->isUnitType(unitName) : false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
