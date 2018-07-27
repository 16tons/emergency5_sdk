// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/asset/AssetSystemTypes.h>

#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	struct EM5_API_EXPORT audio
	{


		static const std::string AUDIO_PATH;

		// Audio categories
		static const std::string AUDIO_CATEGORY_VOICEFEEDBACK;
		static const std::string AUDIO_CATEGORY_UI;
		static const std::string AUDIO_CATEGORY_SIREN;
		static const std::string AUDIO_CATEGORY_VEHICLES;
		static const std::string AUDIO_CATEGORY_PERSONNEL;
		static const std::string AUDIO_CATEGORY_CIVILVEHICLES;
		static const std::string AUDIO_CATEGORY_CIVILPERSONS;
		static const std::string AUDIO_CATEGORY_GANGSTER;
		static const std::string AUDIO_CATEGORY_ANIMALS;
		static const std::string AUDIO_CATEGORY_FIRE;
		static const std::string AUDIO_CATEGORY_BUILDING;
		static const std::string AUDIO_CATEGORY_DESTRUCTION;
		static const std::string AUDIO_CATEGORY_EVENTS;
		static const std::string AUDIO_CATEGORY_MAINEVENTS;
		static const std::string AUDIO_CATEGORY_MUSIC;

		// Patient
		static const std::string PATIENT_MOAN_01;
		static const std::string PATIENT_MOAN_02;
		static const std::string PATIENT_COUGH_01;

		// Command
		static const std::string EM5_COMMAND_02;

		// Fire
		static const std::string EM5_FIRE_SMALL;
		static const std::string EM5_FIRE_MEDIUM;
		static const std::string EM5_FIRE_LARGE;

		static const std::string EM5_AMBIENT_FIRE_01;
		static const std::string EM5_AMBIENT_FIRE_02;
		static const std::string EM5_AMBIENT_FIRE_03;

		static const qsf::GlobalAssetId EM5_FIRE_SMALL_ASSET;
		static const qsf::GlobalAssetId EM5_FIRE_MEDIUM_ASSET;
		static const qsf::GlobalAssetId EM5_FIRE_LARGE_ASSET;

		static const qsf::GlobalAssetId EM5_AMBIENT_FIRE_01_ASSET;
		static const qsf::GlobalAssetId EM5_AMBIENT_FIRE_02_ASSET;
		static const qsf::GlobalAssetId EM5_AMBIENT_FIRE_03_ASSET;

		// Siren
		static const std::string EM5_SIREN_FIREFIGHTERS_01;
		static const std::string EM5_SIREN_FIREFIGHTERS_02;
		static const std::string EM5_SIREN_RESCUE_01;
		static const std::string EM5_SIREN_POLICE_01;
		static const std::string EM5_SIREN_TEC_01;
		static const std::string EM5_SIREN_HISTORIC_01;

		// Vehicles
		static const std::string EM5_VEHICLEDOOR_CLOSE_01;
		static const std::string EM5_VEHICLEDOOR_CLOSE_02;
		static const std::string EM5_VEHICLEDOOR_CLOSE_03;

		static const std::string EM5_VEHICLEDOOR_OPEN_01;
		static const std::string EM5_VEHICLEDOOR_OPEN_02;
		static const std::string EM5_VEHICLEDOOR_OPEN_03;

		static const std::string EM5_RW_01;
		static const std::string EM5_TLF_03;

		static const std::string EM5_ROTOR_01;
		static const std::string EM5_ROTOR_02;
		static const std::string EM5_ROTOR_03;

		static const std::string EM5_ASF_01;
		static const std::string EM5_ASF_02;
		static const std::string EM5_ASF_03;

		static const std::string EM5_BFZ_01;
		static const std::string EM5_BFZ_02;
		static const std::string EM5_BFZ_03;
		static const std::string EM5_BFZ_04;
		static const std::string EM5_BFZ_05;
		static const std::string EM5_BFZ_06;
		static const std::string EM5_BFZ_07;

		static const std::string EM5_BHC_01;
		static const std::string EM5_BHC_02;
		static const std::string EM5_BHC_03;

		static const std::string EM5_PHC_01;

		static const std::string EM5_SEKTFD_01;
		static const std::string EM5_SEKD_01;
		static const std::string EM5_SEKD_02;

		static const std::string EM5_FWD_01;

		static const std::string EM5_TFSEKR_01;
		static const std::string EM5_SEKR_01;

		static const std::string EM5_DEKONP_01;

		static const std::string EM5_DLK_01;
		static const std::string EM5_DLK_02;
		static const std::string EM5_DLK_03;

		static const std::string EM5_FLB_01;

		static const std::string EM5_FMB_01;
		static const std::string EM5_FMB_02;
		static const std::string EM5_FMB_03;

		static const std::string EM5_LFK_02;
		static const std::string EM5_LFK_03;

		static const std::string EM5_LF_01;
		static const std::string EM5_LF_02;
		static const std::string EM5_LF_03;

		// Personnel
		static const std::string EM5_DOCTOR_CASE_01;
		// TODO(db): Change the soundfile when the sound is repaired
		static const std::string EM5_DOCTOR_CASE_02;

		static const std::string EM5_DOCTOR_DEFI_01;

		static const std::string EM5_DOCTOR_EKG_01;
		static const std::string EM5_DOCTOR_EKG_02;

		static const std::string EM5_DOCTOR_VENTILATION_01;

		static const std::string EM5_MEDICS_STRETCHER_01;
		static const std::string EM5_MEDICS_STRETCHER_02;

		static const std::string EM5_RESCUEDOG_01;
		static const std::string EM5_RESCUEDOG_02;
		static const std::string EM5_RESCUEDOG_03;

		static const std::string EM5_ENGINEER_01;
		static const std::string EM5_ENGINEER_BUILDING_REPARING;

		static const std::string EM5_POLICEMAN_PEPPERSPRAY_01;
		static const std::string EM5_POLICEMAN_HANDCUFFS_01;

		static const std::string EM5_POLICEMAN_MEGAPHONE_01;

		static const std::string EM5_SEKSPEC_01;
		static const std::string EM5_SEKSPEC_SHOOT_01;
		static const std::string EM5_SEKSPEC_SHOOT_02;
		static const std::string EM5_SEKSPEC_FLASHGRENADE_01;

		static const std::string EM5_FIREFIGHTER_EXTINGUISHER_01;
		static const std::string EM5_FIREFIGHTER_EXTINGUISHER_02;

		static const std::string EM5_FIREFIGHTER_HOSE_01;
		static const std::string EM5_FIREFIGHTER_HOSE_02;

		static const std::string EM5_FIREFIGHTER_SAW_01;
		static const std::string EM5_FIREFIGHTER_SAW_02;

		static const std::string EM5_FIREFIGHTER_SCISSORS_01;
		static const std::string EM5_FIREFIGHTER_SCISSORS_02;
		static const std::string EM5_FIREFIGHTER_SCISSORS_03;

		static const std::string EM5_FIREFIGHTER_AXE_01;

		static const std::string EM5_FIREFIGHTER_HISTORIC_01;
		static const std::string EM5_FIREFIGHTER_HISTORIC_02;
		static const std::string EM5_FIREFIGHTER_HISTORIC_03;
		static const std::string EM5_FIREFIGHTER_HISTORIC_04;

		static const std::string EM5_DIVER_01;
		static const std::string EM5_DIVER_02;

		static const std::string EM5_HISTORIC_DOCTOR_SEND_HOME;
		static const std::string EM5_HISTORIC_DOCTOR_CLEAN_HOME;
		static const std::string EM5_HISTORIC_DOCTOR_CLEAN_HOME2;

		// civil vehicles
		static const std::string EM5_CIVILCAR_HORN_01;
		static const std::string EM5_CIVILCAR_HORN_02;
		static const std::string EM5_CIVILCAR_HORN_03;
		static const std::string EM5_CIVILCAR_HORN_04;

		static const std::string EM5_TRAM_GO_01;
		static const std::string EM5_TRAM_GO_02;
		static const std::string EM5_TRAM_STOP_01;

		static const std::string EM5_TRAIN01_GO_01;
		static const std::string EM5_TRAIN01_GO_02;
		static const std::string EM5_TRAIN01_STOP_01;
		static const std::string EM5_TRAIN02_GO_01;
		static const std::string EM5_TRAIN02_GO_02;
		static const std::string EM5_TRAIN02_STOP_01;

		static const std::string EM5_FREIGHTER_01;
		static const std::string EM5_CUTTER_01;

		// Civil persons
		static const std::string EM5_CIVIL_SKIING_01;

		// Gangster
		static const std::string EM5_FISTFIGHT_01;
		static const std::string EM5_FISTFIGHT_02;
		static const std::string EM5_FISTFIGHT_03;
		static const std::string EM5_FISTFIGHT_04;

		static const std::string EM5_GANGSTERPISTOL_01;

		// Animals
		static const std::string EM5_COW_01;
		static const std::string EM5_COW_02;
		static const std::string EM5_COW_03;	// Not used at the moment

		// Building
		static const std::string EM5_BUILDING_BREAKDOWN_01;
		static const std::string EM5_BUILDING_BREAKDOWN_02;
		static const std::string EM5_BUILDING_BREAKDOWN_03;
		static const std::string EM5_BUILDING_WINDESTROYED_01;
		static const std::string EM5_BUILDING_ATTACKING_01;
		static const std::string EM5_BUILDING_ATTACKING_02;
		static const std::string EM5_BUILDING_EVACUATE;
		static const std::string EM5_BUILDING_EVACUATE2;

		// Destruction
		static const std::string EM5_TREE_DOWN_01;
		static const std::string EM5_EXPLOSION_01;
		static const std::string EM5_EXPLOSION_02;
		static const std::string EM5_EXPLOSION_03;
		static const std::string EM5_EXPLOSION_04;

		// Events
		static const std::string EM5_TIMEBOMB_01;
		static const std::string EM5_BOMB_EXPLOSION_01;
		static const std::string EM5_DIVESPOT_01;

		static const std::string EM5_CONTROLBOX_01;
		static const std::string EM5_CONTROLBOX_02;
		static const std::string EM5_CONTROLBOX_03;

		// ME01
		static const std::string EM5_SPARKS_SHOWER_GE01;
		static const std::string EM5_KEROSINE_IGNITION_GE01;
		static const std::string EM5_KEROSINE_BURNING_GE01;

		// UI
		static const std::string LEFTMOUSE;
		static const std::string RIGHTMOUSE;
		static const std::string FAILMOUSE;
		static const std::string OBJECTIVE_DONE;
		static const std::string ADVANCEMENT_WON;
		static const std::string FAILED;
		static const std::string TOOLTIP;
		static const std::string ACHIEVEMENT_WON;
		static const std::string MEDAL_WON;
		static const std::string BUTTON_HOVER;
		static const std::string BUY_VEHICLE;
		static const std::string MINIMAP_PING;
		static const std::string WARBLER_TWEET;

		// Music
		static const std::string EM5_BACKGR_GE_01;
		static const std::string EM5_ACTION_01_LOW;
		static const std::string EM5_ACTION_01_HIGHT;
		static const std::string EM5_ACTION_SHORT_01;
		static const std::string EM5_JINGLE_WIN;
		static const std::string EM5_JINGLE_LOSE;

		static const std::string MAINMENU_MUSIC_EM5;
		static const std::string MAINMENU_MUSIC_EM2017;
		static const std::string MAINMENU_MUSIC_EM20YEARS;

		// Soundtrack
		static const std::string SOUNDTRACK_CAMPAGIN_MUNICH;
		static const std::string SOUNDTRACK_CAMPAGIN_HAMBURG;
		static const std::string SOUNDTRACK_CAMPAGIN_BERLIN;
		static const std::string SOUNDTRACK_FREEPLAY;

		static const std::string SOUNDTRACK_CAMPAGIN_MUNICH_2017;
		static const std::string SOUNDTRACK_CAMPAGIN_HAMBURG_2017;
		static const std::string SOUNDTRACK_CAMPAGIN_BERLIN_2017;
		static const std::string SOUNDTRACK_CAMPAGIN_COLOGNE_2017;

		static const std::string SOUNDTRACK_ME01;
		static const std::string SOUNDTRACK_ME02;
		static const std::string SOUNDTRACK_ME03;
		static const std::string SOUNDTRACK_ME04;
		static const std::string SOUNDTRACK_ME05;
		static const std::string SOUNDTRACK_ME06;
		static const std::string SOUNDTRACK_ME07;
		static const std::string SOUNDTRACK_ME08;
		static const std::string SOUNDTRACK_ME09;
		static const std::string SOUNDTRACK_ME10;
		static const std::string SOUNDTRACK_ME11;
		static const std::string SOUNDTRACK_ME12;
		static const std::string SOUNDTRACK_ME13;
		static const std::string SOUNDTRACK_ME14;
		static const std::string SOUNDTRACK_ME15;

		static const std::string SOUNDTRACK_ME201;
		static const std::string SOUNDTRACK_ME202;
		static const std::string SOUNDTRACK_ME203;
		static const std::string SOUNDTRACK_ME204;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
