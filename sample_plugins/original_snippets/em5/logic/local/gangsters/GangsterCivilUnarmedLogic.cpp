// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterCivilUnarmedLogic.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/move/escape/EscapeFootLongAction.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterCivilUnarmedLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterCivilUnarmedLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterCivilUnarmedLogic::GangsterCivilUnarmedLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		mEquippedWeaponTypes.set(weapon::FIST);

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterCivilUnarmedLogic::onIdle()
	{
		// If there's nothing more to do: Escape by foot
		escape(ESCAPE_FOOT_LONG);
	}

	void GangsterCivilUnarmedLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// The civil unarmed type does not distinguish between different police units
		// Whenever a police unit is sighted, the gangster's will start to escape

		// Check if current action is an escape action; if so, do nothing
		if (nullptr == mActionComponent->getCurrentAction() ||
			(nullptr == mActionComponent->getAction<EscapeFootShortAction>() && nullptr == mActionComponent->getAction<EscapeFootLongAction>() && nullptr == mActionComponent->getAction<qsf::WaitAction>()))
		{
			// Clear all actions
			mActionComponent->clearPlan();

			// Escape by foot
			escape(ESCAPE_FOOT_LONG);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
