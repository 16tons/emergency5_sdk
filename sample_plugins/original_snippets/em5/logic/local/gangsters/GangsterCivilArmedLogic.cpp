// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterCivilArmedLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterCivilArmedLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterCivilArmedLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterCivilArmedLogic::GangsterCivilArmedLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::BASEBALLBAT);
		mEquippedWeaponTypes.set(weapon::KNIFE);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterCivilArmedLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Check if the gangster has action, if not, start fleeing (its more or less an backup for stucking somewhere in the landscape)
		qsf::ActionComponent& actionComponent = getEntity()->getComponentSafe<qsf::ActionComponent>();
		if (nullptr == actionComponent.getCurrentAction())
		{
			// Escape by foot, short
			escape(ESCAPE_FOOT_SHORT);
		}
	}

	void GangsterCivilArmedLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// The civil armed 01 type flees on sight of a police vehicle. When only on-foot units are on sight, the gangster becomes a "killer taskforce"

		bool anyPoliceVehicleSighted = false;
		bool anyPoliceUnitOnFootSighted = false;

		for (const SpottedUnit& policeUnit : sightedPoliceUnits)
		{
			if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_POLICE
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK)
			{
				anyPoliceUnitOnFootSighted = true;
			}
			else if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_HELI
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_MTW
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK_HELI
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_STW)
			{
				anyPoliceVehicleSighted = true;
			}
		}

		if (anyPoliceUnitOnFootSighted)
		{
			// Become a "killer taskforce", depending on what weapons are equipped the logic deals with the "fixed" or the "move" type
			getEntity()->getComponentSafe<PersonComponent>().changeGangsterLogic(qsf::StringHash("em5::GangsterKillerTaskforceLogic"));
		}
		else if (anyPoliceVehicleSighted)
		{
			qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

			// Check if current action is an EscapeFootShortAction; if so, do nothing because the gangster is fleeing already
			if (actionComponent->getPlan().isEmpty() || EscapeFootShortAction::ACTION_ID != actionComponent->getCurrentAction()->getTypeId())
			{
				// Clear all actions
				actionComponent->clearPlan();

				// Escape by foot, short
				escape(ESCAPE_FOOT_SHORT);
			}
		}
		else
		{
			// No police units sighted, but this function was called? Something went wrong! Have a look in GangsterBaseLogic::updateSimulation!
			QSF_ERROR("Unknown police unit sighted in GangsterCivilArmed01", QSF_REACT_THROW);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
