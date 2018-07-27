// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	// To access the injuries in content/em5_injury/default/em5_injury.json
	struct EM5_API_EXPORT injury
	{


		// Dummy for testing
		static const qsf::StringHash DUMMY_INJURY;

		// Injuries
		static const qsf::StringHash BURN_3_GRADE;
		static const qsf::StringHash BURN_4_GRADE;
		static const qsf::StringHash BURN_OF_BREATH;
		static const qsf::StringHash SMOKE_POISONING;
		static const qsf::StringHash BROKEN_ARM;
		static const qsf::StringHash BROKEN_LEG;
		static const qsf::StringHash BROKEN_SKULL;
		static const qsf::StringHash BRAIN_BLEEDING;
		static const qsf::StringHash INNER_BLEEDING;
		static const qsf::StringHash LOST_ARM;
		static const qsf::StringHash LOST_LEG;
		static const qsf::StringHash WHIPLASH;
		static const qsf::StringHash GUN_SHOT_WOUND_LUNG;
		static const qsf::StringHash GUN_SHOT_WOUND_BODY;
		static const qsf::StringHash GUN_SHOT_WOUND_BODY_HARD;
		static const qsf::StringHash STAB_WOUND_LUNG;
		static const qsf::StringHash STAB_WOUND_BODY;
		static const qsf::StringHash STAB_WOUND_BODY_INNER_BLEEDING;
		static const qsf::StringHash BITE_WOUND;
		static const qsf::StringHash BITE_WOUND_BLEEDING;
		static const qsf::StringHash HEAD_BODY_LACERATION;
		static const qsf::StringHash ELECTRIC_SHOCK;
		static const qsf::StringHash SHOCK;
		static const qsf::StringHash WEAKNESS;
		static const qsf::StringHash WATER_FILLED_LUNG;
		static const qsf::StringHash HEARTATTACK;
		static const qsf::StringHash STROKE;
		static const qsf::StringHash CIRCULATORYCOLLAPSE;
		static const qsf::StringHash FOOD_POISONING;
		static const qsf::StringHash ALCOHOL_POISONING;
		static const qsf::StringHash LOW_BLOOD_PRESSURE;
		static const qsf::StringHash ANGINA_PECTORIS;
		static const qsf::StringHash SHORTAGE_OF_BREATH;
		static const qsf::StringHash COLLAPSED_LUNG;
		static const qsf::StringHash HYPOTHERMIA;

		// Illnesses, spreading
		static const qsf::StringHash AVIAN_INFLUENZA;
		static const qsf::StringHash AVIAN_INFLUENZA_INJURED;
		static const qsf::StringHash SWINE_INFLUENZA;
		static const qsf::StringHash SWINE_INFLUENZA_INJURED;

		// Illnesses, non-spreading
		static const qsf::StringHash HARD_ASTHMA;
		static const qsf::StringHash HARD_ASTHMA_INJURED;
		static const qsf::StringHash HYDROPHOBIA;
		static const qsf::StringHash HYDROPHOBIA_INJURED;

		// Contaminations
		static const qsf::StringHash BIOLOGICAL_CONTAMINATION;
		static const qsf::StringHash BIOLOGICAL_CONTAMINATION_FOLLOW;
		static const qsf::StringHash CHEMICAL_CONTAMINATION;
		static const qsf::StringHash CHEMICAL_CONTAMINATION_SHORTAGE_OF_BREATH;
		static const qsf::StringHash CHEMICAL_CONTAMINATION_BREATH_BURN;
		static const qsf::StringHash RADIOACTIVE_CONTAMINATION;
		static const qsf::StringHash RADIOACTIVE_CONTAMINATION_ORGAN_BLEEDING;
		static const qsf::StringHash RADIOACTIVE_CONTAMINATION_ORGAN_FAILURE;
		static const qsf::StringHash PLAGUE_CONTAMINATION;
		static const qsf::StringHash PLAGUE_CONTAMINATION_ORGAN_FAILURE;
		static const qsf::StringHash PLAGUE_CONTAMINATION_TEST_POSITIV;
		static const qsf::StringHash PLAGUE_CONTAMINATION_TEST_NEGATIV;
		static const qsf::StringHash DROWNING_NORMAL;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
